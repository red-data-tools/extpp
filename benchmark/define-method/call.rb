#!/usr/bin/env ruby

require "benchmark"
require_relative "define_method"

N = 10000

class RubyMethods
  N.times do |i|
    define_method("method#{i}") do
    end
  end
end

class CMethods
  DefineMethodC.new.define(N, self)
end

class CppMethods
  DefineMethodCPP.new.define(N, self)
end

class CppLazyMethods
  DefineMethodCPP.new.define_lazy(N, self)
end

class CppDefinedMethods
  DefineMethodCPP.new.define_defined(N, self)
end

Benchmark.bmbm do |job|
  ruby_object = RubyMethods.new
  job.report("Ruby") do
    N.times do |i|
      ruby_object.__send__("method#{i}")
    end
  end

  c_object = CMethods.new
  job.report("C") do
    N.times do |i|
      c_object.__send__("method#{i}")
    end
  end

  cpp_object = CppMethods.new
  job.report("C++") do
    N.times do |i|
      cpp_object.__send__("method#{i}")
    end
  end

  cpp_lazy_object = CppLazyMethods.new
  job.report("C++ (lazy)") do
    N.times do |i|
      cpp_lazy_object.__send__("method#{i}")
    end
  end

  cpp_defined_object = CppDefinedMethods.new
  job.report("C++ (defined)") do
    N.times do |i|
      cpp_raw_object.__send__("method#{i}")
    end
  end
end
