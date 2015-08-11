//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

#ifdef _LIBCPP_VERSION // __pph__
_LIBCPP_BEGIN_NAMESPACE_STD
#else // __pph__
namespace std {
#endif // __pph__
    template<class> class shared_ptr;
    template<class> class weak_ptr;
    template<class, class> class unique_ptr;
    template<class> struct char_traits;
#ifdef _LIBCPP_VERSION // __pph__
_LIBCPP_END_NAMESPACE_STD
#else // __pph__
} // std
#endif // __pph__

namespace std {
    template<class> class initializer_list;
} // std

namespace boost {
    template<class> class shared_ptr;
} // boost

namespace boost { namespace di { inline namespace v1 {
    struct no_name { constexpr auto operator()() const noexcept { return ""; } };
    template<class, class = void> struct ctor_traits;
    namespace core { template<class> struct any_type_fwd; template<class> struct any_type_ref_fwd; }
    namespace detail { template<class, class> struct named_type; template<class, class...> class injector; }
}}} // boost::di::v1

#endif

