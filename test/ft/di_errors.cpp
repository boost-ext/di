//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace {

template<class TFStream = std::ofstream>
struct file : std::string, TFStream {
    file(const std::string& name) // non explicit
        : std::string{name}, TFStream{name}
    { }
};

template<class... TArgs>
auto errors(const TArgs&... args) {
    return std::vector<std::string>{(".*" + std::string{args} + ".*")...};
}

auto compail_fail(int id, const std::string& defines, const std::vector<std::string>& errors, const std::string& code) {
    std::stringstream command; {
        file<> source_code{std::string{"error"} + std::to_string(id) + ".cpp"};
        source_code << "#include <boost/di.hpp>" << std::endl;
        source_code << "namespace di = boost::di;" << std::endl;
        source_code << "int main() {" << std::endl;
        source_code << code;
        source_code << "}" << std::endl;

        std::stringstream errors;
        std::string compiler;

        #if defined(__clang__)
            compiler = "clang++";
            errors << "-c -std=c++1y -Wno-all -Werror -Wno-error=deprecated-declarations";
        #elif defined(__GNUC__) && !defined(__clang__)
            compiler = "g++";
            errors << "-c -std=c++1y -Werror ";
        #elif defined(_MSC_VER)
            compiler = "cl";
            errors << "/c /EHsc /W3 /WX";
        #endif

        if (auto cxx = std::getenv("CXX")) {
            compiler = cxx;
        }

        auto include_rgx = std::regex{"<include>"};

        #if defined(_MSC_VER)
            auto include = std::regex_replace(defines, include_rgx, "/FI");
        #else
            auto include = std::regex_replace(defines, include_rgx, "-include");
        #endif

        std::string flags = "-I../include -I../../include"; // bjam, cmake
        if (auto cxxflags = std::getenv("CXXFLAGS")) {
            flags += " " + std::string{cxxflags};
        }

        command << compiler
                << " " << flags
                << " " << include
                << " " << errors.str()
                << " " << source_code
        #if defined(_MSC_VER)
                << " >"
        #else
                << " 2>"
        #endif
                << " error" << id << ".out";
    }

    expect(std::system(command.str().c_str()));

    file<std::ifstream> output{std::string{"error"} + std::to_string(id) + ".out"};
    std::vector<bool> matches(errors.size(), false);
    std::vector<std::string> out;
    auto lines = 0;
    for (std::string line; std::getline(output, line);) {
        out.push_back(line);
        auto i = 0;
        for (const auto& rgx : errors) {
            if (std::regex_match(line, std::regex{rgx})) {
                matches[i] = true;
            }
            ++i;
        }
        ++lines;
    }

    if (!errors.empty()) {
        constexpr auto MAX_ERROR_LINES_COUNT = 64;
        if (lines >= MAX_ERROR_LINES_COUNT) {
            throw std::runtime_error(command.str() + " | lines < MAX_ERROR_LINES_COUNT");
        }
    }

    if (std::find(matches.begin(), matches.end(), false) != matches.end()) {
        for (const auto& line : out) {
            std::clog << line << std::endl;
        }
        throw std::runtime_error(command.str());
    }
}

} // namespace

#define expect_compile_fail(defines, error, ...) \
    compail_fail(__LINE__, defines, error, #__VA_ARGS__)

// ---------------------------------------------------------------------------

test bind_external_with_given_scope = [] {
    expect_compile_fail("", errors(),
        di::make_injector(
            di::bind<int>().in(di::unique).to(42)
        );
    );
};

test bind_external_with_given_type = [] {
    expect_compile_fail("", errors(),
        struct i { };
        struct impl : i { };

        di::make_injector(
            di::bind<i, impl>().to(impl{})
        );
    );
};

test bind_external_with_given_value = [] {
    expect_compile_fail("", errors(),
        di::make_injector(
            di::bind<int, std::integral_constant<int, 0>>.to(42);
        );
    );
};

test bind_has_disallowed_specifiers_expected = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::has_disallowed_specifiers", "=.*int.*\\*"
        #else
            "bind<.*int.*\\*>::has_disallowed_specifiers"
        #endif
    );

    expect_compile_fail("", errors_,
        di::make_injector(
            di::bind<int*>()
        );
    );
};

