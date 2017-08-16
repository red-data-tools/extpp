#include "function.hpp"

namespace rb {
  FunctionNoArgument::FunctionNoArgument(const std::function<VALUE(VALUE)> &function) :
    function_(function) {
  }

  VALUE FunctionNoArgument::call(VALUE self, int argc, VALUE *argv) {
    return function_(self);
  };


  FunctionWithArguments::FunctionWithArguments(const std::function<VALUE(VALUE, int argc, VALUE *argv)> &function) :
    function_(function) {
  }

  VALUE FunctionWithArguments::call(VALUE self, int argc, VALUE *argv) {
    return function_(self, argc, argv);
  };
}
