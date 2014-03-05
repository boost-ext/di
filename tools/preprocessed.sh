#!/bin/bash

dump_file() {
    tmp=`mktemp`
    rm -rf $tmp
    mkdir $tmp

    cat $1 | grep "#include" | grep -v PP_ | grep -v "preprocess" | while read include; do
		if [[ "$include" =~ "di/aux_/config.hpp" ]]; then
            echo
        elif [[ "$include" =~ "di/aux_" ]] ||
		   [[ "$include" =~ "inject.hpp" ]] ||
		   [[ "$include" =~ "di.hpp" ]]; then
			echo $include >> /tmp/i.hpp
		else
			echo $include | sed "s/boost\/di\//boost\/$3\//g" >> /tmp/i.hpp
		fi

		file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`
		mkdir -p $tmp/`dirname $file`
		touch $tmp/$file
	done

	mkdir -p $tmp/`dirname $1`
	cp $1 $tmp/$1

	$CXX -E $1 -CC -I$tmp -include $2 | grep -v "^#" | sed '/\/\*.*\*\// d; /\/\*/,/\*\// d' | grep -v "^/" | cat -s

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

    echo "#ifndef BOOST_DI_AUX_PREPROCESSED_DI_HPP"
    echo "#define BOOST_DI_AUX_PREPROCESSED_DI_HPP"
    echo
}

guard_end() {
    echo
    echo "#endif"
    echo
}

includes() {
    echo "#include \"boost/di/aux_/config.hpp\"" >> /tmp/i.hpp
    cat /tmp/i.hpp | grep -v preprocess | sort -u -r
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

generate_preprocessed() {
    rm -rf boost/di/aux_/preprocessed/di.hpp /tmp/i.hpp /tmp/f.hpp
    mkdir -p boost/di/aux_/preprocessed/

    guard_begin > boost/di/aux_/preprocessed/di.hpp

    echo -n .
    for file in `genereate_files "boost/di.hpp" | cat -n | sort -uk2 | sort -nk1 | cut -f2-`; do
		if [[ "$file" =~ "di/aux_" ]] ||
		   [[ "$file" =~ "inject.hpp" ]] ||
		   [[ "$file" =~ "di.hpp" ]]; then
			continue;
		fi

        echo -n .
		dump_file $file $3 $2 >> /tmp/f.hpp
    done
    includes >> boost/di/aux_/preprocessed/di.hpp
    cat /tmp/f.hpp >> boost/di/aux_/preprocessed/di.hpp

    guard_end >> boost/di/aux_/preprocessed/di.hpp
}

dir=`readlink -f \`dirname $0\``
cd $dir/../include && generate_preprocessed "boost" "di\/aux_\/preprocessed" "$dir/config.hpp"
echo done

