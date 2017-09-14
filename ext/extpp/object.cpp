#include <ruby/object.hpp>

namespace {
  VALUE call_block(RB_BLOCK_CALL_FUNC_ARGLIST(rb_data, rb_block)) {
    auto block = reinterpret_cast<rb::MethodWithoutArguments>(rb_block);
    return block(rb_data);
  }
}

namespace rb {
  Object Object::send(ID name_id) {
    VALUE rb_result = rb_funcall(rb_object_, name_id, 0);
    return Object(rb_result);
  }

  Object Object::send(ID name_id,
                      std::initializer_list<VALUE> args) {
    size_t n = args.size();
    VALUE rb_args[n];
    int i = 0;
    for (auto arg : args) {
      rb_args[i++] = arg;
    }
    VALUE rb_result = rb_funcallv(rb_object_, name_id, n, rb_args);
    return Object(rb_result);
  }

  Object Object::send(ID name_id,
                      std::initializer_list<VALUE> args,
                      MethodWithoutArguments block) {
    size_t n = args.size();
    VALUE rb_args[n];
    int i = 0;
    for (auto arg : args) {
      rb_args[i++] = arg;
    }
    VALUE rb_result = rb_block_call(rb_object_,
                                    name_id,
                                    n,
                                    rb_args,
                                    reinterpret_cast<MethodFunc>(call_block),
                                    reinterpret_cast<VALUE>(block));
    return Object(rb_result);
  }
}
