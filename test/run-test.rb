#!/usr/bin/env ruby

$VERBOSE = true

require "pathname"

base_dir = Pathname.new(__dir__).parent.expand_path

ext_dir = base_dir + "ext" + "extpp"
lib_dir = base_dir + "lib"
test_dir = base_dir + "test"

Dir.chdir(ext_dir.to_s) do
  system("make > /dev/null")
end

$LOAD_PATH.unshift(lib_dir.to_s)

require_relative "helper"

exit(Test::Unit::AutoRunner.run(true, test_dir.to_s))
