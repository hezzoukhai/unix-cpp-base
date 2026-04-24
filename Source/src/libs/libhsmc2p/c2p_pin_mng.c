/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_pin_mng.c                                                          **/
/*                                                                             **/
/* Description                                                                 **/
/*      Pin Change Management pour C2P                                         **/
/********************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		12-11-2015   		Mouloud OUSSYMOUH    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <define.h>
#include <queue.h>
#include <p7_common.h>
#include <iso8583.h>
#include <hsm_c2p_define.h>
#include <hsm_c2p.h>
#include <hsm_c2p_tag_def.h>
#include <resources.h>


/******************************************************************************/
/* Fonction PutIssScript_Pin_Chg.                                              */
/* -----------------------                                                    */
/******************************************************************************/

int     PutIssScript_Pin_Chg  (
                                int       nIndiceCtx,
                                char      *sTlvBuffer,
                                char 	  *sPinBlock,
                                char   	  *sKT_Key
                               )

{

   unsigned char  BuffSnd   [LG_MAX];
   unsigned char  BuffRcv   [LG_MAX];
   unsigned char  sPan          [22+ 1];  /* Preformated Pan/Sequence Nbr */
   unsigned char  sSeqNumber    [8 + 1];  /* Preformated Pan/Sequence Nbr */

   unsigned char  sAtc      [4 + 1]; /* Transaction counter          */
   unsigned char  sUn       [8 + 1]; /* Transaction counter          */
   
   unsigned char  sPlainTextData          [LG_MAX]; /* Data */
   int            nPlainTextLen                   ;
   unsigned char  sCipherTextData         [LG_MAX]; /* Data */
   unsigned char  sDataEncrypted          [1000]; /* Data */
   unsigned char  sMAC                    [100]; /* Data */
   unsigned char  sSecureMsgData          [LG_MAX]; /* Data */
   unsigned char  sIssScriptData          [LG_MAX]; /* Data */
   int            nSecureMsgLen                   ;
   unsigned char  sArqc         [16 + 1];   
   unsigned char  sSMI_Key       [256 + 1];   
   unsigned char  sSMC_Key       [256 + 1];   
   unsigned char  sAC_Key       [256 + 1];   
   unsigned char  cSchemeId     [2  + 1];
   unsigned char  sHsmResultCode[6 + 1];
   char			  sReturnCode[32];
   char			  sComplInfo[32];
   char           szWork[LG_MAX];
   char           stWork[300];
   char           sIccData[LG_MAX];
   char           sTmpField[200];
   char           sTmpData[200];
   char           sLine[MAX_LINE_TRC];
   char			  sData[256];
   char    sKeyRaw[32];
   TSTagIso       IccInfo;
   TSTlvPrivate   tlvInfo;
   TSC2PCmd		  kC2PCmd;
   int      	  nRetCode = 0;
   int     		  nLenWork = 0;
   int      	  nLenData = 0;
   int      	  nLength = 0;
   int      	  nTmpLen = 0;
   int      	  nIccLen = 0;
   int			  nLengthSnd;
   int			  nLengthRcv;
   int     		  nlength;
   int		      nResult;
   pid_t    	  nPid;
   char			  sChipIAD [64+1];
   char			  sCardInfo[30+1];
   int			  nDki;


   trace_event("Start PutIssScript_Pin_Chg(WWW)",PROCESSING);

   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );

   memset(  sPlainTextData        , 0, sizeof(sPlainTextData));
   memset(  sCipherTextData       , 0, sizeof(sCipherTextData));
   memset(  sSecureMsgData        , 0, sizeof(sSecureMsgData));
   memset(  sIssScriptData        , 0, sizeof(sIssScriptData));
   memset(  sDataEncrypted        , 0, sizeof(sDataEncrypted));
   memset(  sMAC        		  , 0, sizeof(sMAC));
   memset(  sPan        		  , 0, sizeof(sPan));
   memset(  sArqc        		  , 0, sizeof(sArqc));
   memset(  sAtc        		  , 0, sizeof(sAtc));
   memset(  sUn        			  , 0, sizeof(sUn));
   memset(  sHsmResultCode 		  , 0, sizeof(sHsmResultCode));
   memset(  sSMI_Key      	      , 0, sizeof(sSMI_Key));
   memset(  sSMC_Key      		  , 0, sizeof(sSMC_Key));
   memset(  szWork       		  , 0, sizeof(szWork));
   memset(  sTmpField    		  , 0, sizeof(sTmpField));
   memset(  sTmpField    		  , 0, sizeof(sTmpField));
   memset(  sTmpData      		  , 0, sizeof(sTmpData));
   memset(sReturnCode, 0, sizeof( sReturnCode ) );
   memset(sComplInfo, 0, sizeof( sComplInfo ) );
   memset(sChipIAD, '\0', sizeof(sChipIAD));	
   memset(sCardInfo, '\0', sizeof(sCardInfo));	

