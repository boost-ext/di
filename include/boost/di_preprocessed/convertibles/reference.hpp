#ifndef BOOST_DI_CONVERTIBLES_REFERENCE_HPP
#define BOOST_DI_CONVERTIBLES_REFERENCE_HPP

#include "boost/di_preprocessed/named.hpp"
#include <boost/type.hpp>
#include <boost/ref.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class reference
{
public:
    reference(const reference_wrapper<T>& object)
        : object_(object)
    { }

    template<typename I>
    I operator()(const type<I>&) const {
        return object_;
    }

    template<typename I>
    I& operator()(const type<I&>&) const {
        return object_;
    }

    template<typename I, typename TName>
    named<I, TName> operator()(const type<named<I, TName> >&) const {
        return named<I, TName>(object_);
    }

    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    reference_wrapper<T> object_;
};

}
}
}

#endif

