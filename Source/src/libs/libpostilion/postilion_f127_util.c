#include <stdio.h>
#include <stdlib.h>

#include <iso_postilion.h>
#include <iso_postilion_f127.h>
#include <postilion_fields.h>
#include <tools.h>
#include <define.h>


int GetPostilionF127Field(int field_no, TSPostilionF127Info *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szPostilionF127FldLen[7];
   char sLine[MAX_LINE_TRC];

/**
   sprintf(sLine,"Start GetPostilionF127Field(%d)", field_no);
   trace_event(sLine,PROCESSING);
**/

   memset( szPostilionF127FldLen, '\0', sizeof(szPostilionF127FldLen));

   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionF127FieldType(field_no))
      {
      case POSTILION_F127_LL_BIN :
           memcpy( szPostilionF127FldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionF127FldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
           *nLength = length;
           break;

      case POSTILION_F127_LLL_BIN :
           memcpy( szPostilionF127FldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
           length = atoi(szPostilionF127FldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
           *nLength = length;
           break;

      case POSTILION_F127_LLLL_BIN :
           memcpy( szPostilionF127FldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 4);
           length = atoi(szPostilionF127FldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+4, length);
           *nLength = length;
           break;

      case POSTILION_F127_LLLLLL_BIN :
           memcpy( szPostilionF127FldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 6);
           length = atoi(szPostilionF127FldLen);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+6, length);
           *nLength = length;
           break;

      case POSTILION_F127_FIX_BIN   :
      case POSTILION_F127_FIX_ALPHA :
           length = GetPostilionF127FieldLength(field_no);
           memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
           data[length] = '\0';
/***
           sprintf(sLine, "==> msgInfo->nFieldPos[%d]=[%d]", field_no+1, msgInfo->nFieldPos[field_no]);
           trace_event(sLine, PROCESSING);
***/
           *nLength = length;
           break;

      case POSTILION_F127_FIX_HEX :
           length = GetPostilionF127FieldLength(field_no);
           nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no],
                                                           data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_F127_LL_HEX :
           memcpy( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
           length = atoi(szPostilionF127FldLen);
           nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2,
                                                              data, length);
           data[length] = '\0';
           *nLength = length;
           break;

      case POSTILION_F127_LLL_HEX :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionF127FldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_F127_LLLL_HEX :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 4);
         length = atoi(szPostilionF127FldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+4, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_F127_LLLLLL_HEX :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionF127FldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+6, data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_F127_LL_ALPHA :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szPostilionF127FldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_F127_LLL_ALPHA :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szPostilionF127FldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case POSTILION_F127_LLLL_ALPHA :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 4);
         length = atoi(szPostilionF127FldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+4, length);
         data[length] = '\0';
         *nLength = length;
         break;
/* MC 310507 */
      case POSTILION_F127_LLLLL_ALPHA :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 5);
         length = atoi(szPostilionF127FldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+5, length);
         data[length] = '\0';
         *nLength = length;
         break;
/* MC 310507 */
      case POSTILION_F127_LLLLLL_ALPHA :
         memcpy ( szPostilionF127FldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 6);
         length = atoi(szPostilionF127FldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+6, length);
         data[length] = '\0';
         *nLength = length;
         break;



      default :
         return(ECHEC);
         break;

      }
/**
      trace_event("End GetPostilionF127Field(SUCCES)");
**/
      return(SUCCES);
   }
/**
   trace_event("End GetPostilionF127Field(ECHEC)");
**/
   return(ECHEC);

}

int AddPostilionF127Field(int field_no, TSPostilionF127Info *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char szPostilionF127FldLen[7];
   char  sLine[MAX_LINE_TRC];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetPostilionF127FieldType(field_no))
   {
   case POSTILION_F127_FIX_ALPHA :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LL_ALPHA :
   sprintf(sLine,"Start GetPostilionF127Field(%d)", field_no);
   trace_event(sLine,PROCESSING);
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 2);
      msgInfo->nLength += 2;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 3);
      msgInfo->nLength += 3;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 4);
      msgInfo->nLength += 4;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

