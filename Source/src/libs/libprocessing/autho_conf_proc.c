/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*                                                                             */
/*                                                                             */
/* Description                                                                 */
/*                                                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version	
0002LHI				08-09-2006			Loubna Hidraoui				Modification AJout de la fonction  AuthorConfirmPrecessingEMV
---------------------------------------------------------------------------------------------------------------------------------*/
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
#include<ndc_param.h>

#include <iso_hps.h>
#include <ndcstatus.h>
#include <string.h>


#define  CONFIRM_MANAGEMENT	'N'

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les Confirmation autorisations                          */
/****************************************************************************/
/****************************************************************************/

int AuthorConfirmPrecessing_OLD( int nIndiceCtx, TSIsoInfo *IsoInfo , char *sTlvBuffer )
{
	int                         nRetCode;
	int                         length;
	char 						sActionCode       [ 3 + 1 ];
	TSTlvAutho                  sTlvInfo;
	TSTlvPrivate                sTlvPrivateInfo;
	char  tmpField [  LG_MAX];
	char  sCaptureCode   [  6 + 1];
	char  sRoutingCode   [  6 + 1];
	char  TableIndicator [  3 + 1];
	char  sPrimResource  [ 6 + 1 ];
	char  sSecResource   [ 6 + 1 ];
	char  sLine		[MAX_LINE_TRC];
	int   nLength;

	trace_event ( "Start AuthorConfirmPrecessing ()", PROCESSING );
	memset(sActionCode, 0, sizeof(sActionCode));
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	PrintTlvBuffer ( sTlvBuffer );

	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sCaptureCode   , &nLength );
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sRoutingCode   , &nLength );
	GetTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, TableIndicator , &nLength ); 
	GetTlvAutho ( AUTO_RESPONSE_CODE       , &sTlvInfo, sActionCode    , &nLength ); 

	PutTlvAutho ( AUTO_AUTHO_FLAG         , &sTlvInfo, "C" ,  1);
	length = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer ( sTlvBuffer );

	if(!memcmp(sActionCode, "000", 3))
	{
		PrintTlvBuffer ( sTlvBuffer );
		/* Modif Nabil 130607 , il faut faire l'appel avec 3 parametres*/
		nRetCode = UpdateAuthoActivityForAtmAdvice ( nIndiceCtx , sTlvBuffer,"1" );
	}

	trace_event ( "End AuthorConfirmPrecessing (OK)", PROCESSING );
	return ( OK );
}
/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les Confirmation autorisations  dans le cas d'une    */
/* Transaction EMV Wincor		Loubna Hidraoui 07/09/2006	    */
/****************************************************************************/
/****************************************************************************/
void AuthorConfirmPrecessingEMV(int nIndiceCtx, TSIsoInfo *IsoInfo , char *sTlvBuffer,SStatusInfo *StatusInfo) 
{
	int                         nRetCode;
	int                         length;
	char 			sActionCode       [ 3 + 1 ];
	TSTlvAutho                  sTlvInfo;
	TSTlvPrivate                sTlvPrivateInfo;
	char  tmpField [  LG_MAX];
	char  sCaptureCode    [  6 + 1];
	char  sRoutingCode    [  6 + 1];
	char  TableIndicator  [  3 + 1];
	char  sPrimResource   [ 6 + 1 ];
	char  sSecResource    [ 6 + 1 ];
	char  sLine		[MAX_LINE_TRC];
	int   nLength;
	char  sDataPrivate    [ 112 + 1 ];
	char  sTmp    [ 112 + 1 ];
	char  sCryptoInfoData [ 16 + 1 ];
	char	*sPtr;
	int   nLenTag;
	int   nLen;
	int   nOffset;
	char  lEmvIso     [LG_MAX + 1];
	TSTagIso      IsoIccInfo;

	trace_event ( "Start AuthorConfirmPrecessing EMV WINCOR()", PROCESSING );

	memset(sActionCode, 0, sizeof(sActionCode));
	InitTlvAutho(&sTlvInfo);

	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	PrintTlvBuffer ( sTlvBuffer );

	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sCaptureCode   , &nLength );
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sRoutingCode   , &nLength );
	GetTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, TableIndicator , &nLength ); 
	GetTlvAutho ( AUTO_RESPONSE_CODE       , &sTlvInfo, sActionCode    , &nLength ); 

	/****Check s'il s'agit d'une transaction EMV Wincor********/
	/*SKO290509*/  
	if(StatusInfo->fDeviceId[0]=='e')
	{
		nLen= strlen(StatusInfo->fTransStatus);
		trace_event("StatusInfo->fDeviceId[0]='e'",FATAL);
		sprintf(sLine,"EMV DATA                  : %d|%s|", nLen,StatusInfo->fTransStatus);
		trace_event(sLine,TRACE);

		nOffset = 0;
		memset(lEmvIso, 0, sizeof(lEmvIso));

		NdcEmvToIsoIc (StatusInfo->fTransStatus,  nLen, lEmvIso, &nOffset);

		sprintf(sLine,"=========>>>>nOffset = %d",nOffset);
		trace_event(sLine,FATAL);
		if(PutIsoField ( ISO_ICC_DATA , IsoInfo , lEmvIso ,  nOffset )!=SUCCES)
			trace_event("=>>>>>Erreur PutIsoIccData",FATAL);

		AnalyseIsoIc  ( lEmvIso, nOffset , &IsoIccInfo );

		dump_p7_struct(IsoInfo);
		trace_event("start ISO TO TLV UPDATE",FATAL);

		if(IsoToTlv (nIndiceCtx ,IsoInfo,&sTlvInfo)!=SUCCES)
			trace_event("Erreur Convert Iso To TLV",FATAL);

		trace_event("End Iso TO TLV Update",FATAL);
	}

	PutTlvAutho ( AUTO_AUTHO_FLAG         , &sTlvInfo, "C" ,  1); 

	memset(sTlvBuffer, 0, LG_MAX);
	length = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer ( sTlvBuffer );

	if  ( (!memcmp(sActionCode, APPROVED_ACTION , 3) )
		||   (!memcmp(sActionCode, UPDATE_TRACK3 , 3) ) )
	{
		PrintTlvBuffer ( sTlvBuffer );
		nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
	}
	if ( CONFIRM_MANAGEMENT == 'Y' )
	{
		if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 ) 
		{
			trace_event ( " INSERTION IN SATND_IN ", PROCESSING );
			InsertionSaf ( nIndiceCtx , sTlvBuffer,ResourceId, sPrimResource);
		}
	}
	trace_event ( "End AuthorConfirmPrecessing EMV WINCOR(OK)", PROCESSING );
}

