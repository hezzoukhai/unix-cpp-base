# directory: libhsid


set(lib_sources 	dump_hsid.c
					hsid_analyse.c
					hsid_ini.c
					hsid_util.c)


add_library (phsid ${lib_sources})
install (TARGETS phsid
		 ARCHIVE
		PERMISSIONS ${P7_DFLT_LIB_PERM}
		DESTINATION lib)