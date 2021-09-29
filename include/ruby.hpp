#pragma once

#include <ruby.h>

#define RB_BEGIN_DECLS extern "C" {
#define RB_END_DECLS }

#include <ruby/cast.hpp>
#include <ruby/class.hpp>
#include <ruby/object.hpp>
#include <ruby/protect.hpp>
