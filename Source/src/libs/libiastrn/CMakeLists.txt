# directory: libiastrn

set(lib_iastrn_src		ias_auth_request_proc.c 
						ias_balance_request_proc.c 
						ias_cheque_request_proc.c 
						ias_deposit_request_proc.c 
						ias_payment_req_proc.c 
						ias_statement_req_proc.c 
						ias_transfer_req_proc.c)


set(lib_iastrnlib_src 	${lib_iastrn_src})


add_library (iastrnlib 			STATIC	${lib_iastrnlib_src})
add_library (iastrn 			MODULE	${lib_iastrn_src})


install (	TARGETS iastrn iastrnlib iastrn
			LIBRARY 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib			
			)

