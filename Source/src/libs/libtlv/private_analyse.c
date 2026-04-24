#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <define.h>
#include <tlv_private.h>

int AnalyseTlvPrivate(char *buffer, int nLength, TSTlvPrivate *tlvInfo)
{
   char  *sPtr;
   int   offset;
   int   nTlvLength, nBytes;
   int   nTlvIndice;
   int   nLenTlv; 
   char  szTlvName [ 5 ];
   char  szTlvValueLen [ 5 ];
   int   nTlvValueLen ;

   InitTlvPrivate(tlvInfo);

   offset = 0;
   tlvInfo->nLength = nLength;
   memcpy (tlvInfo->sTlvData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szTlvName,     '0', 4);
      memset( szTlvValueLen,  0,  4);
      memcpy( szTlvName,     sPtr,       3);
      memcpy( szTlvValueLen, sPtr+3,     3);

      nTlvValueLen = atoi(szTlvValueLen);
      nTlvIndice   = GetTlvPrivateIndice( szTlvName); 

      if ( nTlvIndice >= 0)
      {
         tlvInfo->nPosTlv [ nTlvIndice ] = offset;
         tlvInfo->nPresent[ nTlvIndice ] = PRESENT;
      }

      nTlvLength =  3 + 3 +  nTlvValueLen;
      offset += nTlvLength;
      sPtr   += nTlvLength;
   }

   /* EBE121214: sPtr has moved to the end. we should reset buffer instead
   memset (sPtr, 0, nLength); *//**PA DSS*/
   memset (buffer, 0, nLength);

    /*SKO290509*/ return(OK);
}

