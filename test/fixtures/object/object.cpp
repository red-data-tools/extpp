#include <ruby.hpp>

extern "C" void
Init_object(void)
{
  rb::Class("ObjectMethods").
    define_method("to_bool_true", [](VALUE self) {
        rb::Object object(Qtrue);
        if (object) {
          return Qtrue;
        } else {
          return Qfalse;
        }
      }).
    define_method("to_bool_false", [](VALUE self) {
        rb::Object object(Qfalse);
        if (object) {
          return Qtrue;
        } else {
          return Qfalse;
        }
      }).
    define_method("to_bool_nil", [](VALUE self) {
        rb::Object object(Qnil);
        if (object) {
          return Qtrue;
        } else {
          return Qfalse;
        }
      }).
    define_method("to_ruby", [](int argc, VALUE *argv, VALUE self) {
        VALUE rb_object;
        rb_scan_args(argc, argv, "1", &rb_object);
        rb::Object object(rb_object);
        return static_cast<VALUE>(object);
      });
}
