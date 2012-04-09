.SECONDEXPANSION:

CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind

ifndef BOOST
	BOOST:=$(shell ls ../Externals/boost | xargs -i% echo -I../Externals/boost/%/include | xargs echo . )
endif

ifndef GTEST
	GTEST:=../Externals/gtest
endif

GTEST_LIB:=$(GTEST)/libgtest.a
INCLUDES:=-I$(BOOST) -I$(GTEST)/include -I..
CXXFLAGS:=$(INCLUDES) -DQDEPS_FUNCTION_ARITY_LIMIT_SIZE=4 -DGTEST_HAS_TR1_TUPLE=0 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants

mt:=$(patsubst %.cpp, %.o, $(shell find MT -iname "*.cpp")) main.o
ut_back:=$(patsubst %.cpp, %.o, $(shell find UT/Back -iname "*.cpp")) main.o
ut_front:=$(patsubst %.cpp, %.o, $(shell find UT/Front -iname "*.cpp")) main.o
ut_utility:=$(patsubst %.cpp, %.o, $(shell find UT/Utility -iname "*.cpp")) main.o

.PHONY: all gtest test cov cppcheck scan-build valgrind clean

all: test cov cppcheck scan-build valgrind
test: gtest ut_back ut_front ut_utility mt
valgrind: valgrind_ut_back valgrind_ut_front valgrind_ut_utility valgrind_mt
cov: cov_ut_back cov_ut_front cov_ut_utility cov_mt

gtest:
	@[ -e $(GTEST_LIB) ] || (cd $(GTEST) && cmake . && make)

%: $$($$@)
	@$(CXX) $(CXXFLAGS) -o $@.exe $? $(GTEST_LIB) -lpthread
	@LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) ./$@.exe

cppcheck:
	$(CPPCHECK) $(INCLUDES) --check-config .

scan-build:
	$(SCAN_BUILD) make test

cov_%:
	@lcov --base-directory . --directory . --zerocounters -q
	@make $*
	@lcov --base-directory . --directory . -c -o $*.info
	@lcov --remove $*.info "/usr*" -o $*.info
	@lcov --remove $*.info "*Externals*" -o $*.info
	@rm -rf $@
	@genhtml -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@genhtml -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@rm -f $* $*.info

valgrind_%:
	@make $*
	LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) $(VALGRIND) --leak-check=full --track-origins=yes ./$*.exe

clean:
	@find . -iname "*.o" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -f *.exe
	@rm -rf cov_*

