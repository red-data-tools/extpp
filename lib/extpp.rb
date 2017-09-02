require "extpp/compiler"

compiler = Extpp::Compiler.new($CXXFLAGS)
compiler.check
$CXXFLAGS = compiler.cxx_flags

include_path = File.expand_path(File.join(__dir__, "..", "include"))
$INCFLAGS += " -I#{include_path.quote}"

header_files = Dir.chdir(include_path) do
  Dir.glob("**/*.*").collect do |path|
    File.join(include_path, path)
  end
end
header_files_dependency_injector = Module.new do
  define_method(:create_makefile) do |*args, &block|
    super(*args, &block)
    File.open("Makefile", "ab") do |makefile|
      makefile.print <<-MAKEFILE

extpp_headers = #{header_files.join(" ")}
$(OBJS): $(extpp_headers)
      MAKEFILE
    end
  end
end
extend(header_files_dependency_injector)

[
  File.join(__dir__, "..", "ext", "extpp"),
  __dir__,
].each do |candidate_dir|
  so_name = "libruby-extpp.#{RbConfig::CONFIG["DLEXT"]}"
  lib_path = File.expand_path(File.join(candidate_dir, so_name))
  $LIBS += " #{lib_path.quote}" if File.exist?(lib_path)
end
