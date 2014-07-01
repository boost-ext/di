//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[uml_visitor
//````C++98/03/11/14```
//<-
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/mpl/size.hpp>
#include <boost/units/detail/utility.hpp>
#include "common/data.hpp"
//->
#include <boost/di.hpp>

//<-
namespace mpl = boost::mpl;
namespace utils = boost::units::detail;
//->
namespace di = boost::di;

/**
 * http://plantuml.sourceforge.net/objects.html
 * ./uml_visitor | java -jar plantuml.jar -p > uml_visitor.png
 */
/*<<define `plant_uml` printer>>*/
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

/*<<define `uml_visitor` with policy parameter>>*/
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

    uml_visitor& operator=(const uml_visitor&);

public:
    explicit uml_visitor(std::stringstream& stream)
        : stream_(stream) {
        this->on_begin(stream_);
    }

    ~uml_visitor() {
        this->on_end(stream_);
    }

    /*<<Definition of the visitor call operator requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        std::size_t call_stack_size = mpl::size<typename T::call_stack>::value;

        while (!context_.empty() && context_.back().context_size >= call_stack_size) {
            context_.pop_back();
        }

        context_.push_back(
            dependency(
                utils::demangle(typeid(typename T::dependency::expected).name())
              , utils::demangle(typeid(typename T::dependency::given).name())
              , utils::demangle(typeid(typename T::dependency::scope).name())
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

int main() {
    /*<<define injector>>*/
    di::injector<c0> injector;

    /*<<iterate through created objects with `uml_visitor`>>*/
    std::stringstream stream;
    injector.visit<c3>(uml_visitor<plant_uml>(stream));
    std::cout << stream.str();

    /*<<output [@images/uml_visitor.png [$images/uml_visitor.png [width 75%] [height 75%] ]]>>*/
    return 0;
}

//]

