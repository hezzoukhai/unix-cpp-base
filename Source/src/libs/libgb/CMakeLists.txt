# directory: libgb


set(lib_sources 			
					gb_analyse.c
					gb_ini.c
					gb_util.c
					)




add_library (pgb ${lib_sources})


install (TARGETS 	pgb
		 	ARCHIVE
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
