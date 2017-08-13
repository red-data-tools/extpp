require "benchmark"
require_relative "define_method"

n = 10000
Benchmark.bmbm do |job|
  job.report("Ruby") do
    Class.new do
      n.times do |i|
        define_method("method#{i}") do
        end
      end
    end
  end

  job.report("C") do
    DefineMethodC.new.define(n, Class.new)
  end

  job.report("C++") do
    DefineMethodCPP.new.define(n, Class.new)
  end
end
