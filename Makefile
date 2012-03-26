ifndef COVERAGE_DIR
	COVERAGE_DIR=Coverage
endif

QDEPS:=QDeps
TMP:=/tmp
DOC:=Doc
WIKI:=wiki
CPPCHECK:=cppcheck
SCAN_BUILD:=scan-build
VALGRIND:=valgrind
OBJCOPY:=objcopy
COVERAGE_DIR:=Coverage
PREFIX:=/usr/include
BOOST:=$(shell ls Externals/boost | xargs -i% echo -IExternals/boost/%/include | xargs)
VIMWIKI:=Externals/vimwiki
QPOOL:=Externals/QPool
GTEST:=Externals/gtest
GTEST_LIB:=$(GTEST)/libgtest.a
INCLUDES:=-I$(BOOST) -I$(QPOOL) -I$(GTEST)/include -I.
CXXFLAGS:=$(INCLUDES) -DGTEST_HAS_TR1_TUPLE=0 -g -O0 --coverage -std=c++98 -Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-variadic-macros -Wno-long-long -Wno-multichar -Wno-four-char-constants
QDEPS_PCH:=$(QDEPS)/$(QDEPS).hpp
REV:=$(shell git log --oneline | head -1 | cut -d ' ' -f1)
#OBJS=$(patsubst %.cpp,%.o,$(shell find Test -iname "*.cpp"))
#OBJS=$(patsubst %.cpp,%.o,$(shell find Test/MT -iname "*.cpp")) main.o
OBJS=Test/UT/Back/FactoryTest.o Test/main.o

all: test cov cppcheck scan-build valgrind doc

doc: clean
	@cp -r $(DOC)/$(WIKI) $(TMP)/$(WIKI)
	@find $(TMP)/$(WIKI) -type f -iname "*.wiki" | xargs -i@ sh -c "cat @ | grep \"?include\" | sed \"s/[^\\\"]*\\\"\(.*\)\\\".*/\1/\" | xargs -i% sed -i\".bak\" -e\"/<?include\s*=\s*".*"/r $(DOC)/%\" -e\"/<?include\s*=\s*".*"/d\" @"
	@touch $(VIMWIKI)/syntax/syntax.vim
	@VIMRUNTIME="$(VIMWIKI)" vim -n --noplugins -u NONE\
		-c "set nocompatible | filetype plugin on | syntax on"\
		-c "let g:vimwiki_list = [{'path': '$(TMP)/$(WIKI)', 'path_html': '$(DOC)/html/wiki', 'template_path': '$(TMP)/$(WIKI)/templates/', 'template_default': 'wiki', 'template_ext': '.tpl'}]"\
		-c "source $(VIMWIKI)/plugin/vimwiki.vim"\
		-c "source $(VIMWIKI)/ftplugin/vimwiki.vim"\
		-c "source $(VIMWIKI)/syntax/vimwiki.vim"\
		-c "VimwikiAll2HTML"\
		-c "quit"\
		$(TMP)/$(WIKI)/index.wiki
	@rm -f $(VIMWIKI)/syntax/syntax.vim

install:
	@cp -r $(QDEPS) $(PREFIX)

gtest:
	@[ -e $(GTEST_LIB) ] || (cd $(GTEST) && cmake . && make)

test: gtest clean $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(OBJS) $(GTEST_LIB) -lpthread
	@LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) ./$(TEST_TARGET)

compress:
	@$(OBJCOPY) --compress-debug-sections $(TEST_TARGET)

cppcheck:
	$(CPPCHECK) $(INCLUDES) --check-config .

scan-build:
	$(SCAN_BUILD) make test

cov:
	@lcov --base-directory . --directory . --zerocounters -q
	@make test
	@lcov --base-directory . --directory . -c -o $(TEST_TARGET).info
	@lcov --remove $(TEST_TARGET).info "/usr*" -o $(TEST_TARGET).info
	@lcov --remove $(TEST_TARGET).info "*Externals*" -o $(TEST_TARGET).info
	@rm -rf $(COVERAGE_DIR)
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TEST_TARGET).info
	@genhtml -o $(COVERAGE_DIR) -t "QDeps" --num-spaces 4 --highlight --legend $(TEST_TARGET).info
	@rm -f $(TEST_TARGET) $(TEST_TARGET).info

valgrind: test
	LD_LIBRARY_PATH=$(dirname $(GTEST_LIBS)) $(VALGRIND) --leak-check=full --track-origins=yes ./$(TEST_TARGET)

clean:
	@find . -iname "*.o" -or -iname "*.gcda" -or -iname "*.gcno" | xargs --no-run-if-empty rm -f
	@rm -rf $(COVERAGE_DIR)

pch:
	@echo "//" > $(QDEPS_PCH)
	@echo "// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)" >> $(QDEPS_PCH)
	@echo "//" >> $(QDEPS_PCH)
	@echo "// Distributed under the Boost Software License, Version 1.0." >> $(QDEPS_PCH)
	@echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)" >> $(QDEPS_PCH)
	@echo "//" >> $(QDEPS_PCH)
	@echo "// This file is GENERATED to use with precompiled headers !" >> $(QDEPS_PCH)
	@echo "// DO NOT modify by hand !" >> $(QDEPS_PCH)
	@echo "//" >> $(QDEPS_PCH)
	@echo "#ifndef QDEPS_QDEPS_HPP" >> $(QDEPS_PCH)
	@echo "#define QDEPS_QDEPS_HPP" >> $(QDEPS_PCH)
	@echo >> $(QDEPS_PCH)
	@rm -f $(QDEPS_PCH).tmp 2>/dev/null
	@find $(QDEPS) -iname "*.hpp" | xargs grep "^#.*include" | grep -v "(" | cut -d':' -f2 | sed "s/#\(.*\)include\(.*\)/#include\2/" > $(QDEPS_PCH).tmp
	@find $(QDEPS) -iname "*.hpp" | sed "s/\(.*\)/#include \"\1\"/" >> $(QDEPS_PCH).tmp
	@sort -u $(QDEPS_PCH).tmp >> $(QDEPS_PCH)
	@rm -f $(QDEPS_PCH).tmp 2>/dev/null
	@echo >> $(QDEPS_PCH)
	@echo "#endif" >> $(QDEPS_PCH)
	@echo >> $(QDEPS_PCH)
	$(CXX) $(CXXFLAGS) $(QDEPS_PCH)

clean_pch:
	@rm -f $(QDEPS_PCH)*

clean: clean_pch
	@cd Test && make clean
	@rm -rf $(TMP)/$(WIKI)

