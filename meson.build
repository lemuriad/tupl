project('tupl', 'cpp',
  default_options : ['cpp_std=c++20','warning_level=3'],
  meson_version : '>=1.0.0',
  version : '0.5'
)

cpp = meson.get_compiler('cpp')

# meson_options.txt build options: tests, tupl_dev, arity, namespace_id
is_subproj = meson.is_subproject()
# tests:    auto disabled when used as a subproject
# tupl_dev: disable for no IREPEAT dependency download
tests    = get_option('tests').disable_auto_if(is_subproj).allowed()
tupl_dev = get_option('tupl_dev')
arity = get_option('tupl_max_arity')
no_lupl = get_option('no_lupl')
namespace_id = get_option('namespace_id')

c_array_support_dep = subproject(
     'c_array_support').get_variable('c_array_support_dep')

IREPEAT_dep = dependency('IREPEAT', required : tupl_dev.allowed())

subdir('tupl_dev')

tupl_dep = declare_dependency(
  dependencies : [c_array_support_dep, IREPEAT_dep, tupl_impl_dep],
  include_directories : include_directories('.','tupl_dev')
)

if tests

if get_option('snitch_cmake')
snitch_cmake = import('cmake').subproject('snitch')
snitch_dep = snitch_cmake.dependency('snitch')
else
snitch_dep = subproject('snitch').get_variable('snitch_dep')
endif

subdir('tests')

endif
