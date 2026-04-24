#include <define.h>
#include <br_cmd_fields.h>

static br_tlv_info_t  tab_TlvBrCmdInfo[] = 
{
	{BRCMD_COMMAND, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_PAN, 					BRCMD_TLV_VAR, 0,0 },
	{BRCMD_PROCESSING_CODE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AMOUNT_TRANSACTION, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AMOUNT_SETTLEMENT, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AMOUNT_CARD_BILLING, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AMOUNT_CARD_BILL_FEE, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_CONV_RATE_SETTLEMENT, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_CONV_RATE_CARD_BILL, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_INTERNAL_STAN, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_DATE_TIME_LOCAL_TRAN, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_EFFECTIVE_DATE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_EXPIRY_DATE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_SETTLEMENT_DATE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_CONVERSION_DATE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_CAPTURE_DATE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_MERCHANT_TYPE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ACQU_INST_COUNTRY, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_POS_DATA, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_CARD_SEQUENCE_NUMBER, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_FUNCTION_CODE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_MESSAGE_REASON_CODE, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_RRN, 					BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AUTH_NUMBER, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_RESPONSE_CODE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ADDITIONAL_RESP_DATA, 	BRCMD_TLV_VAR, 0,1 },
	{BRCMD_AMOUNT_FEE, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ADD_DATA_PRIVATE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_TRAN_CURRENCY_CODE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_SETL_CURRENCY_CODE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_BILL_CURRENCY_CODE, 		BRCMD_TLV_VAR, 0,1 },
	{BRCMD_PIN_DATA, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_SECURITY_DATA, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ADD_AMOUNTS, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ORIGINAL_DATA, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_MESSAGE_NUMBER, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_UF_FILE_RECORD, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_UF_ACTION_DATE, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_UF_FILENAME, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ACCOUNT_ID_1, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_ACCOUNT_ID_2, 			BRCMD_TLV_VAR, 0,1 },

	{BRCMD_REQUESTOR_ID, 			BRCMD_TLV_VAR, 0,1 },
	{BRCMD_REQ_KEY, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_EVENT_CODE, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_MESSAGE, 				BRCMD_TLV_VAR, 0,1 },
	{BRCMD_RESULT, 					BRCMD_TLV_VAR, 0,1 },
};

void  InitTlvBrCmd  (TSTlvBrCmd * tlvInfo)
{
   int i=0;
  
   for (i = 0; i < MAX_BRCMD_TLV; i++)
   {
      tlvInfo->nPosTlv [i] = 0;
      tlvInfo->nPresent[i] = NOT_PRESENT;
   }

   tlvInfo->nLength  = 0;
   memset(tlvInfo->sTlvData,   '\0', sizeof(tlvInfo->sTlvData));

}

void AdjustTlvBrCmd (TSTlvBrCmd * tlvInfo, int nIndice, int nLength)
{
   int i;
   for (i = 0; i < MAX_BRCMD_TLV; i++)
   {
       if ( tlvInfo->nPresent[i] == PRESENT && tlvInfo->nPosTlv [i] > tlvInfo->nPosTlv [nIndice] )
       {
            tlvInfo->nPosTlv [i]  += nLength;
       }
   }
}

int GetTlvBrCmdIndice ( char * szTlvName)
{
   int nIndice, nSize;
   nSize = sizeof(tab_TlvBrCmdInfo) /sizeof(br_tlv_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTlvName, tab_TlvBrCmdInfo[nIndice].sTlvName, 3) == 0)
         return(nIndice);
      nIndice++;

   }while (nIndice < nSize);
   return(-1);
}

int GetTlvBrCmdType ( int nIndice )
{
   return ( tab_TlvBrCmdInfo[nIndice].nType);
}

int GetTlvBrCmdLength ( int nIndice )
{
   return ( tab_TlvBrCmdInfo[nIndice].nLength);
}

int GetTlvBrCmd (char  *tlv_name, TSTlvBrCmd *tlvInfo, char *data , int *length)
{
   int		nIndice;
   int		nTlvLength;
   char		szAscTlvLen [4];
   int		nTlvType;


   memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
   data[0]='\0';
   (*length)=0;

   if ( (nIndice = GetTlvBrCmdIndice(tlv_name)) == -1)
      return(ECHEC);

   nTlvType    = GetTlvBrCmdType(nIndice) ;
   nTlvLength  = GetTlvBrCmdLength(nIndice); 

   if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
	memcpy ( szAscTlvLen,  tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,3);
	*length = atoi(szAscTlvLen);

	memcpy( data,tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, *length);
	data[*length] = '\0';

   return(SUCCESS);
}


