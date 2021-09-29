class ObjectTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      return unless self == ObjectTest
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("object")) do
        system("make", "distclean") if File.exist?("Makefile")
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

  sub_test_case("send") do
    def test_no_arguments
      assert_equal("1", ObjectMethods.new.send_no_arguments(1, "to_s"))
    end

    def test_two_arguments
      assert_equal("ell",
                   ObjectMethods.new.send_two_arguments("hello",
                                                        "[]",
                                                        1,
                                                        3))
    end

    def test_block
      assert_equal([1, 4, 9],
                   ObjectMethods.new.send_block([1, 2, 3], "collect"))
    end
  end

  sub_test_case("instance variable") do
    def test_set
      object = Object.new
      methods = ObjectMethods.new
      assert_equal("me", methods.ivar_set(object, "@name", "me"))
      assert_equal("me", object.instance_variable_get("@name"))
    end

    def test_get
      object = Object.new
      object.instance_variable_set("@name", "me")
      methods = ObjectMethods.new
      assert_equal("me", methods.ivar_get(object, "@name"))
    end
  end
end
