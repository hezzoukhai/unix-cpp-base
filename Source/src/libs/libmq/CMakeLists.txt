# directory: libmq

set(lib_netmq_src   mq_com.c
                    mq_tools.c  )

include_directories($ENV{MQ_PATH}/inc)
add_library (mq     STATIC  ${lib_netmq_src})
install (	TARGETS mq 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
