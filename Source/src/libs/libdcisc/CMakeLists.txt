# directory: libdcisc




set(lib_dcisc_utils_sources	dcisc_ini.c 
							dcisc_analyse.c
							#dcisc_mac_mng.c
							dcisc_util.c
							dump_dcisc.c)




add_library (pdcisc 		STATIC	${lib_dcisc_utils_sources})
add_library (pdcisc_sh          SHARED  ${lib_dcisc_utils_sources})

install (	TARGETS  	pdcisc
				pdcisc_sh
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib			
	)
