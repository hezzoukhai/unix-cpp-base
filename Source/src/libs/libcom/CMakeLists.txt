# directory: libcom



set(lib_nettls1_src	tcp_ssl.c  )

set(lib_netcom_src	tcp_com.c 
					tcp_visa.c )
					
if(HSM_BOX MATCHES "ATALLA")
	set(lib_nethsm_src	tcp_com.c 
						tcp_hsm_atalla.c )
else()
	set(lib_nethsm_src	tcp_com.c 
						tcp_hsm.c )
endif()
											
set(lib_netemea_src	tcp_com.c 
					tcp_emea.c )					
set(lib_nethost_src	tcp_com.c 
					tcp_host.c )

set(lib_netT24_src     				tcp_T24.c
						tcp_com_ssl.c )
if(  NOT   P7_COMM_NO_TLS)
        set(lib_netT24_src              ${lib_netT24_src};
					ssl_tcp_com.c 
					ssl_tcp_T24.c  
						)
endif()


set(lib_netndc_src	tcp_com_wrap.c
					tcp_ndc.c )
					
if(	NOT P7_COMM_NO_TLS)
	set(lib_netndc_src 		${lib_netndc_src};
							tcp_com_tls.c )
endif()

set(lib_netpiso_src	tcp_com_tls.c 
					tcp_com_wrap.c
					tcp_posiso.c )

set(lib_netfraud_src	tcp_com.c
						tcp_fraud.c )
set(lib_netgabd_src	tcp_com.c 
					tcp_gabd.c )
set(lib_netmds_src	tcp_com.c 
					tcp_mds.c )
set(lib_netcis_src	tcp_com.c 
					tcp_cis.c )

set(lib_netsid_src	tcp_com_tls.c 
					tcp_com_wrap.c
					tcp_sid.c )

set(lib_netamex_src	tcp_com.c 
					tcp_amex.c )
set(lib_netbnt_src	tcp_com.c 
					tcp_bnt.c )
set(lib_netcmd_src	tcp_com.c 
					tcp_cmd.c )
set(lib_netbic_src	tcp_com.c 
					tcp_bic.c )

set(lib_dcisc_src	tcp_com_tls.c 
					tcp_com_wrap.c
					tcp_dcisc.c )

		
set(lib_jcb_src       tcp_com.c
                                        tcp_jcb.c )

set(lib_cup_src       tcp_com.c
                                        tcp_cup.c )
		
set(lib_netuaes_src	tcp_uaeswitch.c )					

set(lib_netist_src      tcp_ist.c )

set(lib_netmq_src      mq.c )
					
set(lib_netmxg_src		tcp_amex_gns.c ) 
	
set(lib_netsid3_src	tcp_sid3.c)
	


set(lib_dscover_src       tcp_com.c
                                        tcp_discover.c )
                                        

set(lib_netlink_src		tcp_link.c )

# MRY HFC 050723
set(lib_netpstln_src	tcp_com_tls.c 
					tcp_com_wrap.c 
					tcp_postilion.c )
#END MRY HFC 060723

set(lib_gb_src     	tcp_com.c
                	tcp_gb.c )

#NAB08102020 PLUTONL-2288

set(lib_netB24_src     				b24_tcp.c )
										
add_library (nettls1 	STATIC	${lib_nettls1_src})


add_library (netcom 	STATIC	${lib_netcom_src})
add_library (nethsm 	STATIC	${lib_nethsm_src})
add_library (netemea 	STATIC	${lib_netemea_src})
add_library (nethost 	STATIC	${lib_nethost_src})
add_library (netT24 	STATIC	${lib_netT24_src})
add_library (netpiso 	STATIC	${lib_netpiso_src})
add_library (netndc 	STATIC	${lib_netndc_src})
add_library (netfra 	STATIC	${lib_netfraud_src})
add_library (netgabd 	STATIC	${lib_netgabd_src})
add_library (netmds 	STATIC	${lib_netmds_src})
add_library (netcis 	STATIC	${lib_netcis_src})
add_library (netsid 	STATIC	${lib_netsid_src})
add_library (netamex 	STATIC	${lib_netamex_src})
add_library (netbnt 	STATIC	${lib_netbnt_src})
add_library (netcmd 	STATIC	${lib_netcmd_src})
add_library (netbic 	STATIC	${lib_netbic_src})
add_library (netuaes 	STATIC	${lib_netuaes_src})
add_library (netist 	STATIC	${lib_netist_src})
add_library (netdcisc 	STATIC	${lib_dcisc_src})
add_library (netjcb     STATIC  ${lib_jcb_src})
add_library (netcup     STATIC  ${lib_cup_src})
add_library (netamxg 	STATIC	${lib_netmxg_src})
add_library (netsid3	STATIC	${lib_netsid3_src})
add_library (netdscover STATIC	${lib_dscover_src})
add_library (netlink 	STATIC	${lib_netlink_src})
add_library (netgb      STATIC  ${lib_gb_src})
add_library (netB24 	STATIC	${lib_netB24_src})
# MRY HFC 050723
add_library (netpstln 	STATIC	${lib_netpstln_src})
# END MRY HFC 050723

if( MQ_SERVER )
	include_directories($ENV{MQ_PATH}/inc)
	add_library (netmq     STATIC  ${lib_netmq_src})
	install (	TARGETS netmq 
				ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib)
endif()

install (	TARGETS netcom 
					nethsm 
					netemea 
					nethost 
					netT24 
					netpiso 
					netndc 
					netfra
					netgabd 
					netmds 
					netcis 
					netsid 
					netamex 
					netbnt 
					netcmd
					netbic
					netuaes
					netist
					netdcisc
					netjcb
					netcup
					nettls1
					netamxg 
					netsid3 
					netdscover
					netlink
					netgb
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)

