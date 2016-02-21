#
# Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
BOOST_DI?=../..
DICPP?=../../frameworks/dicpp
GOOGLE_FRUIT?=../../frameworks/google.fruit
GOOGLE_GUICE?=../../frameworks/google.guice
DAGGER2?=../../frameworks/dagger2
NINJECT?=../../frameworks/ninject

benchmark_%:
	cd create_$* && time $(CXX) -O2 -std=c++1y -I $(BOOST_DI)/include di.cpp -o di.out && strip di.out && ls -lh di.out && time ./di.out
	#cd create_$* && time $(CXX) -O2 -std=c++11 -ftemplate-depth=512 -I $(DICPP)/include $(DICPP)/lib/src/*.cpp $(DICPP)/lib/src/scopes/*.cpp -lboost_system-mt -lboost_thread-mt dicpp.cpp -o dicpp.out && strip dicpp.out && ls -lh dicpp.out #time ./dicpp.out 
	#cd create_$* && time $(CXX) -O2 -std=c++11 -ftemplate-depth=2048 -DFRUIT_NO_LOOP_CHECK -I $(GOOGLE_FRUIT)/include -I $(GOOGLE_FRUIT)/build/include -L $(GOOGLE_FRUIT)/build/src -lfruit fruit.cpp -o fruit.out && strip fruit.out && ls -lh fruit.out #time LD_LIBRARY_PATH=$(GOOGLE_FRUIT)/build/src ./fruit.out
#	cd create_$* && rm -rf dagger2; mkdir dagger2 && time javac -d dagger2 -cp .:`ls ../../frameworks/dagger2/*.jar | xargs | tr ' ' ':'` dagger2.java && cd dagger2 && java -cp .:`ls ../$(DAGGER2)/*.jar | xargs | tr ' ' ':'` dagger2
#	cd create_$* && rm -rf guice; mkdir guice && time javac -cp .:`ls $(GOOGLE_GUICE)/*.jar | xargs | tr ' ' ':'` guice.java -d guice #&& cd guice && time java -cp .:`ls ../$(GOOGLE_GUICE)/*.jar | xargs | tr ' ' ':'` guice
#	cd create_$* &&	time mcs /r:$(NINJECT)/Ninject.dll ninject.cs #&& time mono ninject.exe

