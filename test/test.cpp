#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

#include "data.hpp"

#include "fake_visitor.hpp"
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
   /* BOOST_DI_INJECT(c, int i, double d, a a_)*/
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

/*BOOST_AUTO_TEST_CASE(ehh) {*/
    //auto c8_ = injector<
        //c0if0
      //, bind_int<2>::in_call<c8>
    //>().create<c8>();

    //BOOST_CHECK_EQUAL(c8_.i, 2);
//}

//BOOST_AUTO_TEST_CASE(blah) {
    //auto inj = make_injector(
        //bind<int>::to(42)
      //, bind<double>::to(87.0)
      //, bind<float>::to(12.0)
      //, bind<i, impl>()
    //);

    //auto c_ = inj.create<cc>();

    //BOOST_CHECK_EQUAL(42, c_.i_);
    //BOOST_CHECK_EQUAL(87.0, c_.d);
    //BOOST_CHECK_EQUAL(12.0, c_.f);
    //BOOST_CHECK(dynamic_cast<impl*>(c_.q.get()));
    //BOOST_CHECK_EQUAL(c_.q, c_.q2);
    //BOOST_CHECK_EQUAL(c_.q, c_.q2);
    //BOOST_CHECK_EQUAL(42, c_.a_.i);
    //BOOST_CHECK_EQUAL(87.0, c_.a_.d);
/*}*/

template<typename T>
struct qq
{
	qq(T){}
	operator T() { return {}; }
};

struct c10_
{
    BOOST_DI_INJECT_TRAITS(
        const named<int, mpl::string<'1'>>&
      , const named<int, mpl::string<'2'>>&
    );

    c10_(int i1, int i2)
        : i1(i1), i2(i2)
    { }

	//c10_(const named<int, mpl::string<'1'>>& i1, const named<int, mpl::string<'2'>>& i2, const qq<int>&)
		//: i1(i1), i2(i2)
	//{ }

	int i1;
	int i2;
};

/*BOOST_AUTO_TEST_CASE(named_ehh) {*/
	//const int i1 = 42;
	//const int i2 = 87;

	//auto ii = make_injector(
		//bind<int>::in_name<mpl::string<'1'>>::to(i1)
	  //, bind<int>::in_name<mpl::string<'2'>>::to(i2)
	//);

	//auto obj = ii.create<c10_>();

	//BOOST_CHECK_EQUAL(i1, obj.i1);
	//BOOST_CHECK_EQUAL(i2, obj.i2);
//}

//BOOST_AUTO_TEST_CASE(to_in_call_in_name) {
	//const int i1 = 42;
	//const int i2 = 87;

	//auto injector_ = injector<>()(
		//bind<int>::in_call<c4>::in_name<mpl::string<'1'>>::to(i1)
	  //, bind<int>::in_call<c4>::in_name<mpl::string<'2'>>::to(i2)
	  //, bind<c0if0>()
	//);

	//auto c6_ = injector_.create<c6>();

	//BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
	//BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
//}

//BOOST_AUTO_TEST_CASE(to_in_name_in_call) {
	//const int i1 = 42;
	//const int i2 = 87;

	//auto injector_ = injector<>()(
		//bind<int>::in_name<mpl::string<'1'>>::in_call<c4>::to(i1)
	  //, bind<int>::in_name<mpl::string<'2'>>::in_call<c4>::to(i2)
	  //, bind<c0if0>()
	//);

	//auto c6_ = injector_.create<c6>();

	//BOOST_CHECK_EQUAL(i1, c6_.c4_->i1);
	//BOOST_CHECK_EQUAL(i2, c6_.c4_->i2);
/*}*/

using injector_provider_t = injector<
	unique<
		transaction_provider
	>
>;

struct cx
{
    cx(int i, int& e, double* p, aux::shared_ptr<float> s, const aux::shared_ptr<float>& s2)
        : i_(i), e_(e), p_(p), s_(s), s2_(s2)
    { }

    int i_;
    int& e_;
    double* p_;
    aux::shared_ptr<float> s_;
    aux::shared_ptr<float> s2_;
};

BOOST_AUTO_TEST_CASE(basic_visitor) {
    //injector_provider_t injector;

    //fake_visitor<
        //mpl::vector<
            //transaction_usage
          //, aux::shared_ptr<provider<aux::shared_ptr<transaction>>>
          //, aux::shared_ptr<c3>
          //, int
        //>
    //> visitor;

    //injector.visit<c18>(visitor);

    int i = 42;

    auto obj = make_injector(
        bind<int>::to(i)
      , bind<double>::to(12.0)
      , bind<float>::to(87.0)
    ).create<cx>();

    //i = 32;

    BOOST_CHECK_EQUAL(i, obj.i_);
    BOOST_CHECK_EQUAL(i, obj.e_);
    BOOST_CHECK_EQUAL(12.0, *obj.p_);
    BOOST_CHECK_EQUAL(87.0, *obj.s_);
}

BOOST_AUTO_TEST_CASE(named_ref) {
    struct injector_type
        : injector<
              unique<
                  c0if0
              >
          >
    { } injector_;

    auto c8_ = injector_.create<aux::shared_ptr<c8>>();

}

BOOST_AUTO_TEST_CASE(aaab) {
    const int i = 42;
    const double d = 87.0;

    auto injector_ = injector<>()(
        bind<int>::to(i)
      , bind<double>::to(d)
    );

    auto c14_ = injector_.create<c14>();

    BOOST_CHECK_EQUAL(i, c14_.i);
    BOOST_CHECK_EQUAL(d, c14_.d);
}


} // namespace di
} // namespace boost

