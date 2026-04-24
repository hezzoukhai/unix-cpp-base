# directory: libctrlunits


set(lib_sources		ctrl_unit_auth.c 
					ctrl_unit_rep.c
					ctrl_unit_risk.c
					ctrl_unit_serv.c
					)
						

add_library (	ctrlunits ${lib_sources})

set_property(TARGET ctrlunits 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)

install (	TARGETS ctrlunits 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)





