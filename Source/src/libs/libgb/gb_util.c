#include <stdio.h>
#include <stdlib.h>

#include <iso_gb.h>
#include <header_gb.h>
#include <tools.h>
#include <define.h>


int GetGBField(int field_no, TSGBInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szIsoFldLen[5];
   char szAscFldLen[5];
   char szGBFldLen[5];
   int nFieldMaxLength;
   char sLine[MAX_LINE_TRC];

   memset(szIsoFldLen, '\0', sizeof(szIsoFldLen));
   memset(szAscFldLen, '\0', sizeof(szAscFldLen));
   memset(szGBFldLen, '\0', sizeof(szGBFldLen));

   nFieldMaxLength = GetGBFieldLength(field_no);

   if (CheckBit(msgInfo->sBitMap, field_no))
   {
		
      switch (GetGBFieldType(field_no))
      {
      case GB_FIX_BIN:
			length = nFieldMaxLength;
			memcpy( data, msgInfo->sData+msgInfo->nFieldPos[field_no], length);
			*nLength = length;
			break;
      case GB_FIX_ALPHA:
			/*EBETODO: Bcd to ascii for Alpha?? */
         length = nFieldMaxLength;
			BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no], data, length);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LL_ALPHA_N:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 1);
         BcdToAscii(szGBFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);

         if (length > nFieldMaxLength)
         {
            sprintf(sLine, "Invalid length (%d)(Max:%d) for field %d", length, nFieldMaxLength, field_no);
            trace_event(sLine, ERROR);
            return ECHEC;
         }

         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 1, data, length);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_L_ALPHA:
			/*EBETODO: quelle diff entre GB_LL_ALPHA_N et GB_L_ALPHA_N?? */
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 1);
         BcdToAscii(szGBFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);

         if (length > nFieldMaxLength)
         {
            sprintf(sLine, "Invalid length (%d)(Max:%d) for field %d", length, nFieldMaxLength, field_no);
            trace_event(sLine, ERROR);
            return ECHEC;
         }

         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LL_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         BcdToAscii(szGBFldLen, szAscFldLen, 4);
         length = atoi(szAscFldLen);

         if (length > nFieldMaxLength)
         {
            sprintf(sLine, "Invalid length (%d)(Max:%d) for field %d", length, nFieldMaxLength, field_no);
            trace_event(sLine, ERROR);
            return ECHEC;
         }

         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 2, data, length * 2);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LLLL_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         BcdToAscii(szGBFldLen, szAscFldLen, 4);
         length = atoi(szAscFldLen);

         if (length > nFieldMaxLength)
         {
            sprintf(sLine, "Invalid length (%d)(Max:%d) for field %d", length, nFieldMaxLength, field_no);
            trace_event(sLine, ERROR);
            return ECHEC;
         }

         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no] + 2, length);
         data[length] = '\0';
         *nLength = length;
         break;

      default:
         return (ECHEC);
         break;
      }
      return (SUCCES);
   }
   return (ECHEC);
}

