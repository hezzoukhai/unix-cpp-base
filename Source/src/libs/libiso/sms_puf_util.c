/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      sms_puf_util.c                                                        */
/*                                                                             */
/* Description                                                                 */
/*      SMS PUF  Tools Management                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include <sms_puf.h>
#include <tools.h>
#include <define.h>

int GetSmsPufSubField(int field_no, TSSmsPufInfo *pufInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (pufInfo->sBitMap, field_no))
   {
      switch (GetSmsPufFieldType(field_no))
      {
      case SMS_PUF_FIX_BIN :
         length = GetSmsPufFieldLength(field_no);
         memcpy( data, pufInfo->sData + pufInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case SMS_PUF_FIX_HEX :                           
         length = GetSmsPufFieldLength(field_no);
         nBytes = HexToAscii((unsigned char *)pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_PUF_FIX_BCD :                           
         length = GetSmsPufFieldLength(field_no);
         nBytes = BcdToAscii((unsigned char *)pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_PUF_VAR_BCD :                           
         length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii((unsigned char *)pufInfo->sData+pufInfo->nFieldPos[field_no]+1, 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_PUF_FIX_ALPHA :                           
         length = GetSmsPufFieldLength(field_no);
         EbcdicToAscii((unsigned char *)pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            (unsigned char *)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_PUF_VAR_ALPHA :                           
         length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
         EbcdicToAscii((unsigned char *)pufInfo->sData+pufInfo->nFieldPos[field_no]+1, 
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

int AddSmsPufSubField(int field_no, TSSmsPufInfo *pufInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char sLine[LG_MAX];


   if ( CheckBit( pufInfo->sBitMap, field_no))
      return(ECHEC);

   pufInfo->nFieldPos[field_no] = pufInfo->nLength;
   switch (GetSmsPufFieldType(field_no))
   {
   case SMS_PUF_FIX_BIN :
      nBytes = GetSmsPufFieldLength(field_no);
      memcpy( pufInfo->sData+pufInfo->nLength, data, nBytes);
      break;

   case SMS_PUF_FIX_BCD :
      if ( GetSmsPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd((unsigned char *) data,(unsigned char *) pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case SMS_PUF_FIX_HEX :
      if ( GetSmsPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex((unsigned char *) data,(unsigned char *) pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case SMS_PUF_FIX_ALPHA :
      if ( GetSmsPufFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic((unsigned char *) data,(unsigned char *) pufInfo->sData+pufInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case SMS_PUF_VAR_BCD :
      *(pufInfo->sData + pufInfo->nLength) = nLength;
      pufInfo->nLength++;
      nBytes = AsciiToBcd((unsigned char *) data, (unsigned char *)pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case SMS_PUF_VAR_ALPHA :
      *(pufInfo->sData + pufInfo->nLength) = nLength;
      pufInfo->nLength++;
      AsciiToEbcdic((unsigned char *)data,(unsigned char *) pufInfo->sData+pufInfo->nLength, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   pufInfo->nLength += nBytes;
   SetBit(pufInfo->sBitMap, field_no+1);
   return(SUCCES);
}

int SmsPufBuildField (char * buffer_puf, TSSmsPufInfo *pufInfo)
{
   char sLine[LG_MAX];

   memcpy( buffer_puf, pufInfo->sBitMap, SMS_PUF_BITMAP_LEN);
   memcpy( buffer_puf + SMS_PUF_BITMAP_LEN , pufInfo->sData, pufInfo->nLength);
   return( SMS_PUF_BITMAP_LEN + pufInfo->nLength);
}

