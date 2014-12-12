#!/bin/bash
#
# Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

main() {
    genereate_pph() {
        echo $1 >> $2/tmp.hpp
        cat $1 | egrep "^#include" | grep "boost\/di" | while read include; do
            file=`echo $include | sed "s/[^<^\"]*[\"<]\([^>\"]*\)[\">].*/\1/"`

            if [[ "`cat $2/tmp.hpp | grep $file`" == "" ]]; then
                genereate_pph $file $2
                cat $file | egrep "^#include" | grep -v "boost\/di" >> $2/includes.hpp
                echo >> $2/pph.hpp
                tail -n +8 $file | head -n -3 | egrep -v "^#include" | cat -s >> $2/pph.hpp
            fi
        done
    }

    tmp_dir=`mktemp -d`

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
    echo "// config"
    echo "#include \"boost/di/config.hpp"\"
    echo
    echo "// annotations"
    echo "#include \"boost/di/named.hpp\""
    echo
    echo "// bindings"
    echo "#include \"boost/di/bindings.hpp"\"
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
    genereate_pph "boost/di.hpp" "$tmp_dir"
    cat $tmp_dir/includes.hpp | sort -u
    cat $tmp_dir/pph.hpp
    echo
    echo "#endif"
    echo
    echo "#endif"
    echo

    rm -rf $tmp_dir
}

cd "`readlink -f \`dirname $0\``/../include" && main "boost/di.hpp" > "boost/di.hpp"

