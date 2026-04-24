#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tlv_emea.h>
#include <define.h>

int GetTlvEmea (char  *tlv_name, TSTlvEmea *tlvInfo, char *data, int *length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);

   if ( (nIndice = GetTlvEmeaIndice(tlv_name)) == -1)
      return(ECHEC);
   nTlvType    = GetTlvEmeaType(nIndice) ;
   nTlvLength  = GetTlvEmeaLength(nIndice); 

   if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   memcpy ( szAscTlvLen, 
            tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,
            3);
   *length = atoi(szAscTlvLen);

   switch ( nTlvType )
   {
   case EMEA_TLV_FIX :
   case EMEA_TLV_VAR :
      memcpy( data,
              tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
              *length);
      break;

   }
   return(SUCCES);
}

int AddTlvEmea (char  *tlv_name, TSTlvEmea *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);

   if ( length == 0 )
      return(ECHEC);

   if ( (nIndice = GetTlvEmeaIndice(tlv_name)) == -1)
      return(ECHEC);

   nTlvType    = GetTlvEmeaType(nIndice) ;
   nTlvLength  = GetTlvEmeaLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   switch ( nTlvType )
   {
   case EMEA_TLV_FIX :
   case EMEA_TLV_VAR :
      memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, 
              data, 
              length);
      break;

   }

   memcpy (tlvInfo->sTlvData + tlvInfo->nLength    , tlv_name,    3);
   memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
   tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
   tlvInfo->nPresent[ nIndice ] = PRESENT;
   tlvInfo->nLength +=length + 3 + 3;
   return(SUCCES);
}

int PutTlvEmea (char  *tlv_name, TSTlvEmea *tlvInfo, char *in_data, int length)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   char data [ LG_MAX ];
   char szAscTlvLen [ 4 ];
   char szOldTlvLen [ 4 ];
   char szLastPart  [ MAX_EMEA_TLV_LEN ];
   char sLine  [ MAX_LINE_TRC ];
   int nTlvType;


   memset (data , 0 , LG_MAX );
   memcpy (data , in_data , length );

   sprintf(sLine, "PutTlvEmea [%s] [%s] [%d]",tlv_name,data,length);
   trace_event(sLine, ERROR);
   memset (szAscTlvLen, '\0', 4);
   memset (szOldTlvLen, '\0', 4);

   if ( length == 0 )
      return(ECHEC);

   if ( (nIndice = GetTlvEmeaIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }
/*
   sprintf(sLine, "PutTlvEmea indice : %d, Present = %d", nIndice, tlvInfo->nPresent[nIndice]);
   trace_event(sLine, ERROR);
   sprintf(sLine, "TlvLen     : %d ", tlvInfo->nLength);
   trace_event(sLine, ERROR);
   dump_buffer(tlvInfo->sTlvData, tlvInfo->nLength, 'A', 'X', 'X'); 
*/

   nTlvType    = GetTlvEmeaType(nIndice) ;
   nTlvLength  = GetTlvEmeaLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   memset (szLastPart,  '\0', sizeof(szLastPart));
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {
      switch ( nTlvType )
      {
      case EMEA_TLV_FIX :
      case EMEA_TLV_VAR :
         /********Sauvegarder le reste du message ****/
         memcpy( szOldTlvLen, 
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
                 3);
         nOldTlvLen = atoi(szOldTlvLen);


         memcpy( szLastPart,
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 + nOldTlvLen, 
                 tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]- 6 - nOldTlvLen);

        /**************Formater La nouvelle valeur du TAG ***/
         memcpy (tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
                 szAscTlvLen, 
                 3);
         memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
                 data, 
                 length);

        /*************Restaurer le reste du message ****/
         memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 +length, 
                 szLastPart, 
                 tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]-6 - nOldTlvLen);

         /***********Mettre a jour la structure *******/
         tlvInfo->nLength += length - nOldTlvLen;
         AdjustTlvEmea( tlvInfo, nIndice+1, length-nOldTlvLen);

         break;

      }
      return(SUCCES);
   }
   
   switch ( nTlvType )
   {
   case EMEA_TLV_FIX :
   case EMEA_TLV_VAR :
      memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, 
              data, 
              length);
      break;

   }

   memcpy (tlvInfo->sTlvData + tlvInfo->nLength    , tlv_name,    3);
   memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
   tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
   tlvInfo->nPresent[ nIndice ] = PRESENT;
   tlvInfo->nLength +=length + 3 + 3;
   return(SUCCES);
}

int EmeaBuildTlv (char * buffer_snd, TSTlvEmea *tlvInfo)
{
    char sLLLL [ 4 + 1 ];

    memset (  sLLLL  , 0 ,  sizeof ( sLLLL ));
    sprintf(  sLLLL  , "%04d" , tlvInfo->nLength );

    memcpy ( buffer_snd + 0     , sLLLL          , 4 );
    memcpy ( buffer_snd + 4     , tlvInfo->sTlvData , tlvInfo->nLength);
   
  return(tlvInfo->nLength + 4 );
}


