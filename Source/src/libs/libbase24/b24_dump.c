/*#include <base24.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <define.h>
#include <base24/iso_base24.h>
#include <base24/base24_tokens.h>
#include <base24/base24_define.h>
#include <base24/iso_base24_atm_fields.h>
#include <base24/iso_base24_pos_fields.h>



#define SUCCES  0
#define ECHEC  -1
/***************************************************************************/
void DumpBase24Header( TSBase24Header *msgHeader )
{
	
	trace_message(TRACE,"Product indicator:      %c%c",msgHeader->sProductIndicator[0],msgHeader->sProductIndicator[1]);
	trace_message(TRACE,"Release number:         %c%c",msgHeader->sReleaseNumber[0],msgHeader->sReleaseNumber[1]);
	trace_message(TRACE,"Status:                 %c%c%c",msgHeader->sStatus[0],msgHeader->sStatus[1],msgHeader->sStatus[2]);
	trace_message(TRACE,"Originator code:        %c",msgHeader->sOriginatorCode);
	trace_message(TRACE,"Responder code:         %c",msgHeader->sResponderCode);
}
/***************************************************************************/
void DumpStructBase24( TSBase24Info *msgInfo )
{

	unsigned char   sWork[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	int             nLength;
	int             iMapCtr,i,j,nPos=0;
	char            CardTrunc[22 + 1];
	TSTokens		Tokens;

    trace_event("Start DumpStructBase24()",PROCESSING);
    /* EBE: Avoid calling trace_event in case not in appropriate trace_level */
    if (trace_level < TRACE )
    {
        return;
    }
    memset(CardTrunc, 0, sizeof(CardTrunc));
	InitTokens(&Tokens);
	
    trace_message(TRACE,"- HEADER     :");
	DumpBase24Header(&msgInfo->sHeader);
	

	nPos=0;

	iMapCtr = 1;
	while ( ( msgInfo->sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 2) )
		++iMapCtr;

	memcpy(sLine ,"- BIT MAP    : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8 ;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char)msgInfo->sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);


	trace_event     ("-----------------------------------",TRACE);
	trace_message   (TRACE,"- M.T.I      : [%04d]" , msgInfo->nMsgType );
	trace_event     ("- FLD (FIELD): LENGTH : DATA        ",TRACE);
	trace_event     ("-----------------------------------",TRACE);


	for( i = 1 ; i <= iMapCtr * 64 ; i++)
	{
		memset(sWork, '\0', sizeof(sWork));
		if ( GetBase24Field(i, msgInfo, sWork, &nLength ) != SUCCES )
		{
		    continue;
		}
        if( debug_option != 1 )
        {
            if( i == B24_ATM_CARD_NBR )
            {
                TrunCard(sWork, CardTrunc);
                sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,CardTrunc );
                trace_event(sLine,TRACE);
                continue;
            }
            else if(	i == B24_ATM_T2_DATA ||
                        i == B24_ATM_PIN_DATA)
            {
                sprintf(sLine,"- FLD (%03d)     (%03d)    [**********************]" , i+1, nLength );
                trace_event(sLine,TRACE);
                continue;
            }
        }

		if( ( msgInfo->nProductIndicator == B24_PRODUCT_IND_ATM_T && i == B24_ATM_ADD_DATA)	||
			( msgInfo->nProductIndicator == B24_PRODUCT_IND_POS_T && i == B24_POS_ADD_DATA)	)
		{
			sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
			trace_event(sLine,TRACE);

			AnalyseTokens(sWork,&Tokens);
			DumpTokens(&Tokens);
			continue;
		}

        if (    GetBase24FieldType(i,msgInfo->nProductIndicator) != BASE24_VAR_BIN &&
                GetBase24FieldType(i,msgInfo->nProductIndicator) != BASE24_FIX_BIN)
        {
            sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
            trace_event(sLine,TRACE);
        }
        else
        {
            nPos = 0;
            sprintf(sLine,"- FLD (%03d)     (%03d)    [" , i+1, nLength); nPos+=26;
            for(j=0; j<nLength; j++)
            {
                sprintf(sLine + nPos ,"%02X|" , sWork[j] );
                nPos += 3;
            }
            sprintf(sLine + nPos,"]" );
            trace_event(sLine,TRACE);
        }
	}

    trace_event("End   DumpStructBase24()",PROCESSING);
	return;
}


void DumpBase24Data( unsigned char *data )
{
	int             nResult;
    TSBase24Info    msgInfo;

    /* EBE: Avoid calling trace_event in case not in appropriate trace_level */
    if (trace_level < TRACE )
    {
        return;
    }

	

	nResult = AnalyseBase24 ( data, &msgInfo);
	if( nResult != OK )
    {
        trace_event("Error returned by AnalyseBase24()", ERROR);
        return;
    }
    DumpStructBase24(&msgInfo);

}

/****************************************************************************/
/***************************************************************************/
void DumpBase24(unsigned char* data)
{

	trace_event("Start DumpBase24()",PROCESSING);
	trace_event("-----------------------------------",TRACE);

	DumpBase24Data(data);

	trace_event("End   DumpBase24()",PROCESSING);
}

/****************************************************************************/
