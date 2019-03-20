#pragma once

#include <ruby.h>

#define RB_BEGIN_DECLS extern "C" {
#define RB_END_DECLS }

#ifdef _WIN32
#  ifdef RB_EXTPP_COMPILATION
#    define RB_EXTPP_EXPORT __declspec(dllexport)
#  else
#    define RB_EXTPP_EXPORT __declspec(dllimport)
#  endif
#else
#  define RB_EXTPP_EXPORT __attribute__((visibility("default")))
#endif

#include <ruby/cast.hpp>
#include <ruby/class.hpp>
#include <ruby/object.hpp>
#include <ruby/protect.hpp>
