# -*- ruby -*-

clean_white_space = lambda do |entry|
  entry.gsub(/(\A\n+|\n+\z)/, '') + "\n"
end

require_relative "lib/extpp/version"

Gem::Specification.new do |spec|
  spec.name = "extpp"
  spec.version = ExtPP::VERSION
  spec.homepage = "https://github.com/red-data-tools/extpp"
  spec.authors = ["Kouhei Sutou"]
  spec.email = ["kou@clear-code.com"]
  readme = File.read("README.md")
  readme.force_encoding("UTF-8")
  entries = readme.split(/^\#\#\s(.*)$/)
  description = clean_white_space.call(entries[entries.index("Description") + 1])
  spec.summary, spec.description, = description.split(/\n\n+/, 3)
  spec.licenses = ["BSD-2-Clause"]
  spec.files = ["README.md", "Rakefile"]
  spec.files += ["LICENSE.txt"]
  spec.files += Dir.glob("lib/**/*.rb")
  spec.files += Dir.glob("ext/**/*.cpp")
  spec.files += Dir.glob("include/**/*.hpp")
  spec.files += Dir.glob("doc/text/**/*.*")
  spec.extensions = ["ext/extpp/extconf.rb"]
  spec.test_files += Dir.glob("test/**/*")

  spec.add_development_dependency("bundler")
  spec.add_development_dependency("rake")
  spec.add_development_dependency("yard")
  spec.add_development_dependency("kramdown")
  spec.add_development_dependency("packnga")
  spec.add_development_dependency("test-unit")
end
