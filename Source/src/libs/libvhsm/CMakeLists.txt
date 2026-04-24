# directory: libvhsm


set(lib_sources		vhsm_commands.c 
					vhsm_mac_mng.c 
					vhsm_cry_key.c 
					vhsm_dec_key.c 
					vhsm_gen_cvk.c 
					vhsm_gen_cvv.c 
					vhsm_gen_pin.c 
					vhsm_gen_pvk.c 
					vhsm_gen_pvv.c 
					vhsm_gen_tmk.c 
					vhsm_gen_zmk.c 
					vhsm_get_lmk.c 
					vhsm_int_cvk.c 
					vhsm_int_pin.c 
					vhsm_int_pvk.c 
					vhsm_ver_pin.c 
					vhsm_ver_cvv.c 
					vhsm_ver_pvv.c 
					vhsm_gen_mac.c 
					vhsm_ver_mac.c 
					vhsm_compute_arqc.c 
					vhsm_util.c 
					vhsm.c 
					des_tools.c)
						

add_library (vhsm 		STATIC	${lib_sources})
add_library (vhsm_sh 	SHARED	${lib_sources})

install (	TARGETS vhsm 
					vhsm_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
