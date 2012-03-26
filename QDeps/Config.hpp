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
 * struct Defaults<, QDeps::Specialized>
 * {
 * };
 * } // namespace QDeps
 * @endcode
 */
class Specialized { };
template<typename, typename = Specialized> class Defaults;

namespace Back
{
template<typename = void, typename = void, typename = void> class Policy;
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
class Defaults<Detail::Policy, TDefault>
{
public:
    typedef Back::Policy
    <
        //DisallowCircularDependencies,
        //VerifyCtorBinding,
        //DisallowModuleDependencyRepetition,
        //CheckForNotSharedSingletons
    >
    type;
};

} // namespace QDeps

#endif

