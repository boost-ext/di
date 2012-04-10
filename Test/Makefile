.SECONDEXPANSION:

CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind

ifndef BOOST
	BOOST:=../Externals/boost
endif

QDEPS:=..
CXXFLAGS:=-I$(BOOST) -I$(QDEPS) -DQDEPS_FUNCTION_ARITY_LIMIT_SIZE=4 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)

mt:=$(patsubst %.cpp, %.o, $(shell find MT -iname "*.cpp")) main.o
ut_back:=$(patsubst %.cpp, %.o, $(shell find UT/Back -iname "*.cpp")) main.o
ut_front:=$(patsubst %.cpp, %.o, $(shell find UT/Front -iname "*.cpp")) main.o
ut_utility:=$(patsubst %.cpp, %.o, $(shell find UT/Utility -iname "*.cpp")) main.o

.PHONY: all test cov cppcheck scan-build valgrind clean

all: test cov cppcheck scan-build valgrind
test: ut_back ut_front ut_utility mt
valgrind: valgrind_ut_back valgrind_ut_front valgrind_ut_utility valgrind_mt
cov: cov_ut_back cov_ut_front cov_ut_utility cov_mt

%: $$($$@)
	@$(CXX) $(CXXFLAGS) -o $@.exe $?
	./$@.exe

cppcheck:
	$(CPPCHECK) -I$(BOOST) -I$(QDEPS) --check-config .

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
	$(VALGRIND) --leak-check=full --track-origins=yes ./$*.exe

clean:
	@find . -iname "*.o" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -f *.exe
	@rm -rf cov_*

