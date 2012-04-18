//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BACK_DETAIL_CONVERTER_HPP
#define BOOST_DI_BACK_DETAIL_CONVERTER_HPP

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "di/utility/named.hpp"

namespace di
{
namespace back
{
namespace detail
{

template<typename, typename> class converter;

template<typename TScope, typename TDest>
class converterImpl : public boost::static_visitor<TDest>
{
public:
    template<typename T> TDest operator()(T member) const
    {
        return converter<TScope, TDest>::execute(member);
    }
};

template<typename TScope, typename TDest>
class converter
{
public:
    template<typename TSrc> static TDest execute(const TSrc& src)
    {
        return src;
    }

    template<typename TSrc> static TDest execute(const boost::shared_ptr<TSrc>& src)
    {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, TDest>(), src);
    }
};

template<typename TScope, typename TDest, typename TName>
class converter<TScope, utility::named<TDest, TName> >
{
public:
    template<typename TSrc> static utility::named<TDest, TName> execute(const TSrc& src)
    {
        return utility::named<TDest, TName>(src);
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static utility::named<TDest, TName> execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, utility::named<TDest, TName> >(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, TDest&>
{
public:
    template<typename TSrc> static TDest& execute(const TSrc& src)
    {
        return src;
    }

    template<typename TSrc> static TDest& execute(const boost::shared_ptr<TSrc>& src)
    {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, TDest&>(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const TDest&>
{
public:
    template<typename TSrc> static const TDest& execute(const TSrc& src)
    {
        return src;
    }

    template<typename TSrc> static const TDest& execute(const boost::shared_ptr<TSrc>& src)
    {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, const TDest&>(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc src)
    {
        return &src;
    }

    template<typename TSrc> static TDest* execute(const boost::shared_ptr<TSrc>& src)
    {
        return src.get();
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc src)
    {
        return &src;
    }

    template<typename TSrc> static const TDest* execute(const boost::shared_ptr<TSrc>& src)
    {
        return src.get();
    }
};

template<typename TScope, typename TDest>
class converter<TScope,  boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest> execute(const TSrc& src)
    {
        return boost::make_shared<TDest>(src);
    }

    template<typename TSrc> static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& src)
    {
        return src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, boost::shared_ptr<TDest> >(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const TSrc& src)
    {
        return boost::make_shared<TDest>(src);
    }

    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& src)
    {
        return src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src)
    {
        return boost::apply_visitor(converterImpl<TScope, boost::shared_ptr<TDest> >(), src);
    }
};

} // namespace detail
} // namespace back
} // namespace di

#endif

