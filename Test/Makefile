.SECONDEXPANSION:
.SECONDARY:

MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind
GENHTML:=genhtml
LCOV:=lcov
BOOST:=../Externals/boost
QDEPS:=..
CXXFLAGS:=-I$(BOOST) -I$(QDEPS) -DQDEPS_FUNCTION_ARITY_LIMIT_SIZE=4 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)

-include $(patsubst %.dep,.deps/%.dep,$(patsubst %.cpp,%.dep,$(shell find . -iname "*.cpp")))

all: test cov cppcheck scan-build valgrind

lib_boost_unit_test_framework.a: $(patsubst %.cpp,%.o,$(shell find $(BOOST)/libs/test/src -iname "*.cpp"))
	ar rcs $@ $?

test: lib_boost_unit_test_framework.a $(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_MODULE=$@ -o $@ $(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp")) lib_boost_unit_test_framework.a
	./$@

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
	@find . $(BOOST) -iname "*.o" -or -iname "*.a" -or -iname "*.gcda" -or -iname "*.gcno" -or -iname "*.info" | xargs --no-run-if-empty rm -f
	@rm -rf test cov .deps

.deps:
	@mkdir $@

.deps/%.dep: %.cpp .deps
	@mkdir -p $(shell dirname $@) 2>/dev/null
	@$(lastword $(CXX)) $(CXXFLAGS) -MM $< > $@
	@sed -i".bak" -e 's|.*:|$*.o:|' $@

