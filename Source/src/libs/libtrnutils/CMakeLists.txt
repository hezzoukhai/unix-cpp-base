# directory: libtrnutils




set(lib_trn_utils_sources	auth_routing.c 
							gen_reversal.c
							services_utils.c
							)

set(lib_srv_utils_sources	exception_proc.c
							gen_reversal.c
							standin_request_proc.c)


add_library (trnutils 		STATIC	${lib_trn_utils_sources})
add_library (srvutils 		STATIC	${lib_srv_utils_sources})
#add_library (trnutils_sh 	SHARED	${lib_trn_utils_sources})

install (	TARGETS  	trnutils
						srvutils
						#trnutils_sh
			LIBRARY 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib			
			)
			
			
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (trnutils_pic 		STATIC	${lib_trn_utils_sources})


set_property(TARGET trnutils_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS trnutils_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
