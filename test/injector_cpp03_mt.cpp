#include "boost/di/module.hpp"

#include <boost/test/unit_test.hpp>
#include "boost/di/concepts.hpp"
#include "boost/di/injector.hpp"
#include "boost/di/make_module.hpp"

#include "data.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {

BOOST_AUTO_TEST_CASE(tefdsst) {
    using module_c0 = module<
        c0if0
    >;

auto m2 = make_module(
    singleton<
        c0if0
    >()
  , bind<int>::to(42)
  , bind<double>::to(87.0)
);

    auto m = module<>()(
        module_c0()
      , bind<int>::to(32)

      //,singleton<c0if0>()
    );

    typedef decltype(m) mm;

    //injector<decltype(m2)> injector_(m2);
    //shared_ptr<c9> c9_ = injector_.create<shared_ptr<c9>>();


    std::cout << "dupa:" << boost::units::detail::demangle(typeid(mm::deps::type).name()) << std::endl;

    BOOST_CHECK(0);
}

} // namespace di
} // namespace boost

