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
#include <string>
#include <vector>
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

        #if defined(__GNUC__)
            compiler = "g++";
            errors << "-c -std=c++1y -Werror ";
        #elif defined(__clang__)
            compiler = "clang++";
            errors << "-c -std=c++1y -Wno-all -Werror -Wno-error=deprecated-declarations";
        #elif defined(_MSC_VER)
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
                << " " << source_code << " "
        #if defined(_MSC_VER)
                << ">"
        #else
                << "2>"
        #endif
                << " error" << id << ".out";
    }

    expect(std::system(command.str().c_str()));

    file<std::ifstream> output{std::string{"error"} + std::to_string(id) + ".out"};
    std::vector<bool> matches(errors.size(), false);
    auto lines = 0;
    for (std::string line; std::getline(output, line);) {
        std::cout << "dupa: " << line << std::endl;
        auto i = 0;
        for (const auto& rgx : errors) {
            if (std::regex_match(line, std::regex{rgx})) {
        std::cout << "match: " << line << std::endl;
                matches[i] = true;
            }
            ++i;
        }
        ++lines;
    }

    if (!errors.empty()) {
        constexpr auto MAX_ERROR_LINES_COUNT = 50;
        expect(lines < MAX_ERROR_LINES_COUNT);
    }

    if (std::find(matches.begin(), matches.end(), false) != matches.end()) {
        throw std::runtime_error(command.str());
    }
}

} // namespace

