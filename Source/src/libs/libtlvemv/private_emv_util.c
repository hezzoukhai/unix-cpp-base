/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      private_emv_util.c                                                     */
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

#include <tlv_private_emv.h>
#include <define.h>
#include <tlv_fields.h>

int GetTlvPrivateEmv (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int *length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);

   if ( (nIndice = GetTlvPrivateEmvIndice(tlv_name)) == -1)
      return(ECHEC);
   nTlvType    = GetTlvPrivateEmvType(nIndice) ;
   nTlvLength  = GetTlvPrivateEmvLength(nIndice); 

   if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   memcpy ( szAscTlvLen, 
            tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,
            3);
   *length = atoi(szAscTlvLen);

   switch ( nTlvType )
   {
   case PRIVATE_TLV_FIX :
   case PRIVATE_TLV_VAR :
      memcpy( data,
              tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
              *length);
      break;

   }
   return(SUCCES);
}

int AddTlvPrivateEmv (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);

   if ( (nIndice = GetTlvPrivateEmvIndice(tlv_name)) == -1)
      return(ECHEC);
   nTlvType    = GetTlvPrivateEmvType(nIndice) ;
   nTlvLength  = GetTlvPrivateEmvLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   switch ( nTlvType )
   {
   case PRIVATE_TLV_FIX :
   case PRIVATE_TLV_VAR :
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

int PutTlvPrivateEmv (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   char szAscTlvLen [ 4 ];
   char szOldTlvLen [ 4 ];
   char szLastPart  [ MAX_PRIVATE_TLV_LEN ];
   int nTlvType;
   
   
   memset (szAscTlvLen, '\0', 4);
   memset (szOldTlvLen, '\0', 4);

   if ( (nIndice = GetTlvPrivateEmvIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }
   nTlvType    = GetTlvPrivateEmvType(nIndice) ;
   nTlvLength  = GetTlvPrivateEmvLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   memset (szLastPart,  '\0', sizeof(szLastPart));
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {
      switch ( nTlvType )
      {
      case PRIVATE_TLV_FIX :
      case PRIVATE_TLV_VAR :
         /********Sauvegarder le reste du message ****/
         memcpy( szOldTlvLen, 
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
                 3);
         nOldTlvLen = atoi(szOldTlvLen);
         memcpy( szLastPart,
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 +nOldTlvLen, 
                 tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]-6 - nOldTlvLen);

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
         AdjustTlvPrivateEmv( tlvInfo, nIndice+1, length-nOldTlvLen);

         break;

      }
      trace_event("End   PutTlvPrivateEmv (SUCCES)",PROCESSING);
      return(SUCCES);
   }
   
   switch ( nTlvType )
   {
   case PRIVATE_TLV_FIX :
   case PRIVATE_TLV_VAR :
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

int PrivateBuildTlvEmv (char * buffer_snd, TSTlvPrivate *tlvInfo)
{
   memcpy (buffer_snd, tlvInfo->sTlvData, tlvInfo->nLength);
   return(tlvInfo->nLength);
}

