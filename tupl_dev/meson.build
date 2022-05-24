compiler_id = meson.get_compiler('cpp').get_id()

if compiler_id == 'msvc' or compiler_id == 'clang-cl'
  prepro_args = ['/std:c++latest','/Zc:preprocessor','/I.','-I..','/I..\subprojects\IREPEAT','/C','/EP','/TP','/P']
  output_flag = '/Fi'
elif compiler_id == 'clang'
  prepro_args = ['-I.','-I..','-I../subprojects/IREPEAT','-MMD','-nostdinc','-C','-E','-P','-Wno-c++20-compat']
  output_flag = '-o'
else
  prepro_args = ['-I.','-I..','-I../subprojects/IREPEAT','-MMD','-nostdinc','-C','-E','-P']
  output_flag = '-o'
endif

tupl_impl_header = custom_target('tupl_impl.hpp',
  input: 'tupl_impl_pre.hpp',
  output: 'tupl_impl.hpp',
  command: [meson.get_compiler('cpp').cmd_array(),
            get_option('cpp_args'),
            prepro_args, output_flag+'@OUTPUT@', '@INPUT@'],
  depfile: 'tupl_impl.d',
  install: true,
  install_dir: meson.current_source_dir()
)

tupl_dev_dep = declare_dependency(
  dependencies: [IREPEAT_dep],
  include_directories : include_directories('.'),
  sources : [tupl_impl_header]
)

test('test tupl',
  executable('test_tupl', 'tests/tupl_test.cpp',
             dependencies: [c_array_support_dep, tupl_dep]
            )
)
