#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tlv_autho.h>
#include <define.h>
#include <dump.h>

int DelTlvAutho (char  *tlv_name, TSTlvAutho *tlvInfo)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   char szAscTlvLen [ 4 ];
   char szOldTlvLen [ 4 ];
   char szLastPart  [ MAX_AUTHO_TLV_LEN ];
   int nTlvType,i;

   memset (szAscTlvLen, '\0', 4);
   memset (szOldTlvLen, '\0', 4);

   if ( (nIndice = GetTlvAuthoIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }

   nTlvType    = GetTlvAuthoType(nIndice) ;
   nTlvLength  = GetTlvAuthoLength(nIndice); 

   memset (szLastPart,  0, sizeof(szLastPart));
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {
      switch ( nTlvType )
      {
      case AUTHO_TLV_FIX :
      case AUTHO_TLV_VAR :
         /********Sauvegarder le reste du message ****/
         memcpy( szOldTlvLen, 
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 
                 3);
         nOldTlvLen = atoi(szOldTlvLen);
         memcpy( szLastPart,
                 tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 6 + nOldTlvLen, 
                 tlvInfo->nLength - tlvInfo->nPosTlv[nIndice] - 6 - nOldTlvLen);

        /*************Restaurer le reste du message ****/
         memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] , 
                 szLastPart, 
                 tlvInfo->nLength - tlvInfo->nPosTlv[nIndice] - 6 - nOldTlvLen);

         /***********Mettre a jour la structure *******/
		 nOldTlvLen += 6;
         tlvInfo->nLength -= nOldTlvLen;
   		 tlvInfo->nPresent[ nIndice ] = NOT_PRESENT;
   		 for (i = nIndice + 1; i < MAX_AUTHO_TLV; i++)
   		{
      		if ( tlvInfo->nPresent[i] == PRESENT)
         	tlvInfo->nPosTlv [i]  -= nOldTlvLen;
   		}
         break;

      }
      return(SUCCES);
   }
   else
      return(SUCCES);
}

int GetTlvAutho (char  *tlv_name, TSTlvAutho *tlvInfo, char *data , int *length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int nTlvType;
   /*char sLine [LG_MAX];*/

   memset (szAscTlvLen, '\0', 4);

   if ( (nIndice = GetTlvAuthoIndice(tlv_name)) == -1)
      return(ECHEC);
   nTlvType    = GetTlvAuthoType(nIndice) ;
   nTlvLength  = GetTlvAuthoLength(nIndice); 

   if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   memcpy ( szAscTlvLen, 
            tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,
            3);
   *length = atoi(szAscTlvLen);

/*
   sprintf (sLine, "szAscTlvLen :%d",atoi(szAscTlvLen));
   trace_event(sLine,PROCESSING);
*/

   switch ( nTlvType )
   {
   case AUTHO_TLV_FIX :
   case AUTHO_TLV_VAR :
      memcpy( data,
              tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
              *length);
      data[*length] = '\0';
      return(SUCCES);
      break;

   }

   /*memset (sLine,  0, sizeof(sLine));*/ /**PA DSS*/

   return(ECHEC);
}

int AddTlvAutho (char  *tlv_name, TSTlvAutho *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   char szAscTlvLen [4];
   int  nLen; 
   int nTlvType;
   char sLine[MAX_LINE_TRC];


   memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
   if ( length == 0 )
      return(ECHEC);

   /*--AER06102018 Add check on the length*/
	if(strlen(data) == 0 || strlen(data) < length)/*IBO20220803 PLUTONL-5012*/
    { 
		sprintf ( sLine ,"End AddTlvAutho(ERROR)(Field-> %s,Length-> %d ,Data length-> %lu )", tlv_name,length,strlen(data));
		trace_event ( sLine,ERROR );
		return(ECHEC); 
    }
    /*--AER06102018*/
   
   if ( (nIndice = GetTlvAuthoIndice(tlv_name)) == -1)
      return(ECHEC);

   nTlvType    = GetTlvAuthoType(nIndice) ;
   nTlvLength  = GetTlvAuthoLength(nIndice); 

   nLen = strlen(data);
   nTlvLength = length;

   if (nLen < length)
      nTlvLength = nLen;

   if ( nLen == 0 )
      return(ECHEC);

   sprintf ( szAscTlvLen, "%.3d", nTlvLength);
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   switch ( nTlvType )
   {
   case AUTHO_TLV_FIX :
   case AUTHO_TLV_VAR :
      memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, 
              data, 
              nTlvLength);
      break;

   }

   memcpy (tlvInfo->sTlvData + tlvInfo->nLength    , tlv_name,    3);
   memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
   tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
   tlvInfo->nPresent[ nIndice ] = PRESENT;
   tlvInfo->nLength +=nTlvLength + 3 + 3;
   return(SUCCES);
}

