clean_guice:
	rm -rf guice data/*.class *.class

build_guice: clean_guice
	mkdir guice
	javac -cp .:`ls ../../frameworks/google.guice/*.jar | xargs | tr ' ' ':'` guice.java data/data_inject_guice.java -d guice

run_guice:
	cd guice && java -cp .:`ls ../../../frameworks/google.guice/*.jar | xargs | tr ' ' ':'` guice

clean_di:
	rm -f di.out

build_di:
	clang++ -std=c++1y -I ../../frameworks/boost.di/include -include data/data_inject_di.hpp di.cpp -o di.out

run_di:
	./di.out

clean_di_auto:
	rm -f di_auto.out

build_di_auto:
	clang++ -std=c++1y -I ../../frameworks/boost.di/include -include data/data.hpp -O2 di.cpp -o di_auto.out

run_di_auto:
	./di_auto.out

clean_dicpp:
	rm -f dicpp.out

build_dicpp:
	clang++ dicpp.cpp -O2 -I ../../frameworks/dicpp/include -include data/data_inject_dicpp.hpp -std=c++11 ../../frameworks/dicpp/lib/src/*.cpp ../../frameworks/dicpp/lib/src/scopes/*.cpp -pthread -lboost_system -lboost_thread -o dicpp.out

run_dicpp:
	./dicpp.out

clean_fruit:
	rm -f fruit.out

build_fruit:
	clang++ fruit.cpp -O2 -I ../../frameworks/google.fruit/include -include data/data_inject_fruit.hpp -std=c++11 ../../frameworks/google.fruit/src/*.cpp -ftemplate-depth=2000 -o fruit.out

run_fruit:
	./fruit.out

clean_dagger2:
	rm -rf dagger2 data/*.class *.class

build_dagger2: clean_dagger2
	mkdir dagger2
	javac -d dagger2 -cp .:`ls ../../frameworks/dagger2/*.jar | xargs | tr ' ' ':'` dagger2.java data/data_inject_dagger2.java

run_dagger2:
	cd dagger2 && java -cp .:`ls ../../../frameworks/dagger2/*.jar | xargs | tr ' ' ':'` dagger2

clean_ninject:
	rm -f *.exe

build_ninject: clean_dagger2
	mcs /r:../../frameworks/ninject/Ninject.dll ninject.cs data/data.cs
	#csc /r:../../frameworks/ninject/Ninject.dll ninject.cs data/data.cs

run_ninject:
	./ninject.exe

clean: clean_guice clean_dagger2 clean_di clean_di_auto clean_dicpp clean_fruit

