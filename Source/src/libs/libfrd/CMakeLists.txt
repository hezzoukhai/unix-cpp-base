# directory: libfrd


set(lib_sources		
			frd_ini.c  
			frd_list.c  
			frd_util.c)			

set(lib_proc_sources	frd_db_utils frd_check_rules)


preproc_pc("${lib_proc_sources}" lib_sources    proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )


add_library (frd 	 STATIC	 ${lib_sources})
install (       TARGETS frd
                        ARCHIVE
                                PERMISSIONS ${P7_DFLT_LIB_PERM}
                                DESTINATION lib
                        LIBRARY
                                PERMISSIONS ${P7_DFLT_LIB_PERM}
                                DESTINATION lib
                        )
				
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

        add_library (frd_pic	STATIC ${lib_sources})
        set_property(TARGET frd_pic PROPERTY POSITION_INDEPENDENT_CODE TRUE)	
        install (       TARGETS frd_pic
                                ARCHIVE
                                        PERMISSIONS ${P7_DFLT_LIB_PERM}
                                        DESTINATION lib
                                LIBRARY
                                        PERMISSIONS ${P7_DFLT_LIB_PERM}
                                        DESTINATION lib
                        )
endif()
