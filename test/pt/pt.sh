#!/bin/bash

bind_interfaces() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__ -DBIND_OTHERS$i(...)= "
    done
}

bind_others() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=  -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

bind_all() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__, -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

benchmark() {
    echo -n "$1 [$2, $3, $4] : "
    CTOR=`[ "$2" == "ctor" ] && echo -n "-DBOOST_DI_INJECT(type, ...)=type(__VA_ARGS__)"`
    EXPOSED_OR_AUTO=`[ "$3" == "auto" ] && echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t2" || echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t1"`
    (time clang++ -O2 di.cpp -std=c++1y -I ../../include "$CTOR" "$EXPOSED_OR_AUTO" `$4` -DCOMPLEX=$1) #|& grep real | awk '{print $2}'
}

for ((i=1; i<10; ++i)); do
    benchmark small_complex ctor auto "bind_others $i"
done

for ((i=1; i<10; ++i)); do
    benchmark small_complex inject auto "bind_others $i"
done

for ((i=1; i<10; ++i)); do
    benchmark small_complex ctor exposed "bind_others $i"
done

for ((i=1; i<10; ++i)); do
    benchmark small_complex ctor exposed "bind_others $i"
done

for ((i=1; i<10; ++i)); do
    benchmark small_complex inject exposed "bind_others $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex ctor auto "bind_interfaces $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex inject auto "bind_interfaces $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex ctor exposed "bind_interfaces $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex inject exposed "bind_interfaces $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex ctor auto "bind_all $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex inject auto "bind_all $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex ctor exposed "bind_all $i"
done

for ((i=1; i<10; ++i)); do
    benchmark medium_complex inject exposed "bind_all $i"
done

benchmark huge_complex ctor auto "bind_interfaces 10"
benchmark huge_complex ctor exposed "bind_interfaces 10"
benchmark huge_complex inject auto "bind_interfaces 10"
benchmark huge_complex inject exposed "bind_interfaces 10"

benchmark huge_complex ctor auto "bind_all 10"
benchmark huge_complex ctor exposed "bind_all 10"
benchmark huge_complex inject auto "bind_all 10"
benchmark huge_complex inject exposed "bind_all 10"

