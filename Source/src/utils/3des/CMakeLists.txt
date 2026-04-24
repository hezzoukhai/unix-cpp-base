# directory: 3des


set(app_3des_enc_src	3des_encrypt.c
						des_tools.c)
set(app_3des_dec_src	3des_decrypt.c
						des_tools.c)



add_executable (3DesEncrypt 			${app_3des_enc_src})
add_executable (3DesDecrypt 			${app_3des_dec_src})

TARGET_LINK_LIBRARIES(3DesEncrypt 		pthread;comora;tools;comora;piso)
TARGET_LINK_LIBRARIES(3DesDecrypt 		pthread;comora;tools;comora;piso)

install (	TARGETS 3DesEncrypt 
					3DesDecrypt 
			RUNTIME
				PERMISSIONS ${P7_DFLT_BIN_PERM}
				DESTINATION bin
		)
