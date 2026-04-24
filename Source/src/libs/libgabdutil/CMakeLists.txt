# directory: libgabdutil


set(lib_sources		gabd_format_field.c
					insert_text.c)

set(lib_proc_sources	
						gabd_get_print;
						gabd_seq_mng;
						gabd_arret;
						gabd_atm_mng;
						gabd_key_mng;
						gabd_get_flag;
						gabd_status_mng;
						gabd_trans_resp_code;
						gabd_terminal_config)
						

preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (gabdutil ${lib_sources})
install (	TARGETS gabdutil 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
