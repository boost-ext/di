#
# Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
VALGRIND:=--memcheck="valgrind --leak-check=full --error-exitcode=1"
DRMEMORY:=--memcheck="drmemory -light -batch -exit_code_if_errors 1"
BS?=cmake
TOOLSET?=clang
GENERATOR?="Unix Makefiles"

.PHONY: all clean doc

all: all_$(BS)

all_bjam:
	@cd test && bjam -j2 -q --toolset=$(TOOLSET) --user-config=../user-config.jam debug-symbols=off $(VARIANT) $($(MEMCHECK)) cxxflags=" $(CXXFLAGS)" linkflags=" $(LDFLAGS)"
	@cd example && bjam -j2 -q --toolset=$(TOOLSET) --user-config=../user-config.jam debug-symbols=off $(VARIANT) $($(MEMCHECK)) cxxflags=" $(CXXFLAGS)" linkflags=" $(LDFLAGS)"

all_cmake:
	@-mkdir build
	@cd build && cmake .. && cmake --build . && ctest --output-on-failure

clean: clean_$(BS)

clean_bjam:
	@bjam --clean

clean_cmake:
	@rm -rf build

pph:
	#@tools/pph.sh

check: check_pph check_style

check_pph: pph
	@git diff --quiet include/boost/di.hpp

check_style:
	@find include example test -iname "*.hpp" -or -iname "*.cpp" | xargs clang-format -i && exit `git ls-files -m | wc -l`

check_static:
	clang-tidy -header-filter='boost/di' `find -type f -iname "*.cpp"` -- -std=c++1y -I ../include -I . -include common/test.hpp | sort -u

doc:
	@cd doc && bjam -j2 -q && TRY_IT_ONLINE=ON scripts/update_html.sh

release: all check

