#include <ruby.hpp>

namespace {
  VALUE FunctionPP = Qnil;

  void free_function(void *data) {
    auto function = static_cast<rb::Function *>(data);
    delete function;
  }

  const rb_data_type_t FunctionPP_type = {
    "FunctionPP",
    {nullptr, free_function, nullptr,},
    nullptr,
    nullptr,
    RUBY_TYPED_FREE_IMMEDIATELY,
  };
}

namespace rb {
  Function *Function::from_ruby(VALUE rb_function) {
    Function *function;
    TypedData_Get_Struct(rb_function, rb::Function, &FunctionPP_type, function);
    return function;
  }

  Function::Function(const std::function<VALUE(VALUE, int, VALUE *)> &function) :
    function_(function) {
  }

  VALUE Function::to_ruby() {
    if (NIL_P(FunctionPP)) {
      FunctionPP = rb_define_class("FunctionPP", rb_cData);
    }
    return TypedData_Wrap_Struct(FunctionPP, &FunctionPP_type, this);
  }

  VALUE Function::call(VALUE self, int argc, VALUE *argv) {
    return function_(self, argc, argv);
  };
}
