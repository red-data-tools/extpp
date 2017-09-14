#include "function.hpp"

#include <unordered_map>
#include <vector>

namespace rb {
  using MethodTable = std::unordered_map<ID, Function *>;

  struct MethodDefinition {
    MethodDefinition(std::string name_, Function *function_) :
      name(name_),
      function(function_) {
    }

    std::string name;
    Function *function;
  };
  using MethodDefinitions = std::vector<MethodDefinition>;
}

namespace {
  VALUE MethodTable = Qnil;

  void free_method_table(void *data) {
    auto method_table = static_cast<rb::MethodTable *>(data);
    delete method_table;
  }

  const rb_data_type_t MethodTableType = {
    "MethodTable",
    {nullptr, free_method_table, nullptr,},
    nullptr,
    nullptr,
    RUBY_TYPED_FREE_IMMEDIATELY,
  };

  rb::MethodTable *method_table_from_ruby(VALUE rb_method_table) {
    rb::MethodTable *method_table;
    TypedData_Get_Struct(rb_method_table,
                         rb::MethodTable,
                         &MethodTableType,
                         method_table);
    return method_table;
  }

  VALUE method_table_to_ruby(rb::MethodTable *method_table) {
    if (NIL_P(MethodTable)) {
      MethodTable = rb_define_class("MethodTable", rb_cData);
    }
    return TypedData_Wrap_Struct(MethodTable, &MethodTableType, method_table);
  }

  VALUE MethodDefinitions = Qnil;

  void free_method_definitions(void *data) {
    auto method_definitions = static_cast<rb::MethodDefinitions *>(data);
    delete method_definitions;
  }

  const rb_data_type_t MethodDefinitionsType = {
    "MethodDefinitions",
    {nullptr, free_method_definitions, nullptr,},
    nullptr,
    nullptr,
    RUBY_TYPED_FREE_IMMEDIATELY,
  };

  rb::MethodDefinitions *method_definitions_from_ruby(VALUE rb_definitions) {
    rb::MethodDefinitions *definitions;
    TypedData_Get_Struct(rb_definitions,
                         rb::MethodDefinitions,
                         &MethodDefinitionsType,
                         definitions);
    return definitions;
  }

  VALUE method_definitions_to_ruby(rb::MethodDefinitions *definitions) {
    if (NIL_P(MethodDefinitions)) {
      MethodDefinitions = rb_define_class("MethodDefinitions", rb_cData);
    }
    return TypedData_Wrap_Struct(MethodDefinitions,
                                 &MethodDefinitionsType,
                                 definitions);
  }

  VALUE call_func(int argc, VALUE *argv, VALUE self) {
    auto rb_method_table =
      rb_ivar_get(rb_obj_class(self), rb_intern("__method_table__"));
    auto method_table = method_table_from_ruby(rb_method_table);
    auto method_name_symbol = rb_funcall(self, rb_intern("__method__"), 0);
    auto function = (*method_table)[rb_sym2id(method_name_symbol)];
    return function->call(self, argc, argv);
  }

  bool flush_method_definitions(VALUE klass) {
    ID id_method_definitions = rb_intern("__method_definitions__");
    auto rb_definitions = rb_ivar_get(klass, id_method_definitions);
    if (NIL_P(rb_definitions)) {
      return false;
    }

    auto definitions = method_definitions_from_ruby(rb_definitions);
    auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
    auto method_table = method_table_from_ruby(rb_method_table);
    for (const auto &definition : *definitions) {
      ID name_id = rb_intern(definition.name.c_str());
      (*method_table)[name_id] = definition.function;
      rb_define_method(klass,
                       definition.name.c_str(),
                       reinterpret_cast<rb::MethodFunc>(call_func),
                       -1);
    }
    rb_ivar_set(klass, id_method_definitions, Qnil);
    return true;
  }

