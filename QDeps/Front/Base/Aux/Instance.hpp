//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_ALLOCATOR_HPP
#define QDEPS_BACK_AUX_ALLOCATOR_HPP

namespace QDeps
{
namespace Back
{
namespace Aux
{

template<typename T, typename TContext = boost::mpl::vector0<>, typename TName = void, typename Enable = void>
class Allocator
{
    typedef boost::variant<const T&, T&, boost::shared_ptr<T> > type;

    class AcquireImpl : public boost::static_visitor<T&>
    {
    public:
        T& operator()(const T& p_member) const
        {
            return const_cast<T&>(p_member);
        }

        T& operator()(T& p_member) const
        {
            return p_member;
        }

        T& operator()(boost::shared_ptr<T> p_member) const
        {
            return *p_member;
        }
    };

public:
    Allocator()
        : m_member(boost::make_shared<T>())
    { }

    explicit Allocator(const T& p_member)
        : m_member(p_member)
    { }

    explicit Allocator(T& p_member)
        : m_member(p_member)
    { }

    explicit Allocator(boost::shared_ptr<T> p_member)
        : m_member(p_member)
    { }

    void set(T& p_new)
    {
        m_member = p_new;
    }

    void set(const T& p_new)
    {
        m_member = p_new;
    }

    void set(boost::shared_ptr<T> p_new)
    {
        m_member = p_new;
    }

    type get()
    {
        return m_member;
    }

    T& acquire()
    {
        return boost::apply_visitor(AcquireImpl(), m_member);
    }

private:
    type m_member;
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif

