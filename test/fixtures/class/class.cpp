#include <ruby.hpp>

extern "C" void
Init_class(void)
{
  rb::Class("Greeting", rb_cObject).
    define_method("hello", [](VALUE self, int argc, VALUE *argv) {
        return rb_str_new_cstr("Hello");
      });
}
