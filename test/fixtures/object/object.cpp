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
      }).
    define_method("send_no_arguments", [](int argc, VALUE *argv, VALUE self) {
        VALUE rb_receiver;
        VALUE rb_method_name;
        rb_scan_args(argc, argv, "2", &rb_receiver, &rb_method_name);
        rb::Object receiver(rb_receiver);
        rb::Object method_name(rb_method_name);
        auto result = receiver.send(method_name);
        return static_cast<VALUE>(result);
      }).
    define_method("send_two_arguments", [](int argc, VALUE *argv, VALUE self) {
        VALUE rb_receiver;
        VALUE rb_method_name;
        VALUE rb_arg1;
        VALUE rb_arg2;
        rb_scan_args(argc, argv, "4",
                     &rb_receiver,
                     &rb_method_name,
                     &rb_arg1,
                     &rb_arg2);
        rb::Object receiver(rb_receiver);
        rb::Object method_name(rb_method_name);
        auto result = receiver.send(method_name, rb_arg1, rb_arg2);
        return static_cast<VALUE>(result);
      });
}
