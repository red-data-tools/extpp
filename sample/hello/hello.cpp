#include <ruby.hpp>

RB_BEGIN_DECLS

void
Init_hello(void)
{
  rb::Class klass("Hello");
  klass.define_method("initialize", [](VALUE self, int argc, VALUE *argv) {
      VALUE rb_name;
      rb_scan_args(argc, argv, "1", &rb_name);
      rb_ivar_set(self, rb_intern("@name"), rb_name);
      return Qnil;
    });
  klass.define_method("greet", [](VALUE self) {
      auto rb_name = rb_ivar_get(self, rb_intern("@name"));
      rb::Object message(rb_str_new_cstr("Hello "));
      return static_cast<VALUE>(message.send("+", {rb_name}));
    });
}

RB_END_DECLS
