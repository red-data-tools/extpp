#include <ruby.hpp>

namespace {
  VALUE call_func(int args, VALUE *argv, VALUE self) {
    auto method_id = rb_sym2id(rb_funcall(self, rb_intern("__method__"), 0));
    auto rb_function = rb_ivar_get(rb_obj_class(self), method_id);
    rb::Function *function = rb::Function::from_ruby(rb_function);
    return function->call(self);
  };
}

namespace rb {
  typedef VALUE (*METHOD_FUNC)(ANYARGS);

  Class::Class(const char *name, VALUE parent) {
    class_ = rb_define_class(name, parent);
  }

  Class &Class::define_method(const char *name,
                              std::function<VALUE(VALUE)> body) {
    Function *function = new Function(body);
    VALUE rb_function = function->to_ruby();
    rb_iv_set(class_, name, rb_function);
    rb_define_method(class_, name, reinterpret_cast<METHOD_FUNC>(call_func), -1);
    return (Class &)*this;
  }
}
