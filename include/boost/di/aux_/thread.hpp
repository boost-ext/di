//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_THREAD_HPP
#define BOOST_DI_AUX_THREAD_HPP

//#define BOOST_DI_STD_THREAD

#if defined(BOOST_DI_STD_THREAD)
    #include <thread>

    namespace boost {
    namespace di {

    namespace this_thread = ::std::this_thread;
    using ::std::thread;

    } // namespace di
    } // namespace boost

#else

    #include <boost/thread.hpp>

    namespace boost {
    namespace di {

    namespace this_thread = ::boost::this_thread;
    using ::boost::thread;

    } // namespace di
    } // namespace boost

#endif

#endif

