# directory: libprocessing

if(FO_ISS_SA_CORE)
	set(lib_sources			
							reversal_processing.c
							bic_issuer_delegation.c)

	set(lib_proc_sources	)
else()
	set(lib_sources	auth_processing_utils.c 
					authentication_processing.c 
					autho_conf_proc.c 
					autho_loan_utils.c 
					autho_processing.c
					balance_processing.c
					bill_payment_processing.c
					cardless_processing.c
					cheque_processing.c
					conversion_rate_processing.c
					deposit_processing.c
					exception_processing.c
					file_update_processing.c
					loan_processing.c
					pin_change_processing.c
					refund_processing.c
					reversal_processing.c
					statement_processing.c
					transfer_processing.c
					bic_issuer_delegation.c)

	set(lib_proc_sources	
								#get_trackII_record; #not used?
								)

endif()


if(	FO_ATM_ST_CORE	OR
	FO_ATM_SA_CORE )
	set(lib_proc_sources 	${lib_proc_sources};
							loan_inquiry)
endif()

if(	FO_POS_ST_CORE 	OR
	FO_POS_SA_CORE	)
	
endif()

if(	FO_POS_ST_CORE 	OR
	(FO_POS_SA_CORE AND FO_ISS_SA_CORE)	)
	set(lib_proc_sources 	${lib_proc_sources};
							top_up_processing;
							update_loan_account)
endif()


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (processing 	STATIC	${lib_sources})
add_library (processing_sh 	SHARED	${lib_sources})
install (	TARGETS processing processing_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
