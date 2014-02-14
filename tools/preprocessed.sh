#!/bin/bash

dump_file() {
    guard=`echo $1 | tr [:lower:] [:upper:] | sed "s/\//_/g" | sed "s/\./_/g"`
    echo "#ifndef $guard"
    echo "#define $guard"
    echo

    cat $1 | grep "#include" | grep -v PP_ | grep -v "boost/preprocessor" | sed "s/boost\/di\//boost\/di_preprocessed\//g"

    tmp=`mktemp`
    rm -rf $tmp
    mkdir $tmp

    for include in `cat $1 | grep "#include" | grep -v PP_ | grep -v "boost/preprocessor" | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`; do
        mkdir -p $tmp/`dirname $include`
        touch $tmp/$include
    done

    mkdir -p $tmp/`dirname $1`
    cp $1 $tmp/$1

    $CXX -E $1 -I$tmp -include $2 | grep -v "^#"

    rm -rf $tmp

    echo
    echo "#endif"
    echo
}

generate_preprocessed() {
    rm -rf boost/$2

    for file in `find $1 -iname "*.hpp"`; do
        new_file=`echo $file | sed "s/di\//di_preprocessed\//g"`

        echo $new_file
        mkdir -p `dirname $new_file` 2>/dev/null

        if [[ "$file" =~ "di.hpp" ]]; then
            continue;
        fi

        if [[ "$file" =~ "inject.hpp" ]] ||
           [[ "$file" =~ "common.hpp" ]] ||
           [[ "$file" =~ "memory.hpp" ]]; then
           cp $file $new_file
        else 
            dump_file $file $3 > $new_file
        fi

    done
}

dir=`dirname $0`
cd $dir/../include && generate_preprocessed "boost" "di_preprocessed" "$dir/config.hpp"

