//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {

template<typename TSeq = mpl::vector0<>>
class fake_visitor
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(first)

    struct element
    {
        element(const std::string& type, const std::string& scope)
            : type(type), scope(scope)
        { }

        std::string type;
        std::string scope;
    };

    typedef std::vector<element> visits_type;

public:
    ~fake_visitor() {
        verify<TSeq>();
    }

    template<typename T>
    void operator()(const T&) const {
        visits.push_back(element(
            typeid(typename T::type).name()
          , typeid(typename T::scope).name()
        ));
    }

private:
    template<typename Seq>
    void verify(int = 0, typename enable_if<mpl::empty<Seq>>::type* = 0) { }

    template<typename Seq>
    void verify(int i = 0, typename disable_if<mpl::empty<Seq>>::type* = 0) {
        typedef typename mpl::front<Seq>::type type;
        verify_impl<type>(i);
        verify<typename mpl::pop_front<Seq>::type>(i + 1);
    }

    template<typename T>
    typename enable_if<has_first<T>>::type verify_impl(int i) {
        BOOST_CHECK_EQUAL(
            typeid(typename T::first).name()
          , visits.at(i).type
        );

        BOOST_CHECK_EQUAL(
            typeid(typename T::second).name()
          , visits.at(i).scope
        );
    }

    template<typename T>
    typename disable_if<has_first<T>>::type verify_impl(int i) {
        BOOST_CHECK_EQUAL(
            typeid(T).name()
          , visits.at(i).type
        );
    }

    mutable visits_type visits;
};

} // namespace di
} // namespace boost

#endif