/** Retrieveing the Card Number **/
   if ( GetTlvBuffer ( AUTO_PAN , sTlvBuffer , sPan ,&nLenData) != SUCCESS )
   {
    memcpy ( sHsmResultCode , "-PAN", 4 );
    trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
    trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
    return(HSM_MALFUNCTION);
   }

/** Retrieveing the Card Seq Number **/
   memset(sSeqNumber, 0, sizeof(sSeqNumber));
   if (GetTlvBuffer (AUTO_CARD_SEQUENCE_NUMBER , sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
   {
    memcpy ( sHsmResultCode , "-PAN", 4 );
    trace_event("ERROR WhiLe GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER) ", ERROR);
    trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
    return(HSM_MALFUNCTION);
   }

   sprintf(sLine, "PAN                         =[%s]", sPan);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "SEQUENCE NUMBER             =[%s]", sSeqNumber);
   trace_event(sLine, PROCESSING);


   trace_event("- Generate SMI----nEw-----------", PROCESSING);

    if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccData ,&nIccLen) != SUCCESS )
	{
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ICC_DATA) ", ERROR);
		trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
    InitTlvPrivate (&tlvInfo);
    AnalyseTlvPrivate(sIccData , nIccLen,&tlvInfo);
	
	if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, sChipIAD, &nLenData) == SUCCESS &&	/*PLUTONL-3519*/ 	
		GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sCardInfo, &nLenData) == SUCCESS)
	{		
		nRetCode = Get_DKI_FROM_IAD(nIndiceCtx, sChipIAD, sCardInfo, &nDki);
	}
		
/*------------------------------- Getting the SMI key -----------------------------------*/
   nRetCode = Get_EMV_Key(nIndiceCtx, sPan, "SMI", sSMI_Key, nDki);	/*PLUTONL-3519*/
    if(nRetCode != OK)
    {
        memcpy ( sHsmResultCode , "-SMI", 4 );
        trace_event("INVALID KEY DMK ", ERROR);
        trace_event("END   PutIssScript_Pin_Chg(KEY_UNAVAILABLE)",PROCESSING);
        return(HSM_MALFUNCTION);
    }

   sprintf(sLine, "SMI KEY IS                  =[%s]:[%d]", sSMI_Key,sizeof(sSMI_Key));
   trace_event(sLine, PROCESSING);
   
/*------------------------------- Getting the SMC key -----------------------------------*/   
   nRetCode = Get_EMV_Key(nIndiceCtx, sPan, "SMC", sSMC_Key, nDki);	/*PLUTONL-3519*/
    if(nRetCode != OK)
    {
        memcpy ( sHsmResultCode , "-SMC", 4 );
        trace_event("INVALID KEY DMK ", ERROR);
        trace_event("END   PutIssScript_Pin_Chg(KEY_UNAVAILABLE)",PROCESSING);
        return(HSM_MALFUNCTION);
    }

   sprintf(sLine, "SMC KEY IS                  =[%s]:[%d]", sSMC_Key,sizeof(sSMC_Key));
   trace_event(sLine, PROCESSING);

/*------------------------------- Getting the AC key -----------------------------------*/   
   nRetCode = Get_EMV_Key(nIndiceCtx, sPan, "AC", sAC_Key, nDki);	/*PLUTONL-3519*/
    if(nRetCode != OK)
    {
        
        trace_event("Get_EMV_Key ", ERROR);
        trace_event("END   PutIssScript_Pin_Chg(KEY_UNAVAILABLE)",PROCESSING);
        return(HSM_MALFUNCTION);
    }

   sprintf(sLine, "AC KEY IS                  =[%s]:[%d]", sAC_Key,sizeof(sAC_Key));
   trace_event(sLine, PROCESSING);

/*------------------------------- PreFormated Pan -----------------------------------*/

   memset(szWork, 0, sizeof(szWork));
   memcpy(sTmpField, sPan, strlen(sPan));
   memcpy(sTmpField + strlen(sPan), sSeqNumber+1, 2);
   nLenWork = strlen(sTmpField);

/*------------------------------- Getting the ICC Data from TLV -----------------------------------*/
    /*Moved above
	 if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccData ,&nIccLen) != SUCCESS )
      {
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ICC_DATA) ", ERROR);
      trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
      }
    InitTlvPrivate (&tlvInfo);
    AnalyseTlvPrivate(sIccData , nIccLen,&tlvInfo);*/
		
