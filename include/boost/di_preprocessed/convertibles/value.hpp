#ifndef BOOST_DI_CONVERTIBLES_VALUE_HPP
#define BOOST_DI_CONVERTIBLES_VALUE_HPP

#include "boost/di_preprocessed/named.hpp"
#include "boost/di_preprocessed/aux_/memory.hpp"
#include <utility>
#include <boost/type.hpp>
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class value
{
public:
    value(T value)
        : value_(value)
    { }

    template<typename I>
    I operator()(const type<I>&) const {
        return value_;
    }

    template<typename I>
    I& operator()(const type<I&>&) const {
        return value_;
    }

    template<typename I, typename TName>
    I operator()(const type<named<I, TName>&>&) const {
        return value_;
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    const aux::shared_ptr<I>& operator()(const type<const aux::shared_ptr<I>& >&) {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    I* operator()(const type<I*>&) {
        return new I(value_);
    }
    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    mutable T value_;
};

}
}
}

#endif

