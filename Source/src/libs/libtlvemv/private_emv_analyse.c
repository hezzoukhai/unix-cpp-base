/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      private_emv_analyse.c                                                  */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   24/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <define.h>
#include <tlv_private_emv.h>

int AnalyseTlvPrivateEmv(char *buffer, int nLength, TSTlvPrivate *tlvInfo)
{
   char  *sPtr;
   int   offset;
   int   nTlvLength, nBytes;
   int   nTlvIndice;
   int   nLenTlv; 
   char  szTlvName [ 5 ];
   char  szTlvValueLen [ 5 ];
   int   nTlvValueLen ;

   InitTlvPrivateEmv(tlvInfo);

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
      nTlvIndice   = GetTlvPrivateEmvIndice( szTlvName); 

      if ( nTlvIndice >= 0)
      {
         tlvInfo->nPosTlv [ nTlvIndice ] = offset;
         tlvInfo->nPresent[ nTlvIndice ] = PRESENT;
      }

      nTlvLength =  3 + 3 +  nTlvValueLen;
      offset += nTlvLength;
      sPtr   += nTlvLength;
   }
    /*SKO290509*/ return(OK);
}

