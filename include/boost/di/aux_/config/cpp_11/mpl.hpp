//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_CPP_11_MPL_HPP
#define BOOST_DI_AUX_CONFIG_CPP_11_MPL_HPP

#include <boost/mpl/x11/accumulate.hpp>
#include <boost/mpl/x11/advance.hpp>
#include <boost/mpl/x11/and.hpp>
#include <boost/mpl/x11/apply.hpp>
#include <boost/mpl/x11/assert.hpp>
#include <boost/mpl/x11/at.hpp>
#include <boost/mpl/x11/aux_/yes_no.hpp>
#include <boost/mpl/x11/back_inserter.hpp>
#include <boost/mpl/x11/begin_end.hpp>
#include <boost/mpl/x11/bool.hpp>
#include <boost/mpl/x11/contains.hpp>
#include <boost/mpl/x11/copy.hpp>
#include <boost/mpl/x11/count.hpp>
#include <boost/mpl/x11/count_if.hpp>
#include <boost/mpl/x11/deref.hpp>
#include <boost/mpl/x11/empty.hpp>
#include <boost/mpl/x11/equal.hpp>
#include <boost/mpl/x11/fold.hpp>
#include <boost/mpl/x11/front.hpp>
#include <boost/mpl/x11/greater.hpp>
#include <boost/mpl/x11/has_xxx.hpp>
#include <boost/mpl/x11/if.hpp>
#include <boost/mpl/x11/integral_c.hpp>
#include <boost/mpl/x11/int.hpp>
#include <boost/mpl/x11/is_sequence.hpp>
#include <boost/mpl/x11/iterator_range.hpp>
#include <boost/mpl/x11/joint_view.hpp>
#include <boost/mpl/x11/limits/vector.hpp>
#include <boost/mpl/x11/max_element.hpp>
#include <boost/mpl/x11/max.hpp>
#include <boost/mpl/x11/not.hpp>
#include <boost/mpl/x11/or.hpp>
#include <boost/mpl/x11/pair.hpp>
#include <boost/mpl/x11/placeholders.hpp>
#include <boost/mpl/x11/plus.hpp>
#include <boost/mpl/x11/pop_front.hpp>
#include <boost/mpl/x11/push_back.hpp>
#include <boost/mpl/x11/size.hpp>
#include <boost/mpl/x11/times.hpp>
#include <boost/mpl/x11/transform.hpp>
#include <boost/mpl/x11/transform_view.hpp>
#include <boost/mpl/x11/vector.hpp>
#include <boost/mpl/x11/vector/vector50.hpp>
#include <boost/mpl/x11/void.hpp>

namespace boost {
namespace mpl {
namespace x11 {
    typedef arg<0> _1;
    typedef arg<1> _2;
}}}

namespace boost {
namespace di {
namespace aux {
    namespace mpl = ::boost::mpl::;
}}}

#endif

