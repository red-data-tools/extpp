#pragma once

#include <ruby/object.hpp>

namespace rb {
  template <typename RETURN_TYPE, typename ARGUMENT_TYPE>
  inline RETURN_TYPE cast(ARGUMENT_TYPE object);

  template <typename RETURN_TYPE,
            typename ARGUMENT_TYPE,
            typename ADDTIONAL_DATA_TYPE1>
  inline RETURN_TYPE cast(ARGUMENT_TYPE object,
                          ADDTIONAL_DATA_TYPE1 data1);

  template <>
  inline int cast<int, Object>(Object rb_object) {
    return NUM2INT(rb_object);
  }

  template <>
  inline Object cast<Object, int>(int n) {
    return INT2NUM(n);
  }


  template <>
  inline uint64_t cast<uint64_t, Object>(Object rb_object) {
    return NUM2ULONG(rb_object);
  }

  template <>
  inline Object cast<Object, uint64_t>(uint64_t n) {
    return ULONG2NUM(n);
  }


  template <>
  inline const char *cast<const char *, Object>(Object rb_object) {
    VALUE rb_object_raw = rb_object;
    return StringValueCStr(rb_object_raw);
  }

  template <>
  inline Object cast<Object, const char *>(const char *c_string) {
    return rb_str_new_cstr(c_string);
  }

  template <>
  inline Object cast<Object, const char *, long>(const char *data, long size) {
    return rb_str_new(data, size);
  }
}
