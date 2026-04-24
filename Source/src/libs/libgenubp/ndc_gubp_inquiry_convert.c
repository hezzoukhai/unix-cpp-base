/*NOT USED*/
#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <ndcmsgcst.h>
#include <atmopcode.h>
#include <hsm_inc.h>
#include <security_data.h>
#include <ndc_param.h>
#include <tlv_private.h>
#include <thr_ndc_extern.h>
#include <gubp_opcode.h>


extern double atof();
 
/*--------------------------------------------------*/

 /***********************************************************************
  **  nom   : To_Ubp_Inquiry_Msg                                       **
  **  desc   : conversion d'un message NDC de demande de balance       **
  **  entree   : NDCBuffer   -> tampon NDC                             **
  **  sortie   : P7Buffer   -> tampon PowerSEVEN                       **
  **  retour   :                                                       **
  **  auteur   : Younes OUADI (30 Oct 1998)                            **
  ***********************************************************************/
int To_Ubp_Inquiry_Msg(int nIndexCtx,
                 						char * NDCBuffer, 
								 						int NDCBuffer_len, 
                 						char      *AcqBank,                 						
                 						TSIsoInfo * P7Buffer	)
{
   char      trackii           [ 40];
   char      lProcessingCode   [  7];
   char      lFunctionCode     [  7];
   char      trackiii          [107];
   char      lOCBuffer         [  9];
   char      pin               [ 17];
   char      lPinComp          [ 17];
   char      transCurrency     [  4];
   char      cardNumber        [ 25];
   char      expiryDate        [  5];
   char      serviceCode       [  4];
   char      term_nbr          [ 18];
   char   	lAcqBank	 		[  6+1 ];/*ICH270715*/
  char      outlet_number     [ 18];
   char      sOutletNbr        [ 18];
   char      acq_country_code  [  4];
   char      card_acc_add      [102];
   char      lGmtTime          [ 13];
   char      lAudit            [  7];
   char      strPid            [  9];
   char      lPvv              [  7];
   char      lCvv              [  4];
   char      tmpField          [LG_MAX];
   char      lTpk1            [256 + 1];
   char      lTpk2            [256 + 1];

   int       lgField;
   int       nIndex = nIndexCtx - RESERVED_THREAD;
   int       mcc, tpk_number;
   int       trackii_len;
   int       trackiii_len;
   int       cpt;
   int       retour;
   int       i;
   TSTlvPrivate  TlvInfo;
   char      sLine             [MAX_LINE_TRC ];

   char      lEmvBuff          [  LG_MAX];
   char      sDestAccountNbr     [24 + 1];
   char      Tempcard  [7]; 
   int       nRetCode;
   
   char      vWbuffer          [256];
   char      amount          [256];
   char      sOpCode          [10];
   
   double    dbAmount;
   int  		 OpCode = 0;
   double    lTransAmount;
   
   memset(lEmvBuff, 0, sizeof(lEmvBuff));



   trace_event("Start To_Ubp_Inquiry_Msg()", PROCESSING);

   /* initialisation des variables */
   InitIsoInfo(P7Buffer);
   InitTlvPrivate(&TlvInfo);
   trace_event("Spy 0", PROCESSING);
   memset(lProcessingCode, 0, sizeof(lProcessingCode) );
   memset(lFunctionCode,   0, sizeof(lFunctionCode)   );
   memset(trackii,         0, sizeof(trackii)         );
   memset(trackiii,        0, sizeof(trackiii)        );
   memset(lOCBuffer,       0, sizeof(lOCBuffer)       );
   memset(pin,             0, sizeof(pin)             );
   memset(lPinComp,          0, sizeof(lPinComp)           );
   memset(transCurrency,   0, sizeof(transCurrency)   );
   memset(cardNumber,      0, sizeof(cardNumber)      );
   memset(expiryDate,      0, sizeof(expiryDate)      );
   memset(serviceCode,     0, sizeof(serviceCode)     );
   memset(term_nbr,        0, sizeof(term_nbr)        );
   memset(outlet_number,   0, sizeof(outlet_number)   );
   memset(acq_country_code,0, sizeof(acq_country_code));
   memset(card_acc_add,    0, sizeof(card_acc_add)    );
   memset(lGmtTime,        0, sizeof(lGmtTime)        );
   memset(lAudit,          0, sizeof(lAudit)        );
   memset(tmpField,        0, sizeof(tmpField)        );   
   memset(sOutletNbr,      0, sizeof(sOutletNbr)      );
   memset(strPid,          0, sizeof(strPid)          );
   memset(lPvv ,           0, sizeof(lPvv )           );
   memset(lCvv ,           0, sizeof(lCvv )           );
   memset(sLine ,          0, sizeof(sLine )          );
   memset(lTpk1,           0, sizeof(lTpk1)       );   
   memset(sDestAccountNbr,           0, sizeof(sDestAccountNbr)       );
   memset(sOpCode,           0, sizeof(sOpCode)       );
   memset(lAcqBank,       0, sizeof(lAcqBank)   );/*ICH270715*/

   
   
    trace_event("Spy 1", PROCESSING);
  /* memset(lTransAmount,           0, sizeof(lTransAmount)       );    */
   trace_event("Spy 2", PROCESSING);

   /* recuperation du contenu du buffer NDC */
   Download_Field(NDCBuffer, TR_TRACKII,       trackii,   		NDCBuffer_len);
   Download_Field(NDCBuffer, TR_TRACKIII,      trackiii,  		NDCBuffer_len);
   Download_Field(NDCBuffer, TR_OPERATION_CODE,lOCBuffer, 		NDCBuffer_len);
   
   memcpy(term_nbr, tab_NdcCtx[nIndex].terminal, 8);
   memcpy(lAcqBank,   tab_NdcCtx[nIndex].AcquirerBank, strlen(tab_NdcCtx[nIndex].AcquirerBank));/*ICH270715*/

	 OpCode = tab_NdcCtx[nIndex].OpReq;
	 
 
   
   	 memset(tab_NdcGUbp[nIndex].gPnr,           0, sizeof(tab_NdcGUbp[nIndex].gPnr));
   	 memset(tab_NdcGUbp[nIndex].gName,           0, sizeof(tab_NdcGUbp[nIndex].gName));   	 
   	 memset(tab_NdcGUbp[nIndex].gMobileNbr,           0, sizeof(tab_NdcGUbp[nIndex].gMobileNbr));
   	 memset(tab_NdcGUbp[nIndex].gAmount,           0, sizeof(tab_NdcGUbp[nIndex].gAmount));
   	 memset(tab_NdcGUbp[nIndex].gTotAmount,           0, sizeof(tab_NdcGUbp[nIndex].gTotAmount));
   	 memset(tab_NdcGUbp[nIndex].gFee,           0, sizeof(tab_NdcGUbp[nIndex].gFee));
   	 memset(tab_NdcGUbp[nIndex].gError_id,           0, sizeof(tab_NdcGUbp[nIndex].gError_id));
   	 memset(tab_NdcGUbp[nIndex].gError_desc,           0, sizeof(tab_NdcGUbp[nIndex].gError_desc));
   	 memset(tab_NdcGUbp[nIndex].gTrans_ID,           0, sizeof(tab_NdcGUbp[nIndex].gTrans_ID));   
   	 memset(tab_NdcGUbp[nIndex].gAccount,           0, sizeof(tab_NdcGUbp[nIndex].gAccount));   	 
   	 memset(tab_NdcGUbp[nIndex].gTran_Flag,           0, sizeof(tab_NdcGUbp[nIndex].gTran_Flag));   
   	 memset(tab_NdcGUbp[nIndex].gTerm_id,           0, sizeof(tab_NdcGUbp[nIndex].gTerm_id));    	 
   	 memset(tab_NdcGUbp[nIndex].gStan,           0, sizeof(tab_NdcGUbp[nIndex].gStan)); 
   	 memset(tab_NdcGUbp[nIndex].gCoord,           0, sizeof(tab_NdcGUbp[nIndex].gCoord)); 
   	 memset(tab_NdcGUbp[nIndex].gOpcode,           0, sizeof(tab_NdcGUbp[nIndex].gOpcode));    	 
   	 memset(tab_NdcGUbp[nIndex].gRecord_data,           0, sizeof(tab_NdcGUbp[nIndex].gRecord_data));    	 
   	    	 
   	 tab_NdcGUbp[nIndex].Account_flag = 1;   	 
   	    
   
   
   /* lOCBuffer[5] = ' '; */
   trackii_len       = strlen(trackii);
   trackiii_len       = strlen(trackiii);

   P7Buffer->nMsgType = 1200;


  if( strlen(trackii)> 2)
   	{
      GetTrack2Data(trackii+1, cardNumber, expiryDate, serviceCode, lPvv, lCvv);
   	}
   /* consultation de la base */
   retour = get_terminal_info(nIndexCtx,
							  							term_nbr,
							  							lAcqBank/*ICH270715*/,
                              outlet_number,
                              AcqBank,
                              &mcc,
                              acq_country_code,
                              card_acc_add,
                              &tpk_number);
   if(retour != OK)
   {
      trace_event("End   To_Ubp_Inquiry_Msg(PROCESSING_ERROR)", PROCESSING);
      return (PROCESSING_ERROR);
   }
   
   memcpy(tab_NdcGUbp[nIndex].gTerm_id, term_nbr, strlen(term_nbr));   
   
   /* () TRANSACTION_LANGUAGE */
   tab_NdcCtx[nIndex].TransLanguage = GetLanguageRequested(lOCBuffer, trackii + 1);

   /* () PRINT FLAG */
   tab_NdcCtx[nIndex].PrintFlag = GetPrintFlag(lOCBuffer);


   /* (2) CARD_NUMBER */
   if(strlen(trackii) < 2)
   		AddIsoField( ISO_CARD_NBR, P7Buffer, "5399999999999999", 16); 
	 else
	 		AddIsoField( ISO_CARD_NBR, P7Buffer, cardNumber, 16); 
	 	
	 
	 
	
   /* (3) PROCESSING_CODE */
   memcpy (lProcessingCode, "950000", 6);
   AddIsoField(ISO_PROC_CODE, P7Buffer, lProcessingCode, 6); 


     AddIsoField( ISO_TRANS_AMOUNT, P7Buffer, "000000000000", 12);

   /* (5) ISO_STLMNT_AMOUNT */

     AddIsoField(ISO_STLMNT_AMOUNT, P7Buffer, "000000000000", 12);

   /* (6) BILLING_AMOUNT */

     AddIsoField(ISO_BILLING_AMOUNT, P7Buffer, "000000000000", 12);


   /* (7) TRANSMISSION_DATE_TIME */
   current_gmt_date(lGmtTime);
   AddIsoField( ISO_XMIT_TIME, P7Buffer, lGmtTime+2, 10); 

   /* (11) TRACE_AUDIT_NUMBER */
   GetInternalStan(nIndexCtx, lAudit);
   AddIsoField( ISO_AUDIT_NBR, P7Buffer, lAudit, 6);    
   memcpy(tab_NdcGUbp[nIndex].gStan, lAudit, strlen(lAudit)); 

   /* (12) TRANSACTION_LOCAL_DATE */
   current_date_yymmdd(tmpField);
   current_hour       (tmpField + 6);
   AddIsoField(ISO_TRANS_DATE_TIME, P7Buffer, tmpField, 12); 

   /* (14) END_EXPIRY_DATE */
    if( strlen(trackii)> 2)
      AddIsoField( ISO_EXPIRY_DATE, P7Buffer, expiryDate, 4); 
    else
    	AddIsoField( ISO_EXPIRY_DATE, P7Buffer, "0101", 4);  

   /* (17) CAPTURE_DATE */
   current_date_mmdd(tmpField);
   AddIsoField( ISO_CAPTURE_DATE, P7Buffer, tmpField, 4); 

   /* (18) MERCHANT_TYPE */
   AddIsoField( ISO_MERCHANT_TYPE, P7Buffer, "6011", 4); 

   /* (19) ACQUIRING_COUNTRY_CODE */
   AddIsoField( ISO_ACQR_COUNTRY, P7Buffer, acq_country_code, 3); 

   /* (21) FORWARDING_INSTITUTION_COUNTRY_CODE */
   AddIsoField( ISO_FORWD_COUNTRY, P7Buffer, acq_country_code, 3); 

   /* (22) POS_DATA */
   AddIsoField( ISO_POS_DATA, P7Buffer, "211401211244", 12); 

   /* (24) FUNCTION_CODE */
   AddIsoField( ISO_FUNCTION_CODE, P7Buffer, "601", 3); 
   
   /* (26) CARD_ACCEPTOR_ACTIVITY_CODE */
   sprintf(tmpField, "%04d", mcc);
   AddIsoField( ISO_ACCEPTOR_ACTIV_CODE, P7Buffer, tmpField, 4);

   /* (27) AUTHORIZATION_CODE_LENGTH */
   AddIsoField( ISO_AUTHOR_ID_LENGTH, P7Buffer, "6", 1); 

   /* (32) ACQUIRER_INSTITUTION_IDENTIFICATION_CODE */
   AddIsoField( ISO_ACQR_ID, P7Buffer, /*AcqBank/*ICH270715*/lAcqBank, 6);

   /* (33) FORWARDING_INSTITUTION_IDENTIFICATION_CODE */
   AddIsoField( ISO_FORWD_ID, P7Buffer, AcqBank, 6); 

   /* (35) TRACKII */
   if(trackii_len > 2)
      AddIsoField( ISO_T2_DATA, P7Buffer, trackii+1, trackii_len-2); 


   /* (37) REFERENCE_NUMBER */
   current_julian_date_qjjj(tmpField);
   current_hour(  tmpField +4);
   memcpy( tmpField+6, lAudit, 6);
   AddIsoField( ISO_REFERENCE_NBR, P7Buffer, tmpField, 12); 

   /* (40) SERVICE_CODE */
   if(trackii_len > 2)
      AddIsoField( ISO_SERVICE_CODE, P7Buffer, serviceCode, 3); 

   /* (41) CARD_ACCEPTOR_TERMINAL_IDENTIFICATION */
   AddIsoField( ISO_TERMINAL_NBR, P7Buffer, term_nbr,  strlen(term_nbr)); 

   /* (42) CARD_ACCEPTOR_IDENTIFICATION_CODE */
   memset(sOutletNbr,' ',16);
   memcpy(sOutletNbr,outlet_number,strlen(outlet_number));
   sOutletNbr[15] = '\0';
   AddIsoField( ISO_OUTLET_NBR, P7Buffer, sOutletNbr,strlen(sOutletNbr));

   /* (43) CARD_ACCEPTOR_NAME_ADDRESS */
   AddIsoField( ISO_TERMINAL_ADR, P7Buffer,card_acc_add,strlen(card_acc_add)); 

   /* (48) TRANSACTION_CURRENCY */
   memset(tmpField, 0, sizeof(tmpField));
   AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, lAudit, 6);
   /*AddTlvPrivate (PAY_BILLPAYMENT_TAG, &TlvInfo, "DED", 3); Utility Name */

    lgField = PrivateBuildTlv (tmpField, &TlvInfo);
    if (lgField > 0)
      AddIsoField (ISO_ADTNL_DATA_PRIV, P7Buffer, tmpField, lgField);

   /* (49) TRANSACTION_CURRENCY */

   GetTransCurrency(nIndexCtx,term_nbr,lAcqBank,lOCBuffer, transCurrency);
   AddIsoField( ISO_TRANS_CRNCY, P7Buffer,transCurrency, 3); 
   AddIsoField( ISO_STLMNT_CRNCY, P7Buffer,transCurrency, 3); 
   AddIsoField( ISO_BILLING_CRNCY, P7Buffer,transCurrency, 3); 


   /* (53) SECURITY_DATA */
   memset(tmpField, 0, sizeof(tmpField));
   sprintf(strPid, "%08d", getpid());
   PutSecurityData( SECURITY_FORMAT         , tmpField,  "01"      , 2);
   PutSecurityData( PIN_BLOCK_FORMAT        , tmpField,  "01"      , 2);
   PutSecurityData( PIN_ENCRYPTION_KEY_INDEX, tmpField,  "010"     , 3);
   PutSecurityData( MAC_KEY_INDEX           , tmpField,  "000"     , 3);
   PutSecurityData( SOURCE_PID              , tmpField,  strPid    , strlen(strPid));
   PutSecurityData( SOURCE_RESOURCE         , tmpField,  ResourceId, 6);
   PutSecurityData( DESTINATION_PID         , tmpField,  "00000000"  , 8);
   PutSecurityData( DESTINATION_RESOURCE    , tmpField,  "000000"  , 6);
   PutSecurityData( USER                    , tmpField,  "ATM            ", 15);
   PutSecurityData( PURGE_TIME              , tmpField,  "000000000",  9);
   AddIsoField( ISO_SECURITY_DATA, P7Buffer, tmpField, strlen(tmpField)); 

    /* (54) ADDITIONNEL AMOUNT */   