int PutTlvBrCmd (char  *tlv_name, TSTlvBrCmd *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   int nTlvType;

   char szAscTlvLen [ 4 + 1 ];
   char szOldTlvLen [ 4  + 1];
   char szLastPart  [ MAX_BRCMD_TLV_LEN ];

   memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
   memset (szOldTlvLen, 0, sizeof(szOldTlvLen));

   if ( length == 0 )
      return(ECHEC);

   if ( (nIndice = GetTlvBrCmdIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }

   nTlvType    = GetTlvBrCmdType(nIndice) ;
   nTlvLength  = GetTlvBrCmdLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   memset (szLastPart,  0, sizeof(szLastPart));

   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {
		/********Sauvegarder le reste du message ****/
		memcpy( szOldTlvLen, 
				tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
				3);
		nOldTlvLen = atoi(szOldTlvLen);
		memcpy( szLastPart,
				tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 +nOldTlvLen, 
				tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]-6 - nOldTlvLen);

		/**************Formater La nouvelle valeur du TAG ***/
		memcpy (tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
				szAscTlvLen, 
				3);
		memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
				data, 
				length);

		/*************Restaurer le reste du message ****/
		memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 +length, 
				szLastPart, 
				tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]-6 - nOldTlvLen);

		/***********Mettre a jour la structure *******/
		tlvInfo->nLength += length - nOldTlvLen;
		AdjustTlvBrCmd( tlvInfo, nIndice, length-nOldTlvLen);

      return(SUCCES);
   }
   
  
	memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, data, length);

  
   memcpy (tlvInfo->sTlvData + tlvInfo->nLength    , tlv_name,    3);
   memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
   tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
   tlvInfo->nPresent[ nIndice ] = PRESENT;
   tlvInfo->nLength +=length + 3 + 3;

   memset (szLastPart,  0, sizeof(szLastPart));

   return(SUCCES);
}


int BrCmdBuildTlv (char * buffer_snd, TSTlvBrCmd *tlvInfo)
{
    char sLLLL [ 4 + 1 ];

    memset (  sLLLL  , 0 ,  sizeof ( sLLLL ));
    sprintf(  sLLLL  , "%04d" , tlvInfo->nLength );

    memcpy ( buffer_snd + 0     , sLLLL          , 4 );
    memcpy ( buffer_snd + 4     , tlvInfo->sTlvData , tlvInfo->nLength);
	buffer_snd[tlvInfo->nLength + 4 ] = '\0';
    return(tlvInfo->nLength + 4 );
}

int AnalyseTlvBrCmd(char *buffer, TSTlvBrCmd *tlvInfo)
{
   char  *sPtr;
   int   offset;
   int   nTlvLength, nBytes;
   int   nTlvIndice;
   int   nLenTlv; 
   char  szTlvName [ 5 ];
   char  szTlvValueLen [ 5 ];
   int   nTlvValueLen ;
   char  sLLLL [ 4 + 1];
   int   nLLLL;

   memset ( sLLLL ,          0 , sizeof ( sLLLL ) );


   InitTlvBrCmd(tlvInfo);
   memcpy ( sLLLL , buffer 	 , 4 );
   nLLLL   = atoi ( sLLLL );

   offset = 0;
   tlvInfo->nLength = nLLLL;
   memcpy (tlvInfo->sTlvData,  buffer + 4 ,  nLLLL);

   sPtr = buffer+4;

   while (offset < nLLLL)
   {
      memset( szTlvName,     '\0', sizeof(szTlvName));
      memset( szTlvValueLen,  '\0', sizeof(szTlvValueLen));
      memcpy( szTlvName,     sPtr,       3);
      memcpy( szTlvValueLen, sPtr+3,     3);

      nTlvValueLen = atoi(szTlvValueLen);
      nTlvIndice   = GetTlvBrCmdIndice( szTlvName); 

      if ( nTlvIndice >= 0)
      {
         tlvInfo->nPosTlv [ nTlvIndice ] = offset;
         tlvInfo->nPresent[ nTlvIndice ] = PRESENT;
      }

      nTlvLength =  3 + 3 +  nTlvValueLen;
      offset += nTlvLength;
      sPtr   += nTlvLength;
   }
    return(OK);
}


void DumpTlvBrCMd(TSTlvBrCmd *tlvInfo)
{
	char	sLine[MAX_LINE_TRC];
	char	sData[1024];
	int		nLength;
	int		i;
	int		nSize;

	nSize = sizeof(tab_TlvBrCmdInfo)/sizeof(tab_TlvBrCmdInfo[0]);
	for(i = 0; i < nSize; i++ )
	{
		if( GetTlvBrCmd(tab_TlvBrCmdInfo[i].sTlvName,tlvInfo,sData,&nLength) != SUCCESS )
			continue;
		if( tab_TlvBrCmdInfo[i].nPrint != 0 )
		{
			sprintf(sLine,"%.3s %.3d %s",tab_TlvBrCmdInfo[i].sTlvName,nLength,sData);
			trace_event(sLine,TRACE);
		}
		else
		{
			sprintf(sLine,"%.3s %.3d %s",tab_TlvBrCmdInfo[i].sTlvName,nLength,"*****");
			trace_event(sLine,TRACE);
		}

	}

}
