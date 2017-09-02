class ObjectTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      return unless self == ObjectTest
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("object")) do
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("object", "object")
    end
  end

  sub_test_case("to_bool") do
    def test_true
      assert_equal(true, ObjectMethods.new.to_bool_true)
    end

    def test_false
      assert_equal(false, ObjectMethods.new.to_bool_false)
    end

    def test_nil
      assert_equal(false, ObjectMethods.new.to_bool_nil)
    end
  end

  def test_to_ruby
    assert_equal("Hello", ObjectMethods.new.to_ruby("Hello"))
  end
end
