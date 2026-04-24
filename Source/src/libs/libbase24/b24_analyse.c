/*#include <base24.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <tools.h>
#include <tlv_switch_header.h>
#include <base24/iso_base24.h>
#include <base24/base24_define.h>

int   AnalyseBase24    (unsigned char *buffer_rec, TSBase24Info *msgInfo)
{
    int     i, nDigits = 0;
    int     iMapCtr = 0;
    int     nBytes  = 0;
    int     nRcvHdrLen = 0;
    char    szMsgType[5];
    char    szField[256];
    int     nType;
    int     nBase24Product;

    unsigned char *sPtr;


    memset(szMsgType,'\0',sizeof(szMsgType));

    InitBase24Info(msgInfo);
    sPtr = buffer_rec + 3; /* skip "ISO" */

    nRcvHdrLen = BASE24_HEADER_LEN ;
    memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
    sPtr += nRcvHdrLen;

    memcpy(szMsgType, sPtr, 4);
    msgInfo->nMsgType = atoi(szMsgType);
    sPtr += 4;

	iMapCtr = 0;
	do
	{
		AsciiToHex(sPtr,msgInfo->sBitMap + (iMapCtr*8), 16); sPtr+=16;
		++iMapCtr;
	} while(( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 2));

    memcpy (msgInfo->sData, sPtr, MAX_BASE24_DATA_LEN);

    nBase24Product = GetBase24Product(msgInfo->sHeader.sProductIndicator);
	
    if( nBase24Product !=  B24_PRODUCT_IND_ATM_T && 
		nBase24Product !=  B24_PRODUCT_IND_POS_T && 
		nBase24Product !=  B24_PRODUCT_IND_BASE_T &&
		nBase24Product !=  B24_PRODUCT_IND_HOST_T)
    {
        trace_message(ERROR, "Unknown Base24 product indicator: %.2s ", msgInfo->sHeader.sProductIndicator);
        return NOK;
    }
    msgInfo->nProductIndicator=nBase24Product;

    for (i = 0; i < iMapCtr * 64; ++i)
    {
        if ( CheckBit( msgInfo->sBitMap, i))
        {

            memset(szField,'\0',sizeof(szField));
            switch ( GetBase24FieldType(i,nBase24Product) )
            {
            case BASE24_FIX_BCD :
                nDigits = GetBase24FieldLength(i,nBase24Product);
                nBytes = (nDigits + 1)/2;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_FIX_HEX :
                nDigits = GetBase24FieldLength(i,nBase24Product);
                nBytes = (nDigits +1)/2;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_FIX_ALPHA :
                nBytes = GetBase24FieldLength(i,nBase24Product);
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_FIX_BIN :
            case BASE24_FIX_BITMAP :
                nBytes = GetBase24FieldLength(i,nBase24Product);
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_VAR_HEX :
                nDigits = *sPtr;
                nBytes = (nDigits + 1)/2 +1;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_VAR_BCD :
                nDigits = *sPtr;
                nBytes = (nDigits +1)/2 +1;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_VAR_SBCD :
                nDigits = *sPtr;
                nBytes = nDigits+1;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_VAR_ALPHA:
                nBytes = *sPtr;
                nBytes++;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

            case BASE24_VAR_BIN    :
            case BASE24_VAR_BITMAP :
                nBytes = *sPtr;
                nBytes++;
                msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
                break;

			case BASE24_LL_ALPHA :
            case BASE24_LL_BIN :
				memcpy (szField, sPtr ,2);
				nBytes  = atoi(szField);
				nBytes +=2;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BASE24_LL_HEX :
				memcpy (szField, sPtr ,2);
				nDigits = atoi(szField);
				nBytes = (nDigits + 1)/2 + 2;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BASE24_LLL_ALPHA :
            case BASE24_LLL_BIN :
				memcpy (szField, sPtr ,3);
				nBytes  = atoi(szField);
				nBytes +=3;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BASE24_LLL_HEX :
				memcpy (szField, sPtr ,3);
				nDigits = atoi(szField);
				nBytes = ( nDigits + 1)/2 +3;
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
   for (i = iMapCtr*64; i< MAX_BASE24_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
   return ( OK ) ;
}

