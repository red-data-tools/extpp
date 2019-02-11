#include <ruby/protect.hpp>

namespace {
  VALUE call_callback(VALUE user_data) {
    auto& callback = *reinterpret_cast<std::function<VALUE()> *>(user_data);
    return callback();
  }
}

namespace rb {
  VALUE protect(RawCallback callback, VALUE callback_data) {
    int state = 0;
    auto result = rb_protect(callback, callback_data, &state);
    if (state != 0) {
      throw State(state);
    }
    return result;
  }

  VALUE protect(std::function<VALUE()> callback) {
    auto callback_data = reinterpret_cast<VALUE>(&callback);
    int state = 0;
    auto result = rb_protect(call_callback, callback_data, &state);
    if (state != 0) {
      throw State(state);
    }
    return result;
  }
}
