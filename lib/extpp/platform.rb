module ExtPP
  class Platform
    def dynamic_library_extension
      RbConfig::CONFIG["SOEXT"]
    end
  end
end
