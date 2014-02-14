#ifndef BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_TYPE_HPP
#define BOOST_DI_CONCEPTS_TYPE_TRAITS_IS_REQ_TYPE_HPP

#include "boost/di_preprocessed/type_traits/make_plain.hpp"
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace di {
namespace concepts {
namespace type_traits {

template<typename T, typename U = mpl::_1>
struct is_req_type
    : mpl::or_<
          is_base_of<typename di::type_traits::make_plain<U>::type, T>
        , is_same<typename di::type_traits::make_plain<U>::type, T>
      >
{ };

}
}
}
}

#endif

