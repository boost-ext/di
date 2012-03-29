ifndef COVERAGE_DIR
	COVERAGE_DIR=Coverage
endif

TARGET:=test
COVERAGE_DIR:=Coverage
CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind
OBJCOPY:=objcopy
BOOST:=$(shell ls ../Externals/boost | xargs -i% echo -I../Externals/boost/%/include | xargs)
QPOOL:=../Externals/QPool
GTEST:=../Externals/gtest
GTEST_LIB:=$(GTEST)/libgtest.a
INCLUDES:=$(BOOST) -I$(QPOOL) -I$(GTEST)/include -I..
CXXFLAGS:=$(INCLUDES) -DGTEST_HAS_TR1_TUPLE=0 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants
#OBJS=$(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))
OBJS=UT/Back/FactoryTest.o main.o

.PHONY: all gtest compress test cov cppcheck scan-build valgrind clean

all: test cov cppcheck scan-build valgrind

gtest:
	@[ -e $(GTEST_LIB) ] || (cd $(GTEST) && cmake . && make)

test: gtest clean $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(GTEST_LIB) -lpthread
	@LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) ./$(TARGET)

compress:
	@$(OBJCOPY) --compress-debug-sections $(TARGET)

cppcheck:
	$(CPPCHECK) $(INCLUDES) --check-config .

scan-build:
	$(SCAN_BUILD) make test

cov:
	@lcov --base-directory . --directory . --zerocounters -q
	@make test
	@lcov --base-directory . --directory . -c -o $(TARGET).info
	@lcov --remove $(TARGET).info "/usr*" -o $(TARGET).info
	@lcov --remove $(TARGET).info "*Externals*" -o $(TARGET).info
	@rm -rf $(COVERAGE_DIR)
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TARGET).info
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TARGET).info
	@rm -f $(TARGET) $(TARGET).info

valgrind: test
	LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) $(VALGRIND) --leak-check=full --track-origins=yes ./$(TARGET)

clean:
	@find . -iname "*.o" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -f $(TARGET)
	@rm -rf $(COVERAGE_DIR)

