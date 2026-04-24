set(lib_sources			                	lg_fields.c 
							lg_msg.c 
							lg_msg_info.c 
							lg_crypto_fields.c
							lg_crypto_utils.c
							)
					


add_library (lggab 		STATIC	${lib_sources})


install (	TARGETS lggab 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
