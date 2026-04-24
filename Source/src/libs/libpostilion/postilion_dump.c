
/****************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <define.h>
#include <iso_postilion.h>
#include <iso_postilion_f127.h>

#define SUCCES  0
#define ECHEC  -1

void DumpPostilionData( char *data )
{
/*START ABM  07022022 Add Structured Data field*/
      //  char sWork[LG_MAX];
        char sWork[LG_MAX_POSTILION];
/*END ABM  07022022 Add Structured Data field*/
    char                   sLine[LG_MAX]; /* MSA24062008 : le champ 127.22 de postillon peut contenir beacoup plus que 512 caractères */ 
    int                    nLength;
    int                    iMapCtr,i,j,k,l,m,nPos=0;
    TSPostilionInfo        msgInfo;
    TSPostilionF127Info    msgF127Info;
    
    
    char CardTrunc[22 + 1];
    memset(CardTrunc, 0, sizeof(CardTrunc));
    
    memset ( sWork    , 0 , sizeof ( sWork ));
    memset ( sLine    , 0 , sizeof ( sLine ));

    InitPostilionInfo(&msgInfo);
    InitPostilionF127Info(&msgF127Info);

    AnalysePostilion  ( data , &msgInfo);
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
        if ( GetPostilionField(i, &msgInfo, sWork, &nLength ) == SUCCES )
        if ( i < 129 )
        {
            if(i == 126)
            {
                AnalysePostilionF127  ( sWork, &msgF127Info );
                for( l = 0 ; l < MAX_POSTILION_F127_FIELDS ; l++)
                {
                      memset(sWork, 0, sizeof(sWork));
                      if ( GetPostilionF127Field(l, &msgF127Info, sWork, &nLength ) == SUCCES )
                    {
                        sprintf(sLine,"- FLD (127.%03d) (%03d)    [%.67s]" , l+1, nLength, sWork );
                        trace_event(sLine,TRACE);
                    }
                }
            
            }
            else if(i == 1)
            {
            if (debug_option == 1)
            {
            sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
            }
            else
            {
            TrunCard(sWork, CardTrunc);
            sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,CardTrunc);
            }
            trace_event(sLine,TRACE);
            }
            else if((i == 34) || (i == 51))
            {
            if (debug_option == 1)
            {
            sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
            }
            else
            {
            sprintf(sLine,"- FLD (%03d)     (%03d)    [*******************] " , i+1 , nLength );
            }
            trace_event(sLine,TRACE);
            }                
            else
            {
                sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
                trace_event(sLine,TRACE);
            }
        }
        else
        {
            nPos = 0;
            sprintf(sLine,"- FLD (%03d)    (%03d)    [" , i+1, nLength); nPos+=24;
            for(j=0; j<nLength; j++)
            {
                    sprintf(sLine + nPos ,"%02X|" , sWork[j] );
                    nPos += 3;
            }
            sprintf(sLine + nPos,"]" );
            trace_event(sLine,TRACE);
        }
    }
}

void DumpPostilion(char *data)
{
    char sLine[MAX_LINE_TRC];

    trace_event("Start DumpPostilion()",PROCESSING);
    sprintf(sLine,"-----------------------------------");
    trace_event(sLine,TRACE);
    DumpPostilionData(data);
    trace_event("End   DumpPostilion()",PROCESSING);
}

