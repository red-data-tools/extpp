#include <ruby.h>

#include <functional>

namespace rb {
  class Function {
  public:
    static Function *from_ruby(VALUE rb_function);

    Function(const std::function<VALUE(VALUE)> &function);

    VALUE to_ruby();
    VALUE call(VALUE self);

  private:
    std::function<VALUE(VALUE)> function_;
  };

  class Class {
  public:
    Class(const char *name, VALUE parent);

    Class &define_method(const char *name, std::function<VALUE(VALUE)> body);

  private:
    VALUE class_;
  };
}