  VALUE method_missing(int argc, VALUE *argv, VALUE self) {
    auto klass = rb_obj_class(self);

    if (flush_method_definitions(klass)) {
      auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
      auto method_table = method_table_from_ruby(rb_method_table);

      VALUE rb_name_symbol;
      VALUE rb_args;
      rb_scan_args(argc, argv, "1*", &rb_name_symbol, &rb_args);
      auto function = (*method_table)[rb_sym2id(rb_name_symbol)];
      if (function) {
        return function->call(self, RARRAY_LEN(rb_args), RARRAY_PTR(rb_args));
      }
    }

    return rb_call_super(argc, argv);
  }

  VALUE respond_to_missing_p(VALUE self,
                             VALUE rb_name_symbol,
                             VALUE rb_include_private) {
    auto klass = rb_obj_class(self);

    if (flush_method_definitions(klass)) {
      auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
      auto method_table = method_table_from_ruby(rb_method_table);

      auto function = (*method_table)[rb_sym2id(rb_name_symbol)];
      if (function) {
        return Qtrue;
      }
    }

    VALUE rb_args[] = {rb_name_symbol, rb_include_private};
    return rb_call_super(2, rb_args);
  }
}

namespace rb {
  class Class::ClassImpl {
  public:
    ClassImpl(VALUE klass) :
      class_(klass),
      method_table_(new MethodTable()),
      lazy_define_method_(false),
      method_definitions_(nullptr) {
      rb_iv_set(class_, "__method_table__", method_table_to_ruby(method_table_));
      rb_iv_set(class_, "__method_definitions__", Qnil);
    }

    void enable_lazy_define_method() {
      if (lazy_define_method_) {
        return;
      }

      lazy_define_method_ = true;
      method_definitions_ = new MethodDefinitions();
      rb_iv_set(class_, "__method_definitions__",
                method_definitions_to_ruby(method_definitions_));
      rb_define_method(class_,
                       "method_missing",
                       reinterpret_cast<MethodFunc>(method_missing),
                       -1);
      rb_define_method(class_,
                       "respond_to_missing?",
                       reinterpret_cast<MethodFunc>(respond_to_missing_p),
                       -1);
    }

    void define_method(const char *name, VALUE (*body)(VALUE self)) {
      rb_define_method(class_,
                       name,
                       reinterpret_cast<MethodFunc>(body),
                       0);
    }

    void define_method(const char *name,
                       VALUE (*body)(int argc, VALUE *argv, VALUE self)) {
      rb_define_method(class_,
                       name,
                       reinterpret_cast<MethodFunc>(body),
                       -1);
    }

    void define_method(const char *name, Function *function) {
      if (lazy_define_method_) {
        method_definitions_->emplace_back(name, function);
      } else {
        ID name_id = rb_intern(name);
        (*method_table_)[name_id] = function;
        rb_define_method(class_,
                         name,
                         reinterpret_cast<MethodFunc>(call_func),
                         -1);
      }
    }

  private:
    VALUE class_;
    MethodTable *method_table_;
    bool lazy_define_method_;
    MethodDefinitions *method_definitions_;
  };

  Class::Class(const char *name, VALUE parent) :
    Object(rb_define_class(name, parent)),
    impl_(new ClassImpl(this->to_ruby())) {
  }

  Class::Class(VALUE klass) :
    Object(klass),
    impl_(new ClassImpl(this->to_ruby())) {
  }

  Class::~Class() {
    delete impl_;
  }

  Class &Class::define_method(const char *name,
                              MethodWithoutArguments body) {
    auto function = new FunctionWithoutArgument(body);
    impl_->define_method(name, function);
    return (Class &)*this;
  }

  Class &Class::define_method(const char *name,
                              MethodWithArguments body) {
    auto function = new FunctionWithArguments(body);
    impl_->define_method(name, function);
    return (Class &)*this;
  }

  Class &Class::define_method(const char *name,
                              MethodWithArgumentsCompatible body) {
    auto function = new FunctionWithArgumentsCompatible(body);
    impl_->define_method(name, function);
    return (Class &)*this;
  }

  Class &Class::enable_lazy_define_method() {
    impl_->enable_lazy_define_method();
    return (Class &)*this;
  }
}
