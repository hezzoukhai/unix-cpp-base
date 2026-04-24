# directory: libiasservices


set(lib_sources	get_sa_unpaid_amnt.c 
				)
						

add_library (iasservices ${lib_sources})
install (	TARGETS iasservices 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
