/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      private_tlv.c                                                          */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management iso fields that have tlv format (48, 60 ...)             */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Add fucntion PutEvent : This function add the tag event and the TAG  */
/*        Message error in field 48 (This function is the equivalent of the    */
/*        function PCRD_P7_GENERAL_TOOLS.put_event                             */
/*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <tlv_private.h>
#include <define.h>
#include <tlv_fields.h>

int GetTlvPrivate (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int *length)
{
	int nIndice;
	int nTlvLength;
	char szAscTlvLen [4];
	int nTlvType;

	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));

	if ( (nIndice = GetTlvPrivateIndice(tlv_name)) == -1)
		return(ECHEC);

	nTlvType    = GetTlvPrivateType(nIndice) ;
	nTlvLength  = GetTlvPrivateLength(nIndice); 

	if (tlvInfo->nPresent[nIndice] == NOT_PRESENT)
	{
		*length = 0;
		data[0]='\0';
		return(ECHEC);
	}
		

	memcpy ( szAscTlvLen, 
		tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3,
		3);
	*length = atoi(szAscTlvLen);

	if(*length == 0)
	{
		data[0]='\0';
		return(ECHEC);
	}
	
	switch ( nTlvType )
	{
	case PRIVATE_TLV_FIX :
	case PRIVATE_TLV_VAR :
		memcpy( data,
			tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, 
			*length);
		break;

	}
	data[*length]='\0';
	return(SUCCES);
}

int AddTlvPrivate (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length)
{
	int nIndice;
	int nTlvLength;
	char szAscTlvLen [4];
	int nTlvType;

	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));

	if ( (nIndice = GetTlvPrivateIndice(tlv_name)) == -1)
		return(ECHEC);

	nTlvType    = GetTlvPrivateType(nIndice) ;
	nTlvLength  = GetTlvPrivateLength(nIndice); 

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

int PutTlvPrivate (char  *tlv_name, TSTlvPrivate *tlvInfo, char *data, int length)
{
	int nIndice;
	int nTlvLength;
	int nOldTlvLen;
	char szAscTlvLen [ 4 ];
	char szOldTlvLen [ 4 ];
	char szLastPart  [ MAX_PRIVATE_TLV_LEN ];
	int nTlvType;


	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szOldTlvLen, '\0', sizeof(szOldTlvLen));

	if ( (nIndice = GetTlvPrivateIndice(tlv_name)) == -1)
	{
		return(ECHEC);
	}

	nTlvType    = GetTlvPrivateType(nIndice) ;
	nTlvLength  = GetTlvPrivateLength(nIndice); 
	
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
			/* EBE130424: Check function definition in private_ini.c
			AdjustTlvPrivate( tlvInfo, nIndice+1, length-nOldTlvLen);*/
			AdjustTlvPrivate( tlvInfo, nIndice, length-nOldTlvLen);

			break;

		}
		/*trace_event("End   PutTlvPrivate (SUCCES)",PROCESSING);*/
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

	memset (szLastPart,  '\0', sizeof(szLastPart)); /**PA DSS*/

	return(SUCCES);
}

int PrivateBuildTlv (char * buffer_snd, TSTlvPrivate *tlvInfo)
{
	if( tlvInfo->nLength > 0 )
	{
		memcpy (buffer_snd, tlvInfo->sTlvData, tlvInfo->nLength);
	}
	return(tlvInfo->nLength);
}

int PutEvent(char *sTlvData, char *sEvent, char *sMsg)
{
	int  nLen = 0;
	TSTlvPrivate    sTlvPrivateInfo;
	char sAddDataPriv[1024]; /* Base1 certif */
	char sLine[MAX_LINE_TRC];


	sprintf(sLine, "Start PutEvent(%.20s,%s,%s,%lu)", sTlvData,sEvent, sMsg,strlen(sTlvData));
	trace_event(sLine,PROCESSING);

	InitTlvPrivate(&sTlvPrivateInfo );
	PutTlvBuffer ( AUTO_EVENT_CODE, sTlvData, sEvent ,  3);
	memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
	GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvData, sAddDataPriv , &nLen);
	AnalyseTlvPrivate(sAddDataPriv,nLen,&sTlvPrivateInfo );
	PutTlvPrivate(MESSAGE_TEXT_TAG ,&sTlvPrivateInfo,sMsg,strlen(sMsg));
	memset(sAddDataPriv, 0, sizeof(sAddDataPriv));
	nLen = PrivateBuildTlv( sAddDataPriv , &sTlvPrivateInfo);
	PutTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvData, sAddDataPriv ,  nLen);
	PrintTlvBuffer(sTlvData);

	InitTlvPrivate(&sTlvPrivateInfo ); /**PA DSS*/
	memset(sLine, 0, sizeof(sLine)); /**PA DSS*/
	memset(sAddDataPriv, 0, sizeof(sAddDataPriv)); /**PA DSS*/

	trace_event("End   PutEvent(OK)",PROCESSING);
	return(OK);
}

