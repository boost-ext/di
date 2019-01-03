#!/bin/bash
#
# Copyright (c) 2012-2019 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

bind_interfaces() {
  for ((i=1; i<=$1; ++i)); do
    echo "#define MODULE$i"
    echo "#define BIND_INTERFACES$i(...) __VA_ARGS__"
    echo "#define BIND_OTHERS$i(...) "
  done
}

bind_others() {
  for ((i=1; i<=$1; ++i)); do
    echo "#define MODULE$i"
    echo "#define BIND_INTERFACES$i(...)"
    echo "#define NO_CTOR_INTERFACES$i"
    echo "#define BIND_OTHERS$i(...) __VA_ARGS__"
  done
}

bind_interfaces_others() {
  for ((i=1; i<=$1; ++i)); do
    if [ "$2" -ge "$1" ]; then
      echo "#define MODULE$i"
      echo "#define BIND_INTERFACES$i(...) __VA_ARGS__,"
      echo "#define BIND_OTHERS$i(...) __VA_ARGS__"
    else
      echo "#define MODULE$i"
      echo "#define BIND_INTERFACES$i(...) __VA_ARGS__"
      echo "#define BIND_OTHERS$i(...)"
    fi
  done
}

bind_all() {
  for ((i=1; i<=$1; ++i)); do
    echo "#define MODULE$i"
    echo "#define BIND_INTERFACES$i(...) __VA_ARGS__,"
    echo "#define BIND_OTHERS$i(...) __VA_ARGS__"
  done
}

benchmark() {
  rm -f /tmp/$0.hpp /tmp/$0.dat 2>/dev/null
  [ "$2" == "ctor" ] && echo -e "#undef BOOST_DI_INJECT\n#define BOOST_DI_INJECT(type, ...) type(__VA_ARGS__)" > /tmp/$0.hpp
  [ "$3" == "auto" ] && echo "#define EXPOSED_OR_AUTO(t1, t2) t2" >> /tmp/$0.hpp || echo "#define EXPOSED_OR_AUTO(t1, t2) t1" >> /tmp/$0.hpp
  echo "`$4`" >> /tmp/$0.hpp
  (time $CXX di.cpp -I ../../include -Wall -Werror $CXXFLAGS $CXXINC "boost/di.hpp" $CXXINC /tmp/$0.hpp $5 -DCOMPLEX=$1) 2> /tmp/$0.dat
  if [[ "`grep error: /tmp/$0.dat`" != "" ]]; then >&2 cat /tmp/$0.dat; exit; fi
  cat /tmp/$0.dat | grep real | awk '{print $2}' | sed "s/0m\(.*\)s/\1/" | tr '\n' ' '
}

graph() {
  rm -f $1.pg $1.dat
  `echo $1` > $1.dat
  echo -e "set terminal png
       set xlabel \"Number of bindings\"
       set ylabel \"Time [s]\"
       set title \"$2\"
       set key reverse Left outside
       set grid
       set style data linespoints
       plot \"$1.dat\" \
          using 1:2 title \"ctor/all\" \
       , \"\" using 1:3 title \"inject/all\" \
       , \"\" using 1:4 title \"ctor/exposed\" \
       , \"\" using 1:5 title \"inject/exposed\"
  " > $1.pg
  gnuplot $1.pg > $1.png
}

small_complexity() {
  for ((i=0; i<=10; ++i)); do
    echo -n "$((i*20)) "
    benchmark small_complexity ctor auto "bind_others $i"
    benchmark small_complexity inject auto "bind_others $i"
    benchmark small_complexity ctor exposed "bind_others $i"
    benchmark small_complexity inject exposed "bind_others $i"
    echo
  done
}

medium_complexity() {
  for ((i=0; i<=10; ++i)); do
    echo -n "$((i*20)) "
    benchmark medium_complexity ctor auto "bind_others $i"
    benchmark medium_complexity inject auto "bind_others $i"
    benchmark medium_complexity ctor exposed "bind_others $i"
    benchmark medium_complexity inject exposed "bind_others $i"
    echo
  done
}

big_complexity() {
  for ((i=0; i<=10; ++i)); do
    n=$((100+(i*20)));
    echo -n "$n "
    benchmark big_complexity ctor auto "bind_interfaces_others 10 $i" "-ftemplate-depth=$((n+10))"
    benchmark big_complexity ctor exposed "bind_interfaces_others 10 $i"
    benchmark big_complexity inject auto "bind_interfaces_others 10 $i" "-ftemplate-depth=$((n+10))"
    benchmark big_complexity inject exposed "bind_interfaces_others 10 $i"
    echo
  done
}

verify() {
  echo -n "$1[$MAX] "
  [ $(echo "$1 < $MAX" | bc) -ne 1 ] && exit -1
}

quick() {
  verify `benchmark small_complexity ctor auto "bind_others 200"`
  verify `benchmark small_complexity ctor exposed "bind_others 200"`
  verify `benchmark small_complexity inject auto "bind_others 200"`
  verify `benchmark small_complexity inject exposed "bind_others 200"`
  exit 0
}

[[ -z "$CXX" ]] && CXX="clang++"
[[ -z "$CXXFLAGS" ]] && CXXFLAGS="-O2 -std=c++1y"
[[ -z "$CXXINC" ]] && CXXINC="-include"
[[ -z "$MAX" ]] && MAX="10.0"
[[ -z "$COMPLEXITY" ]] && COMPLEXITY="small,medium,big"

[[ $COMPLEXITY == *"small"* ]] && graph small_complexity "Small complexity | $CXX $CXXFLAGS"
[[ $COMPLEXITY == *"medium"* ]] && graph medium_complexity "Medium complexity | $CXX $CXXFLAGS"
[[ $COMPLEXITY == *"big"* ]] &&  graph big_complexity "Big complexity | $CXX $CXXFLAGS"
[[ $COMPLEXITY == *"quick"* ]] && quick

