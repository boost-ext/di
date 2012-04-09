//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_DETAIL_CONVERTER_HPP
#define QDEPS_BACK_DETAIL_CONVERTER_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "QDeps/Utility/Named.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{

template<typename TScope, typename TDest> class Converter
{
public:
    template<typename TSrc> static TDest execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static TDest execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TScope, typename TDest, typename TName> class Converter<TScope, Utility::Named<TDest, TName> >
{
public:
    template<typename TSrc> static Utility::Named<TDest, TName> execute(TSrc p_src) { return Utility::Named<TDest, TName>(p_src); }
};

template<typename TScope, typename TDest> class Converter<TScope, TDest&>
{
public:
    template<typename TSrc> static TDest& execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TScope, typename TDest> class Converter<TScope, const TDest&>
{
public:
    template<typename TSrc> static const TDest& execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static const TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TScope, typename TDest> class Converter<TScope, TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src) { return &p_src; }
    template<typename TSrc> static TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
};

template<typename TScope, typename TDest> class Converter<TScope, const TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src) { return &p_src; }
    template<typename TSrc> static const TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
};

template<typename TScope, typename TDest> class Converter<TScope,  boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest> execute(TSrc p_src) { return boost::make_shared<TDest>(p_src); }
    template<typename TSrc> static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
};

template<typename TScope, typename TDest> class Converter<TScope, const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(TSrc p_src) { return boost::make_shared<TDest>(p_src); }
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
};

} // namespace Detail
} // namespace Back
} // namespace QDeps

#endif

