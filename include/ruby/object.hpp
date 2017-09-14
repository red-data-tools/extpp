#pragma once

#include <ruby/type.hpp>

#include <initializer_list>

namespace rb {
  class Object {
  public:
    Object(VALUE rb_object=Qnil) :
      rb_object_(rb_object),
      is_gc_guarding_(false) {
    }

    Object(const char *name) :
      rb_object_(rb_const_get(rb_cObject, rb_intern(name))),
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

    inline operator bool() const {
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

    Object send(ID name_id);

    inline Object send(const char *name) {
      return send(rb_intern(name));
    }

    inline Object send(Object name) {
      return send(rb_intern_str(name));
    }

    Object send(ID name_id, std::initializer_list<VALUE> args);

    inline Object send(const char *name, std::initializer_list<VALUE> args) {
      return send(rb_intern(name), args);
    }

    inline Object send(Object name, std::initializer_list<VALUE> args) {
      return send(rb_intern_str(name), args);
    }

    Object send(ID name_id,
                std::initializer_list<VALUE> args,
                MethodWithoutArguments block);

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

  private:
    VALUE rb_object_;
    bool is_gc_guarding_;
  };
}
