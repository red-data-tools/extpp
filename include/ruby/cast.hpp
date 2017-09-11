#pragma once

#include <ruby/object.hpp>

namespace rb {
  template <typename RETURN_TYPE, typename ARGUMENT_TYPE>
  inline RETURN_TYPE cast(ARGUMENT_TYPE rb_object);

  template <>
  inline int cast<int, Object>(Object rb_object) {
    return NUM2INT(rb_object);
  }

  template <>
  inline uint64_t cast<uint64_t, Object>(Object rb_object) {
    return NUM2ULONG(rb_object);
  }


  template <>
  inline Object cast<Object, int>(int n) {
    return INT2NUM(n);
  }

  template <>
  inline Object cast<Object, uint64_t>(uint64_t n) {
    return ULONG2NUM(n);
  }
}
