require "mkmf"

module ExtPP
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
      check_cxx
      check_version
      check_warning_flags
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

    def check_cxx
      checking_for(checking_message("C++ compiler"), "%s") do
        RbConfig.expand("$(CXX)")
      end
    end

    def check_version
      return unless gcc?

      checking_for(checking_message("g++ version"), "%g%s") do
        version = 0.0
        std = nil

        case `#{RbConfig.expand("$(CXX) --version")}`
        when /g\+\+.+ (\d\.\d)\.\d/
          version = Float($1)
          if version < 5.1
            std = "gnu++11"
          elsif version < 6.1
            std = "gnu++14"
          else
            std = "gnu++17"
          end
        when /\A.*clang version (\d\.\d)\.\d/
          version = Float($1)
          if version < 3.5
            std = "gnu++11"
          elsif version < 5
            std = "gnu++14"
          else
            std = "gnu++17"
          end
        when /\AApple (?:LLVM|clang) version (\d+\.\d+)\.\d/
          version = Float($1)
          # TODO: Is it right?
          if version < 9.0
            std = "gnu++11"
          elsif version < 9.1
            std = "gnu++14"
          else
            std = "gnu++17"
          end
          cxx_flags = "-Wno-deprecated-register"
        end

        if std
          if std == "gnu++17"
            # ruby/intern.h doesn't adapt to C++17. Because it uses
            # "register" for "rb_mem_clear(register VALUE *mem,
            # register long size)".  C++17 removes "register" keyword.
            std = "gnu++14"
          end
          @cxx_flags += " -std=#{std}"
          @cxx_flags += " #{cxx_flags}" if cxx_flags
          [version, " (#{std})"]
        else
          [version, ""]
        end
      end
    end

    def try_cxx_warning_flag(warning_flag)
      conftest_cxx = "#{CONFTEST}.cpp"
      begin
        source = "int main(void) {return 0;}"
        open(conftest_cxx, "wb") do |cxx_file|
          cxx_file.print(source)
        end
        flags = "-Werror #{warning_flag}"
        xsystem(RbConfig.expand("$(CXX) #{flags} -c #{conftest_cxx}"))
      ensure
        log_src(source)
        MakeMakefile.rm_f(conftest_cxx)
      end
    end

    def check_warning_flags
      flags = []
      warning_flags = []
      Shellwords.split(@cxx_flags).each do |flag|
        if flag.start_with?("-W")
          warning_flags << flag
        else
          flags << flag
        end
      end
      warning_flags.each do |warning_flag|
        if try_cxx_warning_flag(warning_flag.gsub(/\A-Wno-/, "-W"))
          flags << warning_flag
        end
      end
      case RUBY_PLATFORM
      when /windows/, /mingw/
        @cxx_flags = flags.join(" ")
      else
        @cxx_flags = Shellwords.join(flags)
      end
    end
  end
end
