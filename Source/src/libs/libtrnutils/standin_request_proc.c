/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Controle Autorisation module                                        */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Initialisation de la structure TLV buffer                            */
/*        See HYJCC120502                                                      */
/*        H.FETHI   21/10/2003 -         V 2.0.2                               */
/*        Ajout du traitement DynamicAdvice                                    */
/*        See HF 21 10 2003                                                    */
/*******************************************************************************/
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
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_services.h>


/******************************************************************************/
int StandinProc(int nIndexCtx, char*	sTlvBuffer, char* sReasonCode)
{

	char				sLine			[MAX_LINE_TRC];
	char				sServContainer	[CONTAINER_NAME_MAX_LEN + 1];
	char				sStandinEntry	[SERVICE_NAME_MAX_LEN + 1];
	char				sStan			[P7_F011_LEN + 1];
	char  				sCaptureCode   [       6 + 1];
	char  				sRoutingCode   [       6 + 1];
	int					nLength;
	int					nRetCode;
	int					nResult;
	fnServiceStandin	fnStandinEntry;


	trace_event("Start StandinProc()",PROCESSING);

	memset(sServContainer,'\0',sizeof(sServContainer));
	memset(sStandinEntry,'\0',sizeof(sStandinEntry));
	memset(sStan,'\0',sizeof(sStan));
	memset ( sCaptureCode   , 	'\0', sizeof(sCaptureCode) );
	memset ( sRoutingCode   , 	'\0', sizeof(sRoutingCode) );

	
	PrintTlvBuffer(sTlvBuffer);

	
	GetTlvBuffer ( AUTO_SERV_CONTAINER , sTlvBuffer , sServContainer ,&nLength);
	GetTlvBuffer ( AUTO_SERV_STANDIN_ENTRY , sTlvBuffer , sStandinEntry ,&nLength);
	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	GetTlvBuffer ( AUTO_ROUTING_CODE        , sTlvBuffer, sRoutingCode   , &nLength );


	if( strlen(sServContainer) == 0 || strlen(sStandinEntry) == 0 )
	{
		
		trace_event("End   StandinProc(No StandIn Process)",PROCESSING);
		return(NOK);
	}


	/*GetInternalStan(nIndexCtx, sStan);
	PutTlvBuffer (AUTO_INTERNAL_STAN    ,sTlvBuffer , sStan,P7_F011_LEN);
     */

	fnStandinEntry = GetStandinService(sServContainer,sStandinEntry);
	if( fnStandinEntry == NULL )
	{
		trace_event("End   IssuerTimeOutProc(Inv StandIn Process)",PROCESSING);
		return(OK);
	}

	PutTlvBuffer (AUTO_MESSAGE_REASON_CODE    ,sTlvBuffer , sReasonCode,P7_F025_LEN);

	nRetCode = (*fnStandinEntry)(nIndexCtx,sTlvBuffer);

	

	nResult = AutSafProcessing(nIndexCtx, sTlvBuffer,sCaptureCode,sRoutingCode);


	sprintf(sLine,"End   StandinProc(%d)",nRetCode);
	trace_event(sLine,PROCESSING);
	return nRetCode;
}

