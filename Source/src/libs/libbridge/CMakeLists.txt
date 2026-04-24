# directory: libbridge


set(lib_sources	br_cmd_tlv.c 
				bridge_commands.c 
				cmd_prod_conso.c )
						

add_library (bridge ${lib_sources})
install (	TARGETS bridge 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
