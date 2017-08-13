require "extpp/compiler"

compiler = Extpp::Compiler.new($CXXFLAGS)
compiler.check
$CXXFLAGS = compiler.cxx_flags

include_path = File.expand_path(File.join(__dir__, "..", "include"))
$INCFLAGS += " -I#{include_path.quote}"

[
  File.join(__dir__, "..", "ext", "extpp"),
  __dir__,
].each do |candidate_dir|
  so_name = "libruby-extpp.#{RbConfig::CONFIG["DLEXT"]}"
  lib_path = File.expand_path(File.join(candidate_dir, so_name))
  $LIBS += " #{lib_path.quote}" if File.exist?(lib_path)
end
