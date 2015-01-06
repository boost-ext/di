#!/bin/bash

#$1=1-10
bind_interfaces() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__ -DBIND_OTHERS$i(...)= "
    done
}

#$1=1-10
bind_others() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=  -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

#$1=1-10
bind_all() {
    for ((i=0; i<$1; ++i)); do
        echo -n "-DMODULE$i -DBIND_INTERFACES$i(...)=__VA_ARGS__, -DBIND_OTHERS$i(...)=__VA_ARGS__ "
    done
}

#$1=small_complex/medium_complex/huge_complex, $2=ctor/inject, $3=auto/exposed
benchmark() {
    echo -n "$1 [$2, $3, $4] : "
    CTOR=`[ "$2" == "ctor" ] && echo -n "-DBOOST_DI_INJECT(type, ...)=type(__VA_ARGS__)"`
    EXPOSED_OR_AUTO=`[ "$3" == "auto" ] && echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t2" || echo -n "-DEXPOSED_OR_AUTO(t1, t2)=t1"`
    time clang++ -O2 di.cpp -std=c++1y -I boost/libs/di/include "$CTOR" "$EXPOSED_OR_AUTO" `$4` -DCOMPLEX=$1
}

benchmark small_complex ctor auto "bind_all 10"

