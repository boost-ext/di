TARGET:=di
HEADERS:=/usr/include
EXTERNALS:=$(PWD)/externals
COMPILERS:=g++ clang++ icc

.PHONY: all test examples doc install release clean

all: verify examples doc

verify:
	@cd test && make all

test:
	@cd test && make test

examples:
	@cd examples && make examples

doc:
	@cd doc && make doc

install:
	@cp -r $(TARGET) $(HEADERS)

externals_boost:
	@cd test && make CXX='clang++' EXTRA_CXXFLAGS='-H' test 2>&1 >/dev/null | grep boost | sed "s/.*\/\(boost.*\)/\1/" | xargs -i% sh -c "[ -e $(EXTERNALS)/% ] || (cd $(HEADERS) && cp --parents % $(EXTERNALS)/boost)"

#release: externals_boost
	# test, diagnostics, ...
	# update externals
	# generate doc
	# update gh-pages
	# create tag from lib version

clean:
	@cd test && make clean
	@cd doc && make clean
	@cd examples && make clean

