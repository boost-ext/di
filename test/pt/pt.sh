#!/bin/bash

bind_interfaces() {
    for ((i=1; i<=$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__ -DBIND_OTHERS$i(...)= "
    done
}

bind_others() {
    for ((i=1; i<=$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=  -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

bind_interfaces_others() {
    for ((i=1; i<=$1; ++i)); do
        if [ "$2" -ge "$1" ]; then
            echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__, -DBIND_OTHERS$i(...)=__VA_ARGS__ "
        else
            echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__ -DBIND_OTHERS$i(...)= "
        fi
    done
}

bind_all() {
    for ((i=1; i<=$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__, -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

benchmark() {
    CTOR=`[ "$2" == "ctor" ] && echo -n "-DBOOST_DI_INJECT(type, ...)=type(__VA_ARGS__)"`
    EXPOSED_OR_AUTO=`[ "$3" == "auto" ] && echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t2" || echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t1"`
    (time clang++ -O2 di.cpp -std=c++1y -I ../../include "$CTOR" "$EXPOSED_OR_AUTO" `$4` -DCOMPLEX=$1) |& grep real | awk '{print $2}' | sed "s/0m\(.*\)s/\1/" | xargs -i% echo -n "% "
}

graph() {
    rm -f $1.pg $1.dat
    echo -e "#!/usr/bin/gnuplot
             set terminal png
             set xlabel \"Number of bindings\"
             set ylabel \"Time [s]\"
             set title \"$1\"
             set key reverse Left outside
             set grid
             set style data linespoints
             plot \"$1.dat\" \
                  using 1:2 title \"ctor/auto\" \
           , \"\" using 1:3 title \"inject/auto\" \
           , \"\" using 1:4 title \"ctor/exposed\" \
           , \"\" using 1:5 title \"inject/exposed\"
    " > $1.pg
    `echo $1` > $1.dat
    chmod +x $1.pg
    ./$1.pg > $1.png
}

small_complex() {
    for ((i=0; i<=10; ++i)); do
        echo -n "$((i*20)) "
        benchmark small_complex ctor auto "bind_others $i"
        benchmark small_complex inject auto "bind_others $i"
        benchmark small_complex ctor exposed "bind_others $i"
        benchmark small_complex inject exposed "bind_others $i"
        echo
    done
}

medium_complex() {
    for ((i=0; i<=10; ++i)); do
        echo -n "$((i*20)) "
        benchmark medium_complex ctor auto "bind_others $i"
        benchmark medium_complex inject auto "bind_others $i"
        benchmark medium_complex ctor exposed "bind_others $i"
        benchmark medium_complex inject exposed "bind_others $i"
        echo
    done
}

huge_complex() {
    for ((i=0; i<=10; ++i)); do
        echo -n "$((100+(i*20))) "
        benchmark huge_complex ctor auto "bind_interfaces_others 10 $i"
        benchmark huge_complex ctor exposed "bind_interfaces_others 10 $i"
        benchmark huge_complex inject auto "bind_interfaces_others 10 $i"
        benchmark huge_complex inject exposed "bind_interfaces_others 10 $i"
        echo
    done
}

graph small_complex #4248897537 instances | 132 types | 10 modules
graph medium_complex #1862039751439806464 instances | 200 types | 10 modules
graph huge_complex #5874638529236910091 instances | 310 types | 100 interfaces | 10 modules

