#include <stdio.h>
#include <stdlib.h>

#include <iso_postilion.h>
#include <iso_postilion_Emv.h>
#include <postilion_fields.h>
#include <tools.h>
#include <define.h>


int GetPostilionEmvField(int field_no, TSPostilionEmvInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szPostilionEmvFldLen[7];
   char sLine[MAX_LINE_TRC];




   memset( szPostilionEmvFldLen, '\0', sizeof(szPostilionEmvFldLen));

   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionEmvFieldType(field_no))
      {
      case POSTILION_EMV_LL_BIN :
           memcpy( szPostilionEmvFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionEmvFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
           *nLength = length;
           break;

      case POSTILION_EMV_LLL_BIN :
           memcpy( szPostilionEmvFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
           length = atoi(szPostilionEmvFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
           *nLength = length;
           break;

      case POSTILION_EMV_LLLL_BIN :
           memcpy( szPostilionEmvFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 4);
           length = atoi(szPostilionEmvFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+4, length);
           *nLength = length;
           break;

      case POSTILION_EMV_LLLLLL_BIN :
           memcpy( szPostilionEmvFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 6);
           length = atoi(szPostilionEmvFldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+6, length);
           *nLength = length;
           break;

      case POSTILION_EMV_FIX_BIN   :
      case POSTILION_EMV_FIX_ALPHA :
           length = GetPostilionEmvFieldLength(field_no);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
           data[length] = '\0';
/***
           sprintf(sLine, "==> msgInfo->nFieldPos[%d]=[%d]", field_no+1, msgInfo->nFieldPos[field_no]);
           trace_event(sLine, PROCESSING);
***/
           *nLength = length;
           break;

      case POSTILION_EMV_FIX_HEX :
           length = GetPostilionEmvFieldLength(field_no);
           nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no],
                                                           data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_EMV_LL_HEX :
           memcpy( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionEmvFldLen);
           nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2,
                                                              data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_EMV_LLL_HEX :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionEmvFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLLL_HEX :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 4);
         length = atoi(szPostilionEmvFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+4, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLLLLL_HEX :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionEmvFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+6, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

/********ASE24122011******/
      case POSTILION_EMV_L_ALPHA :
      		 trace_event(" Transaction Sequence Counter(..ASE..)",PROCESSING);
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 1);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+1,  msgInfo->nFieldPos[field_no]);
         data[length] = '\0';
         *nLength = length;
         break;
/********ASE24122011******/

      case POSTILION_EMV_LL_ALPHA :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLL_ALPHA :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLLL_ALPHA :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 4);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+4, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLLLL_ALPHA :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 5);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+5, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_EMV_LLLLLL_ALPHA :
         memcpy ( szPostilionEmvFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionEmvFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+6, length);
         data[length] = '\0';
         *nLength = length;
         break;



      default :
         return(ECHEC);
         break;

      }
/**
      trace_event("End GetPostilionEmvField(SUCCES)");
**/
      return(SUCCES);
   }
/**
   trace_event("End GetPostilionEmvField(ECHEC)");
**/
   return(ECHEC);

}

int AddPostilionEmvField(int field_no, TSPostilionEmvInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char szPostilionEmvFldLen[7];
   char  sLine[MAX_LINE_TRC];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetPostilionEmvFieldType(field_no))
   {
   case POSTILION_EMV_FIX_ALPHA :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LL_ALPHA :
   sprintf(sLine,"Start GetPostilionEmvField(%d)", field_no);
   trace_event(sLine,PROCESSING);
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 2);
      msgInfo->nLength += 2;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 3);
      msgInfo->nLength += 3;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 4);
      msgInfo->nLength += 4;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

/* MC 310507 */

   case POSTILION_EMV_LLLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.5d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 5);
      msgInfo->nLength += 5;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

/* MC 310507 */
      
   case POSTILION_EMV_LLLLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 6);
      msgInfo->nLength += 6;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_FIX_HEX :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_EMV_LL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_EMV_LLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_EMV_LLLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 4);
      msgInfo->nLength += 4;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_EMV_LLLLLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 6);
      msgInfo->nLength += 6;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_EMV_FIX_BIN :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LLLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 4);
      msgInfo->nLength += 4;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_EMV_LLLLLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionEmvFldLen, 6);
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

int InsertPostilionEmvField(int field_no, TSPostilionEmvInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i;
   char szPostilionEmvFldLen[7];
   char szPostilionEmvData [260];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetPostilionEmvFieldType(field_no))
   {
     case POSTILION_EMV_FIX_ALPHA :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy (szPostilionEmvData, data, nBytes);
      break;

   case POSTILION_EMV_LL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      memcpy( szPostilionEmvData,        szPostilionEmvFldLen, 2);
      nBytes=2;
      memcpy( szPostilionEmvData+2, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_LLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 3);
      memcpy( szPostilionEmvData+3, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_LLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      nBytes = 4;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 4);
      memcpy( szPostilionEmvData+4, data,        nLength);
      nBytes += nLength;
      break;

/* MC 310507 */

	 case POSTILION_EMV_LLLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.5d", nLength);
      nBytes = 5;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 5);
      memcpy( szPostilionEmvData+5, data,        nLength);
      nBytes += nLength;
      break;

