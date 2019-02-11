#pragma once

#include <ruby/type.hpp>

#include <functional>

namespace rb {
  class State {
  public:
    explicit State(int state) :
      state_(state) {
    }

    inline void jump() {
      rb_jump_tag(state_);
    }

  private:
    int state_;
  };

  VALUE protect(RawCallback callback, VALUE callback_data);
  VALUE protect(std::function<VALUE()> callback);
}
