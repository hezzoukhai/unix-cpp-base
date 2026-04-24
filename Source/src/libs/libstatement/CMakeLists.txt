# directory: libstatement


set(lib_sources			statement_req_processing.c)

set(lib_proc_sources	statement_req_inquiry)
						


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (statement ${lib_sources})
install (	TARGETS statement 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
