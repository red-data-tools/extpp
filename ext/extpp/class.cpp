#include <ruby.hpp>
#include "function.hpp"

#include <map>
#include <unordered_map>

namespace rb {
  using MethodFunc = VALUE (*)(ANYARGS);

  using MethodTable = std::unordered_map<std::string, Function *>;
}

namespace {
  VALUE MethodTable = Qnil;

  void free_method_table(void *data) {
    auto method_table = static_cast<rb::MethodTable *>(data);
    delete method_table;
  }

  const rb_data_type_t MethodTable_type = {
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
                         &MethodTable_type,
                         method_table);
    return method_table;
  }

  VALUE method_table_to_ruby(rb::MethodTable *method_table) {
    if (NIL_P(MethodTable)) {
      MethodTable = rb_define_class("MethodTable", rb_cData);
    }
    return TypedData_Wrap_Struct(MethodTable, &MethodTable_type, method_table);
  }

  VALUE call_func(int argc, VALUE *argv, VALUE self) {
    auto rb_method_table =
      rb_ivar_get(rb_obj_class(self), rb_intern("__method_table__"));
    auto method_table = method_table_from_ruby(rb_method_table);
    auto method_id = rb_sym2id(rb_funcall(self, rb_intern("__method__"), 0));
    std::string function_name(rb_id2name(method_id));
    auto function = (*method_table)[function_name];
    return function->call(self, argc, argv);
  }

  VALUE method_missing(int argc, VALUE *argv, VALUE self) {
    auto klass = rb_obj_class(self);
    auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
    auto method_table = method_table_from_ruby(rb_method_table);

    VALUE rb_name_symbol;
    VALUE rb_args;
    rb_scan_args(argc, argv, "1*", &rb_name_symbol, &rb_args);
    auto rb_name = rb_sym2str(rb_name_symbol);
    std::string function_name(RSTRING_PTR(rb_name),
                              RSTRING_LEN(rb_name));
    auto function = (*method_table)[function_name];
    if (function) {
      rb_define_method(klass,
                       function_name.c_str(),
                       reinterpret_cast<rb::MethodFunc>(call_func),
                       -1);
      return function->call(self, RARRAY_LEN(rb_args), RARRAY_PTR(rb_args));
    } else {
      return rb_call_super(RARRAY_LEN(rb_args), RARRAY_PTR(rb_args));
    }
  }

  VALUE respond_to_missing_p(VALUE self,
                             VALUE rb_name_symbol,
                             VALUE rb_include_private) {
    auto klass = rb_obj_class(self);
    auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
    auto method_table = method_table_from_ruby(rb_method_table);

    auto rb_name = rb_sym2str(rb_name_symbol);
    std::string function_name(RSTRING_PTR(rb_name),
                              RSTRING_LEN(rb_name));
    auto function = (*method_table)[function_name];
    if (function) {
      rb_define_method(klass,
                       function_name.c_str(),
                       reinterpret_cast<rb::MethodFunc>(call_func),
                       -1);
      return Qtrue;
    } else {
      VALUE rb_args[] = {rb_name_symbol, rb_include_private};
      return rb_call_super(2, rb_args);
    }
  }
}

namespace rb {
  class Class::ClassImpl {
  public:
    ClassImpl(VALUE klass) :
      class_(klass),
      method_table_(new MethodTable()),
      lazy_define_method_(false) {
      rb_iv_set(class_, "__method_table__", method_table_to_ruby(method_table_));
    }

    void enable_lazy_define_method() {
      if (lazy_define_method_) {
        return;
      }

      lazy_define_method_ = true;
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
      (*method_table_)[name] = function;
      if (!lazy_define_method_) {
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
  };

  Class::Class(const char *name, VALUE parent) :
    impl_(new ClassImpl(rb_define_class(name, parent))) {
  }

  Class::Class(VALUE klass) :
    impl_(new ClassImpl(klass)) {
  }

  Class::~Class() {
    delete impl_;
  }

  Class &Class::define_method_raw(const char *name,
                                  MethodWithoutArgumentsRaw *body) {
    impl_->define_method(name, body);
    return (Class &)*this;
  }

  Class &Class::define_method_raw(const char *name,
                                  MethodWithArgumentsRaw *body) {
    impl_->define_method(name, body);
    return (Class &)*this;
  }

  Class &Class::define_method(const char *name, MethodWithoutArguments body) {
    auto function = new FunctionNoArgument(body);
    impl_->define_method(name, function);
    return (Class &)*this;
  }

  Class &Class::define_method(const char *name, MethodWithArguments body) {
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
