module ExtPP
  class Platform
    def dynamic_library_extension
      RbConfig::CONFIG["SOEXT"]
    end
    alias_method :shared_library_extension, :dynamic_library_extension
  end
end
