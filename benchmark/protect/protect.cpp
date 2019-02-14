#include <ruby.hpp>

RB_BEGIN_DECLS

static VALUE
rb_do_nothing(VALUE user_data)
{
  return Qnil;
}

static VALUE
rb_protect_nothing(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  for (int i = 0; i < n; ++i) {
    rb_do_nothing(Qnil);
  }
  return Qnil;
}

static VALUE
rb_protect_nothing_closure(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  VALUE user_data = Qnil;
  for (int i = 0; i < n; ++i) {
    ([&]() -> VALUE {
       return rb_do_nothing(user_data);
     })();
  }
  return Qnil;
}

static VALUE
rb_protect_c(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  for (int i = 0; i < n; ++i) {
    int state = 0;
    rb_protect(rb_do_nothing, Qnil, &state);
    if (state != 0) {
      rb_jump_tag(state);
    }
  }
  return Qnil;
}

static VALUE
rb_protect_cpp(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  try {
    for (int i = 0; i < n; ++i) {
      rb::protect(rb_do_nothing, Qnil);
    }
  } catch (rb::State& state) {
    state.jump();
  }
  return Qnil;
}

static VALUE
rb_protect_cpp_wrap(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  try {
    for (int i = 0; i < n; ++i) {
      rb::protect([](VALUE user_data) -> VALUE {
                    return rb_do_nothing(user_data);
                  },
                  Qnil);
    }
  } catch (rb::State& state) {
    state.jump();
  }
  return Qnil;
}

static VALUE
rb_protect_cpp_closure(VALUE self, VALUE rb_n)
{
  auto n = NUM2INT(rb_n);
  try {
    VALUE user_data = Qnil;
    for (int i = 0; i < n; ++i) {
      rb::protect([&]() -> VALUE {
                    return rb_do_nothing(user_data);
                  });
    }
  } catch (rb::State& state) {
    state.jump();
  }
  return Qnil;
}

void
Init_protect(void)
{
  auto klass = rb_define_class("Protect", rb_cObject);
  rb_define_method(klass,
                   "protect_nothing",
                   reinterpret_cast<rb::RawMethod>(rb_protect_nothing),
                   1);
  rb_define_method(klass,
                   "protect_nothing_closure",
                   reinterpret_cast<rb::RawMethod>(rb_protect_nothing),
                   1);
  rb_define_method(klass,
                   "protect_c",
                   reinterpret_cast<rb::RawMethod>(rb_protect_c),
                   1);
  rb_define_method(klass,
                   "protect_cpp",
                   reinterpret_cast<rb::RawMethod>(rb_protect_cpp),
                   1);
  rb_define_method(klass,
                   "protect_cpp_wrap",
                   reinterpret_cast<rb::RawMethod>(rb_protect_cpp_wrap),
                   1);
  rb_define_method(klass,
                   "protect_cpp_closure",
                   reinterpret_cast<rb::RawMethod>(rb_protect_cpp_closure),
                   1);
}

RB_END_DECLS