/* MC 310507 */

   case POSTILION_F127_LLLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.5d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 5);
      msgInfo->nLength += 5;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

/* MC 310507 */
      
   case POSTILION_F127_LLLLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 6);
      msgInfo->nLength += 6;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_FIX_HEX :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_F127_LL_HEX :
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_F127_LLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_F127_LLLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 4);
      msgInfo->nLength += 4;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_F127_LLLLLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 6);
      msgInfo->nLength += 6;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case POSTILION_F127_FIX_BIN :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LL_BIN :
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LLLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 4);
      msgInfo->nLength += 4;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case POSTILION_F127_LLLLLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      memcpy( msgInfo->sData+msgInfo->nLength, szPostilionF127FldLen, 6);
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

int InsertPostilionF127Field(int field_no, TSPostilionF127Info *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i;
   char szPostilionF127FldLen[7];
   char szPostilionF127Data [260];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetPostilionF127FieldType(field_no))
   {
     case POSTILION_F127_FIX_ALPHA :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy (szPostilionF127Data, data, nBytes);
      break;

   case POSTILION_F127_LL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      memcpy( szPostilionF127Data,        szPostilionF127FldLen, 2);
      nBytes=2;
      memcpy( szPostilionF127Data+2, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_LLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 3);
      memcpy( szPostilionF127Data+3, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_LLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      nBytes = 4;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 4);
      memcpy( szPostilionF127Data+4, data,        nLength);
      nBytes += nLength;
      break;

/* MC 310507 */

	 case POSTILION_F127_LLLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.5d", nLength);
      nBytes = 5;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 5);
      memcpy( szPostilionF127Data+5, data,        nLength);
      nBytes += nLength;
      break;

