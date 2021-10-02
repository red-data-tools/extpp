#pragma once

#include <ruby/function.hpp>
#include <ruby/object.hpp>

#include <string>
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

  namespace {
    inline MethodTable *method_table_from_ruby(VALUE rb_method_table) {
      return reinterpret_cast<MethodTable *>(NUM2ULL(rb_method_table));
    }

    inline VALUE method_table_to_ruby(MethodTable *method_table) {
      return ULL2NUM(reinterpret_cast<uintptr_t>(method_table));
    }

    inline MethodDefinitions *
    method_definitions_from_ruby(VALUE rb_definitions) {
      return reinterpret_cast<MethodDefinitions *>(NUM2ULL(rb_definitions));
    }

    inline VALUE method_definitions_to_ruby(MethodDefinitions *definitions) {
      return ULL2NUM(reinterpret_cast<uintptr_t>(definitions));
    }

    inline VALUE call_func(int argc, VALUE *argv, VALUE self) {
      auto rb_method_table =
        rb_ivar_get(rb_obj_class(self), rb_intern("__method_table__"));
      auto method_table = method_table_from_ruby(rb_method_table);
      auto method_name_symbol = rb_funcall(self, rb_intern("__method__"), 0);
      auto function = (*method_table)[rb_sym2id(method_name_symbol)];
      return function->call(self, argc, argv);
    }

    inline bool flush_method_definitions(VALUE klass) {
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
                         reinterpret_cast<rb::RawMethod>(call_func),
                         -1);
      }
      rb_ivar_set(klass, id_method_definitions, Qnil);
      return true;
    }

    inline VALUE method_missing(int argc, VALUE *argv, VALUE self) {
      auto klass = rb_obj_class(self);

      if (flush_method_definitions(klass)) {
        auto rb_method_table = rb_ivar_get(klass, rb_intern("__method_table__"));
        auto method_table = method_table_from_ruby(rb_method_table);

        VALUE rb_name_symbol;
        VALUE rb_args;
        rb_scan_args(argc, argv, "1*", &rb_name_symbol, &rb_args);
        auto function = (*method_table)[rb_sym2id(rb_name_symbol)];
        if (function) {
          return function->call(self,
                                static_cast<int>(RARRAY_LEN(rb_args)),
                                RARRAY_PTR(rb_args));
        }
      }

      return rb_call_super(argc, argv);
    }

    inline VALUE respond_to_missing_p(VALUE self,
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

  class Class: public Object {
  public:
    Class(const char *name) :
      Class(RTEST(rb_funcall(rb_cObject,
                              rb_intern("const_defined?"),
                              1,
                              rb_str_new_static(name, strlen(name)))) ?
             rb_funcall(rb_cObject,
                        rb_intern("const_get"),
                        1,
                        rb_str_new_static(name, strlen(name))) :
            rb_define_class(name, rb_cObject)) {
    }

    Class(const char *name, VALUE parent) :
      Class(rb_define_class(name, parent)) {
    }

    Class(VALUE klass) :
      Object(klass),
      class_(klass),
      method_table_(new MethodTable()),
      lazy_define_method_(false),
      method_definitions_(nullptr) {
      rb_iv_set(class_,
                "__method_table__",
                method_table_to_ruby(method_table_));
      rb_iv_set(class_, "__method_definitions__", Qnil);
    }

    inline Class &define_method(const char *name,
                                MethodWithoutArguments body) {
      auto function = new FunctionWithoutArgument(body);
      return define_method(name, function);
    }

    inline Class &define_method(const char *name,
                                MethodWithArguments body) {
      auto function = new FunctionWithArguments(body);
      return define_method(name, function);
    }

    inline Class &define_method(const char *name,
                                MethodWithArgumentsCompatible body) {
      auto function = new FunctionWithArgumentsCompatible(body);
      return define_method(name, function);
    }

    inline Class &define_method(const char *name, Function *function) {
      if (lazy_define_method_) {
        method_definitions_->emplace_back(name, function);
      } else {
        ID name_id = rb_intern(name);
        (*method_table_)[name_id] = function;
        rb_define_method(class_,
                         name,
                         reinterpret_cast<RawMethod>(call_func),
                         -1);
      }
      return (Class &)*this;
    }

    inline Class &enable_lazy_define_method() {
      if (lazy_define_method_) {
        return (Class &)*this;
      }

      lazy_define_method_ = true;
      method_definitions_ = new MethodDefinitions();
      rb_iv_set(class_,
                "__method_definitions__",
                method_definitions_to_ruby(method_definitions_));
      rb_define_method(class_,
                       "method_missing",
                       reinterpret_cast<RawMethod>(method_missing),
                       -1);
      rb_define_method(class_,
                       "respond_to_missing?",
                       reinterpret_cast<RawMethod>(respond_to_missing_p),
                       -1);
      return (Class &)*this;
    }

  private:
    VALUE class_;
    MethodTable *method_table_;
    bool lazy_define_method_;
    MethodDefinitions *method_definitions_;
  };
}
