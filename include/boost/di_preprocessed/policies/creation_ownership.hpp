#ifndef BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP
#define BOOST_DI_POLICIES_CREATION_OWNERSHIP_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>

namespace boost {
namespace di {
namespace policies {

class creation_ownership
{
public:
    template<
        typename
      , typename T
    >
    static typename disable_if<is_reference<typename T::type> >::type assert_policy() { }

    template<
        typename
      , typename T
    >
    static typename enable_if<is_reference<typename T::type> >::type assert_policy() {
        BOOST_MPL_ASSERT_MSG(false, CREATION_OWNERSHIP_IS_NOT_CLEAR, (typename T::type))

         ;
    }
};

}
}
}

#endif