int AddGBField(int field_no, TSGBInfo *msgInfo, char *data, int nLength , int nCompress)
{
   int i, nBytes = 0;
   char szGbFldLen[4 + 1];
   char szEbcFldLen[4 + 1];
   char sLine[MAX_LINE_TRC];
   int nFieldMaxLength;

	memset(szGbFldLen, '\0', sizeof(szGbFldLen));

   if (CheckBit(msgInfo->sBitMap, field_no))
      return (ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;

   nFieldMaxLength = GetGBFieldLength(field_no);
   if (nLength > nFieldMaxLength)
   {
      sprintf(sLine, "Add: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
      trace_event(sLine, ERROR);
      return ECHEC;
   }

   switch (GetGBFieldType(field_no))
   {
   case GB_FIX_ALPHA:
      if (GetGBFieldLength(field_no) != nLength)
         return (ECHEC);
      /*AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);*/ /** HMA - 20052013 **/
		/*EBETODO: ascii to hex? alors que dans le get on fait bcd??*/
      if (nCompress == 1)
      {
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_L_ALPHA:
      /*szGbFldLen[0] = nLength;
        msgInfo->sData[msgInfo->nLength] = szGbFldLen[0];
        msgInfo->nLength += 1;*/

      sprintf(szGbFldLen, "%.2d", nLength);
		AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
		msgInfo->nLength +=1;
      memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case GB_LL_ALPHA_N:
      if (nCompress == 1)
      {
         sprintf(szGbFldLen, "%.2d", nLength);
			AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
			msgInfo->nLength += 1;
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         sprintf(szGbFldLen, "%.2d", nLength);
         memcpy(msgInfo->sData + msgInfo->nLength, szGbFldLen, 2);
         msgInfo->nLength += 2;
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_LL_ALPHA:
      if (nCompress == 1)
      {
			sprintf(szGbFldLen, "%.2d",(nLength +1/ 2));
			AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
			msgInfo->nLength += 1;
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         sprintf(szGbFldLen, "%.2d", nLength);
         memcpy(msgInfo->sData + msgInfo->nLength, szGbFldLen, 2);
         msgInfo->nLength += 2;
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_LLL_ALPHA:
      if (nCompress == 1)
      {
			sprintf(szGbFldLen, "%.3d", (nLength +1/ 2));
			AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 3);
			msgInfo->nLength += 2;
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         sprintf(szGbFldLen, "%.3d", nLength);
         memcpy(msgInfo->sData + msgInfo->nLength, szGbFldLen, 3);
         msgInfo->nLength += 3;
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_LLLL_ALPHA:
      sprintf(szGbFldLen, "%.4d", 1 + 2 + nLength);
		AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 4);
		msgInfo->nLength += 2;
      msgInfo->sData[msgInfo->nLength] = 0x71;
      msgInfo->nLength += 1;
      memset(szGbFldLen, 0, sizeof(szGbFldLen));
      memset(szGbFldLen, '0', 4);
      sprintf(szGbFldLen + 2, "%X", nLength);
      msgInfo->nLength += AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 4);
      memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default:
      return (ECHEC);
   }

   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no + 1);

   if ((field_no > 64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return (SUCCES);
}

int InsertGBField(int field_no, TSGBInfo *msgInfo, char *data, int nLength)
{

   int nFldType, nFldLength;
   int nBytes = 0;
   int i;
   char szGBData[999];
   char sLine[MAX_LINE_TRC];
   char szGbFldLen[4 + 1];
   char szEbcFldLen[4 + 1];
   int nFieldMaxLength;

	memset(szGbFldLen, '\0', sizeof(szGbFldLen));
	memset(szEbcFldLen, '\0', sizeof(szEbcFldLen));

   if (CheckBit(msgInfo->sBitMap, field_no))
      return (ECHEC);

   memset(szGBData, 0, sizeof(szGBData));

   nFldType = GetGBFieldType(field_no);
   nFldLength = GetGBFieldLength(field_no);

   nFieldMaxLength = GetGBFieldLength(field_no);

   if (nLength > nFieldMaxLength)
   {
      sprintf(sLine, "Ins: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
      trace_event(sLine, ERROR);
      return ECHEC;
   }
   switch (nFldType)
   {

   case GB_FIX_ALPHA:
      if (nFldLength != nLength)
		{
			sprintf(sLine, "Ins: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
			trace_event(sLine, ERROR);
         return (ECHEC);
		}
		AsciiToEbcdic(data, szGBData, nLength); /*EBETODO: ebcdic? */
      nBytes = nLength;
      break;

   case GB_LL_ALPHA:
      sprintf(szGbFldLen, "%.2d", nLength);
      AsciiToEbcdic(szGbFldLen, szEbcFldLen, 2);
      memcpy(szGBData, szEbcFldLen, 2);
      nBytes = 2;
      AsciiToEbcdic(data, szGBData + 2, nLength);
      nBytes += nLength;
      break;

   case GB_LLL_ALPHA:
      sprintf(szGbFldLen, "%.3d", nLength);
      nBytes = 3;
      AsciiToEbcdic(szGbFldLen, szEbcFldLen, 3);
      memcpy(szGBData, szEbcFldLen, 3);
      AsciiToEbcdic(data, szGBData + 3, nLength);
      nBytes += nLength;
      break;

   case GB_LLLL_ALPHA:
      sprintf(szGbFldLen, "%.4d", nLength);
      nBytes = 4;
      AsciiToEbcdic(szGbFldLen, szEbcFldLen, 4);
      memcpy(szGBData, szEbcFldLen, 4);
      AsciiToEbcdic(data, szGBData + 4, nLength);
      nBytes += nLength;
      break;

   default:
      return (ECHEC);
   }
   for (i = MAX_GB_FIELDS - 1; i > field_no; i--)
   {
      memcpy(msgInfo->sData + msgInfo->nFieldPos[i] + nBytes,
             msgInfo->sData + msgInfo->nFieldPos[i],
             msgInfo->nFieldPos[i + 1] - msgInfo->nFieldPos[i]);
      msgInfo->nFieldPos[i + 1] += nBytes;
   }
   memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no], szGBData, nBytes);
   msgInfo->nFieldPos[field_no + 1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no + 1);
   if ((field_no > 64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return (SUCCES);
}

int PutGBField(int field_no, TSGBInfo *msgInfo, char *data, int nLength)
{
   int nFldType, nFldLength;
   int nBytes = 0;
   int nBytes1 = 0;
   int i, nOldFldLen, nOldLen;
   char szGBData[999];
   char szWork[LG_MAX];
   char szIsoFldLen[4];
   char szGbFldLen[4 + 1];
   char szEbcFldLen[4 + 1];
   int nFieldMaxLength;
   char sLine[MAX_LINE_TRC];

	memset(szGbFldLen, '\0', sizeof(szGbFldLen));
	memset(szEbcFldLen, '\0', sizeof(szEbcFldLen));

   nFieldMaxLength = GetGBFieldLength(field_no);

   if (nLength > nFieldMaxLength)
   {
      sprintf(sLine, "Put: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
      trace_event(sLine, ERROR);
      return ECHEC;
   }

   if (CheckBit(msgInfo->sBitMap, field_no))
   {
      nFldType = GetGBFieldType(field_no);
      nFldLength = GetGBFieldLength(field_no);

      switch (nFldType)
      {
      case GB_FIX_ALPHA:
         if (nFldLength != nLength)
			{
				sprintf(sLine, "Put: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
				trace_event(sLine, ERROR);
            return (ECHEC);
			}
         AsciiToEbcdic(data, szGBData, nLength);
         nBytes = nLength;
         break;

      case GB_LL_ALPHA:
         sprintf(szGbFldLen, "%.2d", nLength);
         AsciiToEbcdic(szGbFldLen, szEbcFldLen, 2);
         memcpy(szGBData, szEbcFldLen, 2);
         nBytes = 2;
         AsciiToEbcdic(data, szGBData + 2, nLength);
         nBytes += nLength;
         break;

      case GB_LLL_ALPHA:
         sprintf(szGbFldLen, "%.3d", nLength);
         nBytes = 3;
         AsciiToEbcdic(szGbFldLen, szEbcFldLen, 3);
         memcpy(szGBData, szEbcFldLen, 3);
         AsciiToEbcdic(data, szGBData + 3, nLength);
         nBytes += nLength;
         break;

      case GB_LLLL_ALPHA:
         sprintf(szGbFldLen, "%.4d", nLength);
         nBytes = 4;
         AsciiToEbcdic(szGbFldLen, szEbcFldLen, 4);
         memcpy(szGBData, szEbcFldLen, 4);
         AsciiToEbcdic(data, szGBData + 4, nLength);
         nBytes += nLength;
         break;

      default:
         return (ECHEC);
      }

      nOldFldLen = msgInfo->nFieldPos[field_no + 1] -
                   msgInfo->nFieldPos[field_no];
      memset(szWork, 0, sizeof(szWork));
      nOldLen = msgInfo->nLength - msgInfo->nFieldPos[field_no + 1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no + 1], nOldLen);

      for (i = MAX_GB_FIELDS - 1; i > field_no; i--)
         msgInfo->nFieldPos[i + 1] += nBytes - nOldFldLen;

      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no], szGBData, nBytes);
      msgInfo->nFieldPos[field_no + 1] += nBytes - nOldFldLen;
      msgInfo->nLength += nBytes - nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no + 1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ((field_no > 64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return (SUCCES);
   }
   else
      return (InsertGBField(field_no, msgInfo, data, nLength));
}

int GBBuildMsg(char *buffer_snd, TSGBInfo *msgInfo)
{


   char     szMsgType   [5];

   int      iMapCtr;

   memset(szMsgType   ,'\0',sizeof(szMsgType   ));
   
   
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   memcpy( buffer_snd, szMsgType, 4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);

   return( 4 + (iMapCtr*8) + msgInfo->nLength);
}

int GBBuildFld63(char *buffer_snd, TSGBInfo *msgInfo)
{
   SetBit(msgInfo->sBitMap, 1);
   memcpy(buffer_snd, msgInfo->sBitMap, 3);
   memcpy(buffer_snd + 3, msgInfo->sData, msgInfo->nLength);
   return (3 + msgInfo->nLength);
}

int GetGBSubField63(int field_no, TSGBInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   char sLine[MAX_LINE_TRC];

   char szGBFldLen[5];
   char szAscFldLen[5];
   char sWork[256];

   memset(szGBFldLen, '\0', sizeof(szGBFldLen));
   memset(szAscFldLen, '\0', sizeof(szAscFldLen));
   memset(sWork, '\0', sizeof(sWork));

	length = GetGBField63Length(field_no);
   if (CheckBit(msgInfo->sBitMap, field_no))
   {
      switch (GetGBField63Type(field_no))
      {
      case GB_FIX_ALPHA:
			length = (length + 1) / 2;
         /*BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no], data, length);*/
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_FIX_ALPHA_N:
         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no], data, length);
         /*memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);*/
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_L_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 1);
         BcdToAscii(szGBFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         /*BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 1, data, length);*/
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no] + 1, length);
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LL_ALPHA_N:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 1);
         BcdToAscii(szGBFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 1, data, length);
         /*memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+1, length);*/
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LL_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         length = atoi(szGBFldLen);
         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 2, data, length);
         /*memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length);*/
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LLL_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
         length = atoi(szGBFldLen);
         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 3, data, length);
         /*memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length);*/
         data[length] = '\0';
         *nLength = length;
         break;
      case GB_LLLL_ALPHA:
         memcpy(szGBFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 4);
         length = atoi(szGBFldLen);
         BcdToAscii(msgInfo->sData + msgInfo->nFieldPos[field_no] + 4, data, length);
         /*memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+4, length);*/
         data[length] = '\0';
         *nLength = length;
         break;

      default:
         return (ECHEC);
         break;
      }

      return (SUCCES);
   }

   return (ECHEC);
}

