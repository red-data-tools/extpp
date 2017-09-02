#pragma once

#include <ruby.h>

namespace rb {
  using MethodWithoutArguments = VALUE (*)(VALUE self);
  using MethodWithArguments = VALUE (*)(VALUE self, int argc, VALUE *argv);
  using MethodWithArgumentsCompatible =
    VALUE (*)(int argc, VALUE *argv, VALUE self);

  class Class {
  public:
    Class(const char *name, VALUE parent=rb_cObject);
    Class(VALUE klass);
    ~Class();

    Class &define_method(const char *name,
                         MethodWithoutArguments body);
    Class &define_method(const char *name,
                         MethodWithArguments body);
    Class &define_method(const char *name,
                         MethodWithArgumentsCompatible body);

    Class &enable_lazy_define_method();

  private:
    class ClassImpl;
    ClassImpl *impl_;
  };
}
