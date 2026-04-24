# directory: interfaces 

add_subdirectory (bridge_r)
add_subdirectory (bridge_s)

if ( INTERFACE_CIS )
add_subdirectory (cis)
endif (INTERFACE_CIS)

if (INTERFACE_T24)
add_subdirectory (interface_T24)
endif(INTERFACE_T24)


if ( INTERFACE_MDS )
add_subdirectory (mds)
endif (INTERFACE_MDS)

if( INTERFACE_CBAE )
add_subdirectory (cbae)
endif(INTERFACE_CBAE)

if ( INTERFACE_BASE1 )
add_subdirectory (base1)
endif (INTERFACE_BASE1)


if ( INTERFACE_SMS )
add_subdirectory (sms)
endif (INTERFACE_SMS)

if ( INTERFACE_SMT )
add_subdirectory (smt)
endif(INTERFACE_SMT)

#if ( INTERFACE_HOST )
add_subdirectory (interface_host)
#endif(INTERFACE_HOST)

if ( INTERFACE_SID )
add_subdirectory (sid)
endif(INTERFACE_SID)

if ( INTERFACE_HSID )
add_subdirectory (hsid)
endif(INTERFACE_HSID)


if (INTERFACE_UAESWITCH)
add_subdirectory (uaeswitch)
endif(INTERFACE_UAESWITCH) 

if (INTERFACE_IST)
add_subdirectory (ist_interface)
endif(INTERFACE_IST) 

if (INTERFACE_HID)
add_subdirectory (hid)
endif(INTERFACE_HID)

if ( INTERFACE_JCB )
add_subdirectory (jcb_int)
endif (INTERFACE_JCB)

if ( INTERFACE_CUP )
add_subdirectory (cup)
endif (INTERFACE_CUP)

if (INTERFACE_DINERS)
add_subdirectory (dcisc)
endif(INTERFACE_DINERS)

if (INTERFACE_PULSE)
add_subdirectory (pulse)
endif(INTERFACE_PULSE)

if (INTERFACE_AMEX_GNS)
add_subdirectory (amex_gns)
endif(INTERFACE_AMEX_GNS)

if (INTERFACE_AMEX_GCA)
add_subdirectory (amex_gca)
endif(INTERFACE_AMEX_GCA)

if (INTERFACE_FRAUD)
add_subdirectory (int_fraud)
endif(INTERFACE_FRAUD)

if (INTERFACE_DISCOVER)
add_subdirectory (discover_us)
endif(INTERFACE_DISCOVER)

if (INTERFACE_LINK)
add_subdirectory (link)
endif(INTERFACE_LINK)

if (INTERFACE_BASE24)
add_subdirectory (base24)
endif(INTERFACE_BASE24)


add_subdirectory (postilion) #MRY HFC 050723
