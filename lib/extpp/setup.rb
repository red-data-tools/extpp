if Object.const_defined?(:RubyInstaller)
  ext_dir = File.expand_path(File.join(__dir__, "..", "..", "ext", "extpp"))
  RubyInstaller::Runtime.add_dll_directory(ext_dir)
end
