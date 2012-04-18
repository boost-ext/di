MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
DI:=../../..
CXXFLAGS:=-I. -I$(DI) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)
EXAMPLES:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

.PHONY: all clean

all: $(EXAMPLES)

%.o:
	time $(CXX) $(CXXFLAGS) -o $*.exe $*.cpp
	./$*.exe

clean:
	@find . -iname "*.o" -or -iname "*.exe" -or | xargs --no-run-if-empty rm -f

