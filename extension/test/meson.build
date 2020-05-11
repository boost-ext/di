progs = [
  ['bindings_constructor_bindings', 'bindings/constructor_bindings.cpp'],
  ['bindings_contextual_bindings', 'bindings/contextual_bindings.cpp'],
  ['injections_extensible_injector', 'injections/extensible_injector.cpp'],
  ['injections_assisted_injection', 'injections/assisted_injection.cpp'],
  ['injections_factory', 'injections/factory.cpp'],
  ['injections_shared_factory', 'injections/shared_factory.cpp'],
  ['injections_lazy', 'injections/lazy.cpp'],
  ['injections_named_parameters', 'injections/named_parameters.cpp'],
  ['injections_xml_injection', 'injections/xml_injection.cpp'],
  ['policies_serialize', 'policies/serialize.cpp'],
  ['policies_types_dumper', 'policies/types_dumper.cpp'],
  ['policies_uml_dumper', 'policies/uml_dumper.cpp'],
  ['providers_heap', 'providers/heap.cpp'],
  ['providers_mocks_provider', 'providers/mocks_provider.cpp'],
  ['providers_runtime_provider', 'providers/runtime_provider.cpp'],
  ['scopes_scoped', 'scopes/scoped.cpp'],
  ['scopes_session', 'scopes/session.cpp'],
  ['scopes_shared', 'scopes/shared.cpp'],
  ['injector', 'injector.cpp']
]

foreach p : progs
  exe = executable(p[0], p[1], dependencies : boost_di_dep)
  test(p[0], exe)
endforeach
