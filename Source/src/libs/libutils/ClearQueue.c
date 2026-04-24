#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <define.h>
#include <queue.h>
#include <dump.h>

#include <iso_hps.h>
#include <tlv_privfields.h>
#include <queue.h>
#include <tlv_private.h>


int MaskIsoInfoData(TSIsoInfo*	pkIsoInfo)
{
	
	char 			szBuffer[BUF_LG_MAX];
	TSTlvPrivate   	kTlvPrivateInfo;
	int				nLength;
	TSTagIso      	IsoIccInfo;
	
	InitTlvPrivate(&kTlvPrivateInfo);
	InitIsoIcTag  ( &IsoIccInfo );
	
	if ( GetIsoField(ISO_CARD_NBR, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_CARD_NBR, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_ACQR_REFERENCE_DATA, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_ACQR_REFERENCE_DATA, pkIsoInfo, szBuffer, nLength);		
	}
	if ( GetIsoField(ISO_T1_DATA, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_T1_DATA, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_PIN_DATA, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_PIN_DATA, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_ACCOUNT_ID1, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_ACCOUNT_ID1, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_ACCOUNT_ID2, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		memset(szBuffer,'X',nLength);
		PutIsoField(ISO_ACCOUNT_ID2, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_ADTNL_DATA_PRIV, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		AnalyseTlvPrivate(szBuffer,nLength,&kTlvPrivateInfo);
		
		if ( GetTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG,&kTlvPrivateInfo,szBuffer,&nLength) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &kTlvPrivateInfo, szBuffer, nLength);
		}
		if ( GetTlvPrivate(PVV_OFFSET_TAG,&kTlvPrivateInfo,szBuffer,&nLength) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutTlvPrivate(PVV_OFFSET_TAG, &kTlvPrivateInfo, szBuffer, nLength);
		}
		if ( GetTlvPrivate(PVT_DATA_TAG,&kTlvPrivateInfo,szBuffer,&nLength) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutTlvPrivate(PVT_DATA_TAG, &kTlvPrivateInfo, szBuffer, nLength);
		}
		if ( GetTlvPrivate(CVC2_DATA_TAG,&kTlvPrivateInfo,szBuffer,&nLength) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutTlvPrivate(CVC2_DATA_TAG, &kTlvPrivateInfo, szBuffer, nLength);
		}

		nLength = PrivateBuildTlv(szBuffer, &kTlvPrivateInfo);
		PutIsoField(ISO_ADTNL_DATA_PRIV, pkIsoInfo, szBuffer, nLength);
	}
	if ( GetIsoField(ISO_ICC_DATA, pkIsoInfo, szBuffer, &nLength ) == SUCCES )
	{
		AnalyseIsoIc  ( szBuffer, nLength, &IsoIccInfo );
		
		if ( GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, szBuffer,  &nLength ) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutIsoIcTag(ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, szBuffer, nLength);
		}
		if ( GetIsoIcTag (ISO_TAG_ATC, &IsoIccInfo, szBuffer,  &nLength ) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutIsoIcTag(ISO_TAG_ATC, &IsoIccInfo, szBuffer, nLength);
		}
		if ( GetIsoIcTag (ISO_TAG_TRACK2, &IsoIccInfo, szBuffer,  &nLength ) == SUCCES )
		{
			memset(szBuffer,'X',nLength);
			PutIsoIcTag(ISO_TAG_TRACK2, &IsoIccInfo, szBuffer, nLength);
		}
		nLength = IsoBuildIcFld(szBuffer, &IsoIccInfo);
		PutIsoField(ISO_ICC_DATA, pkIsoInfo, szBuffer, nLength);
	}

	return OK;
}

static char*			sHdr1="\n------------------------------------\n";
static char*			sHdr2="- FLD (FIELD): LENGTH :  DATA       \n";

int DumpMaskedIsoInfoData(TSIsoInfo*	pkIsoInfo,int nFd)
{
	int				iMapCtr;
	char 			szFieldData[BUF_LG_MAX];
	char			sLine[BUF_LG_MAX];
	int 			i=0;
	int				nPos=0;
	int				nLength;

	
	iMapCtr = 1;
	while ( ( pkIsoInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", pkIsoInfo->sBitMap[i]);
		nPos += 3;
	}
	write(nFd,sLine,strlen(sLine));
	

	write(nFd,sHdr1,strlen(sHdr1));
	
	sprintf(sLine,"- M.T.I      : %04d\n" , pkIsoInfo->nMsgType );
	write(nFd,sLine,strlen(sLine));
	
	write(nFd,sHdr1,strlen(sHdr1));
	write(nFd,sHdr2,strlen(sHdr2));
	write(nFd,sHdr1,strlen(sHdr1));

	for( i = 1 ; i < 127 ; i++)
	{
		if ( GetIsoField(i, pkIsoInfo, szFieldData, &nLength ) != SUCCES )
		{
			continue;
		}
		sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] \n" , i+1 , nLength ,szFieldData );
		write(nFd,sLine,strlen(sLine));
	}
	
	return OK;
}


