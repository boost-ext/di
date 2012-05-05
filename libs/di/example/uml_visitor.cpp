//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/mpl/size.hpp>
#include <boost/units/detail/utility.hpp>
#include <boost/di.hpp>

namespace mpl   = boost::mpl;
namespace utils = boost::units::detail;
namespace di    = boost::di;

namespace {

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i0>) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, int i, double d, char c) { }
};

struct c3
{
    BOOST_DI_CTOR(c3, boost::shared_ptr<c1>, boost::shared_ptr<c2>) { }
};

} // namespace

/**
 * http://plantuml.sourceforge.net/objects.html
 * ./uml_visitor | java -jar plantuml.jar -p > uml_visitor.png
 */
class plant_uml
{
public:
    void on_begin(std::stringstream& stream) const
    {
        stream << "@startuml uml_visitor.png" << std::endl;
    }

    void on_end(std::stringstream& stream) const
    {
        stream << "@enduml" << std::endl;
    }

    template<typename TDependency>
    void on_call(std::stringstream& stream
               , const TDependency& from
               , const TDependency& to) const
    {
        if (to.expected != to.given) {
            stream << "\"" << to.expected << "\" <|-- \"" << to.given << "\"" << std::endl;
        }

        stream << "\"" << from.expected << "\" .. \"" << to.expected << "\"" << std::endl;
    }
};

template<typename TPolicy>
class uml_visitor : public TPolicy
{
    struct dependency
    {
        dependency(const std::string& expected
                 , const std::string& given
                 , const std::string& scope
                 , std::size_t context_size)
            : expected(expected)
            , given(given)
            , scope(scope)
            , context_size(context_size) { }

        std::string expected;
        std::string given;
        std::string scope;
        std::size_t context_size;
    };

public:
    explicit uml_visitor(std::stringstream& stream)
        : stream_(stream) {
        this->on_begin(stream_);
    }

    ~uml_visitor() {
        this->on_end(stream_);
    }

    template<typename T>
    void operator()() const {
        std::size_t call_stack_size = mpl::size<typename T::context>::value;

        while (!context_.empty() && context_.back().context_size >= call_stack_size) {
            context_.pop_back();
        }

        context_.push_back(
            dependency(
                utils::demangle(typeid(typename T::expected).name())
              , utils::demangle(typeid(typename T::given).name())
              , utils::demangle(typeid(typename T::scope).name())
              , call_stack_size
            )
        );

        std::size_t context_size = context_.size();

        if (context_size > 1 && context_size > (call_stack_size - 2)) {
            this->on_call(
                stream_
              , context_[context_size - 2]
              , context_[context_size - 1]
            );
        }
    }

    std::stringstream& stream_;
    mutable std::vector<dependency> context_;
};

int main()
{
    typedef di::generic_module<
        di::singletons<
            di::bind<i0, c0>
        >
    > visitor_module;

    di::injector<visitor_module> injector;

    std::stringstream stream;
    injector.visit<c3>(uml_visitor<plant_uml>(stream));
    std::cout << stream.str();

    // @startuml uml_visitor.png
    // "(anonymous namespace)::c3" .. "(anonymous namespace)::c1"
    // "(anonymous namespace)::i0" <|-- "(anonymous namespace)::c0"
    // "(anonymous namespace)::c1" .. "(anonymous namespace)::i0"
    // "(anonymous namespace)::c3" .. "(anonymous namespace)::c2"
    // "(anonymous namespace)::c2" .. "int"
    // "(anonymous namespace)::c2" .. "double"
    // "(anonymous namespace)::c2" .. "char"
    // @enduml

    return 0;
}

