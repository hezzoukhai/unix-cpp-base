# directory: libtools

#libtools

set(spec_libs_zlib      z)

set(lib_tools_src	tools.c 
					x_or.c 
					des.c 
					lib_crc.c
					b64.c 
                    # b94.c
					gen_maa.c
				 	compress_tools.c
					AmexFileConverterTools.c
					)

#Crypt
set(app_crypt_src	crypt.c)
set(app_convert_amex_src	Convert_AMEX.c)



add_library (tools 		STATIC	${lib_tools_src})
#add_library (tools_sh 	SHARED	${lib_tools_src})
add_executable (CRYPT 			${app_crypt_src})
TARGET_LINK_LIBRARIES(CRYPT 	tools;${spec_libs_zlib})

add_executable (Convert_AMEX 			${app_convert_amex_src})
TARGET_LINK_LIBRARIES(Convert_AMEX 	tools;comora;piso;iso3;pthread;m)
install (	TARGETS tools
					#tools_sh
					CRYPT
					Convert_AMEX
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			RUNTIME
			PERMISSIONS ${P7_DFLT_BIN_PERM}
			DESTINATION bin
		)
		
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (tools_pic 		STATIC	${lib_tools_src})


set_property(TARGET tools_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS tools_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
