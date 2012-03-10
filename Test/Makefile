TEST_TARGET=test
CPPCHECK=cppcheck
SCAN_BUILD=scan-build
VALGRIND=valgrind
OBJCOPY=objcopy
COVERAGE_DIR=Coverage
GTEST=../Externals/gtest
GTEST_LIB=$(GTEST)/libgtest.a
BOOST=../Externals/boost
QPOOL=../Externals/QPool
INCLUDES=-I$(BOOST) -I$(QPOOL) -I$(GTEST)/include -I..
CXXFLAGS=$(INCLUDES) -DGTEST_HAS_TR1_TUPLE=0 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants
OBJS=$(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))

all: test cov cppcheck scan-build valgrind

.PHONY: gtest
gtest:
	@[ -e $(GTEST_LIB) ] || (cd $(GTEST) && cmake . && make)

.PHONY: test
test: gtest clean $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(OBJS) $(GTEST_LIB) -lpthread
	@$(OBJCOPY) --compress-debug-sections $(TEST_TARGET)
	@LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) ./$(TEST_TARGET)

.PHONY: cppcheck
cppcheck:
	$(CPPCHECK) $(INCLUDES) --check-config .

.PHONY: scan-build
scan-build:
	$(SCAN_BUILD) make test

.PHONY: cov
cov:
	@lcov --base-directory . --directory . --zerocounters -q
	@make test
	@lcov --base-directory . --directory . -c -o $(TEST_TARGET).info
	@lcov --remove $(TEST_TARGET).info "/usr*" -o $(TEST_TARGET).info
	@lcov --remove $(TEST_TARGET).info "*Externals*" -o $(TEST_TARGET).info
	@rm -rf $(COVERAGE_DIR)
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TEST_TARGET).info
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TEST_TARGET).info

.PHONY: valgrind
valgrind: test
	LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) $(VALGRIND) --leak-check=full --track-origins=yes ./$(TEST_TARGET)

.PHONY: clean
clean:
	@find . -iname "*.o" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -f $(TEST_TARGET) $(TEST_TARGET).info
	@rm -rf $(COVERAGE_DIR)

