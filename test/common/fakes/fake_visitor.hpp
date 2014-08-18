//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_VISITOR_HPP
#define BOOST_DI_FAKE_VISITOR_HPP

#include "boost/di/aux_/mpl.hpp"

#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <vector>
#include <string>

#include <iostream>
#include <boost/units/detail/utility.hpp>

namespace boost {
namespace di {

BOOST_DI_HAS_MEMBER_TYPE(first);

template<typename... Ts>
class fake_visitor {
    struct element {
        element(const std::type_info* type, const std::type_info* scope)
            : type(type), scope(scope)
        { }

        const std::type_info* type = nullptr;
        const std::type_info* scope = nullptr;
    };

    typedef std::vector<element> visits_type;

public:
    ~fake_visitor() {
        BOOST_CHECK_EQUAL(visits.size(), static_cast<std::size_t>(sizeof...(Ts)));
        int dummy[]{0, (verify<Ts>(), 0)...};
        (void)dummy;
    }

    template<typename T>
    void operator()(const T&) const {
        visits.push_back(element(
            &typeid(typename T::type)
          , &typeid(typename T::dependency::scope)
        ));
    }

private:
    template<typename T>
    typename std::enable_if<has_first<T>::value>::type verify() {
        verify_type<typename T::first>();
        verify_type<typename T::second>();
    }

    template<typename T>
    typename std::enable_if<!has_first<T>::value>::type verify() {
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