int PutTlvAutho (char  *tlv_name, TSTlvAutho *tlvInfo, char *data, int length)
{
   int nIndice;
   int nTlvLength;
   int nOldTlvLen;
   int nTlvType;

   char szAscTlvLen [ 4 + 1 ];
   char szOldTlvLen [ 4  + 1];
   char szLastPart  [ MAX_AUTHO_TLV_LEN ];
   char sLine[MAX_LINE_TRC];

   memset (szAscTlvLen, 0, 4);
   memset (szOldTlvLen, 0, 4);

   if ( length == 0 )
      return(ECHEC);

   /*--AER06102018 Add check on the length*/
	if(strlen(data) == 0 || strlen(data) < length)/*IBO20220803 PLUTONL-5012*/
    { 
		sprintf ( sLine ,"End PutTlvAutho(ERROR)(Field-> %s,Length-> %d ,Data length-> %lu )", tlv_name,length,strlen(data));
		trace_event ( sLine,ERROR );
		return(ECHEC); 
    }
    /*--AER06102018*/


   if ( (nIndice = GetTlvAuthoIndice(tlv_name)) == -1)
   {
      return(ECHEC);
   }

   nTlvType    = GetTlvAuthoType(nIndice) ;
   nTlvLength  = GetTlvAuthoLength(nIndice); 
   sprintf ( szAscTlvLen, "%.3d", length);
   memset (szLastPart,  0, sizeof(szLastPart));

   /*TODO: control length VS strlen()*/
   
   if (tlvInfo->nPresent[nIndice] == PRESENT)
   {

      switch ( nTlvType )
      {
      case AUTHO_TLV_FIX :
      case AUTHO_TLV_VAR :


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
		 /* EBE130424: Check function definition in autho_ini.c
         AdjustTlvAutho( tlvInfo, nIndice+1, length-nOldTlvLen);*/
		 AdjustTlvAutho( tlvInfo, nIndice, length-nOldTlvLen);
         break;

      }
      return(SUCCES);
   }
   
   switch ( nTlvType )
   {

   case AUTHO_TLV_FIX :
   case AUTHO_TLV_VAR :
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

   memset (szLastPart,  0, sizeof(szLastPart)); /**PA DSS*/

   return(SUCCES);
}

int AuthoBuildTlv (char * buffer_snd, TSTlvAutho *tlvInfo)
{
    char sLLLL [ 4 + 1 ];

    memset (  sLLLL  , 0 ,  sizeof ( sLLLL ));
    sprintf(  sLLLL  , "%04d" , tlvInfo->nLength );

    memcpy ( buffer_snd + 0     , sLLLL          , 4 );
    memcpy ( buffer_snd + 4     , tlvInfo->sTlvData , tlvInfo->nLength);
	buffer_snd[tlvInfo->nLength + 4 ] = '\0';
    return(tlvInfo->nLength + 4 );
}
/******************************************************************************/
/* EBE140417: New tlvbuffer util functions*/
static int GetTlvBufferPos(char *tlv_name, char *tlvbuffer)
{

	char	*sPtr;
	int		nOffset;
	int		nLength;
	int		nTlvLength;
	char	szLength[8];
	
	memset(szLength,'\0',sizeof(szLength));
	memcpy(szLength, tlvbuffer,4); 
	nTlvLength = atoi(szLength);

	nOffset =0;
	sPtr = tlvbuffer + 4;
	memset(szLength,'\0',sizeof(szLength));
	while( nOffset < nTlvLength )
	{
		if( memcmp(tlv_name, sPtr + nOffset,3) == 0 )
		{
			return nOffset + 4;
		}
		memcpy(szLength, sPtr + nOffset + 3,3);
		nLength = atoi(szLength);
		nOffset += 3 + 3 + nLength;
	}
	return(NOK);
}
/******************************************************************************/
int  GetTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int *length)
{
   
	int		nTlvTagPos;
	int		nLength;
	char	szLength[8];
	
	memset(szLength,'\0',sizeof(szLength));
	
	nTlvTagPos = GetTlvBufferPos(tlv_name,tlvbuffer);
	if( nTlvTagPos < 0 )
		return NOK;

	memcpy(szLength, tlvbuffer + nTlvTagPos + 3,3);
	nLength = atoi(szLength);
	memcpy(data,tlvbuffer + nTlvTagPos + 3 + 3,nLength);
	data[nLength]='\0';
	(*length) = nLength;
	return SUCCESS;
}
/******************************************************************************/
int  PutTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length)
{

	int		nTlvTagPos;
	int		nLength;
	int		nPrevLength;
	char	szLength[8];
	char	right_buffer[LG_MAX];
	int		nRightBufferLen;
	int		nTlvBufferLen;
	int		nOffset;
	
	memset(szLength,'\0',sizeof(szLength));
	
	nTlvTagPos = GetTlvBufferPos(tlv_name,tlvbuffer);
	if( nTlvTagPos < 0 )
	{
		if( length == 0 || data == NULL )
			return SUCCESS;

		memcpy(szLength, tlvbuffer,4); 
		nLength = atoi(szLength);

		sprintf(szLength,"%.3d",length);
		memcpy(tlvbuffer + nLength + 4,tlv_name,3);
		memcpy(tlvbuffer + nLength + 4 + 3,szLength,3);
		memcpy(tlvbuffer + nLength + 4 + 3 + 3,data,length);
		nLength+= 3 + 3 + length;
		tlvbuffer[nLength + 4]='\0';
		sprintf(szLength,"%.4d",nLength);
		memcpy(tlvbuffer,szLength,4);
		return SUCCESS;
	}
	memcpy(szLength, tlvbuffer,4);
	nTlvBufferLen = atoi(szLength);

	memset(szLength,'\0',sizeof(szLength));
	memcpy(szLength, tlvbuffer + nTlvTagPos + 3,3);
	nPrevLength = atoi(szLength);
	/* Save the right part */
	nRightBufferLen = nTlvBufferLen - ((nTlvTagPos - 4 )+ 3 + 3 + nPrevLength );
	if( nRightBufferLen > 0 )
		memcpy(right_buffer, tlvbuffer + nTlvTagPos + 3 + 3 + nPrevLength,nRightBufferLen);


	
	if( length > 0 )
	{
		sprintf(szLength,"%.3d",length);
		memcpy(tlvbuffer + nTlvTagPos  + 3,szLength,3);
		memcpy(tlvbuffer + nTlvTagPos  + 3 + 3,data,length);
		nOffset = 3 + 3 + length;
	}
	else
	{
		nOffset=0;
		length-=6; /*tag and length to be deleted too */
	}

	nTlvBufferLen += length - nPrevLength;
	sprintf(szLength,"%.4d",nTlvBufferLen);
	memcpy(tlvbuffer,szLength,4);
	if( nRightBufferLen > 0 )
		memcpy(tlvbuffer + nTlvTagPos  + nOffset, right_buffer,nRightBufferLen);

	tlvbuffer[nTlvBufferLen + 4]='\0';
	memset(right_buffer,'\0',sizeof(right_buffer));
	return SUCCESS;

}

