//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_EXAMPLES_utILITY_HPP
#define DI_EXAMPLES_utILITY_HPP

#if defined(__GNUC__) && !defined(__llvm__)
# include <cxxabi.h>
# include <boost/shared_ptr.hpp>
#endif

inline std::string demangle(const std::string& mangled)
{
#if defined(__GNUC__) && !defined(__llvm__)
    char* demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, 0);

    if (l_demangled)
    {
        boost::shared_ptr<char> result(l_demangled, std::free);
        return std::string(l_demangled);
    }
#endif

    return mangled;
}

#endif

