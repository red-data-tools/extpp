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

    def gcc?
      RbConfig::CONFIG["GCC"] == "yes"
    end
  end
end
