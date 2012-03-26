QDEPS:=QDeps
PREFIX:=/usr/include

.PHONY: all test examples doc install clean

all:
	@cd Test && make test cov cppcheck scan-build valgrind
	@cd Examples && make examples
	@cd Doc && make doc

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

