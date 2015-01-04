//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[uml_dumper
//<-
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace utils = boost::units::detail;

struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(std::shared_ptr<i0>, int) { } };
struct c2 { c2(int, double, char) { } };
struct c3 { c3(std::shared_ptr<c1>, std::shared_ptr<c2>) { } };
//->
namespace di = boost::di;

/**
 * http://plantuml.sourceforge.net/objects.html
 * ./uml_dumper | java -jar plantuml.jar -p > uml_dumper.png
 */
/*<<define `plant_uml` printer>>*/
class plant_uml {
public:
    void on_begin(std::stringstream& stream) const {
        stream << "@startuml uml_dumper.png" << std::endl;
    }

    void on_end(std::stringstream& stream) const {
        stream << "@enduml" << std::endl;
    }

    template<typename TDependency>
    void on_call(std::stringstream& stream
               , const TDependency& from
               , const TDependency& to) const {
        if (to.expected != to.given) {
            stream << "\"" << to.expected << "\" <|-- \"" << to.given << "\"" << std::endl;
        }

        stream << "\"" << from.expected << "\" .. \"" << to.expected << "\"" << std::endl;
    }
};

/*<<define `uml_dumper` with policy parameter>>*/
template<typename TPolicy>
class uml_dumper : public TPolicy {
    struct dependency {
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

    uml_dumper& operator=(const uml_dumper&);

public:
    explicit uml_dumper(std::stringstream& stream)
        : stream_(stream) {
        this->on_begin(stream_);
    }

    ~uml_dumper() {
        this->on_end(stream_);
    }

    /*<<Definition of the dumper call operator requirement>>*/
    template<typename T>
    void operator()(const T& data) const {
        //auto call_stack_size = mpl::size<typename T::call_stack>::value;
        auto call_stack_size = 0;

/*        while (!context_.empty() && context_.back().context_size >= call_stack_size) {*/
            //context_.pop_back();
        /*}*/

        using dependency_t = std::remove_reference_t<decltype(data.dep)>;

        context_.emplace_back(
            dependency(
                utils::demangle(typeid(typename dependency_t::expected).name())
              , utils::demangle(typeid(typename dependency_t::given).name())
              , utils::demangle(typeid(typename dependency_t::scope).name())
              , call_stack_size
            )
        );

        auto context_size = context_.size();

        if (context_size > 1 /*&& context_size > (call_stack_size - 2)*/) {
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

std::stringstream stream_;

class uml_config : public di::config {
public:
    auto& policies() const noexcept {
        static auto s = di::make_policies(uml_dumper<plant_uml>{stream_});
        return s;
    }

    uml_config
};

int main() {
    /*<<define injector>>*/
    auto injector = di::make_injector<uml_config>(
        di::bind<i0, c0>
    );

    /*<<iterate through created objects with `uml_dumper`>>*/
    injector.create<c3>();
    std::clog << stream_.str();

    /*<<output [@images/uml_dumper.png [$images/uml_dumper.png [width 75%] [height 75%] ]]>>*/
    return 0;
}

//]

