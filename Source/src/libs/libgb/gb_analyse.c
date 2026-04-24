#include <stdio.h>
#include <stdlib.h>
#include <iso_gb.h>
#include <tools.h>

#define LG_GB_HDR 62

int AnalyseGB(char *buffer_rec, TSGBInfo *msgInfo)
{
   int i, nDigits = 0;
   int iMapCtr = 0;
   int nBytes = 0;
   int nFldType, nFldLength;
   int nRcvHdrLen = 0;
   char szGBMsgType[4 + 1];
   char szGBFldLen[5];
   char szAscFldLen[5];
   char sLine[MAX_LINE_TRC];
   char *sPtr;



   InitGBInfo(msgInfo);
   sPtr = buffer_rec + LG_GB_HDR;

   memset(szGBMsgType, '\0', sizeof(szGBMsgType));

   HexToAscii(sPtr, szGBMsgType, 4);
   msgInfo->nMsgType = atoi(szGBMsgType);
   sPtr += 2;

   iMapCtr = 1;
   while ((sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2))
      ++iMapCtr;
   memcpy(msgInfo->sBitMap, sPtr, iMapCtr * 8);
   sPtr += (iMapCtr * 8);

   memcpy(msgInfo->sData, sPtr, MAX_GB_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
		memset(szGBFldLen, '\0', sizeof(szGBFldLen));
		memset(szAscFldLen, '\0', sizeof(szAscFldLen));

      if (CheckBit(msgInfo->sBitMap, i))
      {
         nFldType = GetGBFieldType(i);
         nFldLength = GetGBFieldLength(i);

         switch (nFldType)
         {
         case GB_FIX_BIN:
         case GB_FIX_ALPHA:
				nBytes = (nFldLength + 1 / 2);
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LLL_HEX:
            memcpy(szGBFldLen, sPtr, 3);
            EbcdicToAscii(szGBFldLen, szAscFldLen, 3);
            nDigits = atoi(szAscFldLen);
            nBytes = (nDigits + 1) / 2 + 3;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LL_ALPHA_N:
            memcpy(szGBFldLen, sPtr, 1);
            BcdToAscii(szGBFldLen, szAscFldLen, 2);
            nBytes = atoi(szAscFldLen) / 2;
            nBytes += 1;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LL_ALPHA:
            memcpy(szGBFldLen, sPtr, 2);
            BcdToAscii(szGBFldLen, szAscFldLen, 4);
            nBytes = atoi(szAscFldLen);
            nBytes += 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LLL_ALPHA:
            memcpy(szGBFldLen, sPtr, 3);
            EbcdicToAscii(szGBFldLen, szAscFldLen, 3);
            nBytes = atoi(szAscFldLen);
            nBytes += 3;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LLLL_ALPHA:
            memcpy(szGBFldLen, sPtr, 2);
            BcdToAscii(szGBFldLen, szAscFldLen, 4);
            nBytes = atoi(szAscFldLen);
            nBytes += 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         default:
            nBytes = 0;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i];
            break;
         }
         sPtr += nBytes;
      }
      else
         msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i];
   }
   msgInfo->nLength = msgInfo->nFieldPos[iMapCtr * 64];
   for (i = iMapCtr * 64; i < MAX_GB_FIELDS; i++)
      msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[iMapCtr * 64];
}

int AnalyseGBField63(char *buffer_rec, TSGBInfo *msgInfo)
{
   char sWork[256];
   char *sPtr;
   int i, j = 0;
   int nLen = 0;
   int nDigits = 0;
   int iMapCtr = 0;
   int nBytes = 0;
   int nFldType, nFldLength;
   char szGBFldLen[5];
   char szAscFldLen[5];
   char sLine[MAX_LINE_TRC];


   sPtr = buffer_rec;

   sPtr += 1;
   memset(sWork, 0, sizeof(sWork));
   memcpy(sWork, sPtr, 2);
	nLen = ((unsigned char)sWork[0]) * 256 + ((unsigned char)sWork[1]);
   sPtr += 2;

   iMapCtr = 1;
   while ((sPtr[(iMapCtr - 1) + 2] & 0x80) && (iMapCtr < 3))
      ++iMapCtr;
   memcpy(msgInfo->sBitMap, sPtr, 2 + iMapCtr);
   sPtr += 2 + iMapCtr;

   sprintf(sLine, "       _ FLD 63 BIT MAP   : ");
   for (i = 0; i < 2 + iMapCtr; ++i)
   {
      sprintf(sLine + 28 + (i * 3), "%02X ", msgInfo->sBitMap[i]);
   }
   trace_event(sLine, TRACE);

   memcpy(msgInfo->sData, sPtr, nLen - (2 + iMapCtr));

   for (i = 1; i < 16 + (iMapCtr * 8); ++i)
   {
		memset(szGBFldLen, '\0', sizeof(szGBFldLen));
		memset(szAscFldLen, '\0', sizeof(szAscFldLen));

      if (CheckBit(msgInfo->sBitMap, i) && (i != 16 && i != 24))
      {
         nFldType = GetGBField63Type(i);
         nFldLength = GetGBField63Length(i);

         switch (nFldType)
         {
         case GB_FIX_BIN:
         case GB_FIX_ALPHA:
				nBytes = (nFldLength + 1 ) / 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_FIX_ALPHA_N:
				nBytes = (nFldLength + 1 ) / 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LL_ALPHA_N:
            memcpy(szGBFldLen, sPtr, 1);
            BcdToAscii(szGBFldLen, szAscFldLen, 2);
            nBytes = atoi(szAscFldLen) / 2;
            nBytes += 1;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LL_ALPHA:
            memcpy(szGBFldLen, sPtr, 2);
            BcdToAscii(szGBFldLen, szAscFldLen, 4);
            nBytes = atoi(szAscFldLen);
            nBytes += 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_L_ALPHA:
            memcpy(szGBFldLen, sPtr, 1);
            BcdToAscii(szGBFldLen, szAscFldLen, 2);
            nBytes = atoi(szAscFldLen);
            nBytes += 1;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LLL_ALPHA:
            memcpy(szGBFldLen, sPtr, 3);
            EbcdicToAscii(szGBFldLen, szAscFldLen, 3);
            nBytes = atoi(szAscFldLen);
            nBytes += 3;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         case GB_LLLL_ALPHA:
            memcpy(szGBFldLen, sPtr, 2);
            BcdToAscii(szGBFldLen, szAscFldLen, 4);
            nBytes = atoi(szAscFldLen);
            nBytes += 2;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i] + nBytes;
            break;
         default:
            nBytes = 0;
            msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i];
            break;
         }
         sPtr += nBytes;
      }
      else
         msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[i];
   }

   msgInfo->nLength = msgInfo->nFieldPos[16 + (iMapCtr * 8)];

   for (i = 16 + (iMapCtr * 8); i < MAX_GB_FIELDS; i++)
      msgInfo->nFieldPos[i + 1] = msgInfo->nFieldPos[16 + (iMapCtr * 8)];

   return (SUCCES);
}

