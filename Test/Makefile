.SECONDEXPANSION:
.SECONDARY:

MAKEFLAGS+=-j4 -l
CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind

ifndef BOOST
	BOOST:=../Externals/boost
endif

QDEPS:=..
CXXFLAGS:=-I$(BOOST) -I$(QDEPS) -DQDEPS_FUNCTION_ARITY_LIMIT_SIZE=4 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)

lib_boost_unit_test_framework:=$(patsubst %.cpp, %.o, $(shell find $(BOOST)/libs/test/src -iname "*.cpp"))
mt_all:=$(patsubst %.cpp, %.o, $(shell find MT -iname "*.cpp")) main.o
ut_back:=$(patsubst %.cpp, %.o, $(shell find UT/Back -iname "*.cpp")) main.o
ut_front:=$(patsubst %.cpp, %.o, $(shell find UT/Front -iname "*.cpp")) main.o
ut_utility:=$(patsubst %.cpp, %.o, $(shell find UT/Utility -iname "*.cpp")) main.o

all: test cov cppcheck scan-build valgrind
test: ut_back ut_front ut_utility mt
ut: ut_back ut_front ut_utility
mt: mt_all
valgrind: valgrind_ut_back valgrind_ut_front valgrind_ut_utility valgrind_mt
cov: cov_ut_back cov_ut_front cov_ut_utility cov_mt

ut_% mt_%: lib_boost_unit_test_framework.a $$($$@)
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_MODULE=$@ -o $@.exe $($@) lib_boost_unit_test_framework.a
	./$@.exe

%.a: $$($$*)
	ar rcs $@ $?

cppcheck:
	$(CPPCHECK) -I$(BOOST) -I$(QDEPS) --check-config .

scan-build:
	$(SCAN_BUILD) make test

cov_%: $$*
	@lcov --base-directory . --directory . --zerocounters -q
	@lcov --base-directory . --directory . -c -o $*.info
	@lcov --remove $*.info "/usr*" -o $*.info
	@lcov --remove $*.info "*Externals*" -o $*.info
	@rm -rf $@
	@genhtml -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@genhtml -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@rm -f $* $*.info

valgrind_%: $$*
	$(VALGRIND) --leak-check=full --track-origins=yes ./$*.exe

clean:
	@find . $(BOOST) -iname "*.o" -or -iname "*.a" -or -iname "*.exe" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -rf cov_*

