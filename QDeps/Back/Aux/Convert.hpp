//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_CONVERT_HPP
#define QDEPS_BACK_AUX_CONVERT_HPP

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template<typename TDest> class Convert
{
public:
    template<typename TSrc> static TDest execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static TDest execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
    template<typename TSrc> static TDest execute(std::auto_ptr<TSrc> p_src) { return *p_src.release(); }
};

template<typename TDest> class Convert<TDest&>
{
public:
    template<typename TSrc> static TDest& execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
    template<typename TSrc> static TDest& execute(std::auto_ptr<TSrc> p_src) { return *p_src.release(); }
};

template<typename TDest> class Convert<const TDest&>
{
public:
    template<typename TSrc> static const TDest& execute(TSrc p_src) { return p_src; }
    template<typename TSrc> static const TDest& execute(const boost::shared_ptr<TSrc>& p_src) { return *p_src; }
    template<typename TSrc> static const TDest& execute(std::auto_ptr<TSrc> p_src) { return *p_src.release(); }
};

template<typename TDest> class Convert<TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src) { return &p_src; }
    template<typename TSrc> static TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
    template<typename TSrc> static TDest* execute(std::auto_ptr<TSrc> p_src) { return p_src.release(); }
};

template<typename TDest> class Convert<const TDest*>
{
public:
    template<typename TSrc> static TDest* execute(TSrc p_src) { return &p_src; }
    template<typename TSrc> static const TDest* execute(const boost::shared_ptr<TSrc>& p_src) { return p_src.get(); }
    template<typename TSrc> static const TDest* execute(std::auto_ptr<TSrc> p_src) { return p_src.release(); }
};

template<typename TDest> class Convert< boost::shared_ptr<TDest> >
{
public:
    template<typename TSrc> static boost::shared_ptr<TDest> execute(TSrc p_src) { return boost::make_shared<TDest>(p_src); }
    template<typename TSrc> static boost::shared_ptr<TDest> execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
    template<typename TSrc> static boost::shared_ptr<TDest> execute(std::auto_ptr<TSrc> p_src) { return boost::shared_ptr<TSrc>(p_src.release()); }
};

template<typename TDest> class Convert<const boost::shared_ptr<TDest>&>
{
public:
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(TSrc p_src) { return boost::make_shared<TDest>(p_src); }
    template<typename TSrc> static const boost::shared_ptr<TDest>& execute(const boost::shared_ptr<TSrc>& p_src) { return p_src; }
    template<typename TSrc> static boost::shared_ptr<TDest> execute(std::auto_ptr<TSrc> p_src) { return boost::shared_ptr<TSrc>(p_src.release()); }
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

