LIBRARY = libruby-extpp.so

SOURCES = /home/kou/work/ruby/extpp/ext/extpp/class.cpp /home/kou/work/ruby/extpp/ext/extpp/function.cpp
OBJECTS = /home/kou/work/ruby/extpp/ext/extpp/class.o /home/kou/work/ruby/extpp/ext/extpp/function.o
HEADERS = 

INCLUDE_DIR = /home/kou/work/ruby/extpp/include

CXX = g++

RUBY = /usr/bin/ruby2.3
RUBY_HEADER_DIR = /usr/include/ruby-2.3.0
RUBY_ARCH_HEADER_DIR = /usr/include/x86_64-linux-gnu/ruby-2.3.0
LDSHAREDXX = g++ -shared
CCDLFLAGS = -fPIC

INCLUDEFLAGS = 	-I$(INCLUDE_DIR) 	-I$(RUBY_HEADER_DIR) 	-I$(RUBY_ARCH_HEADER_DIR)
CPPFLAGS = -Wdate-time -D_FORTIFY_SOURCE=2  
CXXFLAGS = $(CCDLFLAGS) -g -O2 -fdebug-prefix-map=/build/ruby2.3-8Td9HM/ruby2.3-2.3.3=. -fstack-protector-strong -Wformat -Werror=format-security

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