int main (int argc , char **argv)
{
   
	TSBalMessage 	msg_buffer;
	TSHsmMessage 	hsm_buffer;
	int				nBalId;
	int				nFd;
	char			szDumpFile[256];
	char			szBuffer[LG_MAX];
	int				nRead;
	int				nBalKey;
	TSIsoInfo   	IsoInfo;
	
	int				nLength;
	int				nResult;
	

   

   if ( argc < 3)
   {
		fprintf(stderr,"\nUsage ClearQueue <QueueID> <TraceFile>\n");
        exit(1);
   }

   nBalKey = atoi(argv[1]);
   memset(szDumpFile,'\0',sizeof(szDumpFile));
   nFd = -1;

   

	sprintf(szDumpFile,"%s/%s",getenv("TRACE"), argv[2]);
	nFd = open(szDumpFile, O_RDWR|O_CREAT|O_APPEND, 0640);
   

   nBalId = OpenBalCom(nBalKey);

   if( nBalKey == BAL_HSM_VERIF || nBalKey == BAL_HSM_PROD )
   {
	   while( (nRead = msgrcv( nBalId , &hsm_buffer, sizeof(hsm_buffer), 0 , IPC_NOWAIT)) != -1)
	   {
	   
		   if( nFd < 0 )
		   {
			   continue;
		   }
			sprintf(szBuffer,"================\nPartner Id %d\nSource  Id %d\nType    Id %d\nCommand    %d\nLength     %d\n================\n",
							hsm_buffer.sText.sHeader.nPartnerId,
							hsm_buffer.sText.sHeader.nSourceId,
							(int)hsm_buffer.lType,
							hsm_buffer.sText.sHeader.nCommand,
							hsm_buffer.sText.sHeader.nLen);
			write(nFd,szBuffer,strlen(szBuffer));
			
			dump_buffer_on_file(nFd, hsm_buffer.sText.sBody, 2, 'X','X','X');
			
	   }

   }
   else if( nBalKey == BAL_MSG || nBalKey == BAL_ADM )
   {
	   while( (nRead = msgrcv( nBalId , &msg_buffer, sizeof(msg_buffer), 0 , IPC_NOWAIT)) != -1)
	   {
	   
		   if( nFd < 0 )
		   {
			   continue;
		   }

			sprintf(szBuffer,"================\nPartner Id %d\nSource  Id %d\nType    Id %d\nCommand    %d\nLength     %d\n================\n",
							msg_buffer.sText.sHeader.nPartnerId,
							msg_buffer.sText.sHeader.nSourceId,
							(int)msg_buffer.lType,
							msg_buffer.sText.sHeader.nCommand,
							msg_buffer.sText.sHeader.nLen);
			write(nFd,szBuffer,strlen(szBuffer));

			InitIsoInfo(&IsoInfo);
			AnalyseIso(msg_buffer.sText.sBody, &IsoInfo);
			nResult = MaskIsoInfoData(&IsoInfo);
			
			nLength = IsoBuildMsg(szBuffer, &IsoInfo);
			dump_buffer_on_file(nFd, szBuffer, nLength, 'X','X','X');
			DumpMaskedIsoInfoData(&IsoInfo,nFd);

      }
   }
   else if( nBalKey == BAL_NTW_RAW_MSG )/*PLUTONL-3572*/
   {
	   while( (nRead = msgrcv( nBalId , &msg_buffer, sizeof(msg_buffer), 0 , IPC_NOWAIT)) != -1)
	   {
	   
		   if( nFd < 0 )
		   {
			   continue;
		   }

			sprintf(szBuffer,"================\nPartner Id %d\nSource  Id %d\nType    Id %d\nCommand    %d\nLength     %d\n================\n",
							msg_buffer.sText.sHeader.nPartnerId,
							msg_buffer.sText.sHeader.nSourceId,
							(int)msg_buffer.lType,
							msg_buffer.sText.sHeader.nCommand,
							msg_buffer.sText.sHeader.nLen);
			write(nFd,szBuffer,strlen(szBuffer));

			dump_buffer_on_file(nFd, msg_buffer.sText.sBody, msg_buffer.sText.sHeader.nLen, 'X','X','X');			

      }
   }
   else
   {
	   while( (nRead = msgrcv( nBalId , &msg_buffer, sizeof(msg_buffer), 0 , IPC_NOWAIT)) != -1)
	   {
		   
	   }
   }

   if( nFd > 0 )
   {
	   close(nFd);
   }
   
   return 0;
   
}
