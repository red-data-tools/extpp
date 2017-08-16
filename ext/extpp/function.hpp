#include <ruby.h>

#include <functional>

namespace rb {
  class Function {
  public:
    Function() = default;
    virtual ~Function() = default;

    virtual VALUE call(VALUE self, int argc, VALUE *argv) = 0;
  };

  class FunctionNoArgument : public Function {
  public:
    FunctionNoArgument(const std::function<VALUE(VALUE self)> &function);

    VALUE call(VALUE self, int argc, VALUE *argv) override;

  private:
    std::function<VALUE(VALUE self)> function_;
  };

  class FunctionWithArguments : public Function {
  public:
    FunctionWithArguments(const std::function<VALUE(VALUE self, int argc, VALUE *argv)> &function);

    VALUE call(VALUE self, int argc, VALUE *argv) override;

  private:
    std::function<VALUE(VALUE self, int argc, VALUE *argv)> function_;
  };
}