int AddGBSubField63(int field_no, TSGBInfo *msgInfo, char *data, int nLength, int nCompress)
{
   int i, nBytes = 0;
   char sLine[MAX_LINE_TRC];
   char szGbFldLen[4 + 1];
   char szEbcFldLen[4 + 1];
	int nFieldMaxLength;

	memset(szGbFldLen, '\0', sizeof(szGbFldLen));
	memset(szEbcFldLen, '\0', sizeof(szEbcFldLen));

	nFieldMaxLength = GetGBField63InLength(field_no);

	if (nLength > nFieldMaxLength)
	{
		sprintf(sLine, "Add: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
		trace_event(sLine, ERROR);
		return ECHEC;
	}

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;

   switch (GetGBField63InType(field_no))
   {

   case GB_FIX_HEX:
      if (GetGBField63InLength(field_no) != nLength)
		{
			sprintf(sLine, "Add: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
			trace_event(sLine, ERROR);
         return (ECHEC);
		}
      nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      break;
   case GB_FIX_ALPHA:
      if (GetGBField63InLength(field_no) != nLength)
		{
			sprintf(sLine, "Add: Invalid length (%d)(Max:%d) for field %d", nLength, nFieldMaxLength, field_no);
			trace_event(sLine, ERROR);
         return (ECHEC);
		}
      if (nCompress == 1)
      {
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_L_ALPHA:
      sprintf(szGbFldLen, "%.2d", nLength);
		AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
		msgInfo->nLength += 1;
      memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case GB_LL_ALPHA_N:
      if (nCompress == 1)
      {
         sprintf(szGbFldLen, "%.2d", nLength);
			AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
			msgInfo->nLength += 1;
         nBytes = AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
      }
      else
      {
         sprintf(szGbFldLen, "%.2d", nLength);
         memcpy(msgInfo->sData + msgInfo->nLength, szGbFldLen, 2);
         msgInfo->nLength += 2;
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   case GB_LL_ALPHA:
      if (nCompress == 1)
      {
			sprintf(szGbFldLen, "%.2d", (nLength + 1/ 2));
			AsciiToHex(szGbFldLen, msgInfo->sData + msgInfo->nLength, 2);
			msgInfo->nLength += 1;
         AsciiToHex(data, msgInfo->sData + msgInfo->nLength, nLength);
			nBytes = (nLength + 1/ 2);
      }
      else
      {
         sprintf(szGbFldLen, "%.2d", nLength);
         memcpy(msgInfo->sData + msgInfo->nLength, szGbFldLen, 2);
         msgInfo->nLength += 2;
         memcpy(msgInfo->sData + msgInfo->nLength, data, nLength);
         nBytes = nLength;
      }
      break;

   default:
      return (ECHEC);
   }

   msgInfo->nLength += nBytes;
   for (i = field_no; i < MAX_GB_FIELDS; i++)
      msgInfo->nFieldPos[i + 1] = msgInfo->nLength;

   SetBit(msgInfo->sBitMap, field_no + 1);

   if (field_no > 64)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);

   return (SUCCES);
}

void GBBuildHeader(int nReqResp,
                   TSGBHeader *pNewHeader,
                   TSGBHeader *pIncomingHeader,
                   char *sMerchantId,
                   char *sAcquirerId,
                   char *sTerminalId,
                   char *nGBMsgLength)
{

   trace_event("Start of GBBuildHeader", PROCESSING);

   if (REQUEST == nReqResp)
   {
      memset((char *)pNewHeader, 0, sizeof(TSGBHeader));
      memset(pNewHeader->aMerchantId, ' ', 15);
      memset(pNewHeader->aAcquirerId, ' ', 11);
      memset(pNewHeader->aTerminalId, ' ', 8);

   }
   else
	{
   memcpy((char *)pNewHeader, (char *)pIncomingHeader, sizeof(TSGBHeader));
	}

   memcpy(pNewHeader->aProcotolVersion, "203", 3);
   memcpy(pNewHeader->aRoutingKey, "000000", 6);
   memcpy(pNewHeader->aMerchantId, sMerchantId, strlen(sMerchantId));
   memcpy(pNewHeader->aSiteId, "0000000", 7);

   memcpy(pNewHeader->aAcquirerId, "3103       ", 11); 
	memcpy(pNewHeader->aCurCode, "MAD", 3);					/*EBETODO: hardcoding*/
   memcpy(pNewHeader->aTerminalId, sTerminalId, 8);
   memcpy(pNewHeader->aStatus, "0000", 4);

   sprintf(pNewHeader->aMsgLength, "%05d", atoi(nGBMsgLength)); 


   trace_event("End of GBBuildHeader()", PROCESSING);
}

