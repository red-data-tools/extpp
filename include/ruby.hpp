#include <ruby.h>

#include <functional>

#define RB_BEGIN_DECLS extern "C" {
#define RB_END_DECLS }

namespace rb {
  class Function {
  public:
    static Function *from_ruby(VALUE rb_function);

    Function(const std::function<VALUE(VALUE self, int argc, VALUE *argv)> &function);

    VALUE to_ruby();
    VALUE call(VALUE self, int arc, VALUE *argv);

  private:
    std::function<VALUE(VALUE self, int argc, VALUE *argv)> function_;
  };

  class Class {
  public:
    Class(const char *name, VALUE parent);
    Class(VALUE klass);

    Class &define_method(const char *name,
                         std::function<VALUE(VALUE, int, VALUE *)> body);

  private:
    VALUE class_;
  };
}
