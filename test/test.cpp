#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

template<>
struct ctor_traits<std::basic_string<char>>
{
    //BOOST_DI_CTOR_TRAITS(const char*);
    BOOST_DI_CTOR_TRAITS();
};

struct ii { virtual ~ii() { } };
struct impl : ii { };

class b0
{
public:
    b0(int) {}
};

class b1
{
public:
    explicit b1(int) {}
};

class b2
{
public:
	template<typename T> //not working
    b2(T) {}
};

class a
{
public:
	a(int i, double d)
		: i(i), d(d)
	{ }

	int i = 0;
	double d = 0.0;
};

class c
{
public:
   /* BOOST_DI_INJECT(c, int i, double d, a a_)*/
   /* BOOST_DI_CTOR(c, int i, double d, a a_)*/
		//: i(i), d(d), a_(a_)
	/*{ }*/

	c(b0, b1, int i, const double& d, aux::shared_ptr<ii> q, float f, aux::shared_ptr<ii> q2, a a_, named<int, float> n, std::string s)
		: i(i), d(d), q(q), q2(q2), f(f), a_(a_)
	{ }

    int i = 0;
    double d = 0.0;
    aux::shared_ptr<ii> q;
    aux::shared_ptr<ii> q2;
	float f = 0.0;
	a a_;
};

BOOST_AUTO_TEST_CASE(blah) {
    auto i = make_injector(
        bind<int>::to(42)
      , bind<double>::to(87.0)
      , bind<float>::to(12.0)
      , bind<ii, impl>()
    );

    auto c_ = i.create<c>();

	BOOST_CHECK_EQUAL(42, c_.i);
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

