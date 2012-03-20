//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_CONFIG_HPP
#define QDEPS_CONFIG_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#if !defined(QDEPS_CTOR_CFG_VA_ARGS) && !defined(QDEPS_CTOR_CFG_BRACKET)
# define QDEPS_CTOR_CFG_VA_ARGS
#endif

#if !defined(QDEPS_FUNC_ARITY)
# define QDEPS_FUNC_ARITY 10
#endif

#if !defined(QDEPS_CTOR_UNIQUE_NAME)
# define QDEPS_CTOR_UNIQUE_NAME ctor__
#endif

namespace QDeps
{

/**
 * Example of changing default behavior using specialized class
 *
 * @code
 * #include <QDesp/Config.hpp>
 *
 * namespace QDeps
 * {
 * template<>
 * struct Defaults<int, QDeps::Specialized>
 * {
 *     static boost::shared_ptr<int> create() { return boost::make_shared<int>(42); }
 * };
 * } // namespace QDeps
 * @endcode
 */
class Specialized { };

template<typename T, typename TDefault = Specialized>
struct Defaults
{
public:
    static boost::shared_ptr<T> create() { return boost::make_shared<T>(); }
};

template<typename TDefault>
class Defaults<int, TDefault>
{
public:
    static boost::shared_ptr<int> create() { return boost::make_shared<int>(0); }
};

//Policies

namespace Back
{
namespace Policy
{
template<typename = void, typename = void, typename = void> class Policy;
} // namespace Policy
} // namespace Back

namespace Detail
{
class Policy { };
class CircularDependencies { };
class BindingWithCtors { };
class ModulesDependeciesRepetitions { };
} // namespace Detail

class AllowCircularDependencies { };
class DisallowCircularDependencies { };

class VerifyBindingWithCtors { };
class DontVerifyBindingWithCtors { };

class AllowModulesDependeciesRepetitions { };
class DisallowModulesDependeciesRepetitions { };

template<typename TDefault>
class Defaults<Detail::CircularDependencies, TDefault>
{
public:
    typedef DisallowCircularDependencies type;
};

template<typename TDefault>
class Defaults<Detail::BindingWithCtors, TDefault>
{
public:
    typedef VerifyBindingWithCtors type;
};

template<typename TDefault>
class Defaults<Detail::ModulesDependeciesRepetitions, TDefault>
{
public:
    typedef DisallowModulesDependeciesRepetitions type;
};

template<typename TDefault>
class Defaults<Detail::Policy, TDefault>
{
public:
    typedef Back::Policy::Policy
    <
        Defaults<Detail::CircularDependencies>::type,
        Defaults<Detail::BindingWithCtors>::type,
        Defaults<Detail::ModulesDependeciesRepetitions>::type
    >
    type;
};

} // namespace QDeps

#endif

