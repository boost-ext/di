#ifndef BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP
#define BOOST_DI_TYPE_TRAITS_MAKE_PLAIN_HPP

#include "boost/di_preprocessed/type_traits/remove_accessors.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/has_xxx.hpp>










namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct make_plain;

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct deref_type
{
    typedef T type;
};

template<typename T>
struct deref_type<T, typename enable_if<has_element_type<T> >::type>
{
    typedef typename T::element_type type;
};

template<typename T>
struct deref_type<T, typename enable_if<has_named_type<T> >::type>
{
    typedef typename make_plain<typename T::named_type>::type type;
};

template<typename T>
struct make_plain
    : deref_type<
        typename remove_accessors<
            typename deref_type<
                typename remove_accessors<T>::type
            >::type
        >::type
    >
{ };

}
}
}

#endif

