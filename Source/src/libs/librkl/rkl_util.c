/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      rkl_util.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      RKL Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include <iso_rkl.h>
/*#include <header_rkl.h>*/
#include <tools.h>
#include <define.h>


int GetRklField(int field_no, TSRklInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char sLine[MAX_LINE_TRC];
   char data1[MAX_LINE_TRC];
   char data2[MAX_LINE_TRC];
   char data3[MAX_LINE_TRC];
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetRklFieldType(field_no))
      {
      case RKL_VAR_BIN :
      case RKL_VAR_BITMAP :
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1,length);
         *nLength = length;
         break;

      case RKL_FIX_BIN :
         length = GetRklFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case RKL_FIX_HEX :                           
         length = GetRklFieldLength(field_no);
         nBytes = HexToAscii((unsigned char *) msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                             (unsigned char *)data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case RKL_VAR_HEX :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = HexToAscii((unsigned char*)msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            (unsigned char*)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case RKL_FIX_BCD :                           
         length = GetRklFieldLength(field_no);
         nBytes = BcdToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case RKL_VAR_BCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case RKL_VAR_SBCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            (unsigned char *)data, 2*length); 
         data[2*length] = '\0';
         *nLength = 2*length;
         break;

      case RKL_FIX_ALPHA :                           
         length = GetRklFieldLength(field_no);
	 if ( field_no == 61 ) {
	 trace_event("=================================================",TRACE);
	 /*dump_buffer(msgInfo->sData+msgInfo->nFieldPos[field_no],length,'X','X','X');*/
         EbcdicToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, 17); 
         nBytes = HexToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+17, 
                                                            (unsigned char *)data1, 32); 
         EbcdicToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+17+16, 
                                                            (unsigned char *)data2, 2); 
         nBytes = HexToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+17+16+2, 
                                                            (unsigned char *)data3, 12); 
/*	 dump_buffer(data,17,'X','X','X');
	 dump_buffer(data1,32,'X','X','X');
	 dump_buffer(data2,2,'X','X','X');
	 dump_buffer(data3,12,'X','X','X');
*/
         data[17] = '\0';
         data1[32] = '\0';
         data2[2] = '\0';
         data3[12] = '\0';
	 sprintf(data,"%17s%32s%2s%12s",data,data1,data2,data3);
	 dump_buffer(data,strlen(data),'X','X','X');
	 length = strlen(data);
	 trace_event("=================================================",TRACE);
         *nLength = length;
	 }
	 else {
         EbcdicToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, length); 
	 data[length] = '\0';
	 *nLength = length;
	  } 
         break;

      case RKL_VAR_ALPHA :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         EbcdicToAscii((unsigned char *)msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      default :
         return(ECHEC);
         break;
         
      }
      return(SUCCES);
   }
   return(ECHEC);
}

