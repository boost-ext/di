//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

namespace std {
    template<class> class shared_ptr;
    template<class> class weak_ptr;
    template<class, class> class unique_ptr;
    template<class> struct char_traits;
    template<class> class initializer_list;
} // std

namespace boost {
    template<class> class shared_ptr;
} // boost

namespace boost { namespace di { inline namespace v1 {
    class config;
    struct no_name { constexpr auto operator()() const noexcept { return ""; } };
    template<class...> class injector;
    template<class, class = void> struct ctor_traits;
    namespace providers { class heap; class stack_over_heap; }
    namespace core { template<class> struct any_type_fwd; template<class> struct any_type_ref_fwd; }
}}} // boost::di::v1

#endif

