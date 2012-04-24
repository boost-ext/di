MAKEFLAGS+=-j$(shell grep -c '^processor' /proc/cpuinfo) -l
CXXFLAGS:=-I../../.. -I. -DDI_FUNCTION_ARITY_LIMIT_SIZE=4 -g -O0 -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants $(EXTRA_CXXFLAGS)

#-include $(patsubst %.dep,.deps/%.dep,$(patsubst %.cpp,%.dep,$(shell find . -iname "*.cpp")))

all: $(patsubst %.cpp,%.o,$(shell find . -iname "*.cpp"))
	$(CXX) $(CXXFLAGS) -DBOOST_TEST_MODULE=$@ -o $@ $? /usr/lib/libboost_unit_test_framework.a
	./$@

clean:
	@find . -iname "*.o" | xargs --no-run-if-empty rm -f

.deps:
	@mkdir $@

.deps/%.dep: %.cpp .deps
	@mkdir -p $(shell dirname $@) 2>/dev/null
	@$(lastword $(CXX)) $(CXXFLAGS) -MM $< > $@
	@sed -i -e 's|.*:|$*.o:|' $@

