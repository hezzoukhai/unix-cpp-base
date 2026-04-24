# directory: libtlvemv


set(lib_sources	private_emv_ini.c 
				private_emv_analyse.c 
				private_emv_util.c)
						

add_library (privtlvemv 	STATIC	${lib_sources})
#add_library (privtlvemv_sh 	SHARED	${lib_sources})

install (	TARGETS 	privtlvemv 
						#privtlvemv_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
		
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (privtlvemv_pic 		STATIC	${lib_sources})


set_property(TARGET privtlvemv_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS privtlvemv_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
