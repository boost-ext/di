TEST_TARGET=ut
GTEST=../Externals/gtest
GTEST_LIBS=$(GTEST)/lib/.libs
BOOST=../Externals/boost
QPOOL=../Externals/QPool
CXXFLAGS=-I$(BOOST) -I$(QPOOL) -I$(GTEST)/include -I.. -g -Wall -Wextra -Werror
OBJS=$(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))

.PHONY: gtest
gtest:
	@[ -e $(GTEST_LIBS) ] || (cd $(GTEST) && ./configure && make)

.PHONY: test
test: gtest clean_test $(OBJS)
	@$(CXX) -o $(TEST_TARGET) $(OBJS) $(GTEST_LIBS)/libgtest.a -lpthread && LD_LIBRARY_PATH=$(GTEST_LIBS) ./$(TEST_TARGET)

.PHONY: clean_test
clean_test:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f
	@rm -f $(TEST_TARGET)

