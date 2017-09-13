#pragma once

#include <ruby.h>

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

    template <typename... ARGUMENTS>
    inline Object send(ID name_id, ARGUMENTS... args) {
      VALUE rb_result = rb_funcall(rb_object_,
                                   name_id,
                                   sizeof...(args),
                                   args...);
      return Object(rb_result);
    }

    template <typename... ARGUMENTS>
    inline Object send(const char *name, ARGUMENTS... args) {
      return send(rb_intern(name), args...);
    }

    template <typename... ARGUMENTS>
    inline Object send(Object name, ARGUMENTS... args) {
      return send(rb_intern_str(name), args...);
    }

  private:
    VALUE rb_object_;
    bool is_gc_guarding_;
  };
}
