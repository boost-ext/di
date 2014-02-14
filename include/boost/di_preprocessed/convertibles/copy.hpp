#ifndef BOOST_DI_CONVERTIBLES_COPY_HPP
#define BOOST_DI_CONVERTIBLES_COPY_HPP

#include "boost/di_preprocessed/aux_/memory.hpp"
#include "boost/di_preprocessed/named.hpp"
#include <boost/type.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class copy
{
    typedef function<T*()> object_t;

public:
    template<typename I>
    copy(const I& object)
        : object_(object)
    { }

    template<typename I>
    I operator()(const type<I>&, typename disable_if<is_polymorphic<I> >::type* = 0) const {
        scoped_ptr<I> ptr(object_());
        return *ptr;
    }

    template<typename I>
    const I& operator()(const type<const I&>&) const {
        ref_ = aux::shared_ptr<I>(object_());
        return *ref_;
    }

    template<typename I>
    I* operator()(const type<I*>&) const {
        return object_();
    }

    template<typename I, typename TName>
    I operator()(const type<named<I, TName> >&) const {
        scoped_ptr<I> ptr(object_());
        return *ptr;
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return aux::shared_ptr<I>(object_());
    }

    template<typename I>
    const aux::shared_ptr<I>& operator()(const type<const aux::shared_ptr<I>&>&) const {
        ref_ = aux::shared_ptr<I>(object_());
        return ref_;
    }

    template<typename I, typename TName>
    I* operator()(const type<named<aux::shared_ptr<I>, TName> >&) const {
        return object_();
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I> >&) const {
        return aux::auto_ptr<I>(object_());
    }

    template<typename I, typename TName>
    I* operator()(const type<named<aux::auto_ptr<I>, TName> >&) const {
        return object_();
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I> >&) const {
        return aux::unique_ptr<I>(object_());
    }

    template<typename I, typename TName>
    I* operator()(type<named<aux::unique_ptr<I>, TName> >&) const {
        return object_();
    }

    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    object_t object_;
    mutable aux::shared_ptr<T> ref_;
};

}
}
}

#endif

