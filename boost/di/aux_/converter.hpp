//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONVERTER_HPP
#define BOOST_DI_AUX_CONVERTER_HPP

#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace aux_ {

template<typename, typename>
class converter;

template<typename TScope, typename TDest>
class converter_impl
    : public static_visitor<TDest>
{
public:
    template<typename T>
    TDest operator()(T object) const {
        return converter<TScope, TDest>::execute(object);
    }
};

template<typename TScope, typename TDest>
class converter
{
public:
    template<typename TSrc>
    static TDest execute(const TSrc& src) {
        return src;
    }

    template<typename TSrc>
    static TDest execute(const shared_ptr<TSrc>& src) {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static TDest execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, TDest>(), src);
    }
};

template<typename TScope, typename TDest, typename TName>
class converter<TScope, named<TDest, TName> >
{
public:
    template<typename TSrc>
    static named<TDest, TName> execute(const TSrc& src) {
        return named<TDest, TName>(make_shared<TSrc>(src));
    }

    template<typename TSrc>
    static named<TDest, TName> execute(const boost::shared_ptr<TSrc>& src) {
        return named<TDest, TName>(src);
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static named<TDest, TName> execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, named<TDest, TName> >(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, TDest&>
{
public:
    template<typename TSrc>
    static TDest& execute(const TSrc& src) {
        return src;
    }

    template<typename TSrc>
    static TDest& execute(const shared_ptr<TSrc>& src) {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static TDest execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, TDest&>(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const TDest&>
{
public:
    template<typename TSrc>
    static const TDest& execute(const TSrc& src) {
        return src;
    }

    template<typename TSrc>
    static const TDest& execute(const shared_ptr<TSrc>& src) {
        return *src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static TDest execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, const TDest&>(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, TDest*>
{
public:
    template<typename TSrc>
    static TDest* execute(TSrc src) {
        return &src;
    }

    template<typename TSrc>
    static TDest* execute(const shared_ptr<TSrc>& src) {
        return src.get();
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const TDest*>
{
public:
    template<typename TSrc>
    static TDest* execute(TSrc src) {
        return &src;
    }

    template<typename TSrc>
    static const TDest* execute(const shared_ptr<TSrc>& src) {
        return src.get();
    }
};

template<typename TScope, typename TDest>
class converter<TScope,  shared_ptr<TDest> >
{
public:
    template<typename TSrc>
    static shared_ptr<TDest> execute(const TSrc& src) {
        return make_shared<TDest>(src);
    }

    template<typename TSrc>
    static shared_ptr<TDest> execute(const shared_ptr<TSrc>& src) {
        return src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static shared_ptr<TDest> execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, shared_ptr<TDest> >(), src);
    }
};

template<typename TScope, typename TDest>
class converter<TScope, const shared_ptr<TDest>&>
{
public:
    template<typename TSrc>
    static const shared_ptr<TDest>& execute(const TSrc& src) {
        return make_shared<TDest>(src);
    }

    template<typename TSrc>
    static const shared_ptr<TDest>& execute(const shared_ptr<TSrc>& src) {
        return src;
    }

    template<BOOST_VARIANT_ENUM_PARAMS(typename T)>
    static TDest execute(const variant<BOOST_VARIANT_ENUM_PARAMS(T)>& src) {
        return apply_visitor(converter_impl<TScope, shared_ptr<TDest> >(), src);
    }
};

} // namespace aux_
} // namespace di
} // namespace boost

#endif

