#pragma once

#include <ruby.h>

namespace rb {
  template <typename RETURN_TYPE>
  RETURN_TYPE from_ruby(VALUE &rb_object);

  template <>
  int from_ruby(VALUE &rb_object) {
    return NUM2INT(rb_object);
  }

  template <>
  uint64_t from_ruby(VALUE &rb_object) {
    return NUM2ULONG(rb_object);
  }

  template <typename ARGUMENT_TYPE>
  VALUE to_ruby(ARGUMENT_TYPE &object);

  template <>
  VALUE to_ruby(uint64_t &n) {
    return ULONG2NUM(n);
  }
}
