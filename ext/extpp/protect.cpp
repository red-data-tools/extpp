#include <ruby.hpp>

namespace rb {
  VALUE protect(RawCallback callback, VALUE callback_data) {
    int state = 0;
    auto result = rb_protect(callback, callback_data, &state);
    if (state != 0) {
      throw State(state);
    }
    return result;
  }
}
