# directory: libsid


set(lib_sources 	dump_sid.c
					sid_analyse.c
					sid_ini.c
					sid_util.c
					)




add_library (psid ${lib_sources})


install (TARGETS psid
		 ARCHIVE
		PERMISSIONS ${P7_DFLT_LIB_PERM}
		DESTINATION lib)
