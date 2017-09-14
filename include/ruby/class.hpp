#pragma once

#include <ruby/object.hpp>

namespace rb {
  class Class: public Object {
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
