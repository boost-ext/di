#
# Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
FRAMEWORKS:=.frameworks
PYTHON:=python
CXX?=clang++
JAVA?=java
JAVAC?=javac
SC?=dmcs

benchmark:
	make TYPES=64 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique base di fruit guice dagger2 ninject
	make TYPES=128 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique base di fruit guice dagger2 ninject
	make TYPES=256 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique base di fruit guice dagger2 ninject
	make TYPES=512 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique base di fruit guice dagger2 ninject
	make TYPES=1024 CTOR_ARGS=4 ITERATIONS=1024 TEST=unique base di fruit guice dagger2 ninject

$(FRAMEWORKS)/base:

$(FRAMEWORKS)/fruit:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/google/fruit.git $(FRAMEWORKS)/fruit
	cd $(FRAMEWORKS)/fruit && mkdir .build && cd .build && cmake .. && cmake --build .

$(FRAMEWORKS)/di:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/boost-experimental/di.git $(FRAMEWORKS)/di

$(FRAMEWORKS)/hypodermic:
	@-mkdir -p $(FRAMEWORKS) 2>/dev/null
	git clone https://github.com/ybainier/Hypodermic.git $(FRAMEWORKS)/hypodermic

$(FRAMEWORKS)/guice:
	@-mkdir -p $(FRAMEWORKS)/guice 2>/dev/null
	cd $(FRAMEWORKS)/guice && curl -L -O http://central.maven.org/maven2/com/google/guava/guava/19.0/guava-19.0.jar
	cd $(FRAMEWORKS)/guice && curl -L -O http://central.maven.org/maven2/javax/inject/javax.inject/1/javax.inject-1.jar
	cd $(FRAMEWORKS)/guice && curl -L -O https://github.com/google/guice/releases/download/4.0/guice-4.0.jar
	cd $(FRAMEWORKS)/guice && curl -L -O http://central.maven.org/maven2/aopalliance/aopalliance/1.0/aopalliance-1.0.jar

$(FRAMEWORKS)/dagger2:
	@-mkdir -p $(FRAMEWORKS)/dagger2 2>/dev/null
	cd $(FRAMEWORKS)/dagger2 && curl -L -O https://github.com/krzysztof-jusiak/di_frameworks/raw/master/frameworks/dagger2/dagger-compiler-2.0-20150330.203231-18-jar-with-dependencies.jar
	cd $(FRAMEWORKS)/dagger2 && curl -L -O https://github.com/krzysztof-jusiak/di_frameworks/raw/master/frameworks/dagger2/javax.inject-1.jar
	#cd $(FRAMEWORKS)/dagger2 && curl -L -O http://central.maven.org/maven2/com/google/dagger/dagger/2.4/dagger-2.4.jar
	#cd $(FRAMEWORKS)/dagger2 && curl -L -O http://central.maven.org/maven2/com/google/dagger/dagger-compiler/2.4/dagger-compiler-2.4.jar
	#cd $(FRAMEWORKS)/dagger2 && curl -L -O http://central.maven.org/maven2/org/glassfish/javax.annotation/10.0-b28/javax.annotation-10.0-b28.jar

$(FRAMEWORKS)/ninject:
	@-mkdir -p $(FRAMEWORKS)/ninject 2>/dev/null
	cd $(FRAMEWORKS)/ninject && curl -L -O https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/ninject2/Ninject-3.0.1.10-release-mono-4.0.zip
	cd $(FRAMEWORKS)/ninject && unzip -j Ninject-3.0.1.10-release-mono-4.0.zip

gen_%:
	@make $(FRAMEWORKS)/$*
	@echo "--------------"
	@echo $*
	@echo "--------------"
	@rm -rf .$*
	@mkdir .$* && cd .$* && FRAMEWORK=$* $(PYTHON) ../generate_test.py

base: gen_base
	cd .base && time $(CXX) base.cpp -std=c++11 -O2 -I.. -o base.out
	cd .base && strip base.out && ls -lh base.out 
	cd .base && time ./base.out || exit 1

di: gen_di
	cd .di && time $(CXX) di.cpp -std=c++1y -O2 -I.. -I ../$(FRAMEWORKS)/di/include -o di.out
	cd .di && strip di.out && ls -lh di.out || exit 1
	cd .di && time ./di.out

fruit: gen_fruit
	cd .fruit && time $(CXX) -DFRUIT_NO_LOOP_CHECK fruit.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/fruit/include -I ../$(FRAMEWORKS)/fruit/.build/include -L ../$(FRAMEWORKS)/fruit/.build/src -lfruit -o fruit.out
	cd .fruit strip fruit.out && ls -lh fruit.out
	cd .fruit && time LD_LIBRARY_PATH=../$(FRAMEWORKS)/fruit/.build/src ./fruit.out || exit 1

hypodermic: gen_hypodermic
	cd .hypodermic && time $(CXX) hypodermic.cpp -std=c++11 -O2 -I.. -I ../$(FRAMEWORKS)/hypodermic -o hypodermic.out
	cd .hypodermic && strip hypodermic.out && ls -lh hypodermic.out
	cd .hypodermic && time ./hypodermic.out || exit 1

guice: gen_guice
	cd .guice && time $(JAVAC) -cp .:`ls ../$(FRAMEWORKS)/guice/*.jar| xargs | tr ' ' ':'` guice.java
	cd .guice && time $(JAVA) -cp .:`ls ../$(FRAMEWORKS)/guice/*.jar| xargs | tr ' ' ':'` guice || exit 1

dagger2: gen_dagger2
	cd .dagger2 && time $(JAVAC) -cp .:`ls ../$(FRAMEWORKS)/dagger2/*.jar| xargs | tr ' ' ':'` dagger2.java
	cd .dagger2 && time $(JAVA) -cp .:`ls ../$(FRAMEWORKS)/dagger2/*.jar| xargs | tr ' ' ':'` dagger2 || exit 1

ninject: gen_ninject
	cd .ninject && cp ../$(FRAMEWORKS)/ninject/Ninject.dll .
	cd .ninject && time $(SC) -o+ /r:Ninject.dll ninject.cs
	cd .ninject && time mono --aot=full ./ninject.exe
	cd .ninject && time mono ./ninject.exe || exit 1

