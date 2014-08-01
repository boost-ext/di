//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_VISITOR_HPP
#define BOOST_DI_FAKE_VISITOR_HPP

#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <vector>
#include <string>
#include <boost/utility/enable_if.hpp>

#include <iostream>
#include <boost/units/detail/utility.hpp>

namespace boost {
namespace di {

template<typename TSeq = aux::mpl::vector<>>
class fake_visitor
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(first)

    struct element
    {
        element(const std::type_info* type, const std::type_info* scope)
            : type(type), scope(scope)
        { }

        const std::type_info* type = nullptr;
        const std::type_info* scope = nullptr;
    };

    typedef std::vector<element> visits_type;

public:
    ~fake_visitor() {
        BOOST_CHECK_EQUAL(visits.size(), static_cast<std::size_t>(aux::mpl::size<TSeq>::value));
        verify<TSeq>();
    }

    template<typename T>
    void operator()(const T&) const {
        visits.push_back(element(
            &typeid(typename T::type)
          , &typeid(typename T::dependency::scope)
        ));
    }

private:
    template<typename Seq>
    void verify(typename enable_if<aux::mpl::empty<Seq>>::type* = 0) { }

    template<typename Seq>
    void verify(typename disable_if<aux::mpl::empty<Seq>>::type* = 0) {
        typedef typename aux::mpl::front<Seq>::type type;
        verify_impl<type>();
        verify<typename aux::mpl::pop_front<Seq>::type>();
    }

    template<typename T>
    typename enable_if<has_first<T>>::type verify_impl() {
        verify_type<typename T::first>();
        verify_type<typename T::second>();
    }

    template<typename T>
    typename disable_if<has_first<T>>::type verify_impl() {
        verify_type<T>();
    }

    template<typename T>
    void verify_type() {
        bool found = false;
        for (const auto& v : visits) {
            if (&typeid(T) == v.type || &typeid(T) == v.scope) {
                found = true;
                break;
            }
        }

        if (!found) {
            struct not_found { };
            BOOST_CHECK_EQUAL(typeid(T).name(), typeid(not_found).name());
        }
    }

    mutable visits_type visits;
};

} // namespace di
} // namespace boost

#endif

