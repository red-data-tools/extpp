#pragma once

#include <ruby.h>

#include <functional>

namespace rb {
  using MethodWithoutArgumentsRaw = VALUE (VALUE self);
  using MethodWithArgumentsRaw = VALUE (int argc, VALUE *argv, VALUE self);
  using MethodWithoutArguments = std::function<VALUE (VALUE self)>;
  using MethodWithArguments =
    std::function<VALUE (VALUE self, int argc, VALUE *argv)>;
  using MethodWithArgumentsCompatible =
    std::function<VALUE (int argc, VALUE *argv, VALUE self)>;

  class Class {
  public:
    Class(const char *name, VALUE parent);
    Class(VALUE klass);
    ~Class();

    Class &define_method_raw(const char *name,
                             MethodWithoutArgumentsRaw *body);
    Class &define_method_raw(const char *name,
                             MethodWithArgumentsRaw *body);
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
