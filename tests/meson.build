test('sanity'
    ,executable('tupl_sanity_test'
               ,'tupl_sanity_test.cpp'
               ,override_options: ['b_ndebug=false']
               ,dependencies: [tupl_dep]
               )
)

if codegen.enabled()
test('amalgam'
    ,executable('tupl_amalgam_test'
               ,'tupl_amalgam_test.cpp'
               ,override_options: ['b_ndebug=false']
               ,dependencies: [tupl_dep, tupl_amalgam_dep]
               )
)
endif

if codegen.allowed()
test('tupl_impl'
    ,executable('tupl_impl'
               ,'tupl_test.cpp'
               ,cpp_args: ['-DTUPL_IMPL_PREPROCESS']
               ,override_options: ['b_ndebug=false']
               ,dependencies: [tupl_dep]
               ,include_directories: tupl_impl_dir
               )
)
endif

test('tupl'
    ,executable('tests'
               ,'test.cpp'
               ,cpp_args: ['-DNAMESPACE_ID=']
               ,override_options: ['b_ndebug=false']
               ,dependencies: [tupl_dep, snitch_dep]
               )
)
