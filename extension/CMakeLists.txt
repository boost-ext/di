#
# Copyright (c) 2012-2017 Kris Jusiak (kris at jusiak dot net)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
function(extension extension)
    string(REPLACE "/" "_" tmp ${extension})
    add_executable(extension.${tmp} ${CMAKE_CURRENT_LIST_DIR}/${extension}.cpp)
    add_test(extension.${tmp} extension.${tmp})
endfunction()

extension(bindings/constructor_bindings)
extension(bindings/contextual_bindings)
extension(injections/assisted_injection)
extension(injections/factory)
extension(injections/generics)
extension(injections/lazy)
extension(injections/named_parameters)
extension(injections/xml_injection)
extension(policies/serialize)
extension(policies/types_dumper)
extension(policies/uml_dumper)
extension(providers/mocks_provider)
extension(scopes/scoped_scope)
extension(scopes/session_scope)
extension(scopes/shared_scope)
