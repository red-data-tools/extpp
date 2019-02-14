#!/usr/bin/env ruby

require "benchmark"
require_relative "protect"

GC.disable

N = 100000

Benchmark.bmbm do |job|
  protect = Protect.new

  job.report("Nothing") do
    protect.protect_nothing(N)
  end

  job.report("Nothing (closure)") do
    protect.protect_nothing(N)
  end

  job.report("C") do
    protect.protect_c(N)
  end

  job.report("C++") do
    protect.protect_cpp(N)
  end

  job.report("C++ (wrap)") do
    protect.protect_cpp_wrap(N)
  end

  job.report("C++ (closure)") do
    protect.protect_cpp_closure(N)
  end
end
