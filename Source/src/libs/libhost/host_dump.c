/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583 BANK    (M.C)              */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <iso_bank.h>

#define SUCCES  0
#define ECHEC  -1

void DumpBankData( char *data )
{
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];
	int nLength;
	int iMapCtr,i,j,k,nPos=0;
	TSBankInfo msgInfo;

	char            CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));

	InitBankInfo(&msgInfo);


	AnalyseBank  ( data , &msgInfo);
	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
	++iMapCtr;

	memcpy(sLine ,"- BIT MAP    : ",13); nPos+=13;
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

	for( i = 1 ; i <= iMapCtr * 64 ; i++)
	{
		memset(sWork, 0, sizeof(sWork));
		if ( GetBankField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			if ( i < 129 )
			{
				/*MKB060309 PCI Norme Integration */
				switch(i)
				{
				case BANK_CARD_NBR:
					if (debug_option == 1)
						sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
					else
					{
						TrunCard(sWork, CardTrunc);
					        sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,CardTrunc );
					}
                                                trace_event(sLine,TRACE);
				break;	
			        case BANK_T2_DATA:
				case BANK_T1_DATA:
				case BANK_PIN_DATA:
					if (debug_option == 1)
                                                sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
                                        else
                                                sprintf(sLine,"- FLD (%03d)   (%03d)    [**********************]" , i+1, nLength );
                                        trace_event(sLine,TRACE);
				break;
				default:	
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
					trace_event(sLine,TRACE);
				}
			}
			else
			{
				nPos = 0;
				sprintf(sLine,"- FLD (%03d)   (%03d)    [" , i+1, nLength); nPos+=24;
				for(j=0; j<nLength; j++)		
				{
					sprintf(sLine + nPos ,"%02X|" , sWork[j] );
					nPos += 3;
				}
					sprintf(sLine + nPos,"]" );
					trace_event(sLine,TRACE);
			}
	}

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sWork, 0, sizeof(sWork));
	InitBankInfo(&msgInfo);
}

void DumpBank(data)
char *data;
{
	char sLine[MAX_LINE_TRC];

	trace_event("Start DumpBank()",PROCESSING);

	sprintf(sLine,"-----------------------------------");
	sprintf(sLine,"- HEADER     : |%.15s|", data);
	trace_event(sLine,TRACE);
   
	DumpBankData(data);

	trace_event("End   DumpBank()",PROCESSING);
}
