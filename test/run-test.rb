#!/usr/bin/env ruby

$VERBOSE = true

require "pathname"

base_dir = Pathname.new(__dir__).parent.expand_path

ext_dir = base_dir + "ext" + "extpp"
lib_dir = base_dir + "lib"
test_dir = base_dir + "test"

make = nil
if system("which gmake > #{File::NULL} 2>&1")
  make = "gmake"
elsif system("which make > #{File::NULL} 2>&1")
  make = "make"
end
if make
  Dir.chdir(ext_dir.to_s) do
    if File.exist?("Makefile")
      system("#{make} > #{File::NULL}") or exit(false)
    end
  end
end

$LOAD_PATH.unshift(lib_dir.to_s)

require_relative "helper"

exit(Test::Unit::AutoRunner.run(true, test_dir.to_s))
