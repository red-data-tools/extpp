#pragma once

#include <ruby.h>

namespace rb {
  class Object {
  public:
    Object(VALUE rb_object=Qnil) :
      rb_object_(rb_object) {
      rb_gc_register_address(&rb_object_);
    }

    Object(const char *name) :
      rb_object_(rb_const_get(rb_cObject, rb_intern(name))) {
      rb_gc_register_address(&rb_object_);
    }

    Object(Object const &object) :
      rb_object_(object) {
      rb_gc_register_address(&rb_object_);
    }

    virtual ~Object() {
      rb_gc_unregister_address(&rb_object_);
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

  private:
    VALUE rb_object_;
  };
}
