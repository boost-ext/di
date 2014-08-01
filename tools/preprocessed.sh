#!/bin/bash
#
# Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

[ -z "$CXX" ] && CXX="g++"
tmp_dir=`mktemp -d`

dump_file() {
    args=${@:4}
    tmp=`mktemp -d`

    cat $1 | grep "#include" | grep -v PP_ | grep -v "preprocess" | while read include; do
        if [[ "$include" =~ "di/aux_/config.hpp" ]]; then
            echo
        elif [[ "$include" =~ "di/aux_" ]] ||
           [[ "$include" =~ "inject.hpp" ]] ||
           [[ "$include" =~ "di.hpp" ]]; then
            echo $include >> $tmp_dir/includes.hpp
        else
            echo $include | sed "s/boost\/di\//boost\/$3\//g" >> $tmp_dir/includes.hpp
        fi

        file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`
        mkdir -p $tmp/`dirname $file`
        touch $tmp/$file
    done

    mkdir -p $tmp/`dirname $1`
    cp $1 $tmp/$1

    $CXX -E $1 -CC -I$tmp -include $2 $args | grep -v "^#" | sed '/\/\*.*\*\// d; /\/\*/,/\*\// d' | grep -v "^/" | cat -s

    rm -rf $tmp
}

guard_begin() {
    echo "//"
    echo "// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)"
    echo "//"
    echo "// Distributed under the Boost Software License, Version 1.0."
    echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
    echo "//"
    echo "// Preprocessed version of \"boost::di\""
    echo "// DO NOT modify by hand!"
    echo "//"

    echo "#ifndef BOOST_DI_PREPROCESSED_`echo $1 | tr '[:lower:]' '[:upper:]' | tr '.' '_'`"
    echo "#define BOOST_DI_PREPROCESSED_`echo $1 | tr '[:lower:]' '[:upper:]' | tr '.' '_'`"
    echo
}

guard_end() {
    echo
    echo "#endif"
    echo
}

includes() {
    echo "#include \"boost/di/aux_/config.hpp\"" >> $tmp_dir/includes.hpp
    cat $tmp_dir/includes.hpp | grep -v preprocess | sort -u -r
}

genereate_files() {
    cat $1 | grep "#include" | grep "boost\/di" | grep -v PP_ | grep -v "preprocess" | while read include; do
        file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`
        if [[ "$file" =~ "di/aux_" ]] ||
           [[ "$file" =~ "inject.hpp" ]] ||
           [[ "$file" =~ "di.hpp" ]]; then
            continue;
        fi

        genereate_files $file
        echo $file
    done
}

generate_name() {
    echo " BOOST_DI_CFG_CTOR_LIMIT_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE STD" > $tmp_dir/name.hpp
    $CXX -E -P $@ $tmp_dir/name.hpp | tr ' ' '_' | xargs -i% echo di%_c.hpp
}

generate_preprocessed() {
    args=${@:4}
    name=`generate_name $args`

    rm -f $tmp_dir/includes.hpp $tmp_dir/dump.hpp
    mkdir -p boost/di/preprocessed 2>/dev/null

    guard_begin $name > boost/di/preprocessed/$name

    echo -n .
    for file in `cat $tmp_dir/files.hpp`; do
        if [[ "$file" =~ "di/aux_" ]] ||
           [[ "$file" =~ "inject.hpp" ]] ||
           [[ "$file" =~ "di.hpp" ]]; then
            continue;
        fi

        echo -n .
        dump_file $file $3 $2 $args >> $tmp_dir/dump.hpp
    done
    includes >> boost/di/preprocessed/$name
    cat $tmp_dir/dump.hpp >> boost/di/preprocessed/$name

    guard_end >> boost/di/preprocessed/$name
}

generate_pph() {
    generate_preprocessed "boost" "di\/preprocessed" "$1" ${@:2}
    echo "done -> boost/di/preprocessed/$name"
}

generate_cpp_11() {
    generate_pph `readlink -f $PWD`/boost/di/aux_/cpp_11/config.hpp -DBOOST_DI_CFG_CTOR_LIMIT_SIZE='' -DBOOST_MPL_LIMIT_VECTOR_SIZE='' -DBOOST_DI_INJECTOR=boost_di_injector__ -DSTD=cpp_11
}

generate_cpp_03() {
    generate_pph `readlink -f $PWD`/boost/di/aux_/cpp_03/config.hpp -DBOOST_DI_CFG_CTOR_LIMIT_SIZE=$1 -DBOOST_MPL_LIMIT_VECTOR_SIZE=$2 -DBOOST_DI_INJECTOR=boost_di_injector__ -DSTD=cpp_03
}

cleanup() {
    rm -rf $tmp_dir
}

dir=`readlink -f \`dirname $0\``
cd $dir/../include

genereate_files "boost/di.hpp" | cat -n | sort -uk2 | sort -nk1 | cut -f2- > $tmp_dir/files.hpp

generate_cpp_11
#generate_cpp_03 10 10
#generate_cpp_03 10 20
#generate_cpp_03 10 30
#generate_cpp_03 10 40
#generate_cpp_03 10 50

cleanup

