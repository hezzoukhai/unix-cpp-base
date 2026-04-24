#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>
#include <general_macros.h>
#include <dump.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <string.h>


/******************************************************************************/
int BankFileUpdateProc (	int nIndiceCtx, char *sTlvBuffer)
{
	
	
	int     		nLength;
	int 			nRetCode;
	char 			sActionCode       [ 3 + 1 ];
	char 			sAuthNum	      [ 7 + 1 ];
	char			sFunctionCode	[3 + 1];
	char			sCaptureCode	[ 6 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource   [ 6 + 1 ];
	char			sActionFlag[4];
	char			sLine[MAX_LINE_TRC];
	char			sRoutingCode[RESOURCE_ID_LEN + 1];
	char			sCaptureResInfo[128];
	char			sCaptureNodeId[4 + 1];
	char            sProcCode       [ 6 + 1 ];/*SNO22092015*/

	trace_event("Start BankFileUpdateProc ()",PROCESSING);
	/******************************************************************************/
	/*               Initialisation				                                  */
	/******************************************************************************/
	memset(sActionCode     		, 0, sizeof(sActionCode));
	memset(sCaptureCode     	, 0, sizeof(sCaptureCode));
	memset(sAuthNum     		, 0, sizeof(sAuthNum));
	memset(sFunctionCode     	, 0, sizeof(sFunctionCode));
	memset(sActionFlag     		, 0, sizeof(sActionFlag));
	memset(sRoutingCode		, 0, sizeof(sRoutingCode ));
	memset(sPrimResource		, 0, sizeof(sPrimResource));
	memset(sSecResource		, 0, sizeof(sSecResource));
	memset(sCaptureResInfo		, 0, sizeof(sCaptureResInfo));
	memset(sCaptureNodeId		, 0, sizeof(sCaptureNodeId));
	memset(sProcCode		, 0, sizeof(sProcCode));/*SNO22092015*/

	GetTlvBuffer ( AUTO_FUNCTION_CODE        , sTlvBuffer, sFunctionCode   , &nLength );
	GetTlvBuffer ( AUTO_RESPONSE_CODE        , sTlvBuffer, sActionCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_RES_INFO    , sTlvBuffer, sCaptureResInfo   , &nLength );
	GetTlvBuffer ( AUTO_PROCESSING_CODE    , sTlvBuffer, sProcCode   , &nLength );

	GetResInfoData(RES_INF_RES_NODE,sCaptureResInfo,sCaptureNodeId,&nLength);

	nRetCode = BankFileUpdateProcessingTlv  (   nIndiceCtx ,
				sPrimResource,
				STAND_IN_RESOURCE,
				sTlvBuffer
				);
	trace_event("End   AuthAdviceProc (OK)",PROCESSING);
	return ( OK );

}


