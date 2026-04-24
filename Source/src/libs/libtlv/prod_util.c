#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tlv_prod.h>
#include <define.h>

int GetTlvProd (char  *tlv_name, TSTlvProd *tlvInfo, char *data , int *length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));

   if ( (nIndice = GetTlvProdIndice(tlv_name)) == -1)
      return(ECHEC);
   nTlvType    = GetTlvProdType(nIndice) ;
   nTlvLength  = GetTlvProdLength(nIndice); 

   if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   memcpy ( szAscTlvLen, 
            tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,
            3);
   *length = atoi(szAscTlvLen);

   switch ( nTlvType )
   {
   case PROD_TLV_FIX :
   case PROD_TLV_VAR :
      memcpy( data,
              tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
              *length);
      data[*length] = '\0';
      return(SUCCES);
      break;

   }

   return(ECHEC);
}

int AddTlvProd (char  *tlv_name, TSTlvProd *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);

   if ( length == 0 )
      return(ECHEC);

   if ( (nIndice = GetTlvProdIndice(tlv_name)) == -1)
      return(ECHEC);

   nTlvType    = GetTlvProdType(nIndice) ;
   nTlvLength  = GetTlvProdLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   switch ( nTlvType )
   {
   case PROD_TLV_FIX :
   case PROD_TLV_VAR :
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

int PutTlvProd (char  *tlv_name, TSTlvProd *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   char szAscTlvLen [ 4 ];
   char szOldTlvLen [ 4 ];
   char szLastPart  [ MAX_PROD_TLV_LEN ];
   int nTlvType;

   memset (szAscTlvLen, '\0', 4);
   memset (szOldTlvLen, '\0', 4);

   if ( length == 0 )
      return(ECHEC);

   if ( (nIndice = GetTlvProdIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }

   nTlvType    = GetTlvProdType(nIndice) ;
   nTlvLength  = GetTlvProdLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   memset (szLastPart,  '\0', sizeof(szLastPart));
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {
      switch ( nTlvType )
      {
      case PROD_TLV_FIX :
      case PROD_TLV_VAR :
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
         AdjustTlvProd( tlvInfo, nIndice+1, length-nOldTlvLen);

         break;

      }
      return(SUCCES);
   }
   
   switch ( nTlvType )
   {
   case PROD_TLV_FIX :
   case PROD_TLV_VAR :
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

int ProdBuildTlv (char * buffer_snd, TSTlvProd *tlvInfo)
{
    char sLLLL [ 4 + 1 ];

    memset (  sLLLL  , 0 ,  sizeof ( sLLLL ));
    sprintf(  sLLLL  , "%04d" , tlvInfo->nLength );

    memcpy ( buffer_snd + 0     , sLLLL          , 4 );
    memcpy ( buffer_snd + 4     , tlvInfo->sTlvData , tlvInfo->nLength);
   
  return(tlvInfo->nLength + 4 );
}

/****************************************************************************/
/****************************************************************************/

int  PutProdTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length)
{
    TSTlvProd tlvInfo;
    int  retour;
    char sLine[ MAX_LINE_TRC ];

    sprintf(sLine, "TAG : %s Length = %d, .%.4s", tlv_name, length, tlvbuffer);
    trace_event(sLine, ERROR);
    InitTlvProd(&tlvInfo);

    AnalyseTlvProd( tlvbuffer,  &tlvInfo );
    retour = PutTlvProd( tlv_name , &tlvInfo , data      , length );

    if ( retour != SUCCES ) return ( ECHEC );

    retour = ProdBuildTlv  ( tlvbuffer, &tlvInfo);
    return ( SUCCES );
}


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

int  GetProdTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int *length)
{
    TSTlvProd tlvInfo;
    
	InitTlvProd(&tlvInfo);

    if ( strlen ( tlvbuffer ) < 4 ) 
    {
       trace_event ( "PutTlvBuffer ECHEC LENGTH " , PROCESSING );
       return ( ECHEC );
    }

    AnalyseTlvProd( tlvbuffer , &tlvInfo );
    return ( GetTlvProd( tlv_name , &tlvInfo , data      , length ));
}
