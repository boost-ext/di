//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_SHARED_HPP
#define QDEPS_BACK_SHARED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace QDeps
{
namespace Back
{

class Shared
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

public:
    template<typename T, typename Enable = void> struct Make
    {
        typedef boost::shared_ptr<T> type;
    };

    template<typename T>
    struct Make<T, typename boost::enable_if< has_element_type<T> >::type>
    {
        typedef T type;
    };

    template<typename T, typename Enable = void> struct Deref
    {
        typedef T type;
    };

    template<typename T> struct Deref<T, typename boost::enable_if< has_element_type<T> >::type>
    {
        typedef typename T::element_type type;
    };

    template<typename> class Convert;
};

template<typename TDest> class Shared::Convert
{
public:
    template<typename TSrc>
    static TDest execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }
};

template<typename TDest> class Shared::Convert<TDest&>
{
public:
    template<typename TSrc>
    static TDest& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }
};

template<typename TDest> class Shared::Convert<const TDest&>
{
public:
    template<typename TSrc>
    static const TDest& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }
};

template<typename TDest> class Shared::Convert<TDest*>
{
public:
    template<typename TSrc>
    static TDest* execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src.get();
    }
};

template<typename TDest> class Shared::Convert<const TDest*>
{
public:
    template<typename TSrc>
    static const TDest* execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src.get();
    }
};

template<typename TDest> class Shared::Convert< boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc>
    static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src;
    }
};

template<typename TDest> class Shared::Convert<const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc>
    static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src;
    }
};

template<typename TDest> class Shared::Convert<boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc>
    static boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src;
    }
};

} // namespace Back
} // namespace QDeps

#endif

