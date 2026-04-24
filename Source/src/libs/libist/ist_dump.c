/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/
 
#include <stdio.h>
#include <define.h>
#include <iso_ist.h>
#include <ist_fields.h>
#include <globals.h> 

#define SUCCES  0
#define ECHEC  -1
    
void DumpIstData( char *data )
{
        TSIstInfo           msgInfo;
		char                sWork[LG_MAX];
		char                CardTrunc[22+1];
        char                sLine[MAX_LINE_TRC];
        int                 nLength;
        int                 iMapCtr;
		int                 i;
		int                 j;
		int                 k;
		int                 l;
		int                 m;
		int                 lRet;
		int                 nPos=0;
        

		memset(sLine,       '\0', sizeof(sLine));
		memset(sWork,       '\0', sizeof(sWork));
		memset(CardTrunc,   '\0', sizeof(CardTrunc));


        InitIstInfo(&msgInfo);
        AnalyseIst(data, &msgInfo);
        nPos=0;

        iMapCtr = 1;
        while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 2) )
        ++iMapCtr;

        memcpy(sLine ,"- BIT MAP    : ",13); nPos+=13;
        for (i=0; i<iMapCtr * 8 ;i++)
        {
                sprintf(sLine + nPos," %02X", msgInfo.sBitMap[i]);
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
                if ( GetIstField(i, &msgInfo, sWork, &nLength ) == SUCCES )
                if ( i < 129 )
                {
                   if(i == 126)
                   {
                   		for( l = 0 ; l < 20 ; l++)
                   		{
                   			  memset(sWork, 0, sizeof(sWork));
                   	    }
                   }
                   else
                   {
                        switch(i)
						{
						   case IST_CARD_NBR :
						                              if (debug_option == 1)
                                                      {
 						                                  sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
                                                          trace_event(sLine,TRACE);
						                              }
			                                          else
                                                      {
						                                  TrunCard(sWork, CardTrunc);
						   	                              sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,CardTrunc );
                                                          trace_event(sLine,TRACE);
						                              }
					                                  break;
						   case IST_EXPIRY_DATE :
 						   case IST_T2_DATA:
						   case IST_ADTNL_DATA_NATIONAL :
						   case IST_ADTNL_DATA_PRIV:
						   case IST_PIN_DATA:
                       	                              if (debug_option == 1)
                                                      {
 						                                  sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
                                                          trace_event(sLine,TRACE);
						                              }
			                                          else
                                                      {
						                                  memset(sWork,'*',nLength);
						   	                              sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
                                                          trace_event(sLine,TRACE);
						                              }
					                                  break;
                           case IST_ICC_DATA :
                                                     {
                                                         unsigned char   leftTag;
                                                         unsigned char   rightTag;
                                                         char            sTag [ 4 + 1];
                                                         char            sTagLabel [ 50];
                                                         char            sValue[LG_MAX];
                                                         int             nLen;
                                                         int             nlenTagAsc;
	                                                     
                                                      
                                                         sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
                                                         trace_event(sLine,TRACE);
                                                         
                                                         for( j = 0 ; j < nLength; )
                                                         {
                                                            memset(sTag,      '\0', sizeof(sTag));
                                                            memset(sValue,    '\0', sizeof(sValue));
                                                            memset(sTagLabel, '\0', sizeof(sTagLabel));
                                                      
                                                            if (((unsigned char)sWork[j] & (unsigned char) 0x1F) == (unsigned char) 0x1F)
		                                               	    {
		                                               	    	leftTag=(unsigned char)sWork[j++];
		                                               	    	rightTag=(unsigned char)sWork[j++];
                                                                   sprintf(sTag, "%02X%02X", leftTag, rightTag);
		                                               	    }
                                                                else
		                                               	    {
		                                               	    	leftTag=(unsigned char)sWork[j++];
                                                                   sprintf(sTag, "%02X  ", leftTag);
		                                               	    }
                                                            nLen = sWork[j++]; 
                                                      
                                                            HexToAscii(sWork+j,sValue,nLen*2);
                                                      
                                                            lRet = GetIccTagLabl(sTag,sTagLabel);
                                                            
		                                               	    if (debug_option != 1)
		                                               	    {
		                                               	     if ( memcmp(sTag, "5F34", 4) == 0 ||
                                                                  memcmp(sTag, "9F27", 4) == 0 ||
                                                                  memcmp(sTag, "57",   2) == 0 ||
                                                                  memcmp(sTag, "5A",   2) == 0)
                                                                { 
		                                               	    	 nlenTagAsc=strlen(sValue);
                                                                 memset (sValue, '*',nlenTagAsc);
                                                                 sprintf(sLine,"  > (%.4s) (%.35s) (%03d): [%s]" ,sTag, sTagLabel , strlen(sValue),sValue );
                                                                }                               
		                                               	     else                               
		                                               	    	 sprintf(sLine,"  > (%.4s) (%.35s) (%03d): [%s]" ,sTag, sTagLabel , strlen(sValue), sValue );
		                                               	    }                            
		                                               	     else                        
		                                               	    	 sprintf(sLine,"  > (%.4s) (%.35s) (%03d): [%s]" ,sTag, sTagLabel , strlen(sValue), sValue );
															 trace_event(sLine,TRACE);
                                                      

                                                             j +=  nLen;
                                                         
                                                         }
                                                     }
                                                     break;
					       default :
                                   sprintf(sLine,"- FLD (%03d)     (%03d)    [%s]" , i+1, nLength,sWork );
					        	   trace_event(sLine,TRACE);
					        	   break;	
					
				        }
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
		memset(sLine,       '\0', sizeof(sLine));
		memset(sWork,       '\0', sizeof(sWork));
		memset(CardTrunc,   '\0', sizeof(CardTrunc));
		InitIstInfo(&msgInfo);
}


void DumpIst(data)
char *data;
{
    char sLine[MAX_LINE_TRC];
	

	trace_event("Start DumpIst()",PROCESSING);
    
	memset(sLine,  '\0', sizeof(sLine));
    
    memset(sLine,'-',47);
    trace_event(sLine,TRACE);
	DumpIstData(data);
	trace_event(sLine,TRACE);
    
    trace_event("End DumpIst()",PROCESSING);
}


void DumpStructIst( TSIstInfo *IstInfo )
{
        char sBuffer [ LG_MAX ];
        int  vLen;


        memset(sBuffer, '\0', sizeof(sBuffer) );
        vLen = BuildMsgBuffer( sBuffer, IstInfo);
        DumpIst(sBuffer);
   
        return;
}

