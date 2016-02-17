clean_guice:
	rm -rf guice *.class

build_guice: clean_guice
	mkdir guice
	javac -cp .:`ls ../../frameworks/google.guice/*.jar | xargs | tr ' ' ':'` guice.java -d guice

run_guice:
	cd guice && java -cp .:`ls ../../../frameworks/google.guice/*.jar | xargs | tr ' ' ':'` guice

clean_di:
	rm -f di.out

build_di:
	clang++ -std=c++1y -I ../../frameworks/boost.di/include -O2 di.cpp -o di.out

asm_di:
	gdb -batch -ex 'file ./di.out' -ex 'disassemble main'

run_di:
	./di.out

clean_di_auto:
	rm -f di_auto.out

build_di_auto:
	clang++ -std=c++1y -I ../../frameworks/boost.di/include -O2 di.cpp -o di_auto.out

asm_di_auto:
	gdb -batch -ex 'file ./di_auto.out' -ex 'disassemble main'

run_di_auto:
	./di_auto.out

clean_dicpp:
	rm -f dicpp.out

build_dicpp:
	clang++ dicpp.cpp -O2 -I ../../frameworks/dicpp/include -std=c++11 ../../frameworks/dicpp/lib/src/*.cpp ../../frameworks/dicpp/lib/src/scopes/*.cpp -pthread -lboost_system -lboost_thread -o dicpp.out

asm_dicpp:
	gdb -batch -ex 'file ./dicpp.out' -ex 'disassemble main'

run_dicpp:
	./dicpp.out

clean_fruit:
	rm -f fruit.out

build_fruit:
	clang++ fruit.cpp -O2 -I ../../frameworks/google.fruit/include -std=c++11 ../../frameworks/google.fruit/src/*.cpp -ftemplate-depth=2000 -o fruit.out

asm_fruit:
	gdb -batch -ex 'file ./fruit.out' -ex 'disassemble main'

run_fruit:
	./fruit.out

clean_dagger2:
	rm -rf dagger2 *.class

build_dagger2: clean_dagger2
	mkdir dagger2
	javac -d dagger2 -cp .:`ls ../../frameworks/dagger2/*.jar | xargs | tr ' ' ':'` dagger2.java

run_dagger2:
	cd dagger2 && java -cp .:`ls ../../../frameworks/dagger2/*.jar | xargs | tr ' ' ':'` dagger2

clean_ninject:
	rm -f *.exe

build_ninject: clean_dagger2
	mcs /r:../../frameworks/ninject/Ninject.dll ninject.cs

run_ninject:
	./ninject.exe

clean: clean_guice clean_dagger2 clean_di clean_di_auto clean_dicpp clean_fruit