/* MC 310507 */
      
   case POSTILION_F127_LLLLLL_ALPHA :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 6);
      memcpy( szPostilionF127Data+6, data,        nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_FIX_HEX :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szPostilionF127Data, nLength);
      break;

   case POSTILION_F127_LL_HEX :
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      nBytes1 = 2;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes1);
      nBytes = AsciiToHex( data, szPostilionF127Data+2, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_F127_LLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      nBytes1 = 3;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 3);
      nBytes = AsciiToHex( data, szPostilionF127Data+3, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_F127_LLLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      nBytes1 = 4;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 4);
      nBytes = AsciiToHex( data, szPostilionF127Data+4, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_F127_LLLLLL_HEX :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      nBytes1 = 6;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, 6);
      nBytes = AsciiToHex( data, szPostilionF127Data+6, nLength);
      nBytes += nBytes1;
      break;

   case POSTILION_F127_FIX_BIN :
      if ( GetPostilionF127FieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szPostilionF127Data, data, nBytes);
      break;

   case POSTILION_F127_LL_BIN :
      sprintf ( szPostilionF127FldLen, "%.2d", nLength);
      nBytes = 2;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
      memcpy( szPostilionF127Data+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_LLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.3d", nLength);
      nBytes = 3;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
      memcpy( szPostilionF127Data+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_LLLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.4d", nLength);
      nBytes = 4;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
      memcpy( szPostilionF127Data+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case POSTILION_F127_LLLLLL_BIN :
      sprintf ( szPostilionF127FldLen, "%.6d", nLength);
      nBytes = 6;
      memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
      memcpy( szPostilionF127Data+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_POSTILION_F127_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionF127Data, nBytes);
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutPostilionF127Field(int field_no, TSPostilionF127Info *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int  nBytes1  = 0;
   int i, nOldFldLen;
   char szPostilionF127Data [260];
   char szPostilionF127FldLen[7];

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetPostilionF127FieldType(field_no))
      {
      case POSTILION_F127_FIX_ALPHA :
         if ( GetPostilionF127FieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy (szPostilionF127Data, data, nBytes);
         break;

      case POSTILION_F127_LL_ALPHA :
         sprintf ( szPostilionF127FldLen, "%.2d", nLength);
         memcpy( szPostilionF127Data,        szPostilionF127FldLen, 2);
         nBytes=2;
         memcpy( szPostilionF127Data+2, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_LLL_ALPHA :
         sprintf ( szPostilionF127FldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 3);
         memcpy( szPostilionF127Data+3, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_LLLL_ALPHA :
         sprintf ( szPostilionF127FldLen, "%.4d", nLength);
         nBytes = 4;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 4);
         memcpy( szPostilionF127Data+4, data,        nLength);
         nBytes += nLength;
         break;

/* MC 310507 */
		  case POSTILION_F127_LLLLL_ALPHA :
         sprintf ( szPostilionF127FldLen, "%.5d", nLength);
         nBytes = 5;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 5);
         memcpy( szPostilionF127Data+5, data,        nLength);
         nBytes += nLength;
         break;

/* MC 310507 */
         
      case POSTILION_F127_LLLLLL_ALPHA :
         sprintf ( szPostilionF127FldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 6);
         memcpy( szPostilionF127Data+6, data,        nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_FIX_HEX :
         if ( GetPostilionF127FieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szPostilionF127Data, nLength);
         break;

      case POSTILION_F127_LL_HEX :
         sprintf ( szPostilionF127FldLen, "%.2d", nLength);
         nBytes1 = 2;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes1);
         nBytes = AsciiToHex( data, szPostilionF127Data+2, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_F127_LLL_HEX :
         sprintf ( szPostilionF127FldLen, "%.3d", nLength);
         nBytes1 = 3;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 3);
         nBytes = AsciiToHex( data, szPostilionF127Data+3, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_F127_LLLL_HEX :
         sprintf ( szPostilionF127FldLen, "%.4d", nLength);
         nBytes1 = 4;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 4);
         nBytes = AsciiToHex( data, szPostilionF127Data+4, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_F127_LLLLLL_HEX :
         sprintf ( szPostilionF127FldLen, "%.6d", nLength);
         nBytes1 = 6;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, 6);
         nBytes = AsciiToHex( data, szPostilionF127Data+6, nLength);
         nBytes += nBytes1;
         break;

      case POSTILION_F127_FIX_BIN :
         if ( GetPostilionF127FieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szPostilionF127Data, data, nBytes);
         break;

      case POSTILION_F127_LL_BIN :
         sprintf ( szPostilionF127FldLen, "%.2d", nLength);
         nBytes = 2;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
         memcpy( szPostilionF127Data+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_LLL_BIN :
         sprintf ( szPostilionF127FldLen, "%.3d", nLength);
         nBytes = 3;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
         memcpy( szPostilionF127Data+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_LLLL_BIN :
         sprintf ( szPostilionF127FldLen, "%.4d", nLength);
         nBytes = 4;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
         memcpy( szPostilionF127Data+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case POSTILION_F127_LLLLLL_BIN :
         sprintf ( szPostilionF127FldLen, "%.6d", nLength);
         nBytes = 6;
         memcpy( szPostilionF127Data, szPostilionF127FldLen, nBytes);
         memcpy( szPostilionF127Data+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_POSTILION_F127_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPostilionF127Data, nBytes);
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
      return(InsertPostilionF127Field(field_no, msgInfo, data, nLength));
}

int PostilionF127BuildBuff (char * buffer_snd, TSPostilionF127Info *msgInfo)
{
   char szMsgType[5];
   int  iMapCtr;


   iMapCtr = 1;
   memcpy( buffer_snd, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( (iMapCtr*8) + msgInfo->nLength);
}

int PostilionF127BuildHeader(char*   pNewHeader,int sLen , TSPostilionF127Info   *PostilionF127Info)
{

	char            FieldData[LG_MAX];
	int             FieldLen;

	sprintf(pNewHeader, "%04d",sLen + 15 );
	memcpy(pNewHeader + 4,"ISO60000",8);
	
	return(SUCCESS);

}
