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
#include "common/utils.hpp"

namespace {

template<class... TArgs>
auto errors(const TArgs&... args) {
    return std::vector<std::string>{(".*" + std::string{args} + ".*")...};
}

auto compail_fail(int id
                , const std::string& defines
                , const std::vector<std::string>& errors
                , const std::string& code) {
    std::stringstream command; {
        file<> source_code{std::string{"error"} + std::to_string(id) + ".cpp"};
        source_code << "#include <boost/di.hpp>" << std::endl;
        source_code << "namespace di = boost::di;" << std::endl;
        source_code << code;

        std::stringstream errors;

        #if defined(__clang__)
            errors << "-c -Wno-all -Werror -Wno-error=deprecated-declarations";
        #elif defined(__GNUC__) && !defined(__clang__)
            errors << "-c -Werror ";
        #elif defined(_MSC_VER)
            errors << "/c /EHsc /W3 /WX";
        #endif

        auto include_rgx = std::regex{"<include>"};

        #if defined(_MSC_VER)
            auto include = std::regex_replace(defines, include_rgx, "/FI");
        #else
            auto include = std::regex_replace(defines, include_rgx, "-include");
        #endif

        command << cxx()
                << " " << cxxflags(true)
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

    std::clog << command.str() << std::endl;
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
        #if defined(_MSC_VER)
            constexpr auto MAX_ERROR_LINES_COUNT = 128;
        #else
            constexpr auto MAX_ERROR_LINES_COUNT = 64;
        #endif

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
        int main() {
            di::make_injector(
                di::bind<i>().to(impl{}).to<impl>()
            );
        }
    );
};

test bind_named_to_named = [] {
    expect_compile_fail("", errors(),
        struct a { };
        struct b { };
        int main() {
            di::make_injector(
                di::bind<int>().named(a{}).named(b{})
            );
        }
    );
};

test bind_external_with_given_value = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::make_injector(
                di::bind<int>>.to(42);.to<std::integral_constant<int>().to<0>()
            );
        }
    );
};

test bind_in_not_scopable_type = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "scope<.*>::requires_<.*scope<.*>::is_referable,.*scope<.*>::try_create,.*scope<.*>::create>", "=.*dummy"
        #else
            "scope<.*dummy>::requires_<.*scope<.*>::is_referable,.*scope<.*>::try_create,.*scope<.*>::create>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct dummy {};
        int main() {
            auto injector = di::make_injector(
                di::bind<int>().in(dummy{})
            );
        }
    );
};

test bind_has_disallowed_specifiers_expected = [] {
    auto errors_ = errors(
        "constraint not satisfied", "bind<.*int.*\\*>::has_disallowed_specifiers"
    );

    expect_compile_fail("", errors_,
        int main() {
            di::make_injector(
                di::bind<int*>()
            );
        }
    );
};

test bind_has_disallowed_specifiers_given = [] {
    auto errors_ = errors(
        "constraint not satisfied", "bind<const.*int.*&>::has_disallowed_specifiers"
    );

    expect_compile_fail("", errors_,
        int main() {
            di::make_injector(
                di::bind<int>().to<const int&>()
            );
        }
    );
};

test bind_external_repeated = [] {
    auto errors_ = errors(
        "constraint not satisfied", "bind<.*int>::is_bound_more_than_once"
    );

    expect_compile_fail("", errors_,
        int main() {
            di::make_injector(
                di::bind<int>().to(42)
              , di::bind<int>().to(87)
            );
        }
    );
};

test bind_multiple_times = [] {
    auto errors_ = errors(
        "constraint not satisfied", "bind<.*i>::is_bound_more_than_once"
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };
        int main() {
            di::make_injector(
                di::bind<i>().to<impl1>()
              , di::bind<i>().to<impl2>()
            );
        }
    );
};

test bind_narrowed_type = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_not_related_to<int>",
            "=.*double"
        #else
            "bind<.*double>::is_not_related_to<int>"
        #endif
    );

    expect_compile_fail("", errors_,
        int main() {
            di::make_injector(
                di::bind<int>().to<double>()
            );
        }
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
        int main() {
            di::make_injector(
                di::bind<int>().to<impl>()
            );
        }
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
        int main() {
            di::make_injector(
                di::bind<a, b>().to<c>()
            );
        }
    );
};

test bind_repeated = [] {
    auto errors_ = errors(
            "constraint not satisfied", "bind<.*i>::is_bound_more_than_once"
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };
        int main() {
            di::make_injector(
                di::bind<i>().to<impl1>()
              , di::bind<i>().to<impl1>()
            );
        }
    );
};

test bind_to_different_types = [] {
    auto errors_ = errors(
            "constraint not satisfied", "bind<.*i>::is_bound_more_than_once"
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl1 : i { };
        struct impl2 : i { };
        int main() {
            di::make_injector(
                di::bind<i>().to<impl1>()
              , di::bind<i>().to<impl2>()
            );
        }
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
        int main() {
            di::injector<c, c> injector = di::make_injector();
        }
    );
};

