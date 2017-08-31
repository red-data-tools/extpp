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
    FunctionWithoutArgument(const MethodWithoutArguments &function);

    VALUE call(VALUE self, int argc, VALUE *argv) override;

  private:
    MethodWithoutArguments function_;
  };

  class FunctionWithArguments : public Function {
  public:
    FunctionWithArguments(const MethodWithArguments &function);

    VALUE call(VALUE self, int argc, VALUE *argv) override;

  private:
    MethodWithArguments function_;
  };

  class FunctionWithArgumentsCompatible : public Function {
  public:
    FunctionWithArgumentsCompatible(const MethodWithArgumentsCompatible &function);

    VALUE call(VALUE self, int argc, VALUE *argv) override;

  private:
    MethodWithArgumentsCompatible function_;
  };
}
