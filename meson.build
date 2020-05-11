project('boost_di', 'cpp', 
  version: '1.1.0',
  license: 'boost',
  default_options : ['warning_level=3', 'cpp_std=c++14']
)

boost_di_dep = declare_dependency(
  include_directories: include_directories('include', 'extension/include')
)

if not meson.is_subproject()
  install_headers('include/boost/di.hpp', subdir: 'di')
  
  pkgc = import('pkgconfig')
  pkgc.generate(name: 'boost_di',
    version: meson.project_version(),
    description: 'C++14 Dependency Injection Library'
  )
endif


if get_option('BOOST_DI_OPT_BUILD_TESTS')
  subdir('extension/test')
  subdir('test')
endif

if get_option('BOOST_DI_OPT_BUILD_EXAMPLES')
  subdir('example')
endif
