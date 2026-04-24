# directory: libservices

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


set(appl_libs 	trnutils_pic;saf_pic;actions_pic;${HSM_LIB}_pic;hsmtdes_ext_pic;utils_pic;privtlv_pic;authotlv_pic;prodtlv_pic;queue_ext_pic;comora_ext_pic;piso_pic;iso3_pic;tools_pic;get_pic;pthr;monitoring_pic;actions_pic;m;pthread;frd_pic)

set(atm_libs genubp_pic;${appl_libs})

else()

set(appl_libs srvutils;trnlib;trnutils;iastrnlib;int;iasservices;bridge;saf;frd;actions;${HSM_LIB};hsmtdes_ext;utils;privtlv;authotlv;prodtlv;piso;get;monitoring;ppuf;pcps;psmspuf;psms;iso3;pthr_ext;queue_ext;comora_ext;tools;actions;m;pthread)

set(atm_libs m;pthread;genubp;utils;pthr_ext;${HSM_LIB};hsmtdes_ext;privtlv;authotlv;prodtlv;psmspuf;ppuf;pmds;queue_ext;comora_ext;piso;tools;saf;get;monitoring;actions;trnutils;clntsh;dl)
endif()


set(lib_trn_sources		auth_advice_proc.c 
						auth_request_proc.c 
						cash_out_proc.c 						
						authentication_request_proc.c
						ubp_payment_proc.c
						balance_request_proc.c
						payment_req_proc.c
						pin_change_request_proc.c
						refund_request_proc.c
						reversal_request_proc.c
						cheque_request_proc.c
						statement_req_proc.c
						statement_inq_proc.c
						on_deposit_request_proc.c
						deposit_request_proc.c
						transfer_request_proc.c
						cdm_request_proc.c
						bank_file_update_processing.c
						bank_file_update_proc.c
						rib_inq_proc.c
						auth_preauth_compl_proc.c
						card_activation_request_proc.c
						trn_utils.c
						digitization_request_proc.c
						dcc_request_proc.c
						)

#set(lib_trnatm_sources		ubp_proc.c)



set(lib_trnlib_sources 	${lib_trn_sources};
						file_update_processing.c
						)



add_library (trn 				MODULE	${lib_trn_sources})
add_library (trnlib 			STATIC	${lib_trnlib_sources})


set(TRN_VERSION_MAJOR 1)
set(TRN_VERSION_MINOR 0)
set(TRN_VERSION_PATCH 4)

set(TRN_VERSION_STRING ${TRN_VERSION_MAJOR}.${TRN_VERSION_MINOR}.${TRN_VERSION_PATCH})

#set_target_properties(trn PROPERTIES VERSION ${TRN_VERSION_STRING} SOVERSION ${TRN_VERSION_MAJOR})

target_link_libraries (trn			${appl_libs})

install (	TARGETS		trn 
						trnlib
			LIBRARY 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib			
			)

if( 	FO_ATM_ST_CORE OR
        FO_ATM_SA_CORE)
#add_library (trnatm 			MODULE	${Lib_trnatm_sources})

set(TRNATM_VERSION_MAJOR 1)
set(TRNATM_VERSION_MINOR 0)
set(TRNATM_VERSION_PATCH 1)

set(TRNATM_VERSION_STRING ${TRNATM_VERSION_MAJOR}.${TRNATM_VERSION_MINOR}.${TRNATM_VERSION_PATCH})

#set_target_properties(trnatm PROPERTIES VERSION ${TRNATM_VERSION_STRING} SOVERSION ${TRNATM_VERSION_MAJOR})

#target_link_libraries (trnatm		${atm_libs})

install (	TARGETS		
#						trnatm
			LIBRARY 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib			
			)
endif()

