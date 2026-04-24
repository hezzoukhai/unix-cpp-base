#ifndef P7_ISO8583__H
#define P7_ISO8583__H


#include <p7_common.h>
#include <p7_types.h>
#include <iso8583/std_fields.h>
#include <iso8583/iso8583_define.h>
#include <iso8583/fieldinfo.h>
#include <iso8583/berinfo.h>
#include <iso8583/bitmapinfo.h>
#include <iso8583/msginfo.h>
#include <iso8583/staticinfo.h>
#include <iso8583/structinfo.h>
#include <iso8583/tlvinfo.h>
#include <iso8583/tlvbuffer.h>
#include <iso8583/propmng.h>

typedef enum {
	ISO8583_P7		= 0		,
	ISO8583_PPWM			,
	ISO8583_HOST			,
	ISO8583_SID				,
	ISO8583_VISA_BASE1		,
	ISO8583_VISA_SMS		,
	ISO8583_MCI_CIS			,
	ISO8583_MCI_MDS			,
	ISO8583_HID				,
	ISO8583_AMX_GNS			,
	ISO8583_AMX_GCA			,
	ISO8583_JCB				,
	ISO8583_CUP				,
	ISO8583_DISCOVER		,
	ISO8583_PULSE			,
	ISO8583_HSID			,
	ISO8583_SMT				,
	ISO8583_CB2A			,
	ISO8583_LINK			,

	ISO8583_USER	= 75	,
	ISO8583_B24_ACH_ATM		,
	ISO8583_B24_ACH_POS		,
	ISO8583_SGPOS			,
	


} E_ISO8583_PROTO;

/*
extern E_ISO8583_PROTO eInternalProtocol;
extern E_ISO8583_PROTO eNetworkProtocol;
*/

#endif


