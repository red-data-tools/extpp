require_relative "../../lib/extpp/compiler"

cxxflags = RbConfig::CONFIG["CXXFLAGS"]
compiler = Extpp::Compiler.new(cxxflags)
compiler.check
cxxflags = compiler.cxx_flags

sources = Dir.chdir(__dir__) do
  Dir.glob("*.cpp").collect do |cpp_source|
    File.join(__dir__, cpp_source)
  end
end
objects = sources.collect do |source|
  source.gsub(/\.cpp\z/, ".o")
end

include_dir = File.expand_path(File.join(__dir__, "..", "..", "include"))
headers = Dir.chdir(__dir__) do
  Dir.glob("**/*.hpp").collect do |header|
    File.join(include_dir, header)
  end
end

File.open("Makefile", "w") do |makefile|
  makefile.puts(<<-MAKEFILE)
LIBRARY = libruby-extpp.#{RbConfig::CONFIG["DLEXT"]}

SOURCES = #{sources.collect(&:quote).join(" ")}
OBJECTS = #{objects.collect(&:quote).join(" ")}
HEADERS = #{headers.collect(&:quote).join(" ")}

INCLUDE_DIR = #{include_dir.quote}

CXX = #{RbConfig::CONFIG["CXX"].quote}

RUBY = #{RbConfig.ruby.quote}
RUBY_HEADER_DIR = #{RbConfig::CONFIG["rubyhdrdir"].quote}
RUBY_ARCH_HEADER_DIR = #{RbConfig::CONFIG["rubyarchhdrdir"].quote}
LDSHAREDXX = #{RbConfig::CONFIG["LDSHAREDXX"]}
CCDLFLAGS = #{RbConfig::CONFIG["CCDLFLAGS"]}

INCLUDEFLAGS = \
	-I$(INCLUDE_DIR) \
	-I$(RUBY_HEADER_DIR) \
	-I$(RUBY_ARCH_HEADER_DIR)
CPPFLAGS = #{RbConfig::CONFIG["CPPFLAGS"]}
CXXFLAGS = $(CCDLFLAGS) #{cxxflags}

all: $(LIBRARY)

clean:
	rm -rf $(OBJECTS) $(LIBRARY)

dist-clean:
	$(MAKE) clean
	rm -rf Makefile

install: $(LIBRARY)
	"$(RUBY)" -run -e install -- $(LIBRARY) $(DESTDIR)/tmp/local/lib/

$(LIBRARY): $(OBJECTS)
	$(LDSHAREDXX) -o $@ $^

.cpp.o:
	$(CXX) $(INCLUDEFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

  MAKEFILE
end
