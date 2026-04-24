

/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <header_sms.h>
#include <define.h>

#include <iso_bic.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <bic_param.h>

#define SUCCES  0
#define ECHEC  -1

void DumpBicData( char *data )
{
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];
	char TI[2];
	int nLength;
	int iMapCtr,i,j,k,nPos=0;
	TSBicInfo msgInfo;


	InitBicInfo(&msgInfo);
	memset(sLine, 0, sizeof(sLine));

	AnalyseBic (data, &msgInfo);
	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char)msgInfo.sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);


	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- M.T.I      : [%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	sprintf(sLine,"- FLD (FIELD): LENGTH : DATA        ");
	trace_event(sLine,TRACE);
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine,TRACE);

	/* NAB17042012
	for( i = 1 ; i <= iMapCtr * 64 ; i++)   **/
	int jj = iMapCtr * 64;
	for( i = 1 ; i <= jj ; i++)
	{
		memset(sWork, 0, sizeof(sWork));
		switch (i)
		{
		case 34: /* NAB29112014 Norme PCI */
			if ( GetBicField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
				else
					sprintf(sLine,"- FLD (%03d)   (%03d)    [*******************]" , i+1, nLength);
				trace_event(sLine,TRACE);
			}
			break;

		case 54 : 
			if ( GetBicField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[LG_MAX];
				int  nLen;

				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);
				for( j = 0 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						sprintf(sTag, "%02X%02X", (unsigned char)sWork[j++], (unsigned char)sWork[j+1]);
						j++;
					}
					else
						sprintf(sTag, "%02X  ", (unsigned char)sWork[j++]);

					nLen = sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);

					sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					trace_event(sLine,TRACE);

					j += nLen;
				}
			}
			break;

		default :
			if ( GetBicField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
				trace_event(sLine,TRACE);
			}
			break;
		}
	}

}

void DumpBic(data)
	char *data;
{
	TSBicHeader * header;
	char sLine[MAX_LINE_TRC];

	trace_event("Start DumpBic()",PROCESSING);

	header = ( TSBicHeader *) data ;        
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- H(Header field) : CONTENT");
	trace_event(sLine,TRACE);
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);

	sprintf ( sLine ,"- H(01) Protocol..............: %.3s",header->bicStart );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(02) Product...............: %.2s",header->bicProduct );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(03) Version...............: %.2s",header->bicVersion );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(04) Status................: %.3s",header->bicStatus );
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(05) Originator............: %c",header->bicOriginatorCode);
	trace_event(sLine,TRACE);
	sprintf ( sLine ,"- H(06) Responder.............: %c",header->bicResponderCode);
	trace_event(sLine,TRACE);
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);

	DumpBicData(data);

	trace_event("End   DumpBic()",PROCESSING);
}
void DumpBicStruct(TSBicInfo  *BicInfo)
{
	char sWork[LG_MAX];
	int  Length;

	trace_event("Start DumpBicStruct()",TRACE);
	memset(sWork, 0, sizeof(sWork));

	Length = BicBuildMsg (sWork + BIC_HDR_LEN, BicInfo);
	BicBuildHeader(RESPONSE , &(BicInfo->sHeader), NULL, "00"); 
	memcpy(sWork, (char *) &(BicInfo->sHeader), BIC_HDR_LEN); 

	DumpBicData (sWork);
	trace_event("End   DumpBicStruct()",TRACE);

}



DumpBicStr( TSBicInfo * msgInfo)
{
	char sWork[LG_MAX];
	/* char sLine[MAX_LINE_TRC];  NAB27112014 CORE DUMP */
	char sLine[LG_MAX];
	char TI[2];
	int nLength;
	int iMapCtr,i,j,k,nPos=0;


	trace_event("Start DumpBicStr()",PROCESSING);
	memset(sLine, 0, sizeof(sLine));

	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char)msgInfo->sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);


	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- M.T.I      : [%04d]" , msgInfo->nMsgType );
	trace_event(sLine,TRACE);
	sprintf(sLine,"- FLD (FIELD): LENGTH : DATA        ");
	trace_event(sLine,TRACE);
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine,TRACE);

	/* NAB17042012
	for( i = 1 ; i <= iMapCtr * 64 ; i++)  **/
	int jj = iMapCtr * 64; 
	for( i = 1 ; i <= jj ; i++)
	{
		memset(sWork, 0, sizeof(sWork));
		switch (i)
		{
		default :
			if ( GetBicField(i, msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
				trace_event(sLine,TRACE);
			}
			break;
		}
	}

}
