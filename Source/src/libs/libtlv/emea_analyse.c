#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <define.h>
#include <tlv_emea.h>

int AnalyseTlvEmea(char *buffer, TSTlvEmea *tlvInfo)
{
   char  *sPtr;
   int   offset;
   int   nTlvLength, nBytes;
   int   nTlvIndice;
   int   nLenTlv; 
   char  szTlvName [ 5 ];
   char  szTlvValueLen [ 5 ];
   int   nTlvValueLen ;
   char  sLLLL [ 4 + 1];
   int   nLLLL;

   memset ( sLLLL ,          0 , sizeof ( sLLLL ) );
   if ( strlen ( buffer ) < 4 ) 
   {
       trace_event ( "AnalyseTlvEmea ECHEC LENGTH " , PROCESSING );
       return ( ECHEC );
   }

   InitTlvEmea(tlvInfo);
   memcpy ( sLLLL , buffer 	 , 4 );
   nLLLL   = atoi ( sLLLL );

   offset = 0;
   tlvInfo->nLength = nLLLL;
   memcpy (tlvInfo->sTlvData,  buffer + 4 ,  nLLLL);

   sPtr = buffer+4;

   while (offset < nLLLL)
   {
      memset( szTlvName,     '0', 4);
      memset( szTlvValueLen,  0,  4);
      memcpy( szTlvName,     sPtr,       3);
      memcpy( szTlvValueLen, sPtr+3,     3);

      nTlvValueLen = atoi(szTlvValueLen);
      nTlvIndice   = GetTlvEmeaIndice( szTlvName); 

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


