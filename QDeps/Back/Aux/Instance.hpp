//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_INSTANCE_HPP
#define QDEPS_BACK_AUX_INSTANCE_HPP

#include <boost/mpl/vector.hpp>

namespace QDeps
{
namespace Back
{
namespace Aux
{

template<typename T, typename TContext = boost::mpl::vector0<> >
class Instance
{
public:
    explicit Instance(const T& p_instance)
        : m_instance(p_instance)
    { }

    const T& get() const { return m_instance; }

private:
    const T& m_instance;
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

