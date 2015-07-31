//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/compiler.hpp"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <regex>
#include <string>

namespace {

template<class TFStream = std::ofstream>
struct file : std::string, TFStream {
    file(const std::string& name) // non explicit
        : std::string{name}, TFStream{name}
    { }

    ~file() {
        std::remove(this->c_str());
    }
};

auto compail_fail(const std::string& defines, const std::string&, const std::string& code) {
    std::stringstream command; {
        file<> source_code{"error.cpp"};
        source_code << "#include <boost/di.hpp>" << std::endl;
        source_code << "namespace di = boost::di;" << std::endl;
        source_code << "int main() {" << std::endl;
        source_code << code;
        source_code << "}" << std::endl;

        std::stringstream errors;
        std::string compiler;

        #if defined(BOOST_DI_GCC)
            compiler = "g++";
            errors << "-c -std=c++1y -Werror ";
        #elif defined(BOOST_DI_CLANG)
            compiler = "clang++";
            errors << "-c -std=c++1y -Wno-all -Werror -Wno-error=deprecated-declarations";
        #elif defined(BOOST_DI_MSVC)
            compiler = "cl";
            errors << "/c /EHsc /W3 /WX";
        #endif

        if (auto cxx = std::getenv("CXX")) {
            compiler = cxx;
        }

        command << compiler
                << "  -I../include "
                << defines
                << " " << errors.str()
                << " " << source_code;
    }

    return std::system(command.str().c_str());
}

} // namespace

#define expect_compile_fail(defines, error, ...) \
    expect(compail_fail(defines, error, #__VA_ARGS__))

test bind_external_repeated = [] {
    expect_compile_fail(
        "", ".*",
        di::make_injector(
            di::bind<int>().to(42)
          , di::bind<int>().to(87)
        );
    );
};

test bind_external_with_given_scope = [] {
    expect_compile_fail(
        "", ".*",
        di::make_injector(
            di::bind<int>().in(di::unique).to(42)
        );
    );
};

test bind_external_with_given_type = [] {
    expect_compile_fail(
        "", ".*",
        struct i { };
        struct impl : i { };

        di::make_injector(
            di::bind<i, impl>().to(impl{})
        );
    );
};

test bind_multiple_times = [] {
    expect_compile_fail(
        "", ".*",
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };

        di::make_injector(
            di::bind<i, impl1>()
          , di::bind<i, impl2>()
        );
    );
};

test bind_not_compatible_types = [] {
    expect_compile_fail(
        "", ".*",
        struct i { };
        struct impl : i { };

        auto injector = di::make_injector(
            di::bind<int, impl>()
        );

        injector.create<int>();
    );
};

test bind_repeated = [] {
    expect_compile_fail(
        "", ".*",
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };

        di::make_injector(
            di::bind<i, impl1>()
          , di::bind<i, impl1>()
        );
    );
};

test bind_to_different_types = [] {
    expect_compile_fail(
        "", ".*",
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };

        di::make_injector(
            di::bind<i, impl1>()
          , di::bind<i, impl2>()
        );
    );
};

test create_polymorphic_type_without_binding = [] {
    expect_compile_fail(
        "", ".*",
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };

        struct c {
            c(i*) { }
        };

        di::make_injector().create<c>();
    );
};

test ctor_inject_limit_out_of_range = [] {
    expect_compile_fail(
        "-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", ".*",
        struct c {
            BOOST_DI_INJECT(c, int, int, int, int) { }
        };

        auto injector = di::make_injector();

        injector.create<c>();
    );
};

test ctor_limit_out_of_range = [] {
    expect_compile_fail(
        "-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", ".*",

        struct c {
            c(int, int, int, int) { }
        };

        auto injector = di::make_injector();

        injector.create<c>();
    );
};

test exposed_multiple_times = [] {
    expect_compile_fail(
        "", ".*",
        struct c { };
        di::injector<c, c> injector = di::make_injector();
    );
};

test exposed_not_creatable = [] {
    expect_compile_fail(
        "", ".*",
        struct i {
            virtual ~i() noexcept = default; virtual void dummy() = 0;
        };

        struct c {
            c(int, std::unique_ptr<i>) {}
        };

        di::injector<i> injector = di::make_injector();
    );
};

test exposed_polymorphic_type_without_binding = [] {
    expect_compile_fail(
        "", ".*",
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { std::shared_ptr<i> i_; };

        di::injector<c> injector = di::make_injector(); // di::bind<i, impl>()
    );
};

test inject_limit_out_of_range = [] {
    expect_compile_fail(
        "-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", ".*",
        struct c {
            BOOST_DI_INJECT(c, int, int, int, int) { }
        };

        auto injector = di::make_injector();

        injector.create<c>();
    );
};

test injector_ctor_ambiguous = [] {
    expect_compile_fail(
        "", ".*",
        struct ctor {
            ctor(int, double) { }
            ctor(double, int) { }
        };

        di::make_injector().create<ctor>();
    );
};

test injector_shared_by_copy = [] {
    expect_compile_fail(
        "", ".*",
        struct c {
            c(int*) { }
        };

        auto injector = di::make_injector(
            di::bind<int>().in(di::singleton)
        );

        injector.create<c>();
    );
};

test make_injector_wrong_arg = [] {
    expect_compile_fail(
        "", ".*",
        struct neither_module_nor_injector_nor_module { };
        auto injector = di::make_injector(neither_module_nor_injector_nor_module{});
    );
};

test named_paramater_spelling = [] {
    expect_compile_fail(
        "", ".*",
        auto name = []{};
        struct c {
            BOOST_DI_INJECT(c, (NAMED = name) int) { }
        };
    );
};

test policy_constructible = [] {
    expect_compile_fail(
        "", ".*",
        class config : public di::config {
        public:
            static auto policies(...) noexcept {
                using namespace di::policies;
                return di::make_policies(constructible(is_bound<di::_>{}));
            }
        };

        struct c {
            c(int, double, float) { }
        };

        auto injector = di::make_injector<config>();
        injector.create<c>();
    );
};

test scope_traits_external_not_referable = [] {
    expect_compile_fail(
        "", ".*",
        struct c {
            c(int&) { }
        };

        auto injector = di::make_injector(
            di::bind<int>().to(42) // lvalue can't be converted to reference
        );

        injector.create<c>();
    );
};

test circular_dependencies_simple = [] {
    expect_compile_fail(
        "", ".*",
        struct cd2;
        struct cd1 { cd1(cd2*) { }; };
        struct cd2 { cd2(cd1*) { }; };

        auto injector = di::make_injector();
        injector.create<cd1>();
    );
};

test circular_dependencies_complex = [] {
    expect_compile_fail(
        "", ".*",
        struct cd2;
        struct cd5;
        struct cd1 { cd1(cd2*) { }; };
        struct cd2 { cd2(cd1*) { }; };
        struct cd3 { cd3(cd5*) { }; };
        struct cd4 { cd4(cd3*) { }; };
        struct cd5 { cd5(cd4*) { }; };

        auto injector = di::make_injector();
        injector.create<cd5>();
    );
};

