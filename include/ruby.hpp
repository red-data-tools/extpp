#include <ruby.h>

#include <functional>

#define RB_BEGIN_DECLS extern "C" {
#define RB_END_DECLS }

namespace rb {
  class Class {
  public:
    Class(const char *name, VALUE parent);
    Class(VALUE klass);
    ~Class();

    Class &define_method_raw(const char *name,
                             VALUE (*body)(VALUE self));
    Class &define_method_raw(const char *name,
                             VALUE (*body)(int argc, VALUE *argv, VALUE self));
    Class &define_method(const char *name,
                         std::function<VALUE(VALUE)> body);
    Class &define_method(const char *name,
                         std::function<VALUE(VALUE, int, VALUE *)> body);

    Class &enable_lazy_define_method();

  private:
    class ClassImpl;
    ClassImpl *impl_;
  };
}
