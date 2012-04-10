TARGET:=QDeps
PREFIX:=/usr/include
EXTERNALS:=$(PWD)/Externals
COMPILERS:=g++ clang++

.PHONY: all test examples doc install release clean

#define EXTERNAL_template =
	#$(1): $(1)_external
#endef

#$(foreach compiler, $(COMPILERS), $(eval $(call EXTERNAL_template, $(compiler))))

all: verify examples doc

verify:
	@cd Test && make all

test:
	@cd Test && make test

examples:
	@cd Examples && make examples

doc:
	@cd Doc && make doc

install:
	@cp -r $(TARGET) $(PREFIX)

#externals_boost:
	#@cd Test && make CXX='g++' EXTRA_CXXFLAGS='-H' test 2>&1 >/dev/null | grep boost | sed "s/.*\/\(boost.*\)/\1/" | xargs -i% sh -c "[ -e $(EXTERNALS)/% ] || (cd $(PREFIX) && cp --parents % $(EXTERNALS)/boost)"
	#@cd Test && make CXX='clang++' EXTRA_CXXFLAGS='-H' test 2>&1 >/dev/null | grep boost | sed "s/.*\/\(boost.*\)/\1/" | xargs -i% sh -c "[ -e $(EXTERNALS)/% ] || (cd $(PREFIX) && cp --parents % $(EXTERNALS)/boost)"
	#@cd Test && make CXX='icc' EXTRA_CXXFLAGS='-H' test 2>&1 >/dev/null | grep boost | sed "s/.*\/\(boost.*\)/\1/" | xargs -i% sh -c "[ -e $(EXTERNALS)/% ] || (cd $(PREFIX) && cp --parents % $(EXTERNALS)/boost)"

#release: externals_boost
	# test, diagnostics, ...
	# update externals
	# generate doc
	# update gh-pages
	# create tag from lib version

clean:
	@cd Test && make clean
	@cd Doc && make clean
	@cd Examples && make clean

