#include <stdio.h>
#include <stdlib.h>
#include <iso_postilion.h>
#include <iso_postilion_f127.h>
#include <iso_postilion_Emv.h>
#include <tools.h>

int   AnalysePostilion    (char *buffer_rec, TSPostilionInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 8;
   int  nBytes  = 8;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szPostilionMsgType[5];	
   char *sPtr;
   char *sDataPtr;
   char szPostilionFldLen[7];
   char sLine[MAX_LINE_TRC];
   TSPostilionF127Info msgF127Info;




/**    trace_event("Start AnalysePostilion()",PROCESSING); **/

   InitPostilionInfo(msgInfo);

   sPtr = buffer_rec ;

   /*sPtr += 7;*/

   memcpy(msgInfo->sHeader, sPtr, nRcvHdrLen);
   sPtr += nRcvHdrLen;

   memcpy(szPostilionMsgType,sPtr,4); 
   szPostilionMsgType[4]='\0';	
   msgInfo->nMsgType = atoi(szPostilionMsgType);
   sPtr += 4;

   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy (msgInfo->sData, sPtr, MAX_POSTILION_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetPostilionFieldType(i) )
         {

         case POSTILION_FIX_BIN   :
         case POSTILION_FIX_ALPHA :
            nBytes = GetPostilionFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_FIX_HEX :
            nDigits = GetPostilionFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LL_HEX :
            memcpy (szPostilionFldLen, sPtr ,2);
            nDigits = atoi(szPostilionFldLen);
            nBytes = (nDigits + 1)/2 + 2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLL_HEX :
            memcpy (szPostilionFldLen, sPtr ,3);
            nDigits = atoi(szPostilionFldLen);
            nBytes = ( nDigits + 1)/2 +3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLLLLL_HEX :
            memcpy (szPostilionFldLen, sPtr ,6);
            nDigits = atoi(szPostilionFldLen);
            nBytes = ( nDigits + 1)/2 +6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LL_BIN :
            memcpy (szPostilionFldLen, sPtr ,2);
	    szPostilionFldLen[2]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLL_BIN :
            memcpy (szPostilionFldLen, sPtr ,3);
	    szPostilionFldLen[3]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLLLLL_BIN :
            memcpy (szPostilionFldLen, sPtr ,6);
	    szPostilionFldLen[6]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LL_ALPHA :
            memcpy (szPostilionFldLen, sPtr ,2);
	    szPostilionFldLen[2]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLL_ALPHA :
            memcpy (szPostilionFldLen, sPtr ,3);
	    szPostilionFldLen[3]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_LLLLLL_ALPHA :
            memcpy (szPostilionFldLen, sPtr ,6);
	    szPostilionFldLen[6]=0;
            nBytes  = atoi(szPostilionFldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         default :
            nBytes =0;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
            break;

         }
         sPtr += nBytes;
      }
      else
         msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
   }
   msgInfo->nLength = msgInfo->nFieldPos[iMapCtr*64];
   for (i = iMapCtr*64; i< MAX_POSTILION_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];

   /** trace_event("End AnalysePostilion()",PROCESSING); **/

   return(OK);

}

/** *********************** **/
/** Postilion Field 127 Mng **/
/** *********************** **/
int   AnalysePostilionF127    (char *buffer_rec, TSPostilionF127Info *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 8;
   int  nBytes  = 8;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szPostilionF127MsgType[5];
   char *sPtr;
   char *sDataPtr;
   char szPostilionF127FldLen[7];
   char sLine[MAX_LINE_TRC];



   /** trace_event("Start AnalysePostilionF127()",PROCESSING); **/

   InitPostilionF127Info(msgInfo);
   sPtr = buffer_rec ;


   memcpy( msgInfo->sBitMap, sPtr, 8);
   sPtr += 8;
 
   /*memcpy (msgInfo->sData, sPtr, MAX_POSTILION_F127_DATA_LEN); SWI09012012 insure*/
   memcpy (msgInfo->sData, sPtr, MAX_POSTILION_F127_DATA_LEN - 1);

   for (i = 0; i < MAX_POSTILION_F127_FIELDS; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetPostilionF127FieldType(i) )
         {

         case POSTILION_F127_FIX_BIN   :
         case POSTILION_F127_FIX_ALPHA :
            nBytes = GetPostilionF127FieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_FIX_HEX :
            nDigits = GetPostilionF127FieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LL_HEX :
            memcpy (szPostilionF127FldLen, sPtr ,2);
            nDigits = atoi(szPostilionF127FldLen);
            nBytes = (nDigits + 1)/2 + 2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLL_HEX :
            memcpy (szPostilionF127FldLen, sPtr ,3);
            nDigits = atoi(szPostilionF127FldLen);
            nBytes = ( nDigits + 1)/2 +3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLLL_HEX :
            memcpy (szPostilionF127FldLen, sPtr ,4);
            nDigits = atoi(szPostilionF127FldLen);
            nBytes = ( nDigits + 1)/2 +4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLLLLL_HEX :
            memcpy (szPostilionF127FldLen, sPtr ,6);
            nDigits = atoi(szPostilionF127FldLen);
            nBytes = ( nDigits + 1)/2 +6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LL_BIN :
            memcpy (szPostilionF127FldLen, sPtr ,2);
            szPostilionF127FldLen[2]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLL_BIN :
            memcpy (szPostilionF127FldLen, sPtr ,3);
            szPostilionF127FldLen[3]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLLL_BIN :
            memcpy (szPostilionF127FldLen, sPtr ,4);
            szPostilionF127FldLen[4]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLLLLL_BIN :
            memcpy (szPostilionF127FldLen, sPtr ,6);
            szPostilionF127FldLen[6]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LL_ALPHA :
            memcpy (szPostilionF127FldLen, sPtr ,2);
            szPostilionF127FldLen[2]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLL_ALPHA :
            memcpy (szPostilionF127FldLen, sPtr ,3);
            szPostilionF127FldLen[3]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_F127_LLLL_ALPHA :
            memcpy (szPostilionF127FldLen, sPtr ,4);
            szPostilionF127FldLen[4]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;
            


         case POSTILION_F127_LLLLL_ALPHA :
            memcpy (szPostilionF127FldLen, sPtr ,5);
            szPostilionF127FldLen[5]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=5;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;



         case POSTILION_F127_LLLLLL_ALPHA :
            memcpy (szPostilionF127FldLen, sPtr ,6);
            szPostilionF127FldLen[6]=0;
            nBytes  = atoi(szPostilionF127FldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         default :
            nBytes =0;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
            break;

         }
         sPtr += nBytes;
      }
      else
         msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
   }
