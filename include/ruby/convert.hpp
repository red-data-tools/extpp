#pragma once

#include <ruby.h>

namespace rb {
  template <typename RETURN_TYPE>
  inline RETURN_TYPE from_ruby(VALUE rb_object);

  template <>
  inline int from_ruby<int>(VALUE rb_object) {
    return NUM2INT(rb_object);
  }

  template <>
  inline uint64_t from_ruby<uint64_t>(VALUE rb_object) {
    return NUM2ULONG(rb_object);
  }

  template <typename ARGUMENT_TYPE>
  inline VALUE to_ruby(ARGUMENT_TYPE n);

  template <>
  inline VALUE to_ruby<int>(int n) {
    return INT2NUM(n);
  }

  template <>
  inline VALUE to_ruby<uint64_t>(uint64_t n) {
    return ULONG2NUM(n);
  }
}
