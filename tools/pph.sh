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
    echo "#ifndef BOOST_DI_HPP"
    echo "#define BOOST_DI_HPP"
    echo "#if (__cplusplus < 201305L && _MSC_VER < 1900)"
    echo "#error \"Boost.DI requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)\""
    echo "#elif defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)"
    echo "#include \"boost/di/config.hpp"\"
    echo "#include \"boost/di/bindings.hpp"\"
    echo "#include \"boost/di/inject.hpp\""
    echo "#include \"boost/di/injector.hpp\""
    echo "#include \"boost/di/make_injector.hpp\""
    echo "#include \"boost/di/scopes/deduce.hpp\""
    echo "#include \"boost/di/scopes/external.hpp\""
    echo "#include \"boost/di/scopes/exposed.hpp\""
    echo "#include \"boost/di/scopes/singleton.hpp\""
    echo "#include \"boost/di/scopes/unique.hpp\""
    echo "#include \"boost/di/policies/constructible.hpp\""
    echo "#include \"boost/di/providers/heap.hpp\""
    echo "#include \"boost/di/providers/stack_over_heap.hpp\""
    echo "#else"
    echo "#include <type_traits>"
    rm -rf tmp && mkdir tmp && cp -r boost tmp && cd tmp && touch type_traits
    find . -iname "*.hpp" | xargs sed -i "s/\(.*\)__wknd__/\/\/\/\/\1/"
    tail -n +10 "boost/di/aux_/compiler.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +11 "boost/di/aux_/preprocessor.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +10 "boost/di/fwd.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +11 "boost/di/aux_/utility.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +14 "boost/di/aux_/type_traits.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    tail -n +15 "boost/di/inject.hpp" | head -n -3 | sed '/^$/d' | sed "s/ \/\/\\(.*\)//g"
    g++ -std=c++1y -C -P -nostdinc -nostdinc++ -E -I. "boost/di.hpp" \
        -D"BOOST_DI_HAS_METHOD(name, call_name)=template<class T, class... TArgs> decltype(std::declval<T>().call_name(std::declval<TArgs>()...), std::true_type()) has_##name##_impl(int); template<class, class...> std::false_type has_##name##_impl(...); template<class T, class... TArgs> struct has_##name : decltype(has_##name##_impl<T, TArgs...>(0)) { }" \
        -D"BOOST_DI_HAS_TYPE(name)=template<class, class = void> struct has_##name : std::false_type { }; template<class T> struct has_##name<T, typename aux::void_t<typename T::name>::type> : std::true_type { }" \
        -DBOOST_DI_CFG_NO_PREPROCESSED_HEADERS \
        -DBOOST_DI_AUX_COMPILER_HPP \
        -DBOOST_DI_AUX_PREPROCESSOR_HPP \
        -DBOOST_DI_AUX_TYPE_TRAITS_HPP \
        -DBOOST_DI_AUX_UTILITY_HPP \
        -DBOOST_DI_FWD_HPP \
        -DBOOST_DI_INJECT_HPP 2>/dev/null | sed "s/\/\/\/\///" | g++ -P -E -I. -fpreprocessed -
    cd .. && rm -rf tmp
    echo "#endif"
    echo "#endif"
    echo
}

cd "`readlink -f \`dirname $0\``/../include" && pph > "boost/di.hpp"