#define expect_compile_fail(defines, error, ...) \
    compail_fail(__LINE__, defines, error, #__VA_ARGS__)

test bind_external_repeated = [] {
    auto errors_ = errors(
        #if defined(_MSC_VER)
            "constraint_not_satisfied<.*bound_type<T,TName>::is_bound_more_than_once",
            "T=.*int",
            "TName=.*no_name"
        #else
            "constraint_not_satisfied<.*bound_type<.*int, .*no_name>::is_bound_more_than_once"
        #endif
    );

    expect_compile_fail("", errors_,
        di::make_injector(
            di::bind<int>().to(42)
          , di::bind<int>().to(87)
        );
    );
};

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

test bind_multiple_times = [] {
    auto errors_ = errors(
        #if defined(_MSC_VER)
            "constraint_not_satisfied<.*bound_type<T,TName>::is_bound_more_than_once",
            "T=.*i",
            "TName=.*no_name"
        #else
            "constraint_not_satisfied<.*bound_type<.*i, .*no_name>::is_bound_more_than_once"
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

test bind_not_compatible_types = [] {
    auto errors_ = errors(
        #if defined(_MSC_VER)
            "constraint_not_satisfied<.*bound_type<.*>::is_not_related_to<int>",
            "=.*impl"
        #else
            "constraint_not_satisfied<.*bound_type<.*impl>::is_not_related_to<int>"
        #endif
    );

    expect_compile_fail("", errors_,
        struct i { };
        struct impl : i { };

        auto injector = di::make_injector(
            di::bind<int, impl>()
        );

        injector.create<int>();
    );
};

test bind_repeated = [] {
    auto errors_ = errors(
        #if defined(_MSC_VER)
            "constraint_not_satisfied<.*bound_type<T,TName>::is_bound_more_than_once",
            "T=.*i",
            "TName=.*no_name"
        #else
            "constraint_not_satisfied<.*bound_type<.*i, .*no_name>::is_bound_more_than_once"
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
        #if defined(_MSC_VER)
            "constraint_not_satisfied<.*bound_type<T,TName>::is_bound_more_than_once",
            "T=.*i",
            "TName=.*no_name"
        #else
            "constraint_not_satisfied<.*bound_type<.*i, .*no_name>::is_bound_more_than_once"
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

test create_polymorphic_type_without_binding = [] {
    expect_compile_fail(
        "",
        errors(
            "creatable constraint not satisfied"
          , "create<c>()"
          , "abstract_type<.>::is_not_bound"
          , "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        ),
        struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        struct impl : i { void dummy() override { } };

        struct c {
            c(i*) { }
        };

        di::make_injector().create<c>();
    );
};

//test ctor_inject_limit_out_of_range = [] {
    //expect_compile_fail(
        //"-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3",
        //errors("Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"),
        //struct c {
            //BOOST_DI_INJECT(c, int, int, int, int) { }
        //};

        //auto injector = di::make_injector();
        //injector.create<c>();
    //);
//};

//test ctor_limit_out_of_range = [] {
    //auto errors_ = errors(
    //#if defined(__GNUC__)
        //"number_of_constructor_arguments_is_out_of_range_for<.*>::max<.*>.*= 3.*=.*c"
    //#elif defined(__clang__)
        //"number_of_constructor_arguments_is_out_of_range_for<.*c>::max<3>"
    //#endif
    //);

    //expect_compile_fail(
        //"-DBOOST_DI_CFG_CTOR_LIMIT_SIZE=3", errors_,
        //struct c {
            //c(int, int, int, int) { }
        //};

        //auto injector = di::make_injector();
        //injector.create<c>();
    //);
//};

//test exposed_multiple_times = [] {
    //expect_compile_fail(
        //"",
        //errors("constraint_not_satisfied<.*bound_type<.*c>::is_bound_more_than_once"),
        //struct c { };
        //di::injector<c, c> injector = di::make_injector();
    //);
//};

//test exposed_not_creatable = [] {
    //expect_compile_fail(
        //"-include memory",
        //errors(
            //"creatable constraint not satisfied"
          //, "create<T>"
          //, "abstract_type<.>::is_not_bound"
          //, "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        //),
        //struct i {
            //virtual ~i() noexcept = default; virtual void dummy() = 0;
        //};

        //struct c {
            //c(int, std::unique_ptr<i>) {}
        //};

        //di::injector<i> injector = di::make_injector();
    //);
//};

//test exposed_polymorphic_type_without_binding = [] {
    //expect_compile_fail(
        //"-include memory",
        //errors(
            //"creatable constraint not satisfied"
          //, "create<T>"
          //, "abstract_type<.>::is_not_bound"
          //, "type not bound, did you forget to add: 'di::bind<interface, implementation>'?"
        //),
        //struct i { virtual ~i() noexcept = default; virtual void dummy() = 0; };
        //struct impl : i { void dummy() override { } };
        //struct c { std::shared_ptr<i> i_; };

        //di::injector<c> injector = di::make_injector(); // di::bind<i, impl>()
    //);
//};

//test injector_ctor_ambiguous = [] {
    //expect_compile_fail(
        //"",
        //errors(
            //"creatable constraint not satisfied",
            //"increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters"
        //),
        //struct ctor {
            //ctor(int, double) { }
            //ctor(double, int) { }
        //};

        //di::make_injector().create<ctor>();
    //);
//};

//test injector_shared_by_copy = [] {
    //expect_compile_fail(
        //"",
        //errors(
            //"creatable constraint not satisfied",
            //"type<.*>::is_not_convertible_to<.*>",
            //"wrapper is not convertible to requested type, did you mistake the scope?"
        //),
        //struct c {
            //c(int*) { }
        //};

        //auto injector = di::make_injector(
            //di::bind<int>().in(di::singleton)
        //);

        //injector.create<c>();
    //);
//};

//test bind_wrapper_not_convertible = [] {
    //expect_compile_fail(
        //"",
        //errors(
            //"creatable constraint not satisfied",
            //"type<.*>::is_not_convertible_to<.*>",
            //"wrapper is not convertible to requested type, did you mistake the scope?"
        //),
        //struct c {
            //c(int*) { }
        //};

        //auto injector = di::make_injector(
            //di::bind<int>().to(42)
        //);

        //injector.create<c>();
    //);
//};

//test make_injector_wrong_arg = [] {
    //expect_compile_fail(
        //"",
        //errors("constraint_not_satisfied<.*bound_type<.*neither_module_nor_injector_nor_module>::is_neither_a_dependency_nor_an_injector"),
        //struct neither_module_nor_injector_nor_module { };
        //auto injector = di::make_injector(neither_module_nor_injector_nor_module{});
    //);
//};

//test named_paramater_spelling = [] {
    //expect_compile_fail(
        //"",
        //errors(),
        //auto name = []{};
        //struct c {
            //BOOST_DI_INJECT(c, (NAMED = name) int) { }
        //};
    //);
//};

//test policy_constructible = [] {
    //auto errors_ = errors(
        //"creatable constraint not satisfied",
        //"type disabled by constructible policy, added by BOOST_DI_CFG or make_injector<CONFIG>",
    //#if defined(__GNUC__)
        //"type<.*>::not_allowed_by.*int",
        //"type<.*>::not_allowed_by.*double",
        //"type<.*>::not_allowed_by.*float"
    //#elif defined(__clang__)
        //"type<int>::not_allowed_by",
        //"type<double>::not_allowed_by",
        //"type<float>::not_allowed_by"
    //#else
        //".*"
    //#endif
    //);

    //expect_compile_fail(
        //"", errors_,
        //class config : public di::config {
        //public:
            //static auto policies(...) noexcept {
                //using namespace di::policies;
                //return di::make_policies(constructible(is_bound<di::_>{}));
            //}
        //};

        //struct c {
            //c(int, double, float) { }
        //};

        //auto injector = di::make_injector<config>();
        //injector.create<c>();
    //);
//};

//test scope_traits_external_not_referable = [] {
    //expect_compile_fail(
        //"",
        //errors(
            //"creatable constraint not satisfied",
            //"when_creating<.*>::type<.*>",
            //"reference type not bound, did you forget to add `auto value = ...; di::bind<T>.to\\(value\\)`"
        //),
        //struct c {
            //c(int&) { }
        //};

        //auto injector = di::make_injector(
            //di::bind<int>().to(42) // lvalue can't be converted to a reference
        //);

        //injector.create<c>();
    //);
//};

//test circular_dependencies_simple = [] {
    //expect_compile_fail(
        //"", errors(),
        //struct cd2;
        //struct cd1 { cd1(cd2*) { }; };
        //struct cd2 { cd2(cd1*) { }; };

        //auto injector = di::make_injector();
        //injector.create<cd1>();
    //);
//};

//test circular_dependencies_complex = [] {
    //expect_compile_fail(
        //"", errors(),
        //struct cd2;
        //struct cd5;
        //struct cd1 { cd1(cd2*) { }; };
        //struct cd2 { cd2(cd1*) { }; };
        //struct cd3 { cd3(cd5*) { }; };
        //struct cd4 { cd4(cd3*) { }; };
        //struct cd5 { cd5(cd4*) { }; };

        //auto injector = di::make_injector();
        //injector.create<cd5>();
    //);
//};

