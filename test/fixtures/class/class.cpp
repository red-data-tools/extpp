#include <ruby.hpp>

namespace {
  VALUE rb_hello(VALUE self) {
    return rb_str_new_cstr("Hello");
  }

  VALUE rb_named_hello(int argc, VALUE *argv, VALUE self) {
    VALUE rb_name;
    rb_scan_args(argc, argv, "10", &rb_name);
    return rb_str_plus(rb_str_new_cstr("Hello "), rb_name);
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
    define_method("hello_defined", rb_hello);

  rb::Class("NamedGreeting", rb_cObject).
    define_method("hello", [](VALUE self, int argc, VALUE *argv) {
        VALUE rb_name;
        rb_scan_args(argc, argv, "10", &rb_name);
        return rb_str_plus(rb_str_new_cstr("Hello "), rb_name);
      }).
    define_method("hello_compatible", [](int argc, VALUE *argv, VALUE self) {
        VALUE rb_name;
        rb_scan_args(argc, argv, "10", &rb_name);
        return rb_str_plus(rb_str_new_cstr("Hello "), rb_name);
      }).
    enable_lazy_define_method().
    define_method("hello_lazy", [](VALUE self, int argc, VALUE *argv) {
        VALUE rb_name;
        rb_scan_args(argc, argv, "10", &rb_name);
        return rb_str_plus(rb_str_new_cstr("Hello "), rb_name);
      }).
    define_method("hello_defined", rb_named_hello);
}
