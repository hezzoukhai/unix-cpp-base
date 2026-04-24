set(lib_netcom3_common		p7_com.c 
							p7_com_prop.c 
							p7_com_ssl.c 
							)
					
if(PLATFORM MATCHES "WIN32")

set(lib_netcom3_common			${lib_netcom3_common};
								p7_com_tcp_win32.c)

set(lib_netcom3_src				${lib_netcom3_common};
								p7_com_tcp_win32_def.c
								p7_com_ssl_def.c)


else()

set(lib_netcom3_common			${lib_netcom3_common};
								p7_com_tcp.c)
								

set(lib_netcom3_src				${lib_netcom3_common};
								p7_com_tcp_def.c
								p7_com_ssl_def.c)
endif()

add_library (netcom3 	STATIC	${lib_netcom3_src})


install (	TARGETS netcom3 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)