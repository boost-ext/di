//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_CONVERT_HPP
#define QDEPS_BACK_AUX_CONVERT_HPP

#include <boost/shared_ptr.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template<typename TDest> class Convert
{
public:
    template<typename TSrc> static TDest execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TDest> class Convert<TDest&>
{
public:
    template<typename TSrc> static TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TDest> class Convert<const TDest&>
{
public:
    template<typename TSrc> static const TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
};

template<typename TDest> class Convert<TDest*>
{
public:
    template<typename TSrc> static TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
};

template<typename TDest> class Convert<const TDest*>
{
public:
    template<typename TSrc> static const TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
};

template<typename TDest> class Convert< boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
};

template<typename TDest> class Convert<const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
};

template<typename TDest> class Convert<boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

