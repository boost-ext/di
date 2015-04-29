//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[containers
//<-
#include <cassert>
#include <memory>
#include <vector>
//->

#include <boost/di/aux_/type_traits.hpp>

namespace boost { namespace di { namespace aux {

/*<<transform constructor container type to expected binding type `type[]`>>*/
template<class T>
struct deref_type<std::vector<T>> {
    using type = typename deref_type<T>::type[];
};

} // aux

namespace concepts {

/*<<allow binding `type[]` to anything>>*/
template<class I, class T>
auto boundable_impl(I[], T&&) -> std::true_type;

}}} // boost::di::concepts

#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() noexcept = default; };
struct implementation1 : interface { };
struct implementation2 : interface { };
//->

/*<<all types creator and adder to container type>>*/
template<class... Ts>
class all_of_impl {
public:
    template<class TInjector, class I, class T>
    auto add(const TInjector& injector, std::vector<std::shared_ptr<I>>& v, const di::aux::type<T>&) const {
        v.push_back(injector.template create<std::shared_ptr<T>>());
    }

    template<class TInjector, class I, class T>
    auto add(const TInjector& injector, std::vector<std::unique_ptr<I>>& v, const di::aux::type<T>&) const {
        v.emplace_back(injector.template create<std::unique_ptr<T>>());
    }

    template<class TInjector, class TContainer>
    di::aux::remove_accessors_t<TContainer>
    operator()(const TInjector& injector, const di::aux::type<TContainer>&) const {
        di::aux::remove_accessors_t<TContainer> container;
        [](...){}((add(injector, container, di::aux::type<Ts>{}), 0)...);
        return container;
    }
};

template<class... Ts>
all_of_impl<Ts...> all_of{};

auto vector_shared = []{};
auto vector_unique = []{};

struct example {
    BOOST_DI_INJECT(example,
        (named = vector_shared) const std::vector<std::shared_ptr<interface>>& v1
      , (named = vector_unique) std::vector<std::unique_ptr<interface>> v2
    ) {
        assert(v1.size() == 2);
        assert(v2.size() == 1);
    }
};

int main() {
    auto injector = di::make_injector(
        /*<<interface[] match with vector<interface>, vector<unique_ptr<interface>>, ..., list<interface>, queue<shared_ptr<interface>, ...>>*/
        di::bind<interface[]>.named(vector_shared).to(all_of<implementation1, implementation2>)
      , di::bind<interface[]>.named(vector_unique).to(all_of<implementation1>)
    );

    injector.create<example>();
}

//]