test bind_has_disallowed_specifiers_given = [] {
    auto errors_ = errors(
        "constraint not satisfied", "bind<const.*int.*&>::has_disallowed_specifiers"
    );

    expect_compile_fail("", errors_,
        di::make_injector(
            di::bind<int, const int&>()
        );
    );
};

test bind_external_repeated = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_bound_more_than_once", "=.*int"
        #else
            "bind<.*int>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
        di::make_injector(
            di::bind<int>().to(42)
          , di::bind<int>().to(87)
        );
    );
};

test bind_multiple_times = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_bound_more_than_once", "=.*i"
        #else
            "bind<.*i>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };

        di::make_injector(
            di::bind<i, impl1>()
          , di::bind<i, impl2>()
        );
    );
};

test bind_any_of_not_related = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_not_related_to<.*a>.*bind<.*>::is_not_related_to<.*b>", "=.*c"
        #else
            "bind<.*c>::is_not_related_to<.*a>.*bind<.*c>::is_not_related_to<.*b>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct a { };
        struct b : a { };
        struct c { };

        di::make_injector(
            di::bind<di::any_of<a, b>, c>()
        );
    );
};

test bind_not_compatible_types = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_not_related_to<int>",
            "=.*impl"
        #else
            "bind<.*impl>::is_not_related_to<int>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl : i { };

        di::make_injector(
            di::bind<int, impl>()
        );
    );
};

test bind_repeated = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_bound_more_than_once", "=.*i"
        #else
            "bind<.*i>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
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
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_bound_more_than_once", "=.*i"
        #else
            "bind<.*i>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };

        di::make_injector(
            di::bind<i, impl1>()
          , di::bind<i, impl2>()
        );
    );
};

test exposed_multiple_times = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_bound_more_than_once", "=.*c"
        #else
            "bind<.*c>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
        struct c { };
        di::injector<c, c> injector = di::make_injector();
    );
};

test make_injector_wrong_arg = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_neither_a_dependency_nor_an_injector", "=.*neither_module_nor_injector_nor_module"
        #else
            "bind<.*neither_module_nor_injector_nor_module>::is_neither_a_dependency_nor_an_injector"
        #endif
    );

    expect_compile_fail("", errors_,
        struct neither_module_nor_injector_nor_module { };
        di::make_injector(neither_module_nor_injector_nor_module{});
    );
};

test bind_in_not_scopable = [] {
    expect_compile_fail("", errors(),
        struct not_scopable { };
        di::bind<int>().in(not_scopable{});
    );
};

// ---------------------------------------------------------------------------

test dummy_config = [] {
    expect_compile_fail("", errors(),
        struct dummy_config { };
        auto injector = di::make_injector<dummy_config>();
    );
};

test config_wrong_policy = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "policy<.*>::requires_<.*call_operator>", "=.*int"
        #else
            "policy<.*int>::requires_<.*call_operator>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct test_config : di::config {
            static auto policies(...) { return 42; }
        };
        auto injector = di::make_injector<test_config>();
    );
};

test config_policy_not_callable = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "policy<.*>::requires_<.*call_operator>", "=.*dummy"
        #else
            "policy<.*dummy>::requires_<.*call_operator>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct test_config : di::config {
            struct dummy { };
            static auto policies(...) { return di::make_policies(dummy{}); }
        };
        auto injector = di::make_injector<test_config>();
    );
};

test config_not_providable = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "provider<.*>::requires_<.*get,.*is_creatable>", "=.*dummy"
        #else
            "provider<.*dummy>::requires_<.*get,.*is_creatable>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct test_config : di::config {
            struct dummy { };
            static auto provider(...) { return dummy{}; }
        };
        auto injector = di::make_injector<test_config>();
    );
};

test config_wrong_provider = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "config<.*>::requires_<.*provider<.*providable_type.*(...)>.*policies<.*callable_type.*(...)>", "=.*test_config"
        #else
            "config<.*test_config>::requires_<.*provider<.*providable_type.*(...)>.*policies<.*callable_type.*(...)>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct test_config : di::config {
            static auto provider() { return di::providers::heap{}; }
        };
        auto injector = di::make_injector<test_config>();
    );
};

// ---------------------------------------------------------------------------

test create_polymorphic_type_without_binding = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "create<c>()"
          , "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };

        struct c {
            c(i*) { }
        };

        di::make_injector().create<c>();
    );
};

