//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_ASSERT_HPP
#define BOOST_DI_FAKE_ASSERT_HPP

#include <stdexcept>

#define static_assert(c, msg) \
    if (!c) { throw ::boost::di::assert_exception(#msg); }

namespace boost {
namespace di {

class assert_exception : public std::exception {
public:
    assert_exception(const char* msg)
        : msg_(msg)
    { }

    const char* what() const noexcept override { return msg_; }

private:
    const char* msg_;
};

} // namespace di
} // namespace boost

#endif

