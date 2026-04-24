  /***********************************************************************
  **  nom	: dblstatus.h						**
  **  desc	: traitement des status Dbl				**
  **  auteur	: YO990817						**
  **			(C) 1997-2001 : Hightech Payment Systems	**
  ***********************************************************************/

#ifndef HPS_DBLSTATUS_H
#define HPS_DBLSTATUS_H

/* - Hps Device Status -------------------------------------------------- */
#define	STATUS_OK			0
#define	STATUS_WARNING			1
#define	STATUS_NOK			2
#define	STATUS_ERR			3
#define	STATUS_FATAL			4

/* - Status Descriptor ------------------------------------------------- */
#define	SD_DEVICE_FAULT			'8'
#define	SD_READY			'9'
#define	SD_CMD_REJ			'A'
#define	SD_TRN_REP_SUCCES		'B'
#define	SD_SPEC_MAC_CMD_REJ		'C'
#define	SD_SPEC_CMD_REJ			'D'
#define	SD_TERM_STATE			'F'

/* - Device Identifier -------------------------------------------------- */
#define	DI_POWER_FAILURE		'1'
#define	DI_ALARM			'2'
#define	DI_SUPERVISORY_SWITCH		'3'
#define	DI_PRINTERS			'4'
#define	DI_CARD_READER			'5'
#define	DI_NOT_USED			'6'
#define	DI_CARD_WRITER			'7'
#define	DI_VANDAL_SHIELD		'8'
#define	DI_WITHDRAWAL_AREA_SENSORS	'<'
#define	DI_DISPENSER			'?'
#define	DI_DELIVERY_DOOR		'@'
#define	DI_IDM				'A'
#define	DI_COIN_DISPENSER		'G'
#define	DI_SUPPLY_COUNTS		'G'
#define	DI_ENHANCED_STATUS		'J'

/* - Status Information Fields Position --------------------------------- */
#define SI_DEVICE_ID_POS		0
#define	SI_TRANS_STATUS_POS		1
#define	SI_DEVICE_STATUS_POS		1
#define	SI_ERR_SEVERITY_POS		2
#define	SI_DIAG_STATUS_POS		3
#define	SI_SUPPLIES_STATUS_POS		4

/* - Error Severity ------------------------------------------------------ */
#define	ES_NO_ERR			"0"
#define	ES_ROUTING_ERR			"1"
#define	ES_WARNING_COND			"2"
#define	ES_SUSPEND			"3"
#define	ES_FATAL			"4"

#define GOOD                		3

#define OR_DUMP                         9903 
#define OR_EJECT_CARD                   9904 
#define LABEL_SIZE			100

#define	TRACE_AUDIT_NUMBER		10

/* - Command Reject Structure ------------------------------------------ */
typedef struct SCmdRejT{
    char	*fValue;
    char	*fQualifier;
    char	*fLabel;
} SCmdRej;

/* - Specific Command Reject Structure --------------------------------- */
typedef struct SSpecCmdRejT{
    char	*fValue;
    char	*fQualifier;
    char	*fLabel;
} SSpecCmdRej;

/* - Status information --------------------------------------------------- */
/*BEGIN AHM 10/08/2005*/
/*typedef struct SStatusInfoT{
    char	fStatusDesc	[  2];
    char	fDeviceId	[  2];
    char	fTransStatus	[256];
    char	fErrSeverity	[256];
    char	fDiagStatus	[256];
    char	fSuppliesStatus	[256];
    char	fStatusInfo	[512];
} SStatusInfo;
*/

typedef struct SStatusInfoT{
    char	fStatusDesc	[  2];
    char	fData		[256];
    char	fStatus		[256];
    char	fDenomStatus	[256];
    char	fErrorSource	[256];
} SStatusInfo;
/*END AHM*/

typedef struct sMdsStatusT {
	char            fErrorCode[9];
	char            fErrorLabel[LABEL_SIZE+1];
	char            fErrorSource[5];
	int             fAction;
	int     fStatusCounter;
	int     fMaxStatusCount;
	char        fOption[LABEL_SIZE+1];
} sMdsStatus;


#endif /*** HPS_DBLSTATUS_H ****/
