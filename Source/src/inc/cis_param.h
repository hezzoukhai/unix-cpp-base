#ifndef CIS___PARAM_H
#define CIS___PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
#include <resources.h>

extern int  nFdConnect;
extern int  nFdLocal;

#define CIS_PHONE_NBR_MAX_LEN		24
#define CIS_OPTS_MAX_LEN		24 /*SKA17032023 PLUTONL-6018 Enh 2023Q2*/

/*SKA17032023 PLUTONL-6018 Enh 2023Q2*/
typedef enum {
	CIS_OPT_CTRL_CL_MG		= 0	,

	CIS_CLT_OPTS_LEN
} E_CIS_CTL_OPTS;
/*SKA17032023 PLUTONL-6018 Enh 2023Q2*/

/* Resource Specific Parameters read from P7_CIS_RESOURCE_PARAM table */
extern char		StationId    [ ];
extern char             sIcaId    [ ];
extern char		AcquirerId   [];
extern char		AlphaCode3   [ ];
extern char		AcqCtryCode   [ ];
extern char		ReversalGeneration [];
extern char		sInterfaceMode [ ];
extern char		CisSignOnMode   [];
extern char		CisSignoffMode  [];
extern char		CisSessionMng  [];
extern char		CallCenterPhoneNbr[];
extern char             MsgSecurityCode []; /* MLZ240420 *//*NAB19052020 PLUTONL-1860*/
extern char     AmccUse				[];
extern int   EchoTestTimer;
extern int   MaxSendEchoTest;
extern char 		FileUpSecurityCode []; /* CFE */
extern char        CisOptions		[]; /*SKA17032023 PLUTONL-6018 Enh 2023Q2*/

#define	CIS_ICA_ID                  "C01"
#define	CIS_ACQUIRER_ID             "C02"
#define	CIS_COUNTRY                 "C03"
#define	CIS_ALPHA_CNTR_3            "C04"
#define	CIS_ALPHA_CNTR_2            "C05"
#define	CIS_AMC_USE                 "C06"
#define	CIS_SESSION_MNG             "C07"
#define	CIS_BANK_LOCATION           "C08"
#define	CIS_CELL_CENTER_PHONE       "C09"
#define CIS_MSG_SEC_CODE	    "C10" /* MLZ240420 *//*NAB19052020 PLUTONL-1860*/
/*Start IBO20200622 PLUTONL-1967*/
#define	CIS_INC_REQ	                  "C11"
#define	CIS_OUTG_RESP                 "C12"
#define	CIS_OUTG_REQ                  "C13"
#define	CIS_INC_RESP                  "C14"
/*End IBO20200622 PLUTONL-1967*/
#define CIS_IFU_SEC_COD             "C15" /*CFE Message Security Code for Issuer File update*/
#define CIS_CTL_CTLS_MAG             "C70" /*SKA17032023 PLUTONL-6018 Enh 2023Q2*/
#endif /*** CIS_PARAM_H ***/
