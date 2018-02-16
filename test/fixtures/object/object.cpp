#include <ruby.hpp>

extern "C" void
Init_object(void)
{
  rb::Class klass("ObjectMethods");
  klass.define_method("to_bool_true", [](VALUE self) {
      rb::Object object(Qtrue);
      if (object) {
        return Qtrue;
      } else {
        return Qfalse;
      }
    });
  klass.define_method("to_bool_false", [](VALUE self) {
      rb::Object object(Qfalse);
      if (object) {
        return Qtrue;
      } else {
        return Qfalse;
      }
    });
  klass.define_method("to_bool_nil", [](VALUE self) {
      rb::Object object(Qnil);
      if (object) {
        return Qtrue;
      } else {
        return Qfalse;
      }
    });
  klass.define_method("to_ruby", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_object;
      rb_scan_args(argc, argv, "1", &rb_object);
      rb::Object object(rb_object);
      return static_cast<VALUE>(object);
    });
  klass.define_method("send_no_arguments", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_receiver;
      VALUE rb_method_name;
      rb_scan_args(argc, argv, "2", &rb_receiver, &rb_method_name);
      rb::Object receiver(rb_receiver);
      rb::Object method_name(rb_method_name);
      auto result = receiver.send(method_name);
      return static_cast<VALUE>(result);
    });
  klass.define_method("send_two_arguments", [](int argc, VALUE *argv, VALUE self) {
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
      auto result = receiver.send(method_name, {rb_arg1, rb_arg2});
      return static_cast<VALUE>(result);
    });
  klass.define_method("send_block", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_receiver;
      VALUE rb_method_name;
      rb_scan_args(argc, argv, "2",
                   &rb_receiver,
                   &rb_method_name);
      rb::Object receiver(rb_receiver);
      rb::Object method_name(rb_method_name);
      auto result = receiver.send(method_name,
                                  {},
                                  [](VALUE rb_n) -> VALUE {
                                    auto n = rb::Object(rb_n);
                                    auto n_raw = rb::cast<int32_t>(n);
                                    return rb::cast<rb::Object>(n_raw * n_raw);
                                  });
      return static_cast<VALUE>(result);
    });
  klass.define_method("ivar_set", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_receiver;
      VALUE rb_name;
      VALUE rb_value;
      rb_scan_args(argc, argv, "3",
                   &rb_receiver,
                   &rb_name,
                   &rb_value);
      rb::Object receiver(rb_receiver);
      rb::Object name(rb_name);
      auto result = receiver.ivar_set(name, rb_value);
      return static_cast<VALUE>(result);
    });
  klass.define_method("ivar_get", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_receiver;
      VALUE rb_name;
      rb_scan_args(argc, argv, "2",
                   &rb_receiver,
                   &rb_name);
      rb::Object receiver(rb_receiver);
      rb::Object name(rb_name);
      auto result = receiver.ivar_get(name);
      return static_cast<VALUE>(result);
    });
}
