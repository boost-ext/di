//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONFIG_CPP_11_MPL_HPP
#define BOOST_DI_AUX_CONFIG_CPP_11_MPL_HPP

#include <boost/mpl/if.hpp>
//#include <boost/mpl/x11/accumulate.hpp>
#include <boost/mpl/x11/advance.hpp>
#include <boost/mpl/x11/range_c.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/x11/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/x11/at.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
//#include <boost/mpl/x11/back_inserter.hpp>
#include <boost/mpl/x11/begin_end.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/x11/contains.hpp>
#include <boost/mpl/x11/copy.hpp>
#include <boost/mpl/x11/count.hpp>
//#include <boost/mpl/x11/count_if.hpp>
#include <boost/mpl/x11/deref.hpp>
//#include <boost/mpl/x11/empty.hpp>
#include <boost/mpl/x11/equal.hpp>
#include <boost/mpl/x11/fold.hpp>
//#include <boost/mpl/x11/front.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/x11/if.hpp>
//#include <boost/mpl/x11/integral_c.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/x11/is_sequence.hpp>
#include <boost/mpl/x11/iterator_range.hpp>
#include <boost/mpl/x11/joint_view.hpp>
#include <boost/mpl/x11/min_max_element.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/x11/pair.hpp>
#include <boost/mpl/plus.hpp>
//#include <boost/mpl/x11/pop_front.hpp>
//#include <boost/mpl/x11/push_back.hpp>
#include <boost/mpl/x11/size.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/x11/transform.hpp>
#include <boost/mpl/x11/transform_view.hpp>
#include <boost/mpl/x11/vector.hpp>
#include <boost/mpl/x11/void.hpp>

namespace boost {
namespace mpl {

struct string_tag;
template<typename> struct c_str;

namespace x11 {
    typedef arg<0> _1;
    typedef arg<1> _2;
    typedef arg<-1> _;
    using ::boost::mpl::true_;
    using ::boost::mpl::false_;
    using ::boost::mpl::integral_c;
    using ::boost::mpl::string_tag;
    using ::boost::mpl::c_str;

    namespace aux {
        using ::boost::mpl::aux::yes_tag;
        using ::boost::mpl::aux::no_tag;
    }
}}}

namespace boost {
namespace di {
namespace aux {
    namespace mpl = ::boost::mpl::x11;
}}}

#endif

