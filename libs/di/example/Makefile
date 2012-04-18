MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
DI_PCH:=di_pch.hpp
CXXFLAGS:=-I. -I.. -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)
EXAMPLES:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

.PHONY: examples pch clean

examples: pch $(EXAMPLES)

%.o:
	time $(CXX) $(CXXFLAGS) -include $(DI_PCH) -o $*.exe $*.cpp
	./$*.exe

pch:
	@find ../di -iname "*.hpp" | xargs egrep "^\s*#include.*[\"|<]" | tr -d ' ' | cut -d':' -f2 | sort -u > $(DI_PCH)
	$(CXX) $(CXXFLAGS) $(DI_PCH)

clean:
	@find . -iname "*.o" -or -iname "*.exe" -or -iname "$(DI_PCH)*" | xargs --no-run-if-empty rm -f

