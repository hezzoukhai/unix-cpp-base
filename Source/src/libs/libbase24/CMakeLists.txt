# directory: libbase24


set(lib_sources                 b24_analyse.c
                                b24_analyse_tokens.c
                                b24_csm_utils.c 
								b24_dump.c
								b24_lib_ini.c
								b24_util.c
								#b24_tcp.c				
   )


add_library (pbase24 ${lib_sources})

install (       TARGETS pbase24
                        ARCHIVE
                        PERMISSIONS ${P7_DFLT_LIB_PERM}
                        DESTINATION lib)
