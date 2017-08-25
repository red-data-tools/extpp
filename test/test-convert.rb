class ConvertTest < Test::Unit::TestCase
  extend Helper::Fixture

  class << self
    def startup
      lib_dir = File.join(__dir__, "..", "lib")
      Dir.chdir(fixture_path("convert")) do
        system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
        system("make")
      end
      require fixture_path("convert", "convert")
    end
  end

  def setup
    @converter = Converter.new
  end

  def test_number
    assert_equal(29, @converter.convert_number(29))
  end
end
