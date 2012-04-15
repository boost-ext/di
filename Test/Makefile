.SECONDEXPANSION:
.SECONDARY:

MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind
GENHTML:=genhtml
LCOV:=lcov

ifndef BOOST
	BOOST:=../Externals/boost
endif

QDEPS:=..
CXXFLAGS:=-I$(BOOST) -I$(QDEPS) -DQDEPS_FUNCTION_ARITY_LIMIT_SIZE=4 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)

lib_boost_unit_test_framework_OBJS:=$(patsubst %.cpp, %.o, $(shell find $(BOOST)/libs/test/src -iname "*.cpp"))
test_OBJS:=$(patsubst %.cpp, %.o, $(shell find . -iname "*.cpp"))

DEPENDENCIES:=$(patsubst %.dep,.deps/%.dep,$($(lastword $(MAKECMDGOALS))_OBJS:.o=.dep))
-include $(DEPENDENCIES)

all: test cov cppcheck scan-build valgrind

test: lib_boost_unit_test_framework.a $$($$@_OBJS)
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_MODULE=$@ -o $@ $($@_OBJS) lib_boost_unit_test_framework.a
	./$@

%.a: $$($$*_OBJS)
	ar rcs $@ $?

cppcheck:
	$(CPPCHECK) -I$(BOOST) -I$(QDEPS) --check-config .

scan-build:
	$(SCAN_BUILD) make test

valgrind: test
	$(VALGRIND) --leak-check=full --track-origins=yes ./$?

cov: test
	@$(LCOV) --base-directory . --directory . --zerocounters -q
	@$(LCOV) --base-directory . --directory . -c -o $?.info
	@$(LCOV) --remove $?.info "/usr*" -o $?.info
	@$(LCOV) --remove $?.info "*Externals*" -o $?.info
	@rm -rf $@
	@$(GENHTML) -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@$(GENHTML) -o $@ -t "QDeps" --num-spaces 4 --highlight --legend $*.info
	@rm -f $? $?.info

clean:
	@find . $(BOOST) -iname "*.o" -or -iname "*.a" -or -or -iname "*.gcda" -or -iname "*.gcno" -or -iname "*.info" | xargs --no-run-if-empty rm -f
	@rm -rf test cov .deps

.deps:
	@mkdir $@

.deps/%.dep: %.cpp .deps
	mkdir -p $(shell dirname $@) 2>/dev/null; (echo $@ \\; $(lastword $(CXX)) $(CXXFLAGS) -MM $<) > $@ || (rm $@; false)

