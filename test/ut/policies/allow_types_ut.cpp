//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/policies/allow_types.hpp"

#include <type_traits>
#include <boost/test/unit_test.hpp>

#include "boost/di/scopes/unique.hpp"
#include "common/fakes/fake_data.hpp"

namespace boost {
namespace di {
namespace policies {

template<
    typename T
  , typename TResolve = std::false_type
>
struct fake_dependency {
    using type = T;

    struct resolved {
        using type =  resolved;

        template<typename>
        struct rebind {
            using other = resolved;
        };
    };

    struct binder {
        template<
            typename
          , typename
          , typename TDefault
        >
        using resolve = typename std::conditional<
            TResolve::value
          , resolved
          , TDefault
        >::type;
    };
};

BOOST_AUTO_TEST_CASE(dummy) {
    allow_types(is_copy() || is_ptr()).assert_policy<fake_data<int*, type_list<>, scopes::unique<>>>();
    allow_types(is_type<int>()).assert_policy<fake_dependency<int, std::true_type>>();
    allow_types(is_bound() && is_ptr() && is_type<int>()).assert_policy<fake_dependency<int*, std::true_type>>();
}

//template<typename T>
//bool verify_assert_exception(const assert_exception& e) {
    //return e.what() == std::string("TYPE_HAS_TO_BE_EXPLICITLY_BINDED") &&
           //e.get_type() == typeid(T);
//}

//template<typename T, typename TResolve = aux::mpl::false_>
//struct fake_dependency
//{
    //typedef T type;

    //struct resolved
    //{
        //typedef resolved type;

        //template<typename>
        //struct rebind
        //{
            //typedef resolved other;
        //};
    //};

    //struct binder {
        //template<
            //typename
          //, typename
          //, typename TDefault
        //>
        //struct resolve
            //: aux::mpl::if_<TResolve, resolved, TDefault>::type
        //{ };
    //};
//};

//BOOST_AUTO_TEST_CASE(none) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //creation_permission<>().assert_policy<
                //fake_dependency<int>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<int>
    //);
//}

//BOOST_AUTO_TEST_CASE(resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<>().assert_policy<
                //fake_dependency<int, aux::mpl::true_>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_not_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type<int>>().assert_policy<
                //fake_dependency<int>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_and_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type<int>>().assert_policy<
                //fake_dependency<int, aux::mpl::true_>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_expr_not_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type_expr<is_pod<aux::mpl::_>>>().assert_policy<
                //fake_dependency<int>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_expr_and_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type_expr<is_pod<aux::mpl::_>>>().assert_policy<
                //fake_dependency<int, aux::mpl::true_>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_and_type_expr_not_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type<int>, allow_type_expr<is_pod<aux::mpl::_>>>().assert_policy<
                //fake_dependency<int, aux::mpl::false_>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_type_and_type_expr_and_resolve) {
    //BOOST_CHECK_NO_THROW(
        //(
            //creation_permission<allow_type<int>, allow_type_expr<is_pod<aux::mpl::_>>>().assert_policy<
                //fake_dependency<int, aux::mpl::true_>
            //>()
        //)
    //);
//}

//template<typename T>
//bool verify_assert_exception(const assert_exception& e) {
    //return e.what() == std::string("PARAMETER_NOT_PERMITTED") &&
           //e.get_type() == typeid(T);
//}

//BOOST_AUTO_TEST_CASE(value) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //parameters_permission<>().assert_policy<
                //aux::mpl::identity<int>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<int>
    //);
//}

//BOOST_AUTO_TEST_CASE(none) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //parameters_permission<>().assert_policy<
                //aux::mpl::identity<c2>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<c2>
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_copy) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_copies>().assert_policy<
                //aux::mpl::identity<c2>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_ptr) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_ptrs>().assert_policy<
                //aux::mpl::identity<c2*>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_const_ref) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_const_refs>().assert_policy<
                //aux::mpl::identity<const c2&>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_ref) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_refs>().assert_policy<
                //aux::mpl::identity<c2&>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_rvalue_ref) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_rvalue_refs>().assert_policy<
                //aux::mpl::identity<c2&&>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_smart_ptr) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_smart_ptrs>().assert_policy<
                //aux::mpl::identity<aux::shared_ptr<c2>>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_many_order) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_refs, allow_ptrs>().assert_policy<
                //aux::mpl::identity<const c2*>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_nested_type) {
    //BOOST_CHECK_NO_THROW(
        //(
            //parameters_permission<allow_copies, allow_ptrs>().assert_policy<
                //aux::mpl::identity<std::vector<int*>>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(disallow_ref_from_const_ref) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //parameters_permission<allow_refs>().assert_policy<
                //aux::mpl::identity<const c2&>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<const c2&>
    //);
//}

//BOOST_AUTO_TEST_CASE(disallow_nested_type_copy) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //parameters_permission<>().assert_policy<
                //aux::mpl::identity<std::vector<int*>>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<std::vector<int*>>
    //);
//}

//BOOST_AUTO_TEST_CASE(disallow_nested_type_nested_ptr) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //parameters_permission<allow_copies>().assert_policy<
                //aux::mpl::identity<std::vector<int*>>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<std::vector<int*>>
    //);
//}

//template<typename T>
//bool verify_assert_exception(const assert_exception& e) {
    //return e.what() == std::string("SCOPE_NOT_PERMITTED") &&
           //e.get_type() == typeid(T);
//}

//template<typename TScope>
//struct fake_scope
//{
    //struct dependency
    //{
        //typedef TScope scope;
    //};
//};

//BOOST_AUTO_TEST_CASE(value) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //scopes_permission<>().assert_policy<
                //fake_scope<scopes::unique<>>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<scopes::unique<>>
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_none) {
    //BOOST_REQUIRE_EXCEPTION(
        //(
            //scopes_permission<
                //allow_scope<scopes::unique<>>
              //, allow_scope<scopes::shared<>>
            //>().assert_policy<
                //fake_scope<scopes::session<>>
            //>()
        //)
      //, assert_exception
      //, verify_assert_exception<scopes::session<>>
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_unique) {
    //BOOST_CHECK_NO_THROW(
        //(
            //scopes_permission<allow_scope<scopes::unique<>>>().assert_policy<
                //fake_scope<scopes::unique<>>
            //>()
        //)
    //);
//}

//BOOST_AUTO_TEST_CASE(allow_shared) {
    //BOOST_CHECK_NO_THROW(
        //(
            //scopes_permission<allow_scope<scopes::shared<>>>().assert_policy<
                //fake_scope<scopes::shared<>>
            //>()
        //)
    //);
//}

} // namespace policies
} // namespace di
} // namespace boost

