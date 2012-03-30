BOOST:=$(shell ls ../Externals/boost | xargs -i% echo -I../Externals/boost/%/include | xargs)
QDEPS:=..
QPOOL:=../Externals/QPool
INCLUDES:=$(BOOST) -I$(QPOOL) -I$(QDEPS)
CXXFLAGS:=$(INCLUDES) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants
EXAMPLES:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

.PHONY: examples clean

examples: clean $(EXAMPLES)

%.o:
	@echo "Example: $*"
	@$(CXX) $(CXXFLAGS) -o $*.exe $*.cpp
	@./$*.exe

clean:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f
	@rm -f *.exe