test exposed_not_creatable = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
           "abstract_type<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "create<T>"
          , "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        #endif
    );

    expect_compile_fail("<include> memory", errors_,
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
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "create<T>"
          , "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        #endif
    );

    expect_compile_fail("<include> memory", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { std::shared_ptr<i> i_; };

        di::injector<c> injector = di::make_injector(); // di::bind<i, impl>()
    );
};

test injector_shared_by_copy = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "type<.*>::is_not_convertible_to<.*>"
        #if !defined(_MSC_VER)
          , "wrapper is not convertible to requested type, did you mistake the scope?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct c {
            c(int*) { }
        };

        auto injector = di::make_injector(
            di::bind<int>().in(di::singleton)
        );

        injector.create<c>();
    );
};

test bind_wrapper_not_convertible = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "type<.*>::is_not_convertible_to<.*>"
        #if !defined(_MSC_VER)
          , "wrapper is not convertible to requested type, did you mistake the scope?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct c {
            c(int*) { }
        };

        auto injector = di::make_injector(
            di::bind<int>().to(42)
        );

        injector.create<c>();
    );
};

test scope_traits_external_not_referable = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "when_creating<.*>::type<.*>"
        #if !defined(_MSC_VER)
          , "reference type not bound, did you forget to add `auto value = ...; di::bind<T>.to\\(value\\)`"
        #endif
    );

    expect_compile_fail("", errors_,
        struct c {
            c(int&) { }
        };

        auto injector = di::make_injector(
            di::bind<int>().to(42) // lvalue can't be converted to a reference
        );

        injector.create<c>();
    );
};

test policy_constructible = [] {
    auto errors_ = errors(
    #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
        "creatable constraint not satisfied",
    #endif
    #if defined(__GNUC__) || defined(__clang__)
        "type disabled by constructible policy, added by BOOST_DI_CFG or make_injector<CONFIG>",
    #endif
    #if defined(__GNUC__) && !defined(__clang__)
        "type<.*>::not_allowed_by.*int",
        "type<.*>::not_allowed_by.*double",
        "type<.*>::not_allowed_by.*float"
    #else
        "type<int>::not_allowed_by",
        "type<double>::not_allowed_by",
        "type<float>::not_allowed_by"
    #endif
    );

    expect_compile_fail("", errors_,
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

// ---------------------------------------------------------------------------

test ctor_inject_limit_out_of_range = [] {
    expect_compile_fail("-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3",
        errors("Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"),
        struct c {
            BOOST_DI_INJECT(c, int, int, int, int) { }
        };

        auto injector = di::make_injector();
        injector.create<c>();
    );
};

test ctor_limit_out_of_range = [] {
    auto errors_ = errors(
    #if defined(__GNUC__) && !defined(__clang__)
        "number_of_constructor_arguments_is_out_of_range_for<.*>::max<.*>.*= 3.*=.*c"
    #else
        "number_of_constructor_arguments_is_out_of_range_for<.*c>::max<3>"
    #endif
    );

    expect_compile_fail("-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", errors_,
        struct c {
            c(int, int, int, int) { }
        };

        auto injector = di::make_injector();
        injector.create<c>();
    );
};

test injector_ctor_ambiguous = [] {
    auto errors_ = errors(
        #if defined(__GNUC__) && !defined(__clang__)
            "number_of_constructor_arguments_is_out_of_range_for<.*>::max<.*>.*= 10.*=.*ctor"
        #else
            "number_of_constructor_arguments_is_out_of_range_for<.*ctor>::max<10>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct ctor {
            ctor(int, double) { }
            ctor(double, int) { }
        };

        di::make_injector().create<ctor>();
    );
};

test named_paramater_spelling = [] {
    expect_compile_fail("", errors(),
        auto name = []{};
        struct c {
            BOOST_DI_INJECT(c, (NAMED = name) int) { }
        };
    );
};

test circular_dependencies_simple = [] {
    expect_compile_fail("", errors(),
        struct cd2;
        struct cd1 { cd1(cd2*) { }; };
        struct cd2 { cd2(cd1*) { }; };

        auto injector = di::make_injector();
        injector.create<cd1>();
    );
};

test circular_dependencies_complex = [] {
    expect_compile_fail("", errors(),
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

// ---------------------------------------------------------------------------

