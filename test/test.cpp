#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

#include "data.hpp"

namespace boost {
namespace di {

class b0
{
public:
    b0(int) {}
};

class b1
{
public:
    explicit b1(int) {}
    b1(const b1&) {}
    b1(b1&&) {}
};

class b2
{
public:
	template<typename T> //not working
    b2(T) {}
};

class aa
{
public:
	aa(int i, double d)
		: i(i), d(d)
	{ }

	int i = 0;
	double d = 0.0;
};

class cc
{
public:
   /* BOOST_DI_INJECT(c, int i, double d, a a_)*/
   /* BOOST_DI_CTOR(c, int i, double d, a a_)*/
		//: i(i), d(d), a_(a_)
	/*{ }*/

    cc(b0, b1, int i_, const double& d, aux::shared_ptr<i> q, float f, aux::shared_ptr<i> q2, aa a_, named<int, float> n, const std::string&)
        : i_(i_), d(d), q(q), q2(q2), f(f), a_(a_)
    { }

    int i_ = 0;
    double d = 0.0;
    aux::shared_ptr<i> q;
    aux::shared_ptr<i> q2;
    float f = 0.0;
    aa a_;
};

BOOST_AUTO_TEST_CASE(ehh) {
    auto c8_ = injector<
        c0if0
      , bind_int<2>::in_call<c8>
    >().create<c8>();

    BOOST_CHECK_EQUAL(c8_.i, 2);
}

BOOST_AUTO_TEST_CASE(blah) {
    auto inj = make_injector(
        bind<int>::to(42)
      , bind<double>::to(87.0)
      , bind<float>::to(12.0)
      , bind<i, impl>()
    );

    auto c_ = inj.create<cc>();

    BOOST_CHECK_EQUAL(42, c_.i_);
    BOOST_CHECK_EQUAL(87.0, c_.d);
    BOOST_CHECK_EQUAL(12.0, c_.f);
    BOOST_CHECK(dynamic_cast<impl*>(c_.q.get()));
    BOOST_CHECK_EQUAL(c_.q, c_.q2);
    BOOST_CHECK_EQUAL(c_.q, c_.q2);
    BOOST_CHECK_EQUAL(42, c_.a_.i);
    BOOST_CHECK_EQUAL(87.0, c_.a_.d);
}

} // namespace di
} // namespace boost

