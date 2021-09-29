require "test-unit"

module Helper
  module Fixture
    def fixture_path(*components)
      File.join(__dir__, "fixtures", *components)
    end
  end
end
