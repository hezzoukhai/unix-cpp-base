/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      hsm_pin_chg.c                                                          */
/*                                                                             */
/* Description                                                                 */
/*      Tools Management                                                       */
/*******************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <hsm_inc.h>
#include <define.h>
#include <dump.h>
#include <general_macros.h>
#include <queue.h>
#include <commands.h>
#include <event_auth.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#define PIN_POSITION  2



/************************************************************************/
/*                                                                      */
/************************************************************************/
char *StrMoins(char *sPinBlockC,char *sPinCalc)
{

unsigned char 	diff;
/*EBE140523: Adding static but still not correct unless TLS*/
static P7_TLS char 			dif[5];
char 			sLine[MAX_LINE_TRC];
int 			i = 0;

trace_event("Start StrMoins()", PROCESSING);
/*sprintf(sLine," Pin Block Claire : %s ", sPinBlockC);
trace_event(sLine,PROCESSING);
sprintf(sLine," Pin Block Calculer : %s ", sPinCalc);
trace_event(sLine,PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
for (;i<4;i++)
{
	if (sPinCalc[i]>sPinBlockC[i])
		diff=sPinBlockC[i]+10-sPinCalc[i];
	else
		diff=sPinBlockC[i]-sPinCalc[i];
	dif [i] = diff+'0';
}
dif [4] ='\0';

/*sprintf(sLine,"End StrMoins( %s ) ",dif);
trace_event(sLine,PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("End StrMoins(OK)",PROCESSING);

return(dif);
}
/******************************************************************************/
/******************************************************************************/
int	 ChangePin( int nIndexCtx,
				char *sPinBlck, 
		    	char *sPan,
		    	char *sTrack3, 
		    	char *sPinKey, 
		    	char *sTpkKey,
		    	char *sFunctionCode,
				char *sHsmResultCode
		    	)
{
pid_t   		nPid;
unsigned char	BuffSnd        [LG_MAX];
unsigned char   BuffRcv        [LG_MAX];
char			sPinCalcul	   [ 5];
char			sPinResult     [17];
char			pin_clair_recu [17];
char     		entree2        [17];
char     		sLine     	   [MAX_LINE_TRC];
int				lRet;

	trace_event("Debut ChangePin()",PROCESSING);

	memset(BuffSnd,                0, sizeof(BuffSnd)                 );
	memset(entree2,                0, sizeof(entree2)             );
	memset(BuffRcv,         	   0, sizeof(BuffRcv)         );
	memset(sPinResult,             0, sizeof(sPinResult)         );
	memset(pin_clair_recu,         0, sizeof(pin_clair_recu)         );
	memset(sPinCalcul,             0, sizeof(sPinCalcul) );

/*
   	trace_event("============================================",PROCESSING);
   	sprintf(sLine, "PAN                         : %.20s", sPan );
   	trace_event(sLine, PROCESSING);
   	sprintf(sLine, "PIN BLOCK                   : %.16s", sPinBlck);
   	trace_event(sLine, PROCESSING);
   	sprintf(sLine, "TPK                         : %.16s", sTpkKey );
   	trace_event(sLine, PROCESSING);
   	sprintf(sLine, "TAK                         : %.16s", sPinKey );
   	trace_event(sLine, PROCESSING);
   	trace_event("============================================",PROCESSING);
*/ /** HM - 09022010 - PA_DSS **/
   	command_HG(BuffSnd, sTpkKey, sPinBlck);
	
   	lRet = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );
   	if (  lRet != OK  )
   	{
      	memcpy ( sHsmResultCode , "UNVL", 4 );
      	trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      	trace_event("End   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      	return(HSM_UNAVAILABLE);
   	}
   	lRet = HsmQuery(nIndexCtx,nPid,BuffSnd, BuffRcv);
   	if(lRet == NOT_AVAILABLE)
   	{
      	memcpy ( sHsmResultCode , "UNVL", 4 );
      	trace_event("HSM NOT AVAILABLE.",PROCESSING);
      	trace_event("Fin   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      	return(HSM_UNAVAILABLE);
   	}
   	if(lRet == HSM_MALFUNCTION)
   	{
      	memcpy ( sHsmResultCode , "UNVL", 4 );
      	trace_event("MALFUNCTION ERROR", ERROR);
      	trace_event("Fin   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      	return(HSM_UNAVAILABLE);
   	}
   	lRet = command_HH(BuffRcv, pin_clair_recu , sHsmResultCode );
   	if ( lRet != OK )
   	{
      	switch ( lRet )
      	{
         	case ERROR_COMMAND :
      			trace_event("End   check_pin_data(ERROR_COMMAND)",PROCESSING);
      			return(HSM_MALFUNCTION);
	
         	default :
      			trace_event("End   check_pin_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
      			return(CRYPTOGRAPHIC_ERROR);
      	}
   	}
	memcpy(entree2,                "0000",                4);
	memcpy(entree2 + 4, sPan + strlen((char*)sPan) - 13, 12);
	/** x_or(pin_clair_recu, entree2, sPinResult); **/
        memcpy(sPinResult, pin_clair_recu, 16); /** MAW20030404: PIN Block Format 3 **/
	/*sprintf(sLine,"PIN CLAIRE RECU : %.16s\n", (char *)pin_clair_recu);
	trace_event(sLine,TRACE);*/ /** HM - 09022010 - PA_DSS **/
	lRet = calc_pin(nIndexCtx,sTrack3, sPinKey, sPinCalcul, sFunctionCode);
	if ( lRet != OK )
	{
		trace_event("Fin   ChangePin(NOK)",PROCESSING);
		return(NOK);
	}
	/*sprintf(sLine,"PIN CALC : %.4s\n", (char*)sPinCalcul);
	trace_event(sLine, TRACE);*/ /** HM - 09022010 - PA_DSS **/
	/** memcpy(sTrack3 + OFFS_OFFSET, StrMoins(sPinResult + 2, sPinCalcul), 4); **/
	memcpy(sTrack3 + OFFS_OFFSET, StrMoins(sPinResult, sPinCalcul), 4); /** MAW20030404: PIN Block Format 3 **/
	trace_event("End   ChangePin(OK)",PROCESSING); /** HM - 09022010 - PA_DSS **/
return(OK);
}
/************************************************************************/
/* Fonction UpdateTrack3                                           */
/* --------------------------                                           */
/*                                                                      */
/************************************************************************/
int	UpdateTrack3( int 			nIndexCtx,
			  	  TSTlvAutho    *sTlvInfo
			  	)
{
TSTlvPrivate    sTlvPrivateInfo;
char            sLine       [MAX_LINE_TRC];
char            sPinKey     [SIZE_KEY];
char            sTakKey     [SIZE_KEY];
char			sTrack3		[  105   ]; 
char			sPan		[  23    ]; 
char			sPinBlock	[  17    ]; 
char			sFunctionCode[4];
char			sTerminalNumber[10];
char			sHsmResultCode [10];
char   			sTrack2Data  [ 41];
char   			sExpiryDate  [  5];
char   			sServiceCode [  4];
char   			sPvv         [  6];
char   			sCvv1        [  4];
char   			sSecurityResults[ 21]; 
char            sTlvPrivateBuffer [ LG_MAX ];


int				nTrack3;
int				sRespCode;
int             nLength;
int             lRet;


trace_event("Debut UpdateTrack3()", PROCESSING); 
memset(sTakKey,  			0, SIZE_KEY);
memset(sPinKey,  			0, SIZE_KEY);
memset(sHsmResultCode ,  	0, sizeof(sHsmResultCode));
memset(sTrack2Data ,  		0, sizeof(sTrack2Data));
memset(sExpiryDate ,  		0, sizeof(sExpiryDate));
memset(sServiceCode ,  		0, sizeof(sServiceCode));
memset(sPvv ,  				0, sizeof(sPvv));
memset(sPan ,  				0, sizeof(sPan));
memset(sCvv1 ,  			0, sizeof(sCvv1));
memset(sPinBlock ,  			0, sizeof(sPinBlock));
memset(sSecurityResults ,  	0, sizeof(sSecurityResults));
memset(sTlvPrivateBuffer ,  0, sizeof(sTlvPrivateBuffer));


if(GetTlvAutho(AUTO_TRACK2_DATA,sTlvInfo,sTrack2Data,&nLength)== SUCCES)
	GetTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);

GetTlvAutho     ( AUTO_FUNCTION_CODE ,sTlvInfo,sFunctionCode,&nLength);
GetTlvAutho     ( AUTO_TRACK3_DATA ,sTlvInfo,sTrack3,&nTrack3);
if ( GetTlvAutho( AUTO_ADD_DATA_PRIVATE , sTlvInfo,sTlvPrivateBuffer , &nLength ) == SUCCES )
{
       AnalyseTlvPrivate(sTlvPrivateBuffer   ,nLength,&sTlvPrivateInfo);
	   GetTlvPrivate   ( ENCR_PIN_BLOCK_KEY_TAG,&sTlvPrivateInfo,sTakKey,&nLength);
}
lRet = GetPinKey ( nIndexCtx ,sPan, sPinKey);
if ( lRet != OK )
{
 	FillSecurityResult ( HSM_MALFUNCTION  , sSecurityResults , PIN_POSITION);
    PutHsmRjectEvent ( sTlvInfo , HSM_MALFUNCTION , PIN_POSITION );
    HsmDecline ( sTlvInfo , sSecurityResults , "-TAK" , PIN_POSITION);
    trace_event("End   UpdateTrack3(NOK)",PROCESSING);
    return ( NOK );
}
if( GetTlvAutho ( AUTO_ACQUIRER_REF_DATA, sTlvInfo, sPinBlock, &nLength ) == SUCCES)
{
	sRespCode = ChangePin (nIndexCtx,
					   sPinBlock,
                       sPan,
                       sTrack3,
                       sPinKey,
                       sTakKey,
			    	   sFunctionCode,
					   sHsmResultCode);

	if ( sRespCode != OK )
	{
  		FillSecurityResult ( sRespCode  , sSecurityResults , PIN_POSITION );
    	PutHsmRjectEvent ( sTlvInfo , sRespCode , PIN_POSITION );
    	HsmDecline ( sTlvInfo , sSecurityResults , sHsmResultCode , PIN_POSITION );
    	trace_event("End   UpdateTrack3(NOK)",PROCESSING);
   	 	return ( NOK );
	}
	sprintf(sLine," T3 : %s ",sTrack3);
	trace_event(sLine,TRACE);
	PutTlvAutho(AUTO_TRACK3_DATA,sTlvInfo,sTrack3,nTrack3);
}

trace_event("Fin   UpdateTrack3(OK)", PROCESSING); 
return(OK);
}
