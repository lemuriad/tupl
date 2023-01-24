test('sanity'
    ,executable('tupl_sanity_test'
               ,'tupl_sanity_test.cpp'
               ,cpp_args : ['-DNAMESPACE_ID=lml']
               ,dependencies: [tupl_dep]
               )
)

if tupl_dev.enabled()
test('amalgam'
    ,executable('tupl_amalgam_test'
               ,'tupl_amalgam_test.cpp'
               ,cpp_args : ['-DNAMESPACE_ID=lml']
               ,dependencies: [tupl_dep, tupl_amalgam_dep]
               )
)
endif

if tupl_dev.allowed()
test('tupl_dev'
    ,executable('tupl_dev'
               ,'tupl_test.cpp'
               ,cpp_args : ['-DTUPL_IMPL_PREPROCESS'
                           ,'-DNAMESPACE_ID=lml']
               ,dependencies: [tupl_dep]
               )
)
endif

test('tupl'
    ,executable('tests'
               ,'test.cpp'
               ,cpp_args : ['-DNAMESPACE_ID=']
               ,dependencies: [tupl_dep, snitch_dep]
               )
)
