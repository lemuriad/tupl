if arity < 0x10
  ARITY = '(@0@)'.format(arity)
elif arity < 0x100
  ARITY = '(@0@)(@1@)'.format(arity/0x10,arity%0x10)
elif arity < 0x1000
  ARITY = '(@0@)(@1@)(@2@)'.format(arity/0x100,arity//0x10,arity%0x10)
else
  error('Exceeded the maximum number of tupl elements, 0xfff')
endif

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

# requires tupl_dev dependency IREPEAT
tupl_impl_header = custom_target('tupl_impl.hpp',
  input: 'tupl_impl_pre.hpp',
  output: 'tupl_impl.hpp',
  build_by_default: false,
  command: [meson.get_compiler('cpp').cmd_array(),
            ['-DTUPL_MAX_ARITY='+ARITY],
            prepro_args, output_flag+'@OUTPUT@', '@INPUT@'],
  depfile: 'tupl_impl.d',
  install: true,
  install_dir: meson.current_source_dir()
)

if tests

test('sanity'
    ,executable('tupl_sanity_test'
               ,'tests/tupl_sanity_test.cpp'
               ,cpp_args : ['-DNAMESPACE_ID=lml']
               ,dependencies: [tupl_dep]
               )
)

if tupl_dev
test('tupl_dev'
    ,executable('tupl_dev'
               ,'tests/tupl_test.cpp'
               ,cpp_args : ['-DTUPL_IMPL_PREPROCESS'
                           ,'-DNAMESPACE_ID=lml']
               ,dependencies: [tupl_dep]
               )
)
endif

test('tupl'
    ,executable('tests'
               ,'tests/test.cpp'
               ,cpp_args : ['-DNAMESPACE_ID=']
               ,dependencies: [tupl_dep, snitch_dep]
               )
)

endif
