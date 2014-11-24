//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
// // Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/dependency.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace core {

template<typename TDeps>
struct binder {
	template<typename>
	struct get_name {
		using type = no_name;
	};

	template<typename T, typename TName>
	struct get_name<named<T, TName>> {
		using type = TName;
	};

	template<typename T>
	using get_name_t = typename get_name<T>::type;

	template<typename T>
	using rebind_t = typename T::template rebind<typename scopes::deduce::rebind<T>::other>::other;

    template<typename T, typename TDefault = dependency<scopes::deduce, aux::make_plain_t<T>>>
    using resolve = rebind_t<
        at_key_t<TDefault, dependency_concept<aux::make_plain_t<T>, get_name_t<aux::remove_accessors_t<T>>>, TDeps>
	>;
};

} // namespace core
} // namespace di
} // namespace boost

#endif

