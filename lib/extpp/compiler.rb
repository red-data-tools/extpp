require "mkmf"

module Extpp
  class Compiler
    attr_reader :cxx_flags
    def initialize(cxx_flags)
      @cxx_flags = cxx_flags
    end

    def gcc?
      RbConfig::CONFIG["GCC"] == "yes"
    end

    def check
      check_debug_build
      check_version
    end

    private
    def check_debug_build
      checking_for(checking_message("--enable-debug-build option")) do
        enable_debug_build = enable_config("debug-build", false)
        if enable_debug_build
          @cxx_flags = disable_optimization_build_flag(@cxx_flags)
          @cxx_flags = enable_debug_build_flag(@cxx_flags)
        end
        enable_debug_build
      end
    end

    def check_version
      return unless gcc?
      checking_for(checking_message("g++ version"), "%g%s") do
        version = nil
        std = nil
        if /\Ag\+\+ .+ (\d\.\d)\.\d/ =~ `#{RbConfig.expand("$(CXX) --version")}`
          version = Float($1)
          if version < 5.1
            std = "gnu++11"
          elsif version < 6.1
            std = "gnu++14"
          end
        end
        if std
          @cxx_flags += " -std=#{std}"
          [version, " (#{std})"]
        else
          [version, ""]
        end
      end
    end

    def disable_optimization_build_flag(flags)
      if gcc?
        flags.gsub(/(^|\s)-O\d(\s|$)/, '\\1-O0\\2')
      else
        flags
      end
    end

    def enable_debug_build_flag(flags)
      if gcc?
        flags.gsub(/(^|\s)(?:-g|-g\d|-ggdb\d?)(\s|$)/, '\\1-g3\\2')
      else
        flags
      end
    end
  end
end
