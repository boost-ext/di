//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_THREAD_HPP
#define BOOST_DI_AUX_THREAD_HPP

#include <boost/config.hpp>

#if !defined(BOOST_DI_STD_THREAD) &&   \
    !defined(BOOST_DI_BOOST_THREAD) && \
    !defined(BOOST_DI_NO_THREAD)

    #if (__cplusplus >= 201100L) || defined(BOOST_MSVC)
        #define BOOST_DI_STD_THREAD
    #else
        #define BOOST_DI_BOOST_THREAD
    #endif
#endif

#if defined(BOOST_DI_STD_THREAD)
    #include <thread>
    #include <mutex>

    namespace boost {
    namespace di {
    namespace aux {

    namespace this_thread = ::std::this_thread;

    using ::std::thread;
    using ::std::mutex;

    template<typename T>
    struct tss
    {
        typedef thread_local aux::shared_ptr<T> type;
    };

    typedef ::std::lock_guard< ::std::mutex > scoped_lock;

    } // namespace aux
    } // namespace boost
    } // namespace di

#elif defined(BOOST_DI_BOOST_THREAD)

    #include <boost/thread.hpp>
    #include <boost/thread/tss.hpp>

    namespace boost {
    namespace di {
    namespace aux {

    namespace this_thread = ::boost::this_thread;
    using ::boost::thread;
    using ::boost::mutex;
    using ::boost::thread_specific_ptr;
    typedef ::boost::lock_guard< ::boost::mutex > scoped_lock;

    } // namespace aux
    } // namespace boost
    } // namespace di

#else

    namespace boost {
    namespace di {
    namespace aux {

    struct thread
    {
        struct id { };
    };

    struct mutex { };

    struct thread_specific_ptr { };

    template<typename T>
    struct lock_guard
    {
        lock_guard(const T&);
    };

    typedef lock_guard<mutex> scoped_lock;

    namespace this_thread {
    thread::id get_id();
    }

    } // namespace aux
    } // namespace boost
    } // namespace di

#endif

#endif

