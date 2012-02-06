//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_CTOR_HPP
#define QDEPS_CTOR_HPP

#ifdef __GNUC__
# pragma GCC system_header
#endif

#define QDEPS_CTOR(Type, ...)\
    template<typename T>\
    struct Ctor__\
    {\
        typedef T type;\
    };\
\
    typedef Ctor__<void (__VA_ARGS__)> ctor__;\
\
    Type(__VA_ARGS__)

#endif

