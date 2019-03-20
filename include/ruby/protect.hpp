#pragma once

#include <ruby/type.hpp>

namespace rb {
  class RB_EXTPP_EXPORT State {
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

  VALUE protect(RawCallback callback, VALUE callback_data);

  template <typename CALLBACK>
  VALUE protect(const CALLBACK& callback) {
    struct Data {
      Data(const CALLBACK& callback) :
        callback_(callback) {
      }
      const CALLBACK& callback_;
    } data(callback);
    auto callback_data = reinterpret_cast<VALUE>(&data);
    return protect([](VALUE callback_data) -> VALUE {
                     auto data = reinterpret_cast<Data *>(callback_data);
                     return data->callback_();
                   },
                   callback_data);
  };
}
