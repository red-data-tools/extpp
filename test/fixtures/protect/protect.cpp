#include <ruby.hpp>

namespace {
  class Notifier {
  public:
    Notifier(std::string prefix, rb::Object listener) :
      prefix_(prefix),
      listener_(listener) {
      notify("constructed");
    }

    ~Notifier() {
      notify("destructed");
    }

    void notify(const char* name) {
      auto event = prefix_ + name;
      auto x = rb::Object(rb_str_new(event.data(), event.size()));
      listener_.send("notify", {x});
    }

  private:
    std::string prefix_;
    rb::Object listener_;
  };
};

extern "C" void
Init_protect(void)
{
  rb::Class klass("ProtectMethods");
  klass.define_method("protect_raw", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_listener;
      VALUE rb_block;
      rb_scan_args(argc, argv, "1&", &rb_listener, &rb_block);
      Notifier outer("outer: ", rb::Object(rb_listener));
      try {
        Notifier inner("inner: ", rb::Object(rb_listener));
        rb::protect(
          [](VALUE rb_block) -> VALUE {
            return rb::Object(rb_block).send("call");
          },
          rb_block);
        inner.notify("called");
      } catch (rb::State& state) {
        outer.notify("caught");
        state.jump();
      }
      outer.notify("jumped");
      return Qnil;
    });
  klass.define_method("protect_closure", [](int argc, VALUE *argv, VALUE self) {
      VALUE rb_listener;
      VALUE rb_block;
      rb_scan_args(argc, argv, "1&", &rb_listener, &rb_block);
      Notifier outer("outer: ", rb::Object(rb_listener));
      try {
        Notifier inner("inner: ", rb::Object(rb_listener));
        rb::protect([&rb_block]() -> VALUE {
                      return rb::Object(rb_block).send("call");
                    });
        inner.notify("called");
      } catch (rb::State& state) {
        outer.notify("caught");
        state.jump();
      }
      outer.notify("jumped");
      return Qnil;
    });
}
