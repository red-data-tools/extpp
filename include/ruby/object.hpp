#pragma once

#include <ruby/type.hpp>

#include <initializer_list>

namespace rb {
  namespace internal {
    inline VALUE call_block(RB_BLOCK_CALL_FUNC_ARGLIST(rb_data, rb_block)) {
      auto block = reinterpret_cast<rb::MethodWithoutArguments>(rb_block);
      return block(rb_data);
    }
  }

  class Object {
  public:
    explicit Object(VALUE rb_object=Qnil) :
      rb_object_(rb_object),
      is_gc_guarding_(false) {
    }

    explicit Object(const char *name) :
      rb_object_(rb_funcall(rb_cObject,
                            rb_intern("const_get"),
                            1,
                            rb_str_new_static(name, strlen(name)))),
      is_gc_guarding_(false) {
    }

    Object(Object const &object) :
      rb_object_(object),
      is_gc_guarding_(false) {
    }

    virtual ~Object() {
      if (is_gc_guarding_) {
        rb_gc_unregister_address(&rb_object_);
      }
    }

    inline explicit operator bool() const {
      return RTEST(rb_object_);
    }

    inline VALUE to_ruby() const {
      return rb_object_;
    }

    inline operator VALUE() const {
      return rb_object_;
    }

    inline bool is_nil() const {
      return NIL_P(rb_object_);
    }

    inline void guard_from_gc() {
      if (!is_gc_guarding_) {
        return;
      }

      is_gc_guarding_ = true;
      rb_gc_register_address(&rb_object_);
    }

    inline Object send(ID name_id) {
      VALUE rb_result = rb_funcall(rb_object_, name_id, 0);
      return Object(rb_result);
    }

    inline Object send(const char *name) {
      return send(rb_intern(name));
    }

    inline Object send(Object name) {
      return send(rb_intern_str(name));
    }

    inline Object send(ID name_id, std::initializer_list<VALUE> args) {
      auto n = args.size();
      VALUE rb_args[n];
      int i = 0;
      for (auto arg : args) {
        rb_args[i++] = arg;
      }
      VALUE rb_result = rb_funcallv(rb_object_,
                                    name_id,
                                    static_cast<int>(n),
                                    rb_args);
      return Object(rb_result);
    }

    inline Object send(const char *name, std::initializer_list<VALUE> args) {
      return send(rb_intern(name), args);
    }

    inline Object send(Object name, std::initializer_list<VALUE> args) {
      return send(rb_intern_str(name), args);
    }

    inline Object send(ID name_id,
                       std::initializer_list<VALUE> args,
                       MethodWithoutArguments block) {
      auto n = args.size();
      VALUE rb_args[n];
      int i = 0;
      for (auto arg : args) {
        rb_args[i++] = arg;
      }
      auto rb_result =
        rb_block_call(rb_object_,
                      name_id,
                      static_cast<int>(n),
                      rb_args,
                      reinterpret_cast<RawMethod>(internal::call_block),
                      reinterpret_cast<VALUE>(block));
      return Object(rb_result);
    }

    inline Object send(const char *name,
                       std::initializer_list<VALUE> args,
                       MethodWithoutArguments block) {
      return send(rb_intern(name), args, block);
    }

    inline Object send(Object name,
                       std::initializer_list<VALUE> args,
                       MethodWithoutArguments block) {
      return send(rb_intern_str(name), args, block);
    }

    inline Object ivar_set(ID name_id, VALUE value) {
      return Object(rb_ivar_set(rb_object_, name_id, value));
    }

    inline Object ivar_set(const char *name, VALUE value) {
      return ivar_set(rb_intern(name), value);
    }

    inline Object ivar_set(Object name, VALUE value) {
      return ivar_set(rb_intern_str(name), value);
    }

    inline Object ivar_get(ID name_id) {
      return Object(rb_ivar_get(rb_object_, name_id));
    }

    inline Object ivar_get(const char *name) {
      return ivar_get(rb_intern(name));
    }

    inline Object ivar_get(Object name) {
      return ivar_get(rb_intern_str(name));
    }

  private:
    VALUE rb_object_;
    bool is_gc_guarding_;
  };
}
