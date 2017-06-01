#include <ruby.h>

#include <functional>

extern "C" typedef VALUE (*RUBYPP_METHOD_FUNC)(ANYARGS);

namespace rubypp {
  class MethodPP {
  public:
    MethodPP(const std::function<VALUE(VALUE)> &function) :
      function_(function) {
    }

    VALUE call(VALUE self) {
      return function_(self);
    };
  private:
    std::function<VALUE(VALUE)> function_;
  };
}

static void free_methodpp(void *data) {
  auto methodpp = static_cast<rubypp::MethodPP *>(data);
  delete methodpp;
}

static VALUE rb_cMethodPP;
static const rb_data_type_t rb_cMethodPP_type = {
  "MethodPP",
  {nullptr, free_methodpp, nullptr,},
  nullptr,
  nullptr,RUBY_TYPED_FREE_IMMEDIATELY,
};

namespace rubypp {
  VALUE call_func(int args, VALUE *argv, VALUE self) {
    auto method_id = rb_sym2id(rb_funcall(self, rb_intern("__method__"), 0));
    auto method = rb_ivar_get(rb_obj_class(self), method_id);
    rubypp::MethodPP *methodpp;
    TypedData_Get_Struct(method, rubypp::MethodPP, &rb_cMethodPP_type, methodpp);
    return methodpp->call(self);
  };

  class Class {
  public:
    Class(const char *name, VALUE parent) {
      class_ = rb_define_class(name, parent);
    }

    Class &
    define_method(const char *name,
                  std::function<VALUE(VALUE)> body) {
      MethodPP *methodpp = new MethodPP(body);
      VALUE method =
        TypedData_Wrap_Struct(rb_cMethodPP,
                              &rb_cMethodPP_type,
                              methodpp);
      rb_iv_set(class_, name, method);
      rb_define_method(class_, name, (RUBYPP_METHOD_FUNC)call_func, -1);
      return (Class &)*this;
    }

  private:
    VALUE class_;
  };
}

extern "C" void
Init_a(void)
{
  rb_cMethodPP = rb_define_class("MethodPP", rb_cData);
  rubypp::Class("XXX", rb_cObject).
    define_method("hello", [](VALUE self) {return rb_str_new_cstr("Hello");}).
    define_method("xxx", [](VALUE self) {rb_p(self); return self;});
}