int AddRklField(int field_no, TSRklInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char  sLine[MAX_LINE_TRC];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetRklFieldType(field_no))
   {
   case RKL_FIX_BIN :
      nBytes = GetRklFieldLength(field_no);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nBytes);
      break;

   case RKL_VAR_BIN :
   case RKL_VAR_BITMAP :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case RKL_FIX_BCD :
      if ( GetRklFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd((unsigned char *) data, (unsigned char *)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case RKL_FIX_HEX :
      if ( GetRklFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex((unsigned char *) data, (unsigned char *)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case RKL_FIX_ALPHA :
      if ( GetRklFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic((unsigned char *) data,(unsigned char *) msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case RKL_VAR_HEX :
      sprintf(sLine,"RKL_VAR_HEX %s|%d",data,nLength);
      trace_event(sLine,TRACE);
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToHex((unsigned char *) data, (unsigned char *)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case RKL_VAR_BCD :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case RKL_VAR_SBCD :
      *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
      msgInfo->nLength++;
      nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case RKL_VAR_ALPHA :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      AsciiToEbcdic((unsigned char *)data,(unsigned char *) msgInfo->sData+msgInfo->nLength, nLength);
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

int InsertRklField(int field_no, TSRklInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   char szRklData [260];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetRklFieldType(field_no))
   {
   case RKL_FIX_BIN :
      nBytes = GetRklFieldLength(field_no);
      memcpy( szRklData, data, nBytes);
      break;

   case RKL_VAR_BIN :
   case RKL_VAR_BITMAP :
      *(szRklData) = nLength;
      nBytes = 1;
      memcpy( szRklData+1, data, nLength);
      nBytes += nLength;
      break;

   case RKL_FIX_BCD :
      if ( GetRklFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd((unsigned char *) data, (unsigned char *)szRklData, nLength);
      break;

   case RKL_FIX_HEX :
      if ( GetRklFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( (unsigned char *)data, (unsigned char *)szRklData, nLength);
      break;

   case RKL_FIX_ALPHA :
      if ( GetRklFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic((unsigned char *) data, (unsigned char *)szRklData, nLength);
      nBytes = nLength;
      break;

   case RKL_VAR_SBCD :
      *(szRklData) = (nLength+1)/2;
      nBytes = AsciiToBcd((unsigned char *) data, (unsigned char *)szRklData+1, nLength);
      nBytes++;
      break;

   case RKL_VAR_BCD :
      *(szRklData) = nLength;
      nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) szRklData+1, nLength);
      nBytes++;
      break;

   case RKL_VAR_HEX :
      *(szRklData) = nLength;
      nBytes = AsciiToHex((unsigned char *) data, (unsigned char *)szRklData+1, nLength);
      nBytes++;
      break;

   case RKL_VAR_ALPHA :
      *(szRklData) = nLength;
      nBytes =1;
      AsciiToEbcdic((unsigned char *)data, (unsigned char *)szRklData+1, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_RKL_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szRklData, nBytes);
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutRklField(int field_no, TSRklInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i, nOldFldLen;
   char szRklData [260];

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetRklFieldType(field_no))
      {
      case RKL_FIX_BIN :
         nBytes = GetRklFieldLength(field_no);
         memcpy( szRklData, data, nBytes);
         break;

      case RKL_VAR_BIN :
      case RKL_VAR_BITMAP :
         *(szRklData) = nLength;
         nBytes = 1;
         memcpy( szRklData+1, data, nLength);
         nBytes += nLength;
         break;

      case RKL_FIX_BCD :
         if ( GetRklFieldLength(field_no) != nLength )
            return(ECHEC);
         nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) szRklData, nLength);
         break;

      case RKL_FIX_HEX :
         if ( GetRklFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex((unsigned char *) data,(unsigned char *) szRklData, nLength);
         break;

      case RKL_FIX_ALPHA :
         if ( GetRklFieldLength(field_no) != nLength)
            return(ECHEC);
         AsciiToEbcdic((unsigned char *) data,(unsigned char *) szRklData, nLength);
         nBytes = nLength;
         break;

      case RKL_VAR_SBCD :
         *(szRklData) = (nLength+1)/2;
         nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) szRklData+1, nLength);
         nBytes++;
         break;

      case RKL_VAR_BCD :
         *(szRklData) = nLength;
         nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) szRklData+1, nLength);
         nBytes++;
         break;

      case RKL_VAR_HEX :
         *(szRklData) = nLength;
         nBytes = AsciiToHex((unsigned char *) data,(unsigned char *) szRklData+1, nLength);
         nBytes++;
         break;

      case RKL_VAR_ALPHA :
         *(szRklData) = nLength;
         nBytes =1;
         AsciiToEbcdic((unsigned char *)data, (unsigned char *)szRklData+1, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_RKL_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szRklData, nBytes);
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
      return(InsertRklField(field_no, msgInfo, data, nLength));
}

int RklBuildMsg (char * buffer_snd, TSRklInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  iMapCtr;

   memset(szMsgType, 0, 5);
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   fprintf(stderr, "MsgType : %.4s\n", szMsgType);
   nBytes = AsciiToBcd( (unsigned char *)szMsgType,(unsigned char *) buffer_snd, 4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) /* && (iMapCtr < 2) */ )
      ++iMapCtr;
   memcpy( buffer_snd+2, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+2 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( 2 + (iMapCtr*8) + msgInfo->nLength);
}

/*void RklBuildHeader(int      nReqResp, 
                    TSRklHeader*   pNewHeader, 
                    TSRklHeader*   pIncomingHeader, 
                    int nRklMsgLength,
                    char * szStationId)
{
   if (REQUEST == nReqResp)
   {
      memset( (char*)pNewHeader, 0, sizeof(TSRklHeader));
      AsciiToBcd((unsigned char *)szStationId,(unsigned char *) pNewHeader->abSrcStationId, 6);
   }
   else 
   {
      memcpy((char*)pNewHeader, (char*)pIncomingHeader, sizeof(TSRklHeader));
      memcpy(pNewHeader->abSrcStationId, pIncomingHeader->abDestStationId, 3);
      memcpy(pNewHeader->abDestStationId, pIncomingHeader->abSrcStationId, 3);
   }

   pNewHeader->bHeaderLength = RKL_HDR_LEN;

   memset( pNewHeader->abBaseIIFlags , 0, 2);
   pNewHeader->bHeaderFlag   = 0x01;
   pNewHeader->bTextFormat   = 0x02;
   pNewHeader->abMsgLength[0] =  (nRklMsgLength + RKL_HDR_LEN) / 256;
   pNewHeader->abMsgLength[1] =  (nRklMsgLength + RKL_HDR_LEN) % 256;
}
*/
/*void RklHeaderToBuffer ( TSRklInfo *VisaInfo , char *VisaHeaderBuffer )
{
    RklHeaderBuffer [ 0 ] =   RklInfo->sHeader.bHeaderLength;
    RklHeaderBuffer [ 1 ] =   RklInfo->sHeader.bHeaderFlag;
    RklHeaderBuffer [ 2 ] =   RklInfo->sHeader.bTextFormat;
    memcpy ( RklHeaderBuffer  + 2 ,   RklInfo->sHeader.abMsgLength      , 2 );
    memcpy ( RklHeaderBuffer  + 4 ,   RklInfo->sHeader.abDestStationId  , 3 );
    memcpy ( RklHeaderBuffer  + 7 ,   RklInfo->sHeader.abSrcStationId   , 3 );
    RklHeaderBuffer [ 10 ] =   RklInfo->sHeader.bEchoInfo;
    memcpy ( RklHeaderBuffer + 11  , RklInfo->sHeader.abBaseIIFlags   , 3 );
    memcpy ( RklHeaderBuffer + 14  , RklInfo->sHeader.abMsgFlags      , 3 ); 
    RklHeaderBuffer [ 17 ] =   RklInfo->sHeader.bBatchNumber;
    memcpy ( RklHeaderBuffer  + 18 , RklInfo->sHeader.abReserved    ,  3);
    RklHeaderBuffer [ 21 ] =   RklInfo->sHeader.bUserInfo ;
}*/
