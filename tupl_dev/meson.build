if arity < 0x10
  ARITY = '(@0@)'.format(arity)
elif arity < 0x100
  ARITY = '(@0@)(@1@)'.format(arity/0x10,arity%0x10)
elif arity < 0x1000
  ARITY = '(@0@)(@1@)(@2@)'.format(arity/0x100,arity//0x10,arity%0x10)
else
  error('Exceeded the maximum number of tupl elements, 0xfff')
endif

compiler_id = cpp.get_id()

if compiler_id == 'msvc' or compiler_id == 'clang-cl'
  prepro_args = ['/I..\subprojects\IREPEAT','/std:c++latest','/Zc:preprocessor','/C','/TP','/P']
elif compiler_id == 'clang'
  prepro_args = ['-I../subprojects/IREPEAT','-MMD','-nostdinc','-C','-Wno-c++20-compat','-Wno-pedantic']
else
  prepro_args = ['-I../subprojects/IREPEAT','-MMD','-nostdinc','-C','-Wno-pedantic']
endif

# The preprocessing codegen step, ideally it would accept IREPEAT_dep;
# the -I include path is currently added in compile_args, 'unsandboxed'
tupl_impl = cpp.preprocess('tupl_impl.pp',
            output : '../tupl_impl.hpp',
            compile_args : prepro_args + ['-DTUPL_MAX_ARITY='+ARITY],
            include_directories : include_directories('.','..'),
           )

tupl_impl_dep = declare_dependency(sources : tupl_impl)
