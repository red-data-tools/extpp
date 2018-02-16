#include <ruby.hpp>

RB_BEGIN_DECLS

void
Init_hello(void)
{
  rb::Class klass("Hello");
  klass.define_method("initialize", [](VALUE rb_self, int argc, VALUE *argv) {
      VALUE rb_name;
      rb_scan_args(argc, argv, "1", &rb_name);
      rb::Object self(rb_self);
      self.ivar_set("@name", rb_name);
      return Qnil;
    });
  klass.define_method("greet", [](VALUE rb_self) {
      rb::Object self(rb_self);
      rb::Object prefix(rb_str_new_cstr("Hello "));
      auto message = prefix.send("+", {self.ivar_get("@name")});
      return static_cast<VALUE>(message);
    });
}

RB_END_DECLS
