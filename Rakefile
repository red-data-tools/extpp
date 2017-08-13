# -*- ruby -*-

require "rubygems"
require "bundler/gem_helper"
require "packnga"

base_dir = File.join(File.dirname(__FILE__))

helper = Bundler::GemHelper.new(base_dir)
def helper.version_tag
  version
end

helper.install
spec = helper.gemspec

Packnga::DocumentTask.new(spec) do |task|
  task.original_language = "en"
  task.translate_language = "ja"
end

Packnga::ReleaseTask.new(spec) do
end

desc "Configure"
task :configure => "ext/extpp/Makefile" do
  cd("ext/extpp") do
    ruby("extconf.rb")
  end
end

namespace :configure do
  desc "Configure for debug"
  task :debug => "ext/extpp/Makefile" do
    cd("ext/extpp") do
      ruby("extconf.rb", "--enable-debug-build")
    end
  end
end

desc "Run tests"
task :test do
  ruby("test/run-test.rb")
end

task default: ["configure:debug", :test]
