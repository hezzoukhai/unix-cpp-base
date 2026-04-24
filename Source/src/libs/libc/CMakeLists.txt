# directory: libc

#libqueue
set(lib_queue_src		queue.c)

# libcomora
set(lib_comora_src		signal_mng.c 
						dump.c
						#dump3.c 
						date.c 
						des_tools.c 
						get_cryptogram.c
						dbcon_utils.c 
						dbcon_thread_refresh.c
						cipher.c)
set(lib_comora_proc_src	connect)


preproc_pc("${lib_comora_proc_src}" lib_comora_src	lib_comora_proc_lis_src)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${lib_comora_proc_lis_src}" )

#ClearQeue
#set(app_clearqueue_src	ClearQueue.c)

#GenPassWord
set(app_genpwd_src	generate_password.c)



add_library (comora 		STATIC	${lib_comora_src})
add_library (comora_ext 	STATIC	${lib_comora_src})
#add_library (comora_sh 		SHARED	${lib_comora_src})
#add_library (comora_ext_sh 	SHARED	${lib_comora_src})
set_target_properties(comora_ext 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")
#set_target_properties(comora_ext_sh PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")

add_library (queue 				STATIC	${lib_queue_src})
add_library (queue_ext 			STATIC	${lib_queue_src})
#add_library (queue_sh			SHARED	${lib_queue_src})
#add_library (queue_ext_sh		SHARED	${lib_queue_src})
set_target_properties(queue_ext 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")
#set_target_properties(queue_ext_sh 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")


#add_executable (ClearQueue 			${app_clearqueue_src})
add_executable (GenPassWord 		${app_genpwd_src})

TARGET_LINK_LIBRARIES(GenPassWord 		pthread;comora;tools;comora;piso;iso3;m)
#TARGET_LINK_LIBRARIES(ClearQueue                pthread;comora;tools;comora)

install (	TARGETS comora 
					queue
					queue_ext
					#queue_sh
					#queue_ext_sh
					comora_ext 
					#comora_sh
					#comora_ext_sh
					#ClearQueue
					GenPassWord
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

#add_library (comora_pic 			STATIC	${lib_comora_src})
add_library (comora_ext_pic 		STATIC	${lib_comora_src})
#add_library (queue_pic 				STATIC	${lib_queue_src})
add_library (queue_ext_pic 			STATIC	${lib_queue_src})

#set_property(TARGET comora_pic 			PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET comora_ext_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)
#set_property(TARGET queue_pic 			PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET queue_ext_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)

set_target_properties(comora_ext_pic 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")
set_target_properties(queue_ext_pic 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")


install (	TARGETS 
				#comora_pic  
				comora_ext_pic 
				#queue_pic 
				queue_ext_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
