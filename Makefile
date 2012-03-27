QDEPS:=QDeps
PREFIX:=/usr/include

.PHONY: all test examples doc install clean

all: verify examples doc

verify:
	@cd Test && make test valgrind cppcheck scan-build cov

test:
	@cd Test && make test

examples:
	@cd Examples && make examples

doc:
	@cd Doc && make doc

install:
	@cp -r $(QDEPS) $(PREFIX)

clean:
	@cd Test && make clean
	@cd Doc && make clean
	@cd Examples && make clean