test make_injector_wrong_arg = [] {
    auto errors_ = errors(
            "constraint not satisfied",
        #if defined(_MSC_VER)
            "bind<.*>::is_neither_a_dependency_nor_an_injector", "=.*dummy"
        #else
            "bind<.*dummy>::is_neither_a_dependency_nor_an_injector"
        #endif
    );

    expect_compile_fail("", errors_,
        struct dummy { };
        int main() {
            di::make_injector(dummy{});
        }
    );
};

test bind_in_not_scopable = [] {
    expect_compile_fail("", errors(),
        struct not_scopable { };
        int main() {
            di::bind<int>().in(not_scopable{});
        }
    );
};

// ---------------------------------------------------------------------------

test dummy_config = [] {
    expect_compile_fail("", errors(),
        struct dummy_config { };
        int main() {
            di::make_injector<dummy_config>();
        }
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
        int main() {
            di::make_injector<test_config>();
        }
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
        int main() {
            di::make_injector<test_config>();
        }
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
        int main() {
            di::make_injector<test_config>();
        }
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
        int main() {
            di::make_injector<test_config>();
        }
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
          , "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { c(i*) { } };
        int main() {
            di::make_injector().create<c>();
        }
    );
};

test create_polymorphic_type_without_binding_using_multi_bindings = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "create<c>()"
          , "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?"
        #endif
    );

    expect_compile_fail("<include> memory <include> vector", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { c(std::vector<std::unique_ptr<i>>) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<i*[]>().to<i, i>()
            );
            injector.create<c>();
        }
    );
};

test create_polymorphic_type_without_binding_using_multi_bindings_named = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::named<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "create<c>()"
          , "type is not bound, did you forget to add: 'di::bind<interface>.named\\(name\\).to<implementation>()'?"
        #endif
    );

    expect_compile_fail("<include> memory <include> set", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { c(std::set<std::unique_ptr<i>>) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<i*[]>().to<i, di::named<class I>>()
              , di::bind<i>().to<impl>()
              , di::bind<i>().named<class I>()
            );
            injector.create<c>();
        }
    );
};

test create_polymorphic_type_without_binding_named = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::named<.*>::is_not_bound"
        #if !defined(_MSC_VER)
          , "type is not bound, did you forget to add: 'di::bind<interface>.named\\(name\\).to<implementation>()'?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct dummy { };
        struct c { BOOST_DI_INJECT(c, (named = dummy{}) i*) { } };
        int main() {
            di::make_injector().create<c>();
        }
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
          , "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?"
        #endif
    );

    expect_compile_fail("<include> memory", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct c { c(int, std::unique_ptr<i>) { } };
        int main() {
            di::injector<i> injector = di::make_injector();
        }
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
          , "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?"
        #endif
    );

    expect_compile_fail("<include> memory", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };
        struct c { std::shared_ptr<i> i_; };
        int main() {
            di::injector<c> injector = di::make_injector(); // di::bind<i>().to<impl>()
        }
    );
};

test create_not_fully_implemented_type = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::is_not_fully_implemented"
        #if !defined(_MSC_VER)
          , "create<c>()"
          , "type is not implemented, did you forget to implement all interface methods?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { };
        struct c { c(i*) { } };
        int main() {
            di::make_injector(di::bind<i>().to<impl>()).create<c>();
        }
    );
};

test create_not_fully_implemented_type_named = [] {
    auto errors_ = errors(
        #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
            "creatable constraint not satisfied",
        #endif
            "abstract_type<.*>::named<.*>::is_not_fully_implemented"
        #if !defined(_MSC_VER)
          , "type is not implemented, did you forget to implement all interface methods?"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { };
        struct dummy { };
        struct c { BOOST_DI_INJECT(c, (named = dummy{}) i*) { } };
        int main() {
            di::make_injector(di::bind<i>().named(dummy{}).to<impl>()).create<c>();
        }
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
        struct c { c(int*) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<int>().in(di::singleton)
            );
            injector.create<c>();
        }
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
        struct c { c(int*) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<int>().to(42)
            );
            injector.create<c>();
        }
    );
};

test scope_traits_external_not_referable = [] {
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
        struct c { c(int&) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<int>().to(42) // lvalue can't be converted to a reference
            );
            injector.create<c>();
        }
    );
};

test scope_traits_external_not_referable_named = [] {
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
        struct dummy { };
        struct c { BOOST_DI_INJECT(c, (named = dummy{}) int&) { } };
        int main() {
            auto injector = di::make_injector(
                di::bind<int>().named(dummy{}).to(42) // lvalue can't be converted to a reference
            );
            injector.create<c>();
        }
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

        struct c { c(int, double, float) { } };
        int main() {
            auto injector = di::make_injector<config>();
            injector.create<c>();
        }
    );
};

#if 0
#if defined(__cpp_concepts)
    test create_auto_type_without_binding = [] {
        auto errors_ = errors(
            #if (__clang_major__ == 3) && (__clang_minor__ > 4) || (defined(__GNUC___) && !defined(__clang__)) || defined(_MSC_VER)
                "creatable constraint not satisfied",
            #endif
                "type<.*>::has_not_bound_generic_types"
            #if !defined(_MSC_VER)
              , "create<c>()"
              , "generic type is not bound, did you forget to add di::bind<di::_>.to<implementation>()?"
            #endif
        );

        expect_compile_fail("", errors_,
            struct c { BOOST_DI_INJECT(c, auto) { } };
            int main() {
                di::make_injector().create<c>();
            }
        );
    };
