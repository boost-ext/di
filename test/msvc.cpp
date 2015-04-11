#define BOOST_DI_CFG_NO_PREPROCESSED_HEADERS
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

struct i {
	virtual ~i() noexcept = default;
	virtual void dummy() = 0;
};

struct i2 {
	virtual ~i2() noexcept = default;
	virtual void dummy2() = 0;
};

struct impl2 : i2 {
	void dummy2() override {}
};

struct impl : i{
	impl(int i, std::shared_ptr<i2>){
		assert(i == 87);
	}
	void dummy() override {};
};

struct c {
	//BOOST_DI_INJECT(c, int a, double b, std::shared_ptr<i>){}
	c(int a, double b, std::shared_ptr<i>) {}
};

struct module {
    di::injector<i> configure() const {
		return di::make_injector(
                di::bind<i, impl>()
	          , di::bind<i2, impl2>()
              , di::bind<int>().to(87)
		);
	}
};

int main() {
	auto injector = di::make_injector(
          di::bind<int>().to(42)
		, module{}
	);

	auto object = injector.create<i*>();
	assert(object != nullptr);
	assert(42 == injector.create<int>());

	injector.create<c>();

	return 0;
}

