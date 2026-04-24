# directory: libsaf

set(lib_proc_sources	saf_manage)
						


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (saf 		STATIC	${lib_sources})
#add_library (saf_sh 	SHARED	${lib_sources})
install (	TARGETS saf 
					#saf_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (saf_pic 	STATIC ${lib_sources})
set_property(TARGET saf_pic PROPERTY POSITION_INDEPENDENT_CODE TRUE)
install (	TARGETS saf_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
