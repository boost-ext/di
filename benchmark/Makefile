FRAMEWORKS:=frameworks
PYTHON:=python
CXX?=clang++
JAVA?=java
JAVAC?=javac
SC?=mcs

all: benchmark_unique

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

benchmark_unique:
	@make CXX=clang++ JAVA=java JAVAC=javac SC=mcs TYPES=128 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique benchmark

benchmark:
	@echo "--------------"
	@echo BENCHMARK: TYPES=$(TYPES) CTOR_ARGS=$(CTOR_ARGS) ITERATIONS=$(ITERATIONS) TEST=$(TEST)
	@echo "--------------"
	@make base di fruit guice dagger2 ninject -j1

gen_%:
	@echo "--------------"
	@echo $*
	@echo "--------------"
	@rm -rf .$*
	@mkdir .$* && cd .$* && FRAMEWORK=$* $(PYTHON) ../generate_test.py

base: gen_base
	cd .base && time $(CXX) base.cpp -std=c++11 -O2 -I.. -o base.out
	cd .base && strip base.out && ls -lh base.out
	cd .base && time ./base.out

di: gen_di
	cd .di && time $(CXX) di.cpp -std=c++1y -O2 -I.. -I ../$(FRAMEWORKS)/di/include -o di.out
	cd .di && strip di.out && ls -lh di.out
	cd .di && time ./di.out

fruit: gen_fruit
	cd .fruit && time $(CXX) -DFRUIT_NO_LOOP_CHECK fruit.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/fruit/include -I ../$(FRAMEWORKS)/fruit/build/include -L ../$(FRAMEWORKS)/fruit/build/src -lfruit -o fruit.out
	cd .fruit strip fruit.out && ls -lh fruit.out
	cd .fruit && time LD_LIBRARY_PATH=../$(FRAMEWORKS)/fruit/build/src ./fruit.out

hypodermic: gen_hypodermic
	cd .hypodermic && time $(CXX) hypodermic.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/hypodermic -o hypodermic.out
	cd .hypodermic && strip hypodermic.out && ls -lh hypodermic.out
	cd .hypodermic && time ./hypodermic.out

guice: gen_guice
	cd .guice && time $(JAVAC) -cp .:`ls ~/di_frameworks/frameworks/google.guice/*.jar| xargs | tr ' ' ':'` guice.java
	cd .guice && time $(JAVA) -cp .:`ls ~/di_frameworks/frameworks/google.guice/*.jar| xargs | tr ' ' ':'` guice

dagger2: gen_dagger2
	cd .dagger2 && time $(JAVAC) -cp .:`ls ~/di_frameworks/frameworks/dagger2/*.jar| xargs | tr ' ' ':'` dagger2.java
	cd .dagger2 && time $(JAVA) -cp .:`ls ~/di_frameworks/frameworks/dagger2/*.jar| xargs | tr ' ' ':'` dagger2

ninject: gen_ninject
	cd .ninject && cp /Users/krzysztof.jusiak/di_frameworks/frameworks/ninject/Ninject.dll .
	cd .ninject && time $(SC) /r:Ninject.dll ninject.cs
	cd .ninject && time mono ./ninject.exe

clean:
	@rm -rf $(FRAMEWORKS)

