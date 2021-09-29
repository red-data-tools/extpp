#pragma once

#include <ruby/type.hpp>

namespace rb {
  class State {
  public:
    explicit State(int state) :
      state_(state) {
    }

    [[noreturn]] inline void jump() {
      rb_jump_tag(state_);
    }

  private:
    int state_;
  };

  inline VALUE protect(RawCallback callback, VALUE callback_data) {
    int state = 0;
    auto result = rb_protect(callback, callback_data, &state);
    if (state != 0) {
      throw State(state);
    }
    return result;
  }

  template <typename NoArgumentCallback>
  VALUE protect(const NoArgumentCallback& callback) {
    struct Data {
      Data(const NoArgumentCallback& callback) :
        callback_(callback) {
      }
      const NoArgumentCallback& callback_;
    } data(callback);
    auto callback_data = reinterpret_cast<VALUE>(&data);
    return protect([](VALUE callback_data) -> VALUE {
                     auto data = reinterpret_cast<Data *>(callback_data);
                     return data->callback_();
                   },
                   callback_data);
  };
}
