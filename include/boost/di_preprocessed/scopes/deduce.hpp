#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

#include <boost/none_t.hpp>
#include <boost/mpl/int.hpp>

namespace boost {
namespace di {
namespace scopes {

class deduce
{
public:
    typedef mpl::int_<0> priority;

    template<typename, typename>
    struct scope
    {
        typedef none_t result_type;
    };
};

}
}
}

#endif

