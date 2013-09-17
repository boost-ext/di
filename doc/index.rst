+++++++++++++++++++++++++++++++++++++++++++++++++
 The Boost Dependency Injection Library
+++++++++++++++++++++++++++++++++++++++++++++++++

|(logo)|__

.. |(logo)| image:: ../../../../boost.png
   :alt: Boost

__ ../../../../index.htm

-------------------------------------

:Abstract: The Boost Dependency Injection library ...

-------------------------------------

:Author:        Krzysztof Jusiak
:Contact:       krzysztof at jusiak dot net
:Copyright:     Copyright Krzysztof Jusiak, 2012-2013
:License:       Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

-------------------------------------

.. contents:: **Table of Contents**
   :depth: 2

.. role:: concept
   :class: concept

.. role:: vellipsis
   :class: vellipsis

.. section-numbering::

-------------------------------------

============
 Motivation
============

============
 Factory vs Dependency Injection Framework
============
Seperate creation/instantiation | Business logic
    -> Factories
        writing factories is tedious
            - boilerplate code
            - circular dependecies
            - scopes (singleton by value)
        more up front checking
        -> Dependency injection framework

============
 Features
============
    * injection via constructor
    * configuration via front ends (base, fusion)
    * binding via interface/implementation
    * binding via is_base_of (smart binding)
    * binding via call_stack (path binding)
    * binding via external objects
    * binding via named parameters
    * providers
    * scopes (per_request, singleton, session, custom scope)
    * modules
    * visitors
    * policies (ex. circular dependencies detection)
    * compile time approach (supports C++98, C++11)

============
 Supported Compilers
============
    * gcc   >= 4.3
    * clang >= 3.0
    * msvc

