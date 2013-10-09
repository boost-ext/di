//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_MEMORY_HPP
#define BOOST_DI_MEMORY_HPP

#if defined(BOOST_DI_BOOST_SMART_PTR)

    #include <memory>
    #include <boost/shared_ptr.hpp>
    #include <boost/make_shared.hpp>
    #include <boost/weak_ptr.hpp>

    namespace boost {
    namespace di {

        using std::auto_ptr;
        using boost::shared_ptr;
        using boost::weak_ptr;

        #if !defined(BOOST_NO_CXX11_SMART_PTR)

        //auto_ptr
        //shared_ptr
        //weak_ptr
        //unique_ptr

    } // namespace di
    } // namespace boost

#endif

#if defined(BOOST_DI_STD_SMART_PTR)

#include <memory>

        using std::auto_ptr;
        using std::shared_ptr;
        using std::weak_ptr;
        using std::unique_ptr;


#if defined(BOOST_DI_STD_TR1_SMART_PTR)

#include <memory>
#include <tr1/memory>

    using std::auto_ptr;
    using std::tr1::shared_ptr;
    using std::tr1::weak_ptr;
    using std::tr1::unique_ptr;

#endif

