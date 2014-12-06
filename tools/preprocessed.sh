#!/bin/bash
#
# Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

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
    echo "// injections"
    echo "#include \"boost/di/inject.hpp\""
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

genereate_pph() {
    echo $1 >> $tmp_dir/pph.hpp
    cat $1 | grep "#include" | grep "boost\/di" | while read include; do
        file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`

        if [[ "`cat $tmp_dir/pph.hpp | grep $file`" == "" ]]; then
            genereate_pph $file
            cat $file | egrep "^#include" | grep -v  "boost\/di" >> $tmp_dir/includes.hpp
            tail -n +10 $file | head -n -3 | egrep -v "^#include"
        fi
    done
}

[ -z "$CXX" ] && CXX="g++"
tmp_dir=`mktemp -d`
dir=`readlink -f \`dirname $0\``
cd $dir/../include
guard_begin > $tmp_dir/di.hpp
genereate_pph "boost/di.hpp" >> $tmp_dir/tmp.hpp
cat $tmp_dir/includes.hpp | sort -u >> $tmp_dir/di.hpp
cat $tmp_dir/tmp.hpp >> $tmp_dir/di.hpp
guard_end >> $tmp_dir/di.hpp
cp $tmp_dir/di.hpp  $dir/../include/boost/di.hpp

rm -rf $tmp_dir

