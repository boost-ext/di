//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MEMORY_HPP
#define BOOST_DI_AUX_MEMORY_HPP

#include <boost/config.hpp>

#if !defined(BOOST_DI_STD_SMART_PTR) && \
    !defined(BOOST_DI_TR1_SMART_PTR) && \
    !defined(BOOST_DI_BOOST_SMART_PTR)

    #if !defined(BOOST_NO_CXX11_SMART_PTR)
        #define BOOST_DI_STD_SMART_PTR
    #elif defined(BOOST_HAS_TR1_SHARED_PTR)
        #define BOOST_DI_TR1_SMART_PTR
    #else
        #define BOOST_DI_BOOST_SMART_PTR
    #endif
#endif

#if defined(BOOST_DI_STD_SMART_PTR)

    #include <memory>
    #include <boost/shared_ptr.hpp>
    #include <boost/weak_ptr.hpp>

    namespace boost {
    namespace di {
    namespace aux {

    using ::std::auto_ptr;
    using ::std::unique_ptr;
    using ::std::shared_ptr;
    using ::std::weak_ptr;

    } // namespace aux
    } // namespace boost
    } // namespace di

#elif defined(BOOST_DI_TR1_SMART_PTR)

    #include <memory>

    #if !defined(BOOST_MSVC)
        #include <tr1/memory>
    #endif

    namespace boost {
    namespace di {
    namespace aux {

    using ::std::auto_ptr;
    template<typename> struct unique_ptr; // compile clean
    using ::std::tr1::shared_ptr;
    using ::std::tr1::weak_ptr;

    } // namespace aux
    } // namespace boost
    } // namespace di

#else

    #include <memory>
    #include <boost/shared_ptr.hpp>
    #include <boost/weak_ptr.hpp>

    namespace boost {
    namespace di {
    namespace aux {

    using ::std::auto_ptr;
    template<typename> struct unique_ptr; // compile clean
    using ::boost::shared_ptr;
    using ::boost::weak_ptr;

    } // namespace aux
    } // namespace boost
    } // namespace di

#endif

#endif

