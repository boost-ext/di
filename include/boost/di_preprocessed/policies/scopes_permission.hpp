#ifndef BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP
#define BOOST_DI_POLICIES_SCOPES_PERMISSION_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include "boost/di_preprocessed/detail/binder.hpp"
#include "boost/di_preprocessed/type_traits/ctor_traits.hpp"
#include "boost/di_preprocessed/type_traits/make_plain.hpp"
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/assert.hpp>










namespace boost {
namespace di {
namespace policies {

template<typename TScope>
struct allow_scope
{
    template<typename T>
    struct allow
        : is_same<T, TScope>
    { };
};
template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
class scopes_permission
{
    template<
        typename T
      , typename TCallStackSize
      , typename TCtor =
            typename type_traits::ctor_traits<
                typename type_traits::make_plain<T>::type
            >::type
    >
    struct ctor
        : mpl::if_<
              mpl::and_<
                  mpl::empty<TCtor>
                , mpl::equal_to<TCallStackSize, mpl::int_<1> >
              >
            , mpl::vector1<T>
            , TCtor
          >::type
    { };

    template<
        typename T
      , typename TCallStack
      , typename TBind
    >
    struct binder
        : TBind::template get_dependency<T, TCallStack>::type
    { };

    template<typename TAllow, typename T>
    struct is_allowed_impl
        : TAllow::template allow<T>
    { };

    template<
        typename T
      , typename TAllows = mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19>
    >
    struct is_allowed
        : mpl::bool_<
              mpl::count_if<
                  TAllows
                , is_allowed_impl<mpl::_, T>
              >::value != 0
          >
    { };

    template<typename T>
    struct scope
    {
        typedef typename T::scope type;
    };

    template<
        typename T
      , typename TBind
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
    >
    struct scopes_permission_impl
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given, mpl::size<TCallStack> >
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::if_<
                          is_allowed<
                              scope<binder<mpl::_2, TCallStack, TBind> >
                          >
                        , mpl::_2
                        , mpl::vector1<
                              mpl::pair<
                                  mpl::_2
                                , scope<binder<mpl::_2, TCallStack, TBind> >
                              >
                          >
                      >
                    , scopes_permission_impl<
                          mpl::_2
                        , TBind
                        , mpl::push_back<
                              TCallStack
                            , type_traits::make_plain<mpl::_2>
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >
    { };

    template<typename T, typename TAssert>
    struct verify_impl : T
    {
        BOOST_MPL_ASSERT_MSG(
            !TAssert::value || mpl::empty<T>::value
          , SCOPE_NOT_PERMITTED
          , (T)
        );
    };

public:
    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , template<typename> class TBinder = di::detail::binder
    >
    struct verify
        : verify_impl<
              typename scopes_permission_impl<TGiven, TBinder<TDeps> >::type
            , TAssert
          >
    { };

    template<
        typename TDeps
      , typename T
    >
    static void assert_policy() { }
};

}
}
}

#endif

