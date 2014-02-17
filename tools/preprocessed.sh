#!/bin/bash

dump_file() {
    guard=`echo $1 | tr [:lower:] [:upper:] | sed "s/\//_/g" | sed "s/\./_/g"`

    echo "//"
    echo "// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)"
    echo "//"
    echo "// Distributed under the Boost Software License, Version 1.0."
    echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
    echo "//"

    echo "#ifndef $guard"
    echo "#define $guard"
    echo

    tmp=`mktemp`
    rm -rf $tmp
    mkdir $tmp

    cat $1 | grep "#include" | grep -v PP_ | grep -v "preprocess" | while read include; do
		if [[ "$include" =~ "di/aux_" ]] ||
		   [[ "$include" =~ "inject.hpp" ]] ||
		   [[ "$include" =~ "di.hpp" ]]; then
			echo $include
		else
			echo $include | sed "s/boost\/di\//boost\/$3\//g"
		fi

		file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`
		mkdir -p $tmp/`dirname $file`
		touch $tmp/$file
	done

	mkdir -p $tmp/`dirname $1`
	cp $1 $tmp/$1

	$CXX -E $1 -I$tmp -include $2 | grep -v "^#" | cat -s

    rm -rf $tmp

    echo
    echo "#endif"
    echo
}

generate_preprocessed() {
    rm -rf boost/$2

    for file in `find $1 -iname "*.hpp"`; do
        new_file=`echo $file | sed "s/di\//$2\//g"`

		if [[ "$file" =~ "di/aux_" ]] ||
		   [[ "$file" =~ "inject.hpp" ]] ||
		   [[ "$file" =~ "di.hpp" ]]; then
			continue;
		fi

		echo $new_file
		mkdir -p `dirname $new_file` 2>/dev/null
		dump_file $file $3 $2 > $new_file

    done
}

dir=`readlink -f \`dirname $0\``
cd $dir/../include && generate_preprocessed "boost" "di\/aux_\/preprocessed" "$dir/config.hpp"

