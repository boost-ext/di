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

	$CXX -E $1 -I$tmp -include $2 | grep -v "^#" | cat -s

    rm -rf $tmp
}

guard_begin() {
    echo "//"
    echo "// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)"
    echo "//"
    echo "// Distributed under the Boost Software License, Version 1.0."
    echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
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
    cat /tmp/i.hpp | grep -v preprocess | sort -u -r
}
files="
boost/di/type_traits/remove_accessors.hpp
boost/di/type_traits/make_plain.hpp
boost/di/type_traits/is_same_base_of.hpp
boost/di/type_traits/parameter_types.hpp
boost/di/type_traits/has_ctor.hpp
boost/di/type_traits/ctor_traits.hpp
boost/di/type_traits/create_traits.hpp
boost/di/named.hpp
boost/di/convertibles/shared.hpp
boost/di/convertibles/copy.hpp
boost/di/convertibles/value.hpp
boost/di/convertibles/reference.hpp
boost/di/scopes/shared.hpp
boost/di/scopes/unique.hpp
boost/di/scopes/deduce.hpp
boost/di/scopes/external.hpp
boost/di/scopes/session.hpp
boost/di/type_traits/scope_traits.hpp
boost/di/concepts/type_traits/is_req_type.hpp
boost/di/concepts/type_traits/is_req_name.hpp
boost/di/concepts/type_traits/is_req_call.hpp
boost/di/concepts/bind.hpp
boost/di/concepts/scope.hpp
boost/di/concepts/dependency.hpp
boost/di/detail/binder.hpp
boost/di/policies/arguments_permission.hpp
boost/di/policies/creation_ownership.hpp
boost/di/policies/binding_correctness.hpp
boost/di/policies/circular_dependencies.hpp
boost/di/policies/scopes_permission.hpp
boost/di/detail/pool.hpp
boost/di/detail/creator.hpp
boost/di/detail/module.hpp
boost/di/concepts.hpp
boost/di/inject.hpp
boost/di/provider.hpp
boost/di/injector.hpp
boost/di/make_injector.hpp
"

generate_preprocessed() {
    rm -rf boost/di/aux_/preprocessed/di.hpp /tmp/i.hpp /tmp/f.hpp
    mkdir -p boost/di/aux_/preprocessed/

    guard_begin > boost/di/aux_/preprocessed/di.hpp
    for file in $files; do
        #new_file=`echo $file | sed "s/di\//$2\//g"`

		if [[ "$file" =~ "di/aux_" ]] ||
		   [[ "$file" =~ "inject.hpp" ]] ||
		   [[ "$file" =~ "di.hpp" ]]; then
			continue;
		fi

	    echo $file
		#mkdir -p `dirname $new_file` 2>/dev/null
		dump_file $file $3 $2 >> /tmp/f.hpp
    done
    includes >> boost/di/aux_/preprocessed/di.hpp
    cat /tmp/f.hpp >> boost/di/aux_/preprocessed/di.hpp

    guard_end >> boost/di/aux_/preprocessed/di.hpp
}

dir=`readlink -f \`dirname $0\``
cd $dir/../include && generate_preprocessed "boost" "di\/aux_\/preprocessed" "$dir/config.hpp"

