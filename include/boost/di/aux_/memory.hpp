//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MEMORY_HPP
#define BOOST_DI_AUX_MEMORY_HPP

#if !defined(BOOST_DI_CFG_STD_SMART_PTR) && !defined(BOOST_DI_CFG_BOOST_SMART_PTR)
    #define BOOST_DI_CFG_STD_SMART_PTR
#endif

#if defined(BOOST_DI_CFG_STD_SMART_PTR)

    #include <memory>
    #if !defined(BOOST_DI_CFG_NO_BOOST)
        #include <boost/shared_ptr.hpp>
    #endif

    namespace boost {
    namespace di {
    namespace aux {
        using ::std::auto_ptr;
        using ::std::unique_ptr;
        using ::std::shared_ptr;
        using ::std::weak_ptr;
    } // namespace aux

    namespace aux_ {
        #if !defined(BOOST_DI_CFG_NO_BOOST)
            using ::boost::shared_ptr;
        #endif
    } // namespace aux_

    } // namespace boost
    } // namespace di

#elif defined(BOOST_DI_CFG_BOOST_SMART_PTR)

    #include <memory>
    #if !defined(BOOST_DI_CFG_NO_BOOST)
        #include <boost/shared_ptr.hpp>
        #include <boost/weak_ptr.hpp>
    #endif

    namespace boost {
    namespace di {
    namespace aux {
        using ::std::auto_ptr;
        using ::std::unique_ptr;

        #if !defined(BOOST_DI_CFG_NO_BOOST)
            using ::boost::shared_ptr;
            using ::boost::weak_ptr;
        #endif
    } // namespace aux

    namespace aux_ {
        using ::std::shared_ptr;
    } // namespace aux_

} // namespace boost
} // namespace di

#endif

#endif