/* MC 310507 */
      
   case POSTILION_EMV_LLLLLL_ALPHA :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 6);
      memcpy( szPostilionEmvData+6, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_FIX_HEX :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szPostilionEmvData, nLength);
      break;

   case POSTILION_EMV_LL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      nBytes1 = 2;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes1);
      nBytes = AsciiToHex( data, szPostilionEmvData+2, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_EMV_LLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      nBytes1 = 3;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 3);
      nBytes = AsciiToHex( data, szPostilionEmvData+3, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_EMV_LLLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      nBytes1 = 4;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 4);
      nBytes = AsciiToHex( data, szPostilionEmvData+4, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_EMV_LLLLLL_HEX :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      nBytes1 = 6;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, 6);
      nBytes = AsciiToHex( data, szPostilionEmvData+6, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_EMV_FIX_BIN :
      if ( GetPostilionEmvFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szPostilionEmvData, data, nBytes);
      break;

   case POSTILION_EMV_LL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
      nBytes = 2;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
      memcpy( szPostilionEmvData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_LLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
      memcpy( szPostilionEmvData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_LLLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
      nBytes = 4;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
      memcpy( szPostilionEmvData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_EMV_LLLLLL_BIN :
      sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
      memcpy( szPostilionEmvData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_POSTILION_EMV_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionEmvData, nBytes);
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutPostilionEmvField(int field_no, TSPostilionEmvInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i, nOldFldLen;
   char szPostilionEmvData [260];
   char szPostilionEmvFldLen[7];

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionEmvFieldType(field_no))
      {
      case POSTILION_EMV_FIX_ALPHA :
         if ( GetPostilionEmvFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy (szPostilionEmvData, data, nBytes);
         break;

      case POSTILION_EMV_LL_ALPHA :
         sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
         memcpy( szPostilionEmvData,        szPostilionEmvFldLen, 2);
         nBytes=2;
         memcpy( szPostilionEmvData+2, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_LLL_ALPHA :
         sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 3);
         memcpy( szPostilionEmvData+3, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_LLLL_ALPHA :
         sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
         nBytes = 4;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 4);
         memcpy( szPostilionEmvData+4, data,        nLength);
         nBytes += nLength;
         break;

/* MC 310507 */
		  case POSTILION_EMV_LLLLL_ALPHA :
         sprintf ( szPostilionEmvFldLen, "%.5d", nLength);
         nBytes = 5;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 5);
         memcpy( szPostilionEmvData+5, data,        nLength);
         nBytes += nLength;
         break;

/* MC 310507 */
         
      case POSTILION_EMV_LLLLLL_ALPHA :
         sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 6);
         memcpy( szPostilionEmvData+6, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_FIX_HEX :
         if ( GetPostilionEmvFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szPostilionEmvData, nLength);
         break;

      case POSTILION_EMV_LL_HEX :
         sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
         nBytes1 = 2;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes1);
         nBytes = AsciiToHex( data, szPostilionEmvData+2, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_EMV_LLL_HEX :
         sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
         nBytes1 = 3;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 3);
         nBytes = AsciiToHex( data, szPostilionEmvData+3, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_EMV_LLLL_HEX :
         sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
         nBytes1 = 4;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 4);
         nBytes = AsciiToHex( data, szPostilionEmvData+4, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_EMV_LLLLLL_HEX :
         sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
         nBytes1 = 6;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, 6);
         nBytes = AsciiToHex( data, szPostilionEmvData+6, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_EMV_FIX_BIN :
         if ( GetPostilionEmvFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szPostilionEmvData, data, nBytes);
         break;

      case POSTILION_EMV_LL_BIN :
         sprintf ( szPostilionEmvFldLen, "%.2d", nLength);
         nBytes = 2;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
         memcpy( szPostilionEmvData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_LLL_BIN :
         sprintf ( szPostilionEmvFldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
         memcpy( szPostilionEmvData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_LLLL_BIN :
         sprintf ( szPostilionEmvFldLen, "%.4d", nLength);
         nBytes = 4;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
         memcpy( szPostilionEmvData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_EMV_LLLLLL_BIN :
         sprintf ( szPostilionEmvFldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionEmvData, szPostilionEmvFldLen, nBytes);
         memcpy( szPostilionEmvData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_POSTILION_EMV_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionEmvData, nBytes);
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
      return(InsertPostilionEmvField(field_no, msgInfo, data, nLength));
}

int PostilionEmvBuildBuff (char * buffer_snd, TSPostilionEmvInfo *msgInfo)
{
   char szMsgType[5];
   int  iMapCtr;
   char nConvBytes;
   char HexBitMap[16 + 1];
   char sLine[MAX_LINE_TRC];

   memset(HexBitMap, 0, sizeof(HexBitMap));
   iMapCtr = 2;
   nConvBytes = HexToAscii(msgInfo->sBitMap,HexBitMap,16);
   sprintf(sLine,"=========1>HexBitMap(%s)(%d)", HexBitMap,nConvBytes);
  trace_event(sLine,PROCESSING); 
   memcpy( buffer_snd, HexBitMap, iMapCtr*8);
   memcpy( buffer_snd + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( (iMapCtr*8) + msgInfo->nLength);
}

int PostilionEmvBuildHeader(char*   pNewHeader,int sLen , TSPostilionEmvInfo   *PostilionEmvInfo)
{

	char            FieldData[LG_MAX];
	int             FieldLen;

	sprintf(pNewHeader, "%04d",sLen + 15 );
	memcpy(pNewHeader + 4,"ISO60000",8);
	
	return(SUCCESS);

}
