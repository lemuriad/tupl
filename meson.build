project('tupl', 'cpp',
  default_options : ['cpp_std=c++20','warning_level=3'],
  version : '0.3'
)

c_array_support_dep = subproject(
     'c_array_support').get_variable('c_array_support_dep')
IREPEAT_dep = subproject(
             'IREPEAT').get_variable('IREPEAT_dep')

tupl_dep = declare_dependency(
  dependencies: [c_array_support_dep,IREPEAT_dep],
  include_directories : include_directories('.'),
)

test('sanity',
  executable('tupl_sanity_test', 'tupl_dev/tests/tupl_sanity_test.cpp',
             cpp_args : ['-DTUPL_IMPL_PREPROCESS','-DNAMESPACE_ID='],
             dependencies: [tupl_dep],
            )
)
test('tupl',
  executable('tupl_test', 'tupl_dev/tests/test.cpp',
             cpp_args : ['-DTUPL_IMPL_PREPROCESS','-DNAMESPACE_ID='],
             dependencies: [tupl_dep],
            )
)

subdir('tupl_dev')
