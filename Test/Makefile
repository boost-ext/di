TEST_TARGET=ut
GTEST=../Externals/gtest
GTEST_LIB=$(GTEST)/libgtest.a
BOOST=../Externals/boost
QPOOL=../Externals/QPool
CXXFLAGS=-I$(BOOST) -I$(QPOOL) -I$(GTEST)/include -I.. -g -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar
OBJS=$(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))

.PHONY: gtest
gtest:
	@[ -e $(GTEST_LIB) ] || (cd $(GTEST) && cmake . && make)

.PHONY: test
test: gtest clean_test $(OBJS)
	@$(CXX) -o $(TEST_TARGET) $(OBJS) $(GTEST_LIB) -lpthread && LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) ./$(TEST_TARGET)

.PHONY: clean_test
clean_test:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f
	@rm -f $(TEST_TARGET)

