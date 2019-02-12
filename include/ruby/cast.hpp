#pragma once

#include <string>

#include <ruby/object.hpp>

namespace rb {
  template <typename RETURN_TYPE, typename ARGUMENT_TYPE>
  inline RETURN_TYPE cast(const ARGUMENT_TYPE& object);

  template <typename RETURN_TYPE, typename ARGUMENT_TYPE>
  inline RETURN_TYPE cast(const ARGUMENT_TYPE *object);

  template <typename RETURN_TYPE,
            typename ARGUMENT_TYPE,
            typename ADDTIONAL_DATA_TYPE1>
  inline RETURN_TYPE cast(const ARGUMENT_TYPE& object,
                          ADDTIONAL_DATA_TYPE1 data1);

  template <typename RETURN_TYPE,
            typename ARGUMENT_TYPE,
            typename ADDTIONAL_DATA_TYPE1>
  inline RETURN_TYPE cast(const ARGUMENT_TYPE *object,
                          ADDTIONAL_DATA_TYPE1 data1);

  template <>
  inline int32_t cast<int32_t, Object>(const Object& rb_object) {
    return NUM2INT(rb_object);
  }

  template <>
  inline Object cast<Object, int32_t>(const int32_t& n) {
    return Object(INT2NUM(n));
  }


  template <>
  inline int64_t cast<int64_t, Object>(const Object& rb_object) {
    return NUM2LONG(rb_object);
  }

  template <>
  inline Object cast<Object, int64_t>(const int64_t& n) {
    return Object(LONG2NUM(n));
  }


  template <>
  inline uint32_t cast<uint32_t, Object>(const Object& rb_object) {
    return NUM2UINT(rb_object);
  }

  template <>
  inline Object cast<Object, uint32_t>(const uint32_t& n) {
    return Object(UINT2NUM(n));
  }


  template <>
  inline uint64_t cast<uint64_t, Object>(const Object& rb_object) {
    return NUM2ULONG(rb_object);
  }

  template <>
  inline Object cast<Object, uint64_t>(const uint64_t& n) {
    return Object(ULONG2NUM(n));
  }


  template <>
  inline const char *cast<const char *, Object>(const Object& rb_object) {
    VALUE rb_object_raw = rb_object;
    return StringValueCStr(rb_object_raw);
  }

  template <>
  inline Object cast<Object, char>(const char *c_string) {
    return Object(rb_str_new_cstr(c_string));
  }

  template <>
  inline Object cast<Object, char, long>(const char *data, long size) {
    return Object(rb_str_new(data, size));
  }


  template <>
  inline std::string cast<std::string, Object>(const Object& rb_object) {
    VALUE rb_object_raw = rb_object;
    return std::string(RSTRING_PTR(rb_object_raw),
                       RSTRING_LEN(rb_object_raw));
  }

  template <>
  inline Object cast<Object, std::string>(const std::string& string) {
    return Object(rb_str_new(string.data(), string.size()));
  }
}
