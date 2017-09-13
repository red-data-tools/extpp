class CastTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("cast")) do
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("cast", "cast")
    end
  end

  def setup
    @caster = Caster.new
  end

  def test_number
    assert_equal(29, @caster.cast_number(29))
  end

  def test_string
    assert_equal("Hello", @caster.cast_string("Hello"))
  end
end
