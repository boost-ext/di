TARGET:=QDeps
PREFIX:=/usr/include

.PHONY: all test examples doc install clean

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
	@cd Externals/QPool && make install
	@cp -r $(TARGET) $(PREFIX)

clean:
	@cd Test && make clean
	@cd Doc && make clean
	@cd Examples && make clean

