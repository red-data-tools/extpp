#include "function.hpp"

namespace rb {
  FunctionNoArgument::FunctionNoArgument(const MethodWithoutArguments &function) :
    function_(function) {
  }

  VALUE FunctionNoArgument::call(VALUE self, int argc, VALUE *argv) {
    return function_(self);
  };


  FunctionWithArguments::FunctionWithArguments(const MethodWithArguments &function) :
    function_(function) {
  }

  VALUE FunctionWithArguments::call(VALUE self, int argc, VALUE *argv) {
    return function_(self, argc, argv);
  };

  FunctionWithArgumentsCompatible::FunctionWithArgumentsCompatible(const MethodWithArgumentsCompatible &function) :
    function_(function) {
  }

  VALUE FunctionWithArgumentsCompatible::call(VALUE self, int argc, VALUE *argv) {
    return function_(argc, argv, self);
  };
}