/*   AddIsoField( ISO_ADTNL_AMOUNTS, P7Buffer, "SH", 2);  */
   



/* (72) Record Data : DED Account 9 Digits */


	


    /* (114) CIF  OPCODE*/ 
      
       sprintf(sOpCode,"%d", OpCode);       
       		sprintf(sLine, "sOpCode Number  [ %s ]", sOpCode);
			    trace_event(sLine, PROCESSING); 
			    			    
      AddIsoField( ISO_RESERVED_ISO_5, P7Buffer, sOpCode,strlen(sOpCode));       
       memcpy(tab_NdcGUbp[nIndex].gOpcode, sOpCode, strlen(sOpCode));       
      
    /* (115) CIF  OPCODE*/   
      AddIsoField( ISO_ADTNL_TRACE_DATA, P7Buffer, lOCBuffer,strlen(lOCBuffer)); 
  
  
  
  		
   dump_p7_struct(P7Buffer);
   memset(trackii,         0, sizeof(trackii)         );
   memset(trackiii,        0, sizeof(trackiii)        );
   memset(transCurrency,   0, sizeof(transCurrency)   );
   memset(cardNumber,      0, sizeof(cardNumber)      );
   memset(expiryDate,      0, sizeof(expiryDate)      );
   memset(serviceCode,     0, sizeof(serviceCode)     );
   memset(tmpField,        0, sizeof(tmpField)        );
   memset(sDestAccountNbr, 0, sizeof(sDestAccountNbr) );
   memset(lPvv ,           0, sizeof(lPvv )           );
   memset(lCvv ,           0, sizeof(lCvv )           );
   memset(sLine ,          0, sizeof(sLine )          );
   trace_event("End   To_Ubp_Inquiry_Msg(OK)", PROCESSING);
   return (OK);
}
