#
# Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
VALGRIND:=--memcheck="valgrind --leak-check=full --error-exitcode=1"
DRMEMORY:=--memcheck="drmemory -light -batch -exit_code_if_errors 1"
BS?=cmake
CMAKE?=cmake
CMAKE_GENERATOR?=Unix Makefiles
TOOLSET?=clang
CLANG_FORMAT?=clang-format
CLANG_TIDY?=clang-tidy
PYTHON?=python3
MKDOCS?=mkdocs
MKDOCS_THEME?=boost-modern
MKDOCS_SITE?=site
CXX_STANDARD?=14

.PHONY: all clean doc

all: all_$(BS)

all_bjam:
	@cd test && bjam -j2 -q --toolset=$(TOOLSET) --user-config=../.user-config.jam debug-symbols=off $(VARIANT) $($(MEMCHECK)) cxxflags=" $(CXXFLAGS)" linkflags=" $(LDFLAGS)"
	@cd example && bjam -j2 -q --toolset=$(TOOLSET) --user-config=../.user-config.jam debug-symbols=off $($(MEMCHECK)) cxxflags=" $(CXXFLAGS)" linkflags=" $(LDFLAGS)"
	@cd extension && bjam -j2 -q --toolset=$(TOOLSET) --user-config=../.user-config.jam debug-symbols=off $($(MEMCHECK)) cxxflags=" $(CXXFLAGS)" linkflags=" $(LDFLAGS)"

all_cmake:
	@-mkdir build
	cd build && $(CMAKE) -G "$(CMAKE_GENERATOR)" -DCMAKE_VERBOSE_MAKEFILE=ON -DCXX_STANDARD=$(CXX_STANDARD) .. && $(CMAKE) --build . && ctest --output-on-failure

clean: clean_$(BS)

clean_bjam:
	@bjam --clean

clean_cmake:
	@rm -rf build

pph:
	@tools/pph.sh

check: check_pph check_style

check_pph: pph
	@git diff include/boost/di.hpp
	@git diff --quiet include/boost/di.hpp

check_style: style
	@git diff include example extension test
	@exit `git ls-files -m include example extension test | wc -l`

style:
	@find include example extension test -iname "*.hpp" -or -iname "*.cpp" | xargs $(CLANG_FORMAT) -i

check_static_analysis:
	@$(CLANG_TIDY) -header-filter='boost/di' `find example extension test -type f -iname "*.cpp"` -- -std=c++1y -I include -I test -include common/test.hpp

doc: readme doc_$(MKDOCS_THEME)

doc_boost-modern:
	MKDOCS_THEME_DIR='doc/themes/boost-modern' $(MKDOCS) build --quiet --config-file .$(MKDOCS).yml --clean --site-dir $(MKDOCS_SITE)

doc_boost-classic:
	$(PYTHON) doc/themes/boost-classic/scripts/update_markdown.py . https://raw.githubusercontent.com/boost-ext/di/cpp14
	MKDOCS_THEME_DIR='doc/themes/boost-classic' $(MKDOCS) build --quiet --config-file .$(MKDOCS).yml --clean --site-dir $(MKDOCS_SITE)

readme:
	$(PYTHON) doc/scripts/update_readme_toc.py doc .$(MKDOCS).yml README.md https://boost-ext.github.io/di

release: all check