#endif
#endif

// ---------------------------------------------------------------------------

test ctor_inject_limit_out_of_range = [] {
    expect_compile_fail("-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3",
        errors("Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"),
        struct c { BOOST_DI_INJECT(c, int, int, int, int) { } };
        int main() {
            auto injector = di::make_injector();
            injector.create<c>();
        }
    );
};

test ctor_limit_out_of_range = [] {
    auto errors_ = errors(
    #if defined(__GNUC__) && !defined(__clang__)
        "type<.*>::has_to_many_constructor_parameters::max<.*>.*= 3.*=.*c"
    #else
        "type<.*c>::has_to_many_constructor_parameters::max<3>"
    #endif
    );

    expect_compile_fail("-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", errors_,
        struct c { c(int, int, int, int) { } };
        int main() {
            auto injector = di::make_injector();
            injector.create<c>();
        }
    );
};

test injector_ctor_ambiguous = [] {
    auto errors_ = errors(
        #if defined(__GNUC__) && !defined(__clang__)
            "type<.*>::has_to_many_constructor_parameters::max<.*>.*= 10.*=.*ctor"
        #else
            "type<.*ctor>::has_to_many_constructor_parameters::max<10>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct ctor {
            ctor(int, double) { }
            ctor(double, int) { }
        };
        int main() {
            di::make_injector().create<ctor>();
        }
    );
};

test ctor_number_of_args_is_not_equal = [] {
    auto errors_ = errors(
    #if defined(__GNUC__) && !defined(__clang__)
        "type<.*>::has_ambiguous_number_of_constructor_parameters::given<.*>::expected<.*>.*= 4.*= 2.*=.*c"
    #else
        "type<.*c>::has_ambiguous_number_of_constructor_parameters::given<2>::expected<4>"
    #endif
    );

    expect_compile_fail("", errors_,
        struct c {
            BOOST_DI_INJECT_TRAITS(int, int); // 2
            c(int, int, int, int) { } // 4
        };
        int main() {
            auto injector = di::make_injector();
            injector.create<c>();
        }
    );
};

test named_paramater_spelling = [] {
    expect_compile_fail("", errors(),
        auto name = []{};
        struct c { BOOST_DI_INJECT(c, (NAMED = name) int) { } };
        int main() { }
    );
};

test circular_dependencies_simple = [] {
    expect_compile_fail("", errors(),
        struct cd2;
        struct cd1 { cd1(cd2*) { }; };
        struct cd2 { cd2(cd1*) { }; };
        int main() {
            auto injector = di::make_injector();
            injector.create<cd1>();
        }
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
        int main() {
            auto injector = di::make_injector();
            injector.create<cd5>();
        }
    );
};

// ---------------------------------------------------------------------------

test access_injector_is_creatable = [] {
    expect_compile_fail("", errors(),
        int main() {
            auto injector = di::make_injector();
            using type = decltype(injector)::is_creatable<int>;
        }
    );
};

test access_injector_try_create = [] {
    expect_compile_fail("", errors(),
        int main() {
            auto injector = di::make_injector();
            injector.try_create<int>();
        }
    );
};

test access_injector_create_impl = [] {
    expect_compile_fail("", errors(),
        int main() {
            auto injector = di::make_injector();
            injector.create_impl(di::aux::type<int>{});
        }
    );
};

test access_injector_create_succesful_impl = [] {
    expect_compile_fail("", errors(),
        int main() {
            auto injector = di::make_injector();
            injector.create_successful_impl(di::aux::type<int>{});
        }
    );
};

test access_injector_exposed_is_creatable = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::injector<> injector = di::make_injector();
            using type = decltype(injector)::is_creatable<int>;
        }
    );
};

test access_injector_exposed_try_create = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::injector<> injector = di::make_injector();
            injector.try_create<int>();
        }
    );
};

test access_injector_exposed_create_impl = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::injector<> injector = di::make_injector();
            injector.create_impl(di::aux::type<int>{});
        }
    );
};

test access_injector_exposed_create_succesful_impl = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::injector<> injector = di::make_injector();
            injector.create_successful_impl(di::aux::type<int>{});
        }
    );
};

test access_dependency_try_create = [] {
    expect_compile_fail("", errors(),
        int main() {
            di::bind<int>().try_create<int>();
        }
    );
};

test access_dependency_create = [] {
    expect_compile_fail("", errors(),
        struct provider { };
        int main() {
            di::bind<int>().create<int>(provider{});
        }
    );
};

#if !defined(_MSC_VER)
    test access_dependency_is_referable = [] {
        expect_compile_fail("", errors(),
            int main() {
                using type = std::remove_reference_t<decltype(di::bind<int>())>::is_referable<int>;
            }
        );
    };
#endif

// ---------------------------------------------------------------------------

