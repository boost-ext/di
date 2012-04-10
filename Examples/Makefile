ifndef BOOST
	BOOST:=../Externals/boost
endif

QDEPS:=..
CXXFLAGS:=-I$(BOOST) -I$(QDEPS) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)
EXAMPLES:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

.PHONY: examples clean

examples: $(EXAMPLES)

%.o:
	@echo "Example: $*"
	@$(CXX) $(CXXFLAGS) -o $*.exe $*.cpp
	@./$*.exe

clean:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f
	@rm -f *.exe

