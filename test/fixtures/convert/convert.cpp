#include <ruby.hpp>

extern "C" void
Init_convert(void)
{
  rb::Class("Converter", rb_cObject).
    define_method("convert_number", [](VALUE self, int argc, VALUE *argv) {
        VALUE rb_n;
        rb_scan_args(argc, argv, "1", &rb_n);
        return rb::to_ruby(rb::from_ruby<int>(rb_n));
      });
}
