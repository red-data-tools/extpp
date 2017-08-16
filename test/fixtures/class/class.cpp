#include <ruby.hpp>

namespace {
  VALUE rb_hello(VALUE self) {
    return rb_str_new_cstr("Hello");
  }
}

extern "C" void
Init_class(void)
{
  rb::Class("Greeting", rb_cObject).
    define_method("hello", [](VALUE self) {
        return rb_str_new_cstr("Hello");
      }).
    enable_lazy_define_method().
    define_method("hello_lazy", [](VALUE self) {
        return rb_str_new_cstr("Hello");
      }).
    define_method_raw("hello_raw", rb_hello);
}
