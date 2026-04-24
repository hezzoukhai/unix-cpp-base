# directory: libiso3



set(lib_sources	berinfo.c 
				bitmapinfo.c 
				conv_module.c 
				fieldinfo.c
				#msg_layout.c
				msginfo.c
				propmng.c
				staticinfo.c
				#structinfo.c
				#tlvbuffer.c
				tlvinfo.c
				)
						

add_library (iso3 ${lib_sources})
install (	TARGETS iso3
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
			
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (iso3_pic 		STATIC		${lib_sources})


set_property(TARGET iso3_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS 
				iso3_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
