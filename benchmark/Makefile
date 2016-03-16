FRAMEWORKS:=frameworks

all: benchmark_unique benchmark_shared

install_fruit:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/google/fruit.git $(FRAMEWORKS)/fruit
	cd $(FRAMEWORKS)/fruit && mkdir build && cd build && cmake .. && cmake --build .

install_di:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/boost-experimental/di.git $(FRAMEWORKS)/di

install_hypodermic:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/ybainier/Hypodermic.git $(FRAMEWORKS)/hypodermic

install: install_di install_fruit

benchmark_shared:
	@make CXX=clang++ TYPES=128 CTOR_ARGS=2 ITERATIONS=1000000 TEST=shared benchmark
	@make CXX=g++ TYPES=128 CTOR_ARGS=2 ITERATIONS=1000000 TEST=shared benchmark
	#@make CXX=clang++ TYPES=256 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark
	#@make CXX=g++ TYPES=256 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark
	#@make CXX=clang++ TYPES=512 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark
	#@make CXX=g++ TYPES=512 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark
	#@make CXX=clang++ TYPES=1024 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark
	#@make CXX=g++ TYPES=1024 CTOR_ARGS=2 ITERATIONS=1000 TEST=shared benchmark

benchmark_unique:
	@make CXX=clang++ TYPES=128 CTOR_ARGS=2 ITERATIONS=1000000 TEST=unique benchmark
	@make CXX=g++ TYPES=128 CTOR_ARGS=2 ITERATIONS=1000000 TEST=unique benchmark
	#@make CXX=clang++ TYPES=256 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark
	#@make CXX=g++ TYPES=256 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark
	#@make CXX=clang++ TYPES=512 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark
	#@make CXX=g++ TYPES=512 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark
	#@make CXX=clang++ TYPES=1024 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark
	#@make CXX=g++ TYPES=1024 CTOR_ARGS=2 ITERATIONS=1000 TEST=unique benchmark

benchmark:
	@echo "--------------"
	@echo BENCHMARK: TYPES=$(TYPES) CTOR_ARGS=$(CTOR_ARGS) ITERATIONS=$(ITERATIONS) TEST=$(TEST)
	@echo "--------------"
	@-rm -rf tmp 2>/dev/null
	@mkdir tmp && cd tmp && python ../generate_test.py
	@make  run_hypodermic run_base run_di run_fruit -j1
	@rm -rf tmp

run_base:
	@echo "--------------"
	@echo base
	@echo "--------------"
	@cd tmp && time $(CXX) -DCHECK_COMPILE_TIME base.cpp -std=c++11 -O2 -I..
	@cd tmp && $(CXX) base.cpp -std=c++11 -O2 -I.. -o base.out && strip base.out && ls -lh base.out && ./base.out

run_di:
	@echo "--------------"
	@echo di
	@echo "--------------"
	@cd tmp && time $(CXX) -DCHECK_COMPILE_TIME di.cpp -std=c++1y -O2 -I.. -I ../$(FRAMEWORKS)/di/include
	@cd tmp && $(CXX) di.cpp -std=c++1y -O2 -I.. -I ../$(FRAMEWORKS)/di/include -o di.out && strip di.out && ls -lh di.out && ./di.out

run_fruit:
	@echo "--------------"
	@echo fruit
	@echo "--------------"
	@cd tmp && time $(CXX) -DCHECK_COMPILE_TIME -DFRUIT_NO_LOOP_CHECK fruit.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/fruit/include -I ../$(FRAMEWORKS)/fruit/build/include -L ../$(FRAMEWORKS)/fruit/build/src -lfruit
	@cd tmp && time $(CXX) -DFRUIT_NO_LOOP_CHECK fruit.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/fruit/include -I ../$(FRAMEWORKS)/fruit/build/include -L ../$(FRAMEWORKS)/fruit/build/src -lfruit -o fruit.out && strip fruit.out && ls -lh fruit.out && LD_LIBRARY_PATH=../$(FRAMEWORKS)/fruit/build/src ./fruit.out

run_hypodermic:
	@echo "--------------"
	@echo hypodermic 
	@echo "--------------"
	@cd tmp && time $(CXX) -DCHECK_COMPILE_TIME hypodermic.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/hypodermic
	@cd tmp && time $(CXX) hypodermic.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/hypodermic -o hypodermic.out && strip hypodermic.out && ls -lh hypodermic.out && ./hypodermic.out

clean:
	@rm -rf $(FRAMEWORKS)

