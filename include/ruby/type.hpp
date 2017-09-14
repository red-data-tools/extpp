#pragma once

#include <ruby.h>

namespace rb {
  using MethodFunc = VALUE (*)(ANYARGS);

  using MethodWithoutArguments = VALUE (*)(VALUE self);
  using MethodWithArguments = VALUE (*)(VALUE self, int argc, VALUE *argv);
  using MethodWithArgumentsCompatible =
    VALUE (*)(int argc, VALUE *argv, VALUE self);
}
