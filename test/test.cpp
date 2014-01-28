#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct ii { virtual ~ii() { } };
struct impl : ii { };

class c
{
public:
    BOOST_DI_CTOR_TRAITS(none, none, none);
    c(int i, double d, aux::shared_ptr<ii> q)
        : i(i), d(d), q_(q)
    { }

    int i = 0;
    double d = 0.0;
    aux::shared_ptr<ii> q_;
};

BOOST_AUTO_TEST_CASE(blah) {
    auto i = make_injector(
        bind<int>::to(42)
      , bind<double>::to(87.0)
      , bind<ii, impl>()
    );

    auto c_ = i.create<c>();

    BOOST_CHECK_EQUAL(42, c_.i);
    BOOST_CHECK_EQUAL(87.0, c_.d);
    BOOST_CHECK(dynamic_cast<impl*>(c_.q_.get()));
}

} // namespace di
} // namespace boost

