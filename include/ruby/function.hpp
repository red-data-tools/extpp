#pragma once

#include <ruby.hpp>

namespace rb {
  class Function {
  public:
    Function() = default;
    virtual ~Function() = default;

    virtual VALUE call(VALUE self, int argc, VALUE *argv) = 0;
  };

  class FunctionWithoutArgument : public Function {
  public:
    FunctionWithoutArgument(const MethodWithoutArguments &function) :
      function_(function) {
    }

    VALUE call(VALUE self, int argc, VALUE *argv) override {
      return function_(self);
    }

  private:
    MethodWithoutArguments function_;
  };

  class FunctionWithArguments : public Function {
  public:
    FunctionWithArguments(const MethodWithArguments &function) :
      function_(function) {
    }

    VALUE call(VALUE self, int argc, VALUE *argv) override {
      return function_(self, argc, argv);
    }

  private:
    MethodWithArguments function_;
  };

  class FunctionWithArgumentsCompatible : public Function {
  public:
    FunctionWithArgumentsCompatible(
      const MethodWithArgumentsCompatible &function) :
      function_(function) {
    }

    VALUE call(VALUE self, int argc, VALUE *argv) override {
      return function_(argc, argv, self);
    }

  private:
    MethodWithArgumentsCompatible function_;
  };
}