/*------------------------------- cSchemeId(Visa["02"]) -----------------------------------*/
      
      memset(cSchemeId ,0,sizeof(cSchemeId));
      memcpy(cSchemeId,"02",2);
      sprintf(sLine, " cSchemeId                    : %s", cSchemeId);
   	  trace_event(sLine, PROCESSING);
			
			

/*------------------------------- Getting ATC -----------------------------------*/
    memset(szWork, 0, sizeof(szWork));                                                
    
    if (GetTlvPrivate (CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) != SUCCESS)
    	{
      	memcpy ( sHsmResultCode , "-PAN", 4 );
      	trace_event("ERROR WhiLe GetTlvPrivate(CHIP_ATC_TAG) ", ERROR);
      	trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
      	return(HSM_MALFUNCTION);
   		}
        
    memcpy(sAtc,szWork,  4);
    sprintf(sLine, "ATC                         =[%s]", szWork);
    trace_event(sLine, PROCESSING);
    
/*------------------------------- Getting ARQC -----------------------------------*/
    memset(stWork, 0, sizeof(stWork));
    if (GetTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG , &tlvInfo, stWork, &nLenWork) != SUCCESS)
    	{
      	trace_event("ERROR WhiLe GetTlvPrivate(CHIP_APP_CRYPTOGRAM_TAG) ", ERROR);
      	trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
      	return(HSM_MALFUNCTION);
   		}
        
   	memcpy(sArqc,stWork,16);
	
	sprintf(sLine, "ARQC  =[%s]", stWork);
    trace_event(sLine, PROCESSING);

    /*------------------------------- Chiffrement du PinBlock sous KENC -----------------------------------*/

	sprintf(sLine, "sKT_Key  =[%s]", sKT_Key);
    trace_event(sLine, PROCESSING);
	
    nResult = InitC2PCmd(&kC2PCmd, C2P_MN_PIN_CHG_E8);
	
	AddC2PCmdInfoField (C2P_FN_AC_ALGO_ID, &kC2PCmd, cSchemeId, GetFieldIdLength(C2P_FN_AC_ALGO_ID,&kC2PCmd));
	
	nLength = strlen(sSMC_Key);
	nLength = AsciiToHex(sSMC_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, sPan, GetFieldIdLength(C2P_FN_PAN,&kC2PCmd));
	AddC2PCmdInfoField (C2P_FN_PAN_SEQ_NO, &kC2PCmd, sSeqNumber, GetFieldIdLength(C2P_FN_PAN_SEQ_NO,&kC2PCmd));
	AddC2PCmdInfoField (C2P_FN_SESSION_KEY_GEN_USED_NBR, &kC2PCmd, sAtc, GetFieldIdLength(C2P_FN_SESSION_KEY_GEN_USED_NBR,&kC2PCmd));
	
	nLength = strlen(sKT_Key);
	nLength = AsciiToHex(sKT_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(sAC_Key);
	nLength = AsciiToHex(sAC_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(sPinBlock);
	nLength = AsciiToHex(sPinBlock,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ENC_PIN_BLK, &kC2PCmd, sKeyRaw, nLength);
	sData[0] = '0';
	AddC2PCmdInfoField (C2P_FN_PIN_BLK_FMT, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_PIN_BLK_FMT,&kC2PCmd));
	
 	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PutIssScript_Pin_Chg(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PutIssScript_Pin_Chg(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   PutIssScript_Pin_Chg(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_PIN_CHG_FF08);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);  		   
  	
  	sprintf(sLine, "==> sDataEncrypted       =[%s]", sDataEncrypted);
   	trace_event(sLine, PROCESSING);
   	
   	sprintf(sIssScriptData,"8424000210%.16s",sDataEncrypted);
	 
   /*------------------------------- Issuer Script Data -----------------------------------*/
    nPlainTextLen = 0;
    memset(szWork, 0, sizeof(szWork));
    memcpy(szWork,sIssScriptData,  strlen(sIssScriptData));
    memcpy(sPlainTextData + nPlainTextLen, szWork, 10); 
    nPlainTextLen += 10;

   /*------------------------------- Adding ATC ---------------------------------------*/
    memcpy(sPlainTextData + nPlainTextLen, sAtc, 4); 
    nPlainTextLen += 4;
				
   /*------------------------------- Adding ARQC -----------------------------------*/
   
	memcpy(sPlainTextData + nPlainTextLen, sArqc, 16); 
    nPlainTextLen += 16;
    
	if(strlen(sIssScriptData) > 10)
    {
       memset(szWork, 0, sizeof(szWork));
       memcpy(szWork,sIssScriptData,  strlen(sIssScriptData));
       memcpy(sPlainTextData+nPlainTextLen,szWork+10,(strlen(sIssScriptData)-10)); 
       nPlainTextLen += (strlen(sIssScriptData)-10);
    }

   /*------------------------------- Getting UN -----------------------------------*/

    memset(szWork, 0, sizeof(szWork));
	
    if (GetTlvPrivate (CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) != SUCCESS)
    {
    trace_event("ERROR WhiLe GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG) ", ERROR);
    trace_event("END   PutIssScript_Pin_Chg(NOK)",PROCESSING);
    return(HSM_MALFUNCTION);
   	}
   	
	memcpy(sUn,szWork,  8);
    sprintf(sLine, "UN                          =[%s]", szWork);
    trace_event(sLine, PROCESSING);
		
	/*------------------------------- Secure Messaging For Integrity -----------------------------------*/
 
    memset(BuffSnd, 0, sizeof( BuffSnd ) );
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_MAC_E4);
 
	AddC2PCmdInfoField (C2P_FN_MAC_ALGO_ID, &kC2PCmd, cSchemeId, GetFieldIdLength(C2P_FN_MAC_ALGO_ID,&kC2PCmd));
	
	nLength = strlen(sSMI_Key);
	nLength = AsciiToHex(sSMI_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, sPan, GetFieldIdLength(C2P_FN_PAN,&kC2PCmd));
	AddC2PCmdInfoField (C2P_FN_PAN_SEQ_NO, &kC2PCmd, sSeqNumber, GetFieldIdLength(C2P_FN_PAN_SEQ_NO,&kC2PCmd));
	AddC2PCmdInfoField (C2P_FN_SESSION_KEY_GEN_USED_NBR, &kC2PCmd, sAtc, GetFieldIdLength(C2P_FN_SESSION_KEY_GEN_USED_NBR,&kC2PCmd));
	AddC2PCmdInfoField (C2P_FN_MAC_DATA, &kC2PCmd, sPlainTextData, GetFieldIdLength(C2P_FN_MAC_DATA,&kC2PCmd));
	sData[1] = '4';
	AddC2PCmdInfoField (C2P_FN_MAC_LEN, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_MAC_LEN,&kC2PCmd));
	
	nLength = strlen(sPinBlock);
	nLength = AsciiToHex(sPinBlock,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ENC_PIN_BLK, &kC2PCmd, sKeyRaw, nLength);
	
	sData[0] = '0';
	AddC2PCmdInfoField (C2P_FN_PIN_BLK_FMT, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_PIN_BLK_FMT,&kC2PCmd));

 	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PutIssScript_Pin_Chg(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   PutIssScript_Pin_Chg(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_MAC_FF04);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

   /* MAC */
   memset(szWork, 0, sizeof(szWork));
   memset(sTmpField, 0, sizeof(sTmpField));
   memset(sTmpData, 0, sizeof(sTmpData));
   memcpy(sTmpField, sMAC,  16);
   sprintf(sLine, "==> SECURE MESSAGING MAC       =[%s]", sTmpField);
   trace_event(sLine, PROCESSING);
   nLenWork = 0;
   sprintf(szWork+nLenWork, "86%02X", (strlen(sIssScriptData)+strlen(sTmpField))/2 ); nLenWork += 4;
   memcpy(szWork+nLenWork, sIssScriptData, strlen(sIssScriptData)); nLenWork += strlen(sIssScriptData);
   memcpy(szWork+nLenWork, sTmpField, strlen(sTmpField)); nLenWork += strlen(sTmpField);

   /*-----For script Confidentiality (Data to be encrypted)-----*/
   memcpy(sCipherTextData,szWork,nLenWork);


   PutTlvPrivate (CHIP_ISS_SCRIPT_1, &tlvInfo, szWork, nLenWork);
		

   nIccLen = PrivateBuildTlv (sIccData, &tlvInfo);
   
   if ( 0 != nIccLen )
   {
   	  trace_event("nIccLen<>0", FATAL);
   	  PutTlvBuffer ( AUTO_ICC_DATA, sTlvBuffer, sIccData, nIccLen);
   } 
   else
   
   trace_event("nIccLen=0", FATAL);

   trace_event("End   PutIssScript_Pin_Chg(OK)",PROCESSING);
   return(OK);

}


int     PinFromTpkToLmk(
        int                             nIndexCtx,
        unsigned char   pTpkKey         [],
        unsigned char   pPinBlk         [],
        unsigned char   pPinBlkFmt      [],
        unsigned char   pCardNbr        [],
        unsigned char   pPinLmk         [])
{
        return NOK;

}

int Production(int nIndiceCtx, char *sTlvBuffer)
{
        return NOK;

}
