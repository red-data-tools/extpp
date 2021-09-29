# -*- ruby -*-

require "rubygems"
require "bundler/gem_helper"
require "rake/clean"

base_dir = File.join(File.dirname(__FILE__))

helper = Bundler::GemHelper.new(base_dir)
def helper.version_tag
  version
end

helper.install

desc "Run tests"
task :test do
  ruby("test/run-test.rb")
end

task default: :test
