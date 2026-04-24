# directory: libmonitoring

#libmonitoring
set(lib_sources			msg_flow_mapping.c
						ntw_raw_msg_util.c
						msgflow_ini.c)
set(lib_proc_src		monitoring_db_utils)

preproc_pc("${lib_proc_src}" lib_sources	lib_proc_lis_src)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${lib_proc_lis_src}" )


add_library (monitoring 		STATIC	${lib_sources})
#add_library (monitoring_sh 		SHARED	${lib_sources})

install (	TARGETS monitoring 
					#monitoring_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (monitoring_pic 		STATIC	${lib_sources})


set_property(TARGET monitoring_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS 
				monitoring_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
