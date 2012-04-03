ifndef BOOST
	BOOST:=../Externals/boost
endif

ifndef QPOOL
	QPOOL:=../Externals/QPool
endif

QDEPS:=..
INCLUDES:=-I$(BOOST) -I$(QPOOL) -I$(QDEPS)
CXXFLAGS:=$(INCLUDES) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants
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

