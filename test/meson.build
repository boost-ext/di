if meson.get_compiler('cpp').get_id() == 'msvc'
  cpp_args = ['-DBOOST_DI_NAMESPACE=', '-D_CRT_SECURE_NO_WARNINGS']
  p = 'ft/di_injector_except'
  exe = executable(p.underscorify(), p + '.cpp', 
                   dependencies : boost_di_dep,
                   include_directories : 'common',
                   cpp_args : cpp_args)
  test(p.underscorify(), exe)
else
  cpp_args = ['-DBOOST_DI_NAMESPACE=']
  p = 'ft/di_injector_except'
  exe = executable(p.underscorify(), p + '.cpp', 
                   dependencies : boost_di_dep,
                   include_directories : 'common',
                   cpp_args : [cpp_args, '-fexceptions'])
  test(p.underscorify(), exe)
endif

progs = [
  'ut/inject',
  'ut/aux_/preprocessor',
  'ut/aux_/type_traits',
  'ut/aux_/utility',
  'ut/concepts/boundable',
  'ut/concepts/callable',
  'ut/concepts/configurable',
  'ut/concepts/creatable',
  'ut/concepts/providable',
  'ut/concepts/scopable',
  'ut/core/any_type',
  'ut/core/array',
  'ut/core/binder',
  'ut/core/bindings',
  'ut/core/dependency',
  'ut/core/injector',
  'ut/core/policy',
  'ut/core/pool',
  'ut/core/provider',
  'ut/core/wrapper',
  'ut/policies/constructible',
  'ut/providers/stack_over_heap',
  'ut/scopes/deduce',
  'ut/scopes/instance',
  'ut/scopes/singleton',
  'ut/scopes/unique',
  'ut/type_traits/ctor_traits',
  'ut/type_traits/memory_traits',
  'ut/type_traits/named_traits',
  'ut/type_traits/rebind_traits',
  'ut/type_traits/scope_traits',
  'ut/wrappers/unique',
  'ut/wrappers/shared',
  'ut/bindings',
  'ut/config',
  'ut/injector',
  'ut/make_injector',
  'ft/di_bind',
  'ft/di_config',
  'ft/di_config_global_policies',
  'ft/di_config_global_provider',
  'ft/di_errors',
  'ft/di_inject',
  'ft/di_injector',
  'ft/di_no_memory_inc',
  'pt/di_compile_time',
]

foreach p : progs
  exe = executable(p.underscorify(), p + '.cpp', 
                   dependencies : boost_di_dep,
                   include_directories : 'common',
                   cpp_args : cpp_args)
  test(p.underscorify(), exe)
endforeach

progs = ['ft/di_glibcxx_debug', 'ft/di_no_std_inc']
foreach p : progs
  exe = executable(p.underscorify(), p + '.cpp', 
                   dependencies : boost_di_dep,
                   include_directories : 'common',
                   cpp_args : [cpp_args, '-DBOOST_DI_TEST_HPP'])
  test(p.underscorify(), exe)
endforeach

p = 'pt/di_run_time'
exe = executable(p.underscorify(), p + '.cpp', 
                 dependencies : boost_di_dep,
                 include_directories : 'common',
                 cpp_args : [cpp_args, '-DBOOST_DI_TEST_HPP', '-O3'])
test(p.underscorify(), exe)
