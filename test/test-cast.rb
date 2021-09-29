class CastTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("cast")) do
        system("make", "distclean") if File.exist?("Makefile")
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("cast", "cast")
    end
  end

  def setup
    @caster = Caster.new
  end

  def test_int32
    assert_equal(-(2 ** 31),
                 @caster.cast_int32(-(2 ** 31)))
  end

  def test_int64
    assert_equal(-(2 ** 63),
                 @caster.cast_int64(-(2 ** 63)))
  end

  def test_uint32
    assert_equal(2 ** 32 - 1,
                 @caster.cast_uint32(2 ** 32 - 1))
  end

  def test_uint64
    assert_equal(2 ** 64 - 1,
                 @caster.cast_uint64(2 ** 64 - 1))
  end

  def test_string
    assert_equal("Hello", @caster.cast_string("Hello"))
  end

  def test_std_string
    assert_equal("Hello", @caster.cast_std_string("Hello"))
  end
end
