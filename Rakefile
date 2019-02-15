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

def build_dir
  "ext/extpp"
end

def makefile
  "#{build_dir}/Makefile"
end

def run_extconf(*arguments)
  cd(build_dir) do
    ruby("extconf.rb", *arguments)
  end
end

desc "Configure"
task :configure => makefile do
  run_extconf
end

namespace :configure do
  desc "Configure for debug"
  task :debug => makefile do
    run_extconf("--enable-debug-build")
  end
end

task :clean do
  if File.exists?(makefile)
    cd(File.dirname(makefile)) do
      sh("make", "clean")
    end
  end
end

desc "Run tests"
task :test do
  ruby("test/run-test.rb")
end

task default: ["configure:debug", :test]