/******************************************************************************/
int  DelTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length)
{
	return PutTlvBuffer(tlv_name,tlvbuffer,NULL,0);
}


int DumpTlvTags(char *tlvbuffer)
{


	char	*sPtr;
	int		nOffset;
	int		nLength;
	int		nTlvLength;
	char	szLength[8];
	char	szTlvTag[8];
	char	szTlvData[1024];
	char	sLine[MAX_LINE_TRC];
	
	if( trace_level < TRACE )
		return OK;

	memset(szLength,'\0',sizeof(szLength));
	memcpy(szLength, tlvbuffer,4); 
	nTlvLength = atoi(szLength);

	nOffset =0;
	sPtr = tlvbuffer + 4;
	memset(szLength,'\0',sizeof(szLength));
	while( nOffset < nTlvLength )
	{
		
		memcpy(szTlvTag,sPtr + nOffset,3);szTlvTag[3]='\0';
		memcpy(szLength,sPtr + nOffset + 3,3);szLength[3]='\0';
		nLength = atoi(szLength);
		memcpy(szTlvData,sPtr + nOffset + 3 + 3,nLength);szTlvData[nLength]='\0';
		sprintf(sLine,"TAG %.3s\tLEN %.3s\t [%s]\n",szTlvTag,szLength,szTlvData);
		trace_event(sLine,TRACE);
		nOffset += 3 + 3 + nLength;
	}

	return OK;
}
