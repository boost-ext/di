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

    cat $1 | grep "#include" | while read include; do
        if [[ "$include" =~ "boost/di/aux_" ]]; then
            echo -e "$include" >> $tmp_dir/includes.hpp
        elif ! [[ "$include" =~ "boost/di" ]]; then
            echo -e "$include" >> $tmp_dir/includes.hpp
        fi

        file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`
        mkdir -p $tmp/`dirname $file`
        touch $tmp/$file
    done

    file="boost/config.hpp"
    mkdir -p $tmp/`dirname $file`
    touch $tmp/$file

    mkdir -p $tmp/`dirname $1`
    cp $1 $tmp/$1

    $CXX -nostdinc -CC -E $1 -I$tmp $args | grep -v "^#" | sed '/\/\*.*\*\// d; /\/\*/,/\*\// d' | grep -v "^/" | cat -s

    rm -rf $tmp
}

guard_begin() {
    echo "//"
    echo "// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)"
    echo "//"
    echo "// Distributed under the Boost Software License, Version 1.0."
    echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
    echo "//"

    echo "#ifndef BOOST_DI_HPP"
    echo "#define BOOST_DI_HPP"
    echo
    echo "#include \"boost/di/inject.hpp\""
    echo
    echo "#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)"
    echo
    echo "// annotations"
    echo "#include \"boost/di/named.hpp\""
    echo
    echo "// bindings"
    echo "#include \"boost/di/bindings.hpp"\"
    echo
    echo "// defaults"
    echo "#include \"boost/di/defaults.hpp"\"
    echo
    echo "// injectors"
    echo "#include \"boost/di/injector.hpp\""
    echo "#include \"boost/di/make_injector.hpp\""
    echo
    echo "// scopes"
    echo "#include \"boost/di/scopes/deduce.hpp\""
    echo "#include \"boost/di/scopes/external.hpp\""
    echo "#include \"boost/di/scopes/exposed.hpp\""
    echo "#include \"boost/di/scopes/session.hpp\""
    echo "#include \"boost/di/scopes/singleton.hpp\""
    echo "#include \"boost/di/scopes/shared.hpp\""
    echo "#include \"boost/di/scopes/unique.hpp\""
    echo
    echo "#else"
    echo
}

guard_end() {
    echo
    echo "#endif"
    echo
    echo "#endif"
    echo
}

includes() {
    cat $tmp_dir/includes.hpp | sort -u -r
}

genereate_files() {
    cat $1 | grep "#include" | grep "boost\/di" | while read include; do
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

generate_preprocessed() {
    args=${@:4}
    name="boost/di.hpp"

    rm -f $tmp_dir/includes.hpp $tmp_dir/dump.hpp

    guard_begin $name > $name

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
    includes >> $name
    cat $tmp_dir/dump.hpp >> $name

    guard_end >> $name
}

generate_pph() {
    generate_preprocessed "boost" "" "$1" ${@:2}
    echo "done -> $name"
}

generate_cpp_11() {
    generate_pph `readlink -f $PWD`/boost/di/aux_/config.hpp -DBOOST_DI_INJECTOR=BOOST_DI_INJECTOR -DBOOST_DI_CFG_CTOR_LIMIT_SIZE=BOOST_DI_CFG_CTOR_LIMIT_SIZE
}

cleanup() {
    rm -rf $tmp_dir
}

dir=`readlink -f \`dirname $0\``
cd $dir/../include

genereate_files "boost/di.hpp" | cat -n | sort -uk2 | sort -nk1 | cut -f2- > $tmp_dir/files.hpp

generate_cpp_11

cleanup

