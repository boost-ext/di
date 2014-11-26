//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_PROVIDERS_NOTHROW_HPP
#define BOOST_DI_PROVIDERS_NOTHROW_HPP

#include <utility>
#include <new>
#include <boost/di/aux_/utility.hpp>

namespace boost {
namespace di {
namespace providers {

template<typename TDeps>
class nothrow {
public:
    explicit nothrow(TDeps& deps)
        : deps(deps)
    { }

    template<class TDependency, class... TArgs>
	aux::enable_if_t<
        //std::is_polymorphic<typename TDependency::given>::value &&
        !std::is_base_of<core::dependency<scopes::deduce, typename TDependency::given>, TDeps>::value
      , typename TDependency::given*
    >
    get(TArgs&&... args) const noexcept {
        return new (std::nothrow) typename TDependency::given{std::forward<TArgs>(args)...};
    }

    //singleton requires ptr
   /* template<class TDependency, class... TArgs>*/
	//aux::enable_if_t<
        //!std::is_polymorphic<typename TDependency::given>::value &&
        //!std::is_base_of<core::dependency<scopes::deduce, typename TDependency::given>, TDeps>::value
      //, typename TDependency::given&&
    //>
    //get(TArgs&&... args) const noexcept {
		//return std::move(typename TDependency::given{std::forward<TArgs>(args)...});
	/*}*/

    template<class TDependency, class... TArgs>
    aux::enable_if_t<
        std::is_base_of<core::dependency<scopes::deduce, typename TDependency::given>, TDeps>::value
      , typename TDependency::given*
    >
    get(TArgs&&...) const noexcept {
        return static_cast<core::dependency<scopes::deduce, typename TDependency::given>&>(deps).ptr;
    }

private:
    TDeps& deps;
};

} // namespace providers
} // namespace di
} // namespace boost

#endif

