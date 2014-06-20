//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[configuration_cpp_14
//`[h6 C++ 14]
//<-
#include <tuple>
#include <utility>
//->

#include <boost/di.hpp>

namespace di = boost::di;

//<-
namespace {

template<typename F, typename T, std::size_t... N>
auto invoke_impl(const F& f, const T& t, const std::index_sequence<N...>&) {
	return f(std::get<N>(t)...);
}

template<typename F, typename T>
auto invoke(const F& f, const T& t) {
	return invoke_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>());
}

} // namespace

//->

struct i { virtual ~i() { } };
struct impl : i { };

struct app {
	app(std::unique_ptr<i> up, int i) {
        assert(dynamic_cast<impl*>(up.get()));
		assert(i == 42);
	}
};

struct module1 {
	auto configure() const {
		return di::make_injector(
            di::bind<i, impl>()
        );
	}
};

struct module2 {
	auto configure() const {
		return di::make_injector(
            di::bind_int<42>()
        );
	}
};

int main() {
	auto injector =
		invoke([](const auto&... args) {
			return di::make_injector(args.configure()...);
		}
	  , std::make_tuple(module1(), module2())
	);

	injector.create<app>();

	return 0;
}

//`full code example: [@example/cpp_14/configuration.cpp configuration.cpp]
//]

