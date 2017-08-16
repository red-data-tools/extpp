class ClassTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("class")) do
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("class", "class")
    end
  end

  def test_define_method
    assert_equal("Hello", Greeting.new.hello)
  end

  def test_define_method_lazy
    assert_equal("Hello", Greeting.new.hello_lazy)
  end

  def test_define_method_raw
    assert_equal("Hello", Greeting.new.hello_raw)
  end
end
