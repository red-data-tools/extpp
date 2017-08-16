#include <ruby.hpp>

RB_BEGIN_DECLS

static VALUE
rb_method(VALUE self)
{
}

static VALUE
rb_define(VALUE self, VALUE rb_n, VALUE rb_klass)
{
  auto n = NUM2INT(rb_n);
  for (int i = 0; i < n; ++i) {
    char method_name[256];
    snprintf(method_name, sizeof(method_name), "method%d", i);
    rb_define_method(rb_klass,
                     method_name,
                     reinterpret_cast<VALUE(*)(ANYARGS)>(rb_method),
                     0);
  }
  return Qnil;
}

void
Init_define_method(void)
{
  {
    auto klass = rb_define_class("DefineMethodC", rb_cObject);
    rb_define_method(klass,
                     "define",
                     reinterpret_cast<VALUE(*)(ANYARGS)>(rb_define),
                     2);
  }

  {
    rb::Class("DefineMethodCPP", rb_cObject).
      define_method("define", [](VALUE self, int argc, VALUE *argv) {
          VALUE rb_n;
          VALUE rb_klass;
          rb_scan_args(argc, argv, "2", &rb_n, &rb_klass);
          auto n = NUM2INT(rb_n);
          rb::Class klass(rb_klass);
          for (int i = 0; i < n; ++i) {
            char method_name[256];
            snprintf(method_name, sizeof(method_name), "method%d", i);
            klass.define_method(method_name, [](VALUE self) {
                return Qnil;
              });
          }
          return Qnil;
        }).
      define_method("define_lazy", [](VALUE self, int argc, VALUE *argv) {
          VALUE rb_n;
          VALUE rb_klass;
          rb_scan_args(argc, argv, "2", &rb_n, &rb_klass);
          auto n = NUM2INT(rb_n);
          rb::Class klass(rb_klass);
          klass.enable_lazy_define_method();
          for (int i = 0; i < n; ++i) {
            char method_name[256];
            snprintf(method_name, sizeof(method_name), "method%d", i);
            klass.define_method(method_name, [](VALUE self) {
                return Qnil;
              });
          }
          return Qnil;
        });
  }
}

RB_END_DECLS
