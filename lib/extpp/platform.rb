module ExtPP
  class Platform
    def dynamic_library_extension
      case RUBY_PLATFORM
      when /windows/, /mingw/
        "dll"
      when /darwin/
        "dylib"
      else
        RbConfig::CONFIG["DLEXT"]
      end
    end
  end
end