/**
   msgInfo->nLength = msgInfo->nFieldPos[iMapCtr*64];
   for (i = iMapCtr*64; i< MAX_POSTILION_F127_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
**/

   /** trace_event("End AnalysePostilionF127()",PROCESSING); **/

   return(OK);
}

/** *********************** **/
/** Postilion Emv Field Mng **/
/** *********************** **/
int   AnalysePostilionEmv    (char *buffer_rec, TSPostilionEmvInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 8;
   int  nBytes  = 8;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szPostilionEmvMsgType[5];
   char *sPtr;
   char *sDataPtr;
   char szPostilionEmvFldLen[7];
   char sLine[MAX_LINE_TRC];
   char nConvBytes;
   char HexBitMap[16 + 1];


  
    memset(HexBitMap, 0, sizeof(HexBitMap));
   InitPostilionEmvInfo(msgInfo);
   sPtr = buffer_rec ;

   nConvBytes = AsciiToHex(sPtr, HexBitMap, 16);
   memcpy( msgInfo->sBitMap,HexBitMap,8);
   sPtr += 16;
   
  sprintf(sLine,"=========2>HexBitMap(%s)(%d)", HexBitMap,nConvBytes);
  trace_event(sLine,PROCESSING); 
   memcpy (msgInfo->sData, sPtr, MAX_POSTILION_EMV_DATA_LEN);

   for (i = 0; i < MAX_POSTILION_EMV_FIELDS; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
        
 switch ( GetPostilionEmvFieldType(i) )
         {

         case POSTILION_EMV_FIX_BIN   :
         case POSTILION_EMV_FIX_ALPHA :
            nBytes = GetPostilionEmvFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_FIX_HEX :
            nDigits = GetPostilionEmvFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LL_HEX :
            memcpy (szPostilionEmvFldLen, sPtr ,2);
            nDigits = atoi(szPostilionEmvFldLen);
            nBytes = (nDigits + 1)/2 + 2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLL_HEX :
            memcpy (szPostilionEmvFldLen, sPtr ,3);
            nDigits = atoi(szPostilionEmvFldLen);
            nBytes = ( nDigits + 1)/2 +3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLLL_HEX :
            memcpy (szPostilionEmvFldLen, sPtr ,4);
            nDigits = atoi(szPostilionEmvFldLen);
            nBytes = ( nDigits + 1)/2 +4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLLLLL_HEX :
            memcpy (szPostilionEmvFldLen, sPtr ,6);
            nDigits = atoi(szPostilionEmvFldLen);
            nBytes = ( nDigits + 1)/2 +6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LL_BIN :
            memcpy (szPostilionEmvFldLen, sPtr ,2);
            szPostilionEmvFldLen[2]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLL_BIN :
            memcpy (szPostilionEmvFldLen, sPtr ,3);
            szPostilionEmvFldLen[3]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLLL_BIN :
            memcpy (szPostilionEmvFldLen, sPtr ,4);
            szPostilionEmvFldLen[4]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLLLLL_BIN :
            memcpy (szPostilionEmvFldLen, sPtr ,6);
            szPostilionEmvFldLen[6]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;
      
/********ASE24122011******/ 
        case POSTILION_EMV_L_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,1);
            szPostilionEmvFldLen[1]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=1;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

/********ASE24122011******/

         case POSTILION_EMV_LL_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,2);
            szPostilionEmvFldLen[2]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLL_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,3);
            szPostilionEmvFldLen[3]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=3;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POSTILION_EMV_LLLL_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,4);
            szPostilionEmvFldLen[4]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=4;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;
            


         case POSTILION_EMV_LLLLL_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,5);
            szPostilionEmvFldLen[5]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=5;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;



         case POSTILION_EMV_LLLLLL_ALPHA :
            memcpy (szPostilionEmvFldLen, sPtr ,6);
            szPostilionEmvFldLen[6]=0;
            nBytes  = atoi(szPostilionEmvFldLen);
            nBytes +=6;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         default :
            nBytes =0;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
            break;

         }
         sPtr += nBytes;
      }
      else
         msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
   }


   return(OK);
}
