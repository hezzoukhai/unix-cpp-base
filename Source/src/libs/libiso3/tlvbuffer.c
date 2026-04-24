#include <p7_common.h>


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
	return SUCCESS;

}

/******************************************************************************/
int  DelTlvBuffer(char *tlv_name, char *tlvbuffer)
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
		trace_message(P7_TL_TRACE,"TAG %.3s\tLEN %.3s\t [%s]",szTlvTag,szLength,szTlvData);
		nOffset += 3 + 3 + nLength;
	}

	return OK;
}





