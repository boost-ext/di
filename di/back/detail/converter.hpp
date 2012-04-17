//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_BACK_DETAIL_CONVERTER_HPP
#define DI_BACK_DETAIL_CONVERTER_HPP

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "di/utility/named.hpp"

namespace di
{
namespace Back
{
namespace Detail
{

template<typename, typename> class Converter;

template<typename TScope, typename TDest>
class ConverterImpl : public boost::static_visitor<TDest>
{
public:
    template<typename T> TDest operator()(T p_member) const
    {
        return Converter<TScope, TDest>::execute(p_member);
    }
};

template<typename TScope, typename TDest>
class Converter
{
public:
    template<typename TSrc> static TDest execute(const TSrc& p_src)
    {
        return p_src;
    }

    template<typename TSrc> static TDest execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, TDest>(), p_src);
    }
};

template<typename TScope, typename TDest, typename TName>
class Converter<TScope, Utility::Named<TDest, TName> >
{
public:
    template<typename TSrc> static Utility::Named<TDest, TName> execute(const TSrc& p_src)
    {
        return Utility::Named<TDest, TName>(p_src);
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static Utility::Named<TDest, TName> execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, Utility::Named<TDest, TName> >(), p_src);
    }
};

template<typename TScope, typename TDest>
class Converter<TScope, TDest&>
{
public:
    template<typename TSrc> static TDest& execute(const TSrc& p_src)
    {
        return p_src;
    }

    template<typename TSrc> static TDest& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, TDest&>(), p_src);
    }
};

template<typename TScope, typename TDest>
class Converter<TScope, const TDest&>
{
public:
    template<typename TSrc> static const TDest& execute(const TSrc& p_src)
    {
        return p_src;
    }

    template<typename TSrc> static const TDest& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return *p_src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, const TDest&>(), p_src);
    }
};

template<typename TScope, typename TDest>
class Converter<TScope, TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src)
    {
        return &p_src;
    }

    template<typename TSrc> static TDest* execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src.get();
    }
};

template<typename TScope, typename TDest>
class Converter<TScope, const TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src)
    {
        return &p_src;
    }

    template<typename TSrc> static const TDest* execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src.get();
    }
};

template<typename TScope, typename TDest>
class Converter<TScope,  boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest> execute(const TSrc& p_src)
    {
        return boost::make_shared<TDest>(p_src);
    }

    template<typename TSrc> static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, boost::shared_ptr<TDest> >(), p_src);
    }
};

template<typename TScope, typename TDest>
class Converter<TScope, const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const TSrc& p_src)
    {
        return boost::make_shared<TDest>(p_src);
    }

    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src)
    {
        return p_src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)> static TDest execute(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& p_src)
    {
        return boost::apply_visitor(ConverterImpl<TScope, boost::shared_ptr<TDest> >(), p_src);
    }
};

} // namespace Detail
} // namespace Back
} // namespace di

#endif

