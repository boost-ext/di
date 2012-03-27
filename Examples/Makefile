TARGET:=examples
BOOST:=$(shell ls ../Externals/boost | xargs -i% echo -I../Externals/boost/%/include | xargs)
QDEPS:=..
QPOOL:=../Externals/QPool
INCLUDES:=$(BOOST) -I$(QPOOL) -I$(QDEPS)
CXXFLAGS:=$(INCLUDES) -O2 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-multichar -Wno-four-char-constants
OBJS=$(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))

.PHONY: examples clean

examples: clean $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@./$(TARGET)

clean:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f
	@rm -f $(TARGET)

