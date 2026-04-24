

set(lib_sources	int1_adm.c 
				#int1_aut_adv_from_nw.c 
				int1_aut_adv_to_nw.c 
				int1_rev_adv_to_nw.c
				int1_aut_rep_from_nw.c 
				int1_aut_rep_to_nw.c 
				int1_aut_req_from_nw.c 
				int1_adv_req_from_nw.c
				int1_aut_req_to_nw.c 
				int1_bal.c 
				int1_ctl.c 
				int1_fct_shut_down.c 
				int1_ini.c 
				int1_line.c 
				int1_rev_adv_from_nw.c 
				#int1_reversal.c 
				int1_saf.c 
				int1_saf_processing.c 
				int1_services.c 
				int1_sig.c 
				int1_timeout.c 
				int1_trait_adm.c 
				int1_trait_bal.c 
				int1_trait_time.c 
				int1_utils.c
                                )




add_library (int1 	STATIC	${lib_sources})




install (	TARGETS int1 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)

