# directory: libs
#add_subdirectory (libsaf)
add_subdirectory (libuaeswitch)
add_subdirectory (libactions)
add_subdirectory (libasync)
add_subdirectory (libc)
add_subdirectory (libcis)
add_subdirectory (libcom)
add_subdirectory (libcom3)
add_subdirectory (libget)
add_subdirectory (libhost)
#add_subdirectory (libhsm)
add_subdirectory (libhsmtdes)
add_subdirectory (libhsmc2p)
add_subdirectory (libhsmatalla)
add_subdirectory (libiccprod)
add_subdirectory (libiso)
add_subdirectory (libiso3)
add_subdirectory (libmonitoring)
#add_subdirectory (libprocessing)		#EBE: libprocessing is no more used! do not uncomment
add_subdirectory (libtrn)
#add_subdirectory (libtrnm)
add_subdirectory (libtrnutils)
add_subdirectory (libiastrn)
add_subdirectory (libthr)
add_subdirectory (libtlv)
add_subdirectory (libtlvemv)
add_subdirectory (libtools)
add_subdirectory (libutils)
add_subdirectory (libvhsm)
add_subdirectory (libbic)
add_subdirectory (libint)
add_subdirectory (libint1)
add_subdirectory (libT24)
add_subdirectory (libist)
add_subdirectory (libiasservices)
add_subdirectory (libbridge)
add_subdirectory (libctrlunits)
add_subdirectory (libcb2a)
add_subdirectory (libfrd)
add_subdirectory (liblink)
add_subdirectory (libbase24)
#MRY HFC 050723
add_subdirectory (libpostilion)
#END MRY HFC 050723

#Diners lib
if( INTERFACE_DINERS )
        add_subdirectory (libdcisc)
endif()

#cbae libs

if( INTERFACE_CBAE )
	add_subdirectory (libcbae)
endif()

if( INTERFACE_CBAE OR FO_POS_CB2A)
	add_subdirectory (libcbcom)
endif()

#Diners lib
if( INTERFACE_JCB )
        add_subdirectory (libjcb)
endif()

#HSID lib
if( INTERFACE_HSID )
        add_subdirectory (libhsid)
endif()


#SID lib

add_subdirectory (libsid)

#include batchutils if we have batches


if ( 	BO_ISS_ST_CORE OR  
		BO_ACQ_ST_CORE OR
		BO_POS_ST_CORE OR
		BO_ATM_ST_CORE)
add_subdirectory (batchutils)
endif ()

#When should libamex be included?
add_subdirectory (libamex)


#include ATM libs
if( FO_ATM_ST_CORE OR
	FO_ATM_SA_CORE)
add_subdirectory (libatm)
#add_subdirectory (libdblutil)
#add_subdirectory (libgabdutil)  #not used
add_subdirectory (libndcutil)
#add_subdirectory (libgenubp)
#add_subdirectory (libstatement) #comment for now until we std
endif ()


# include libsaf for, ATM, POS and Switch
if( FO_ATM_ST_CORE OR
	FO_ATM_SA_CORE OR
	FO_POS_ST_CORE OR
	FO_POS_SA_CORE OR 
	FO_SWI_SA_CORE OR
	FO_SWI_ST_CORE)
add_subdirectory (libsaf)
endif()


if(	MQ_SERVER )
add_subdirectory (libmq)
endif()

