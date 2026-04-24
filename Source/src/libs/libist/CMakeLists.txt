# directory: libist
 
set(lib_sources	ist_analyse.c
                ist_dump.c
                ist_ini.c
                ist_util.c)

add_library (ist ${lib_sources})

install ( TARGETS ist
          ARCHIVE 			
          PERMISSIONS ${P7_DFLT_LIB_PERM}			
          DESTINATION lib)			
