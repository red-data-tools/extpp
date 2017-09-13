#include <ruby.hpp>

extern "C" void
Init_cast(void)
{
  rb::Class("Caster").
    define_method("cast_number", [](VALUE self, int argc, VALUE *argv) -> VALUE {
        VALUE rb_n;
        rb_scan_args(argc, argv, "1", &rb_n);
        return rb::cast<rb::Object>(rb::cast<int>(rb::Object(rb_n)));
      }).
    define_method("cast_string", [](VALUE self, int argc, VALUE *argv) -> VALUE {
        VALUE rb_string;
        rb_scan_args(argc, argv, "1", &rb_string);
        return rb::cast<rb::Object>(rb::cast<const char *>(rb::Object(rb_string)));
      });
}
