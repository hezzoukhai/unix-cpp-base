# directory: libthr


set(lib_sources	list_thr.c 
				list_ini.c 
				alarm_thr.c 
				saf_manage.c 
				sign_manage.c 
				cut_over_manage.c 
				bal_prod_conso.c 
				line_prod_conso.c 
				shutdown_common.c 
				time_prod_conso.c
				thr_utils.c
				hsm_prod_conso.c)
						

add_library (pthr 		STATIC	${lib_sources})
add_library (pthr_ext 	STATIC	${lib_sources})
set_target_properties(pthr_ext PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")

#add_library (pthr_sh 		SHARED	${lib_sources})
#add_library (pthr_ext_sh 	SHARED	${lib_sources})
#set_target_properties(pthr_ext_sh PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")

install (	TARGETS pthr 
					pthr_ext 
					#pthr_sh
					#pthr_ext_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (pthr_ext_pic 	STATIC	${lib_sources})
set_target_properties(pthr_ext_pic PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")


set_property(TARGET pthr_ext_pic PROPERTY POSITION_INDEPENDENT_CODE TRUE)

install (	TARGETS pthr_ext_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()



