class ClassTest < Test::Unit::TestCase
  def fixture_path(*components)
    File.join(__dir__, "fixtures", *components)
  end

  def test_define_method
    lib_dir = File.join(__dir__, "..", "lib")
    Dir.chdir(fixture_path("class")) do
      system(RbConfig.ruby, "-w", "-I", lib_dir, "extconf.rb")
      system("make")
    end
    require fixture_path("class", "class")
    assert_equal("Hello", Greeting.new.hello)
  end
end
