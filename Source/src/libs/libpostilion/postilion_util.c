#include <stdio.h>
#include <stdlib.h>

#include <iso_hps.h>
#include <iso_postilion.h>
#include <postilion_param.h>
#include <iso_postilion_f127.h>
#include <postilion_fields.h>
#include <postilion.h>
#include <tools.h>
#include <define.h>

#include <string.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <hsm_inc.h>

int GetPostilionField(int field_no, TSPostilionInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szPostilionFldLen[7];
   char sLine[MAX_LINE_TRC];


/**
   sprintf(sLine,"Start GetPostilionField(%d)", field_no);
   trace_event(sLine,PROCESSING);
**/

   memset( szPostilionFldLen, '\0', sizeof(szPostilionFldLen));

   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionFieldType(field_no))
      {
      case POSTILION_LL_BIN :
           memcpy( szPostilionFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
           *nLength = length;
           break;

      case POSTILION_LLL_BIN :
           memcpy( szPostilionFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
           length = atoi(szPostilionFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
           *nLength = length;
           break;

      case POSTILION_LLLLLL_BIN :
           memcpy( szPostilionFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 6);
           length = atoi(szPostilionFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+6, length);
           *nLength = length;
           break;

      case POSTILION_FIX_BIN   :
      case POSTILION_FIX_ALPHA :
           length = GetPostilionFieldLength(field_no);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_FIX_HEX :
           length = GetPostilionFieldLength(field_no);
           nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no],
                                                           data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_LL_HEX :
           memcpy( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionFldLen);
           nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2,
                                                              data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_LLL_HEX :
         memcpy ( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_LLLLLL_HEX :
         memcpy ( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+6, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_LL_ALPHA :
         memcpy ( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szPostilionFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_LLL_ALPHA :
         memcpy ( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_LLLLLL_ALPHA :
         memcpy ( szPostilionFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+6, length);
         data[length] = '\0';
         *nLength = length;
         break;

      default :
         return(ECHEC);
         break;

      }

      /** trace_event("End GetPostilionField(SUCCES)"); **/
      return(SUCCES);
   }

   /** trace_event("End GetPostilionField(ECHEC)"); **/
   return(ECHEC);
}

int AddPostilionField(int field_no, TSPostilionInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char szPostilionFldLen[7];
   char  sLine[MAX_LINE_TRC];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   /* JGH 160102010 */
   /*sprintf(sLine,"Start AddPostilionField(%d) data: %s", field_no, data);*/
   /*trace_event(sLine,PROCESSING);*/

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;

   switch (GetPostilionFieldType(field_no))
   {
   case POSTILION_FIX_ALPHA :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LL_ALPHA :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 2);
      msgInfo->nLength += 2;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LLL_ALPHA :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 3);
      msgInfo->nLength += 3;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LLLLLL_ALPHA :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 6);
      msgInfo->nLength += 6;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_FIX_HEX :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      trace_event ("start AsciiToHex", PROCESSING);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;


   case POSTILION_LL_HEX :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_LLL_HEX :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_LLLLLL_HEX :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 6);
      msgInfo->nLength += 6;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_FIX_BIN :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LL_BIN :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LLL_BIN :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_LLLLLL_BIN :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionFldLen, 6);
      msgInfo->nLength += 6;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;


   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128) */)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int InsertPostilionField(int field_no, TSPostilionInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i;
   char szPostilionFldLen[7];
   char szPostilionData [260];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetPostilionFieldType(field_no))
   {
     case POSTILION_FIX_ALPHA :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy (szPostilionData, data, nBytes);
      break;

   case POSTILION_LL_ALPHA :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      memcpy( szPostilionData,        szPostilionFldLen, 2);
      nBytes=2;
      memcpy( szPostilionData+2, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_LLL_ALPHA :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionData, szPostilionFldLen, 3);
      memcpy( szPostilionData+3, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_LLLLLL_ALPHA :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionData, szPostilionFldLen, 6);
      memcpy( szPostilionData+6, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_FIX_HEX :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szPostilionData, nLength);
      break;

   case POSTILION_LL_HEX :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      nBytes1 = 2;
      memcpy( szPostilionData, szPostilionFldLen, nBytes1);
      nBytes = AsciiToHex( data, szPostilionData+2, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_LLL_HEX :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      nBytes1 = 3;
      memcpy( szPostilionData, szPostilionFldLen, 3);
      nBytes = AsciiToHex( data, szPostilionData+3, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_LLLLLL_HEX :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      nBytes1 = 6;
      memcpy( szPostilionData, szPostilionFldLen, 6);
      nBytes = AsciiToHex( data, szPostilionData+6, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_FIX_BIN :
      if ( GetPostilionFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szPostilionData, data, nBytes);
      break;

   case POSTILION_LL_BIN :
      sprintf ( szPostilionFldLen, "%.2d", nLength);
      nBytes = 2;
      memcpy( szPostilionData, szPostilionFldLen, nBytes);
      memcpy( szPostilionData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_LLL_BIN :
      sprintf ( szPostilionFldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionData, szPostilionFldLen, nBytes);
      memcpy( szPostilionData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_LLLLLL_BIN :
      sprintf ( szPostilionFldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionData, szPostilionFldLen, nBytes);
      memcpy( szPostilionData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_POSTILION_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionData, nBytes);
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutPostilionField(int field_no, TSPostilionInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i, nOldFldLen;
   char szPostilionData [260];
   char szPostilionFldLen[7];

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionFieldType(field_no))
      {
      case POSTILION_FIX_ALPHA :
         if ( GetPostilionFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy (szPostilionData, data, nBytes);
         break;

      case POSTILION_LL_ALPHA :
         sprintf ( szPostilionFldLen, "%.2d", nLength);
         memcpy( szPostilionData,        szPostilionFldLen, 2);
         nBytes=2;
         memcpy( szPostilionData+2, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_LLL_ALPHA :
         sprintf ( szPostilionFldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionData, szPostilionFldLen, 3);
         memcpy( szPostilionData+3, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_LLLLLL_ALPHA :
         sprintf ( szPostilionFldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionData, szPostilionFldLen, 6);
         memcpy( szPostilionData+6, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_FIX_HEX :
         if ( GetPostilionFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szPostilionData, nLength);
         break;

      case POSTILION_LL_HEX :
         sprintf ( szPostilionFldLen, "%.2d", nLength);
         nBytes1 = 2;
         memcpy( szPostilionData, szPostilionFldLen, nBytes1);
         nBytes = AsciiToHex( data, szPostilionData+2, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_LLL_HEX :
         sprintf ( szPostilionFldLen, "%.3d", nLength);
         nBytes1 = 3;
         memcpy( szPostilionData, szPostilionFldLen, 3);
         nBytes = AsciiToHex( data, szPostilionData+3, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_LLLLLL_HEX :
         sprintf ( szPostilionFldLen, "%.6d", nLength);
         nBytes1 = 6;
         memcpy( szPostilionData, szPostilionFldLen, 6);
         nBytes = AsciiToHex( data, szPostilionData+6, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_FIX_BIN :
         if ( GetPostilionFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szPostilionData, data, nBytes);
         break;

      case POSTILION_LL_BIN :
         sprintf ( szPostilionFldLen, "%.2d", nLength);
         nBytes = 2;
         memcpy( szPostilionData, szPostilionFldLen, nBytes);
         memcpy( szPostilionData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_LLL_BIN :
         sprintf ( szPostilionFldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionData, szPostilionFldLen, nBytes);
         memcpy( szPostilionData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_LLLLLL_BIN :
         sprintf ( szPostilionFldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionData, szPostilionFldLen, nBytes);
         memcpy( szPostilionData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_POSTILION_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      if (field_no >= 128)
         SetBit(msgInfo->sBitMap, 65);
      return(SUCCES);
   }
   else
      return(InsertPostilionField(field_no, msgInfo, data, nLength));
}

int PostilionBuildMsg (char * buffer_snd, TSPostilionInfo *msgInfo)
{
   char szMsgType[5];
   char sLine[ MAX_LINE_TRC ];
   int  iMapCtr;


   trace_event(" START   PostilionBuildMsg()", PROCESSING);

   memset(szMsgType, 0, 4); 
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);

   sprintf(sLine, "szMsgType : [%s] ", szMsgType);
   trace_event(sLine, PROCESSING);

   /*fprintf(stderr, "MsgType : %.4s\n", szMsgType); SWI09012012 insure*/

   memcpy(buffer_snd, szMsgType, 4); 


   iMapCtr = 1;
   while (( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) /* && (iMapCtr < 2) */ ) 
      		++iMapCtr;


   memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);

   memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);

   sprintf(sLine, "END  PostilionBuildMsg(%d)", 4 + (iMapCtr*8) + msgInfo->nLength);
   trace_event(sLine, PROCESSING);

   return( 4 + (iMapCtr*8) + msgInfo->nLength);
}

int PostilionBuildHeader(char*   pNewHeader,int sLen , TSPostilionInfo   *PostilionInfo)
{

	char            FieldData[LG_MAX];
	int             FieldLen;

	sprintf(pNewHeader, "%04d",sLen + 15 );
	memcpy(pNewHeader + 4,"ISO60000",8);

	if(GetPostilionField (POSTILION_ADTNL_DATA_PRIV, PostilionInfo, FieldData, &FieldLen)                                                                 == SUCCESS )
	{
		memcpy(pNewHeader,FieldData,3);
	}
	else
		memcpy(pNewHeader,"000",3);
}

/***********************************************************************************
**
**
**
**
**
**
************************************************************************************/

int GetTableInd( TSIsoInfo *IsoInfo, char *sAutTabInd, char *sAcquirerBank)
{

       char            slProcCode         [ 6 + 1 ] ;
       char            sMerchActv         [ 4 + 1 ] ;
       int             nLength                      ;
       char            sLine       [ MAX_LINE_TRC ] ;


       trace_event(" START     GetTableInd()", PROCESSING);

       /*memset( sAutTabInd, 0, sizeof(sAutTabInd)); SWI09012012 insure*/
	memset( sAutTabInd, 0, 4);

       GetIsoField( ISO_PROC_CODE       , IsoInfo , slProcCode      , &nLength ) ;
       sprintf(sLine, "   slProcCode  = [%s]", slProcCode);
       trace_event(sLine, PROCESSING);

       GetIsoField( ISO_MERCHANT_TYPE   , IsoInfo , sMerchActv      , &nLength ) ;
       sprintf(sLine, "   sMerchActv  = [%s]", sMerchActv);
       trace_event(sLine, PROCESSING);
	   
	   sprintf(sLine, "   sAcquirerBank  = [%s]", sAcquirerBank); /**ADD AHA 270522**/
       trace_event(sLine, PROCESSING);
	   
       /*memcpy(sAutTabInd, "ADM", 3);*/
       switch(IsoInfo->nMsgType)
        {
		case 1200 : memcpy(sAutTabInd, "ASM", 3); break;
		case 1220 : memcpy(sAutTabInd, "ASM", 3); break;
		case 1100 : memcpy(sAutTabInd, "ADM", 3); break;
		default	  : memcpy(sAutTabInd, "ADM", 3); 

	}

/**Start RHA120110**/
	if( !memcmp(sAcquirerBank, DEFAULT_BANK, 6) )
		memcpy(sAutTabInd, "ASM", 3);
        else
        	memcpy(sAutTabInd, "ADM", 3);
/**End RHA**/

       if( !memcmp(sMerchActv, "6011", 4) )
       {
          if( !memcmp(slProcCode, "38", 2) || !memcmp(slProcCode, "31", 2) 
	     || !memcmp(slProcCode, "30", 2) || !memcmp(slProcCode, "90", 2) || ! memcmp(slProcCode, "39", 2) )   /*JGH 01102010*/
             memcpy(sAutTabInd, "MSC", 3);
	  else 
		memcpy(sAutTabInd, "ASM", 3); /*JGH 04112010*/
       }

       /* Start JGH01112010   Test if the terminal is POS*/
	else
        {
       	if( !memcmp(slProcCode, "38", 2) || !memcmp(slProcCode, "31", 2)
             || !memcmp(slProcCode, "30", 2) || !memcmp(slProcCode, "90", 2)|| ! memcmp(slProcCode, "39", 2) )   /*JGH 01102010*/
            {
		 memcpy(sAutTabInd, "MSC", 3);
	    }
        else
	    {
		//memcpy(sAutTabInd, "ADM", 3);  
		memcpy(sAutTabInd, "ASM", 3);   /**ADD AHA 270522**/
	    }
	}
       /*End JGH */
       sprintf(sLine, " END       GetTableInd(%s)", sAutTabInd);
       trace_event(sLine, PROCESSING);
       return (OK);

}
/* START KES19082011 GET tab ind for national switch  **/
/***********************************************************************************
**
**
**
**
**
**
************************************************************************************/

int GetTableIndSwitch( TSIsoInfo *IsoInfo, char *sAutTabInd, char *sAcquirerBank)
{

       char            slProcCode         [ 6 + 1 ] ;
       char            sMerchActv         [ 4 + 1 ] ;
       int             nLength                      ;
       char            sLine       [ MAX_LINE_TRC ] ;


       trace_event(" START     GetTableIndSwitch()", PROCESSING);

       memset( sAutTabInd, 0, sizeof(sAutTabInd));

       GetIsoField( ISO_PROC_CODE       , IsoInfo , slProcCode      , &nLength ) ;
       sprintf(sLine, "   slProcCode  = [%s]", slProcCode);
       trace_event(sLine, PROCESSING);

       GetIsoField( ISO_MERCHANT_TYPE   , IsoInfo , sMerchActv      , &nLength ) ;
       sprintf(sLine, "   sMerchActv  = [%s]", sMerchActv);
       trace_event(sLine, PROCESSING);

       memcpy(sAutTabInd, "ASM", 3);


       if( !memcmp(sMerchActv, "6011", 4) )
       {
          if( !memcmp(slProcCode, "38", 2) || !memcmp(slProcCode, "31", 2)
             || !memcmp(slProcCode, "30", 2) || !memcmp(slProcCode, "90", 2) || ! memcmp(slProcCode, "39", 2) )
             memcpy(sAutTabInd, "MSC", 3);
          else
                memcpy(sAutTabInd, "ASM", 3); /*JGH 04112010*/
       }

       /* Start JGH01112010   Test if the terminal is POS*/
        else
        {
        if( !memcmp(slProcCode, "38", 2) || !memcmp(slProcCode, "31", 2)
             || !memcmp(slProcCode, "30", 2) || !memcmp(slProcCode, "90", 2)|| ! memcmp(slProcCode, "39", 2) ) 
            {
                 memcpy(sAutTabInd, "MSC", 3);
            }
        else
            {
                memcpy(sAutTabInd, "ASM", 3);
            }
        }
       sprintf(sLine, " END       GetTableIndSwitch(%s)", sAutTabInd);
       trace_event(sLine, PROCESSING);
       return (OK);

}
/* END KES19082011  Get Table indicator for national switch  **/

/***********************************************************************************
**
**
**
**
**
**
************************************************************************************/
int CalculPosMAC (int  nIndexCtx ,
               char *ressource_id,
               char *buffer,
               int  lg_buffer ,
               char *valMAC,
               char *sMacData)
{

	char mac_key    [ 256 + 1 ];
	int  result;
	int  lg_message_mac;
	char sHsmResultCode [4 + 1 ];
  int  vHeadLength;
  char sLine [ MAX_LINE_TRC ];
  int  nBytes;
  char sMacHex[ 16 + 1 ];   
  char mac_data[ 16 + 1 ];   

  result = MALFUNCTION; 

	memset(mac_key, 	0, sizeof(mac_key));
	memset(sMacHex, 	0, sizeof(sMacHex));
	memset(mac_data, 	0, sizeof(mac_data));
	memset(sHsmResultCode, 	0, sizeof(sHsmResultCode));
	memset(sLine, 	0, sizeof(sLine));



	trace_event ("Start  CalculMAC()", PROCESSING);
  sprintf(sLine, "==> valMAC    =[%s]" , valMAC);
	 trace_event(sLine, FATAL);

  if(memcmp(valMAC,"31",2)== 0) 
	      result = GetMAC1  ( nIndexCtx, ressource_id, mac_key) ;
  else
        result = GetMAC2  ( nIndexCtx, ressource_id, mac_key) ;
        
	if ( result != OK )
	{	
		return(MALFUNCTION);
	}


	result = generate_mac( nIndexCtx, 
                               mac_key,
                               mac_data,
                               buffer ,
                               lg_buffer  ,
                               sHsmResultCode ) ;
   sprintf(sLine, "==> ******mac_data     =[%s]" , mac_data);
	 trace_event(sLine, FATAL);

   memcpy(sMacData ,mac_data,8);
   if(memcmp(valMAC,"31",2)== 0) 
   memcpy(sMacData+8,"31",2);
   else
   memcpy(sMacData+8,"32",2);
   memcpy(sMacData+10,"202020",6);
   sprintf(sLine, "==> ******sMacData     =[%s]" , sMacData);
   trace_event(sLine, FATAL);


	trace_event ("End  CalculMAC()", PROCESSING);
	

	return(result);
}
/***********************************************************************************
**
**
**
**
**
**
************************************************************************************/

int CheckPosMAC (int  nIndexCtx ,
              char *ressource_id,
              char *buffer,
              int  lg_buffer )
{

	char message_mac [LG_MAX ];
	int  lg_message_mac;
	char mac_data    [ 9 ];
	char mac_key    [ 256 + 1 ];
	int  result;
	char sHsmResultCode [4 + 1 ];
  char sLine[80];
  char valMAC[2 + 1 ];
  int  nBytes;


        trace_event("Start CheckMAC()",PROCESSING);

	memset( message_mac, 0, sizeof(message_mac));
	memset( mac_data   , 0, sizeof(mac_data));
	memset( mac_key   , 0, sizeof(mac_key));

	lg_message_mac = lg_buffer - 18 ;
	memcpy( mac_data , buffer + lg_message_mac  , 8 );
	memcpy( valMAC , buffer + lg_message_mac +8, 2 );


  
	lg_message_mac -= 35 ;
	memcpy( message_mac , buffer + 35 , lg_message_mac);

        dump_buffer( message_mac, lg_message_mac, 'A', 'X', 'X');

	if(valMAC=="31")
	      result = GetMAC1  ( nIndexCtx, ressource_id, mac_key) ;
  else
        result = GetMAC2  ( nIndexCtx, ressource_id, mac_key) ;

	if ( result != OK )
	{	
                trace_event("End  CheckMAC ( MALFUNCTION )",PROCESSING);
		return(MALFUNCTION);
	}
	
	sprintf(sLine, "------> lg_buffer ( %d )",lg_buffer);
                trace_event(sLine,PROCESSING);
  sprintf(sLine, "------> lg_message_mac ( %d )",lg_message_mac);
                trace_event(sLine,PROCESSING);
	result =  verify_mac( nIndexCtx, 
                              mac_key,
                              mac_data,
                              message_mac,
                              lg_message_mac,
                              sHsmResultCode ) ;
	if ( result != OK )
	{
                sprintf(sLine, "End  CheckMAC ( %d )",result);
                trace_event(sLine,PROCESSING);
		return(result);
	}

        trace_event("End  CheckMAC ( OK )",PROCESSING);
	return(OK);
}