//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_ALLOCATOR_HPP
#define BOOST_DI_FAKE_ALLOCATOR_HPP

#include <limits>
#include <cassert>

namespace boost {
namespace di {

int allocate_calls = 0;
int deallocate_calls = 0;

template<typename T>
class fake_allocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template<typename U>
    struct rebind
    {
        typedef fake_allocator<U> other;
    };

    fake_allocator() throw() { }

    fake_allocator(const fake_allocator&) throw() { }

    template<typename U>
    fake_allocator(const fake_allocator<U>&) throw() { }

    ~fake_allocator() throw() { }

    pointer allocate(size_type n, const void* = 0) {
        allocate_calls++;
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type) {
        deallocate_calls++;
        ::operator delete(p);
    }
};

} // namespace di
} // namespace boost

#endif

