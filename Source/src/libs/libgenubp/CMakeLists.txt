# directory: libgubp

set(lib_sources			ndc_gubp_inquiry_reply.c 
				ndc_gubp_inter_reply.c				
				ndc_gubp_payment_reply.c)
				#ndc_gubp_payment_processing.c 
				#ndc_gubp_services.c
				#ndc_gubp_inquiry_processing.c
				#ndc_gubp_payment_convert.c
				#ndc_gubp_inquiry_convert.c)
        
        

set(lib_proc_sources	ndc_gubp_webservice)
						


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (genubp    STATIC ${lib_sources})
#add_library (genubp_sh SHARED ${lib_sources})

install (	TARGETS genubp
			#genubp_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
                        LIBRARY
                                PERMISSIONS ${P7_DFLT_LIB_PERM}
                                DESTINATION lib
	)


if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (genubp_pic                 STATIC  ${lib_sources})


set_property(TARGET genubp_pic  PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (       TARGETS genubp_pic
                        ARCHIVE
                                PERMISSIONS ${P7_DFLT_LIB_PERM}
                                DESTINATION lib
                        LIBRARY
                                PERMISSIONS ${P7_DFLT_LIB_PERM}
                                DESTINATION lib
                )
endif()


