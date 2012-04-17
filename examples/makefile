MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
BOOST:=../Externals/boost
QDEPS:=..
QDEPS_PCH:=.QDeps.hpp
CXXFLAGS:=-I. -I$(BOOST) -I$(QDEPS) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)
EXAMPLES:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

.PHONY: examples pch clean

examples: pch $(EXAMPLES)

%.o:
	time $(CXX) $(CXXFLAGS) -include $(QDEPS_PCH) -o $*.exe $*.cpp
	./$*.exe

pch:
	@find $(QDEPS)/QDeps -iname "*.hpp" | xargs egrep "^\s*#include.*[\"|<]" | tr -d ' ' | cut -d':' -f2 | sort -u > $(QDEPS_PCH)
	$(CXX) $(CXXFLAGS) $(QDEPS_PCH)

clean:
	@find . -iname "*.o" -or -iname "*.exe" -or -iname "$(QDEPS_PCH)*" | xargs --no-run-if-empty rm -f

