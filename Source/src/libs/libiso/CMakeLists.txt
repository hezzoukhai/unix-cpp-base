# directory: libiso


set(lib_piso_src			iso_ini.c 
							iso_analyse.c 
							iso_util.c 
							tlv_switch_header.c)
set(lib_piso_proc_src		#private_load_tags  #EBE: not yet ready
							)


set(lib_peuro_src		euro_ini.c 
						euro_analyse.c 
						euro_util.c)
set(lib_peccf_src		eccf_ini.c 
						eccf_analyse.c 
						eccf_util.c)
set(lib_pvisa_src		visa_ini.c 
						visa_analyse.c 
						visa_util.c)
set(lib_pmds_src		mds_ini.c 
						mds_analyse.c 
						mds_util.c)
set(lib_psms_src		sms_ini.c 
						sms_analyse.c 
						sms_util.c )
set(lib_pcps_src		cps_ini.c 
						cps_analyse.c 
						cps_util.c)
set(lib_ppuf_src		puf_ini.c 
						puf_analyse.c 
						puf_util.c)
set(lib_pvisapuf_src	visa_puf_ini.c 
						visa_puf_analyse.c 
						visa_puf_util.c )
set(lib_psmspuf_src		sms_puf_ini.c 
						sms_puf_analyse.c 
						sms_puf_util.c)
set(lib_ppos_src		pos_ini.c 
						pos_analyse.c 
						pos_util.c 
						dump_pos.c)
set(lib_pjcc_src		jcc_ini.c 
						jcc_analyse.c 
						jcc_util.c)

set(lib_cup_src                                 cup_analyse.c
						cup_ini.c
						#cup_mac_mng.c
						cup_puf_util.c
						cup_util.c)

set(lib_pcup_src                                cup_puf_analyse.c
						cup_puf_ini.c
						cup_puf_util.c)


set(lib_ppulse_src                		pulse_ini.c
						pulse_analyse.c
                                                pulse_data_analyse.c
                                                pulse_util.c)
                                                
set(lib_iso3nw_sources	iso3_sid.c
						iso3_visa.c
						iso3_cis.c
						iso3_mds.c
						iso3_host.c
						iso3_cup.c
						iso3_jcb.c
						)





preproc_pc("${lib_piso_proc_src}" lib_piso_src proc_lis_files)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )


add_library (piso 		STATIC	${lib_piso_src})
add_library (peuro 		STATIC	${lib_peuro_src})
add_library (peccf 		STATIC	${lib_peccf_src})
add_library (pvisa 		STATIC	${lib_pvisa_src})
add_library (pmds 		STATIC	${lib_pmds_src})
add_library (psms 		STATIC	${lib_psms_src})
add_library (pcps 		STATIC	${lib_pcps_src})
add_library (ppuf 		STATIC	${lib_ppuf_src})
add_library (pvisapuf 	STATIC	${lib_pvisapuf_src})
add_library (psmspuf 	STATIC	${lib_psmspuf_src})
add_library (ppos 		STATIC	${lib_ppos_src})
#add_library (ppos223 	STATIC	${lib_ppos_src} )
add_library (pjcc 		STATIC	${lib_pjcc_src})
#add_library (psid 		STATIC	${lib_psid_src})
#add_library (piso_sh 	SHARED	${lib_piso_src})
add_library (pcup                 STATIC  ${lib_cup_src})
add_library (pcuppuf              STATIC  ${lib_pcup_src})
add_library (ppulse              STATIC  ${lib_ppulse_src})
add_library (piso3nw 		STATIC	${lib_iso3nw_sources})

#set_target_properties(ppos223 PROPERTIES COMPILE_DEFINITIONS "PPWM_223")

install (	TARGETS piso peuro peccf pvisa pmds psms pcps ppuf pvisapuf psmspuf ppos pjcc  pcup pcuppuf ppulse piso3nw   
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (piso_pic 		STATIC	${lib_piso_src})


set_property(TARGET piso_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS 
				piso_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()