# directory: libtlv


set(lib_authotlv_src	autho_ini.c 
						autho_analyse.c 
						autho_util.c 
						dump_tlv.c)
set(lib_privtlv_src		private_ini.c 
						private_analyse.c 
						private_util.c)
set(lib_emeatlv_src		emea_ini.c 
						emea_analyse.c 
						emea_util.c)
set(lib_prodtlv_src		prod_ini.c 
						prod_analyse.c 
						prod_util.c 
						dump_tlv_prod.c)


add_library (authotlv 		STATIC	${lib_authotlv_src})
add_library (privtlv 		STATIC	${lib_privtlv_src})
add_library (emeatlv 		STATIC	${lib_emeatlv_src})
add_library (prodtlv 		STATIC	${lib_prodtlv_src})

#add_library (authotlv_sh 	SHARED	${lib_authotlv_src})
#add_library (privtlv_sh 	SHARED	${lib_privtlv_src})
#add_library (prodtlv_sh 	SHARED	${lib_prodtlv_src})

install (	TARGETS 	authotlv  
						privtlv 
						emeatlv 
						prodtlv 
						#authotlv_sh 
						#privtlv_sh 
						#prodtlv_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
		
	
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (authotlv_pic 		STATIC	${lib_authotlv_src})
add_library (privtlv_pic 		STATIC	${lib_privtlv_src})
add_library (prodtlv_pic 		STATIC	${lib_prodtlv_src})




set_property(TARGET authotlv_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET privtlv_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET prodtlv_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)

install (	TARGETS authotlv_pic privtlv_pic prodtlv_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
