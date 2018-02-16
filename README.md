# README

## Name

Ext++

## Description

Ext++ is a Ruby extension that provides C++ API for writing Ruby extension.

You can write your Ruby extension easier than Ruby's C API. Because
Ext++'s C++ API reduces duplicated code needed for Ruby's C API.

You can use all Ruby's C API without any adapter layer. Because you
can use C API directory from C++.

## Install

```console
% gem install extpp
```

## How to use

Here is an example to create `Hello` class.

Create `hello.cpp`:

```c++
#include <ruby.hpp>

RB_BEGIN_DECLS

void
Init_hello(void)
{
  rb::Class klass("Hello");
  klass.define_method("initialize", [](VALUE self, int argc, VALUE *argv) {
      VALUE rb_name;
      rb_scan_args(argc, argv, "1", &rb_name);
      rb_ivar_set(self, rb_intern("@name"), rb_name);
      return Qnil;
    });
  klass.define_method("greet", [](VALUE self) {
      auto rb_name = rb_ivar_get(self, rb_intern("@name"));
      rb::Object message(rb_str_new_cstr("Hello "));
      return static_cast<VALUE>(message.send("+", {rb_name}));
    });
}

RB_END_DECLS
```

This code equals to the following Ruby code:

```ruby
class Hello
  def initialize(name)
    @name = name
  end

  def greet
    "Hello " + @name
  end
end
```

Create `extconf.rb`:

```ruby
require "extpp"

create_makefile("hello")
```

Create `Makefile`:

```console
% ruby extconf.rb
```

Build this extension:

```console
% make
```

Now, you can use this extension:

```console
% ruby -r ./hello -e 'p Hello.new("me").greet'
"Hello me"
```

## License

Copyright (C) 2017-2018 Kouhei Sutou

The 2-Clause BSD License. See [LICENSE.txt](LICENSE.txt) for details.
