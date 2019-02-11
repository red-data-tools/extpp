class ProtectTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      return unless self == ProtectTest
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("protect")) do
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("protect", "protect")
    end
  end

  class Listener
    attr_reader :events
    def initialize
      @events = []
    end

    def notify(event)
      @events << event
    end
  end

  def test_protect
    listener = Listener.new
    assert_raise(RuntimeError.new("message")) do
      ProtectMethods.new.protect(listener) do
        raise "message"
      end
    end
    assert_equal([
                   "outer: constructed",
                   "inner: constructed",
                   "inner: destructed",
                   "outer: caught",
                 ],
                 listener.events)
  end
end
