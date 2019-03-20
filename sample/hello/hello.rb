require "extpp/setup"
require "./hello.so"

puts Hello.new("me").greet
