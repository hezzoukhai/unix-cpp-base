# directory: libbic


set(lib_sources		bic_ini.c
					bic_analyse.c
					bic_util.c
					bic_ic_util.c
					bic_ic_ini.c
					dump_bic.c 
					bic_emv_tools.c)

#set(lib_proc_sources			bic_download_data) #not used?


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (pbic2 ${lib_sources})


install (	TARGETS pbic2
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
