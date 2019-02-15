# -*- ruby -*-

require "rubygems"
require "bundler/gem_helper"

base_dir = File.join(File.dirname(__FILE__))

helper = Bundler::GemHelper.new(base_dir)
def helper.version_tag
  version
end

helper.install

def run_extconf(*arguments)
  cd("ext/extpp") do
    ruby("extconf.rb", *arguments)
  end
end

desc "Configure"
task :configure do
  run_extconf
end

namespace :configure do
  desc "Configure for debug"
  task :debug do
    run_extconf("--enable-debug-build")
  end
end

desc "Run tests"
task :test do
  ruby("test/run-test.rb")
end

task default: ["configure:debug", :test]
