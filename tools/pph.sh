#!/bin/bash
#
# Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

pph() {
    echo "//"
    echo "// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)"
    echo "//"
    echo "// Distributed under the Boost Software License, Version 1.0."
    echo "// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
    echo "//"
    echo "#pragma once"
    echo "#if (__cplusplus < 201305L && _MSC_VER < 1900)"
    echo "#error \"Boost.DI requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)\""
    echo "#else"
    echo "#if defined(__clang__)"
    echo "    #pragma clang diagnostic push"
    echo "#elif defined(__GNUC__)"
    echo "    #pragma GCC diagnostic push"
    echo "#elif defined(_MSC_VER)"
    echo "    #pragma warning(push)"
    echo "#endif"
    # BOOST_DI_VERSION % 100 is the patch level
    # BOOST_DI_VERSION / 100 % 1000 is the minor version
    # BOOST_DI_VERSION / 100000 is the major version
    echo "#define BOOST_DI_VERSION 100000"
    rm -rf tmp && mkdir tmp && cp -r boost tmp && cd tmp && touch type_traits
    find . -iname "*.hpp" | xargs sed -i "s/\(.*\)__pph__/\/\/\/\/\1/"
    tail -n +10 "boost/di/aux_/compiler.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"

    echo '#include "boost/di/config.hpp"
          #include "boost/di/bindings.hpp"
          #include "boost/di/inject.hpp"
          #include "boost/di/injector.hpp"
          #include "boost/di/make_injector.hpp"
          #include "boost/di/scopes/deduce.hpp"
          #include "boost/di/scopes/external.hpp"
          #include "boost/di/scopes/exposed.hpp"
          #include "boost/di/scopes/singleton.hpp"
          #include "boost/di/scopes/unique.hpp"
          #include "boost/di/policies/constructible.hpp"
          #include "boost/di/providers/heap.hpp"
          #include "boost/di/providers/stack_over_heap.hpp"' > tmp.hpp

    g++ -std=c++1y -C -P -nostdinc -nostdinc++ -E -I. \
        -DBOOST_DI_AUX_COMPILER_HPP \
        -DBOOST_DI_AUX_PREPROCESSOR_HPP \
        -DBOOST_DI_INJECT_HPP tmp.hpp 2>/dev/null | \
            sed "s/\/\/\/\///" | \
            sed "s/[ $]*#define/##define/g" | \
            g++ -P -E -I. -fpreprocessed - 2>/dev/null | \
            sed "s/^##define/#define/g"
    tail -n +10 "boost/di/aux_/preprocessor.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +15 "boost/di/inject.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    cd .. && rm -rf tmp
    echo "#endif"
    echo "#if defined(__clang__)"
    echo "    #pragma clang diagnostic pop"
    echo "    #pragma clang diagnostic warning \"-Wdeprecated-declarations\""
    echo "#elif defined(__GNUC__)"
    echo "    #pragma GCC diagnostic pop"
    echo "#elif defined(_MSC_VER)"
    echo "    #pragma warning(push)"
    echo "#endif"
    echo
}

cd "`readlink -f \`dirname $0\``/../include" && pph > "boost/di.hpp"

