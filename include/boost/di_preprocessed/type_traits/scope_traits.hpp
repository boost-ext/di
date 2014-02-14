#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#include "boost/di_preprocessed/aux_/memory.hpp"
#include "boost/di_preprocessed/type_traits/make_plain.hpp" // has_named_type
#include "boost/di_preprocessed/type_traits/remove_accessors.hpp"
#include "boost/di_preprocessed/scopes/unique.hpp"
#include "boost/di_preprocessed/scopes/shared.hpp"
#include "boost/di_preprocessed/scopes/external.hpp"
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename = void>
struct scope_traits
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T&>
{
    typedef scopes::external<> type;
};

template<typename T>
struct scope_traits<const T&>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::auto_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::shared_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::weak_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::unique_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T&&>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const T&&>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T, typename enable_if<has_named_type<T> >::type>
{
    typedef typename scope_traits<typename T::named_type>::type type;
};

}
}
}

#endif

