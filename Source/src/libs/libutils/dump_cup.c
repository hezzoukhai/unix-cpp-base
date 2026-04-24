/****************************************************************************/
/***************************************************************************/
/*            dump.c  : UTILITAIRE POUR COPIER ET AFFICHER     		    */
/*                           MESSAGE  CUP 			            */
/*   Author: K.ESSABRI                         			            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_cup.h>
#include <define.h>
#include <dump.h>
#include <globals.h>
#include <iso_cup.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <sys/time.h>

#define SUCCES  0
#define ECHEC  -1






void DumpFileCup( char *data )
{
        TSCupInfo  msgInfo;
        char       CardTrunc[22 + 1];
        char       sWork[1024];
        char       sLine[MAX_LINE_TRC];
        char       sTag [ 4 + 1], sValue[256];
	char	   szKeyZmk[32 + 1];
        int        nLen;
        int        nLength;
        int        iMapCtr,i,j,k;
        int        nPos = 0;
		
	int        nCounter = 0;
	int        nLoops   = 0;
	char       sLine2[MAX_LINE_TRC];
	char       sTlvField[MAX_LINE_TRC + 40];


	unsigned char leftTag,rightTag;


	memset(sWork,     0, sizeof(sWork));
	memset(sLine,     0, sizeof(sLine));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sTag,      0, sizeof(sTag));
	memset(sValue,    0, sizeof(sValue));
    	memset(szKeyZmk,  0, sizeof(szKeyZmk));
	
	memset(sLine2, '\0', sizeof(sLine2));
    	memset(sTlvField, '\0', sizeof(sTlvField));


   

	trace_event("Start DumpFileCup()",PROCESSING);


	InitCupInfo(&msgInfo);

	AnalyseCup  ( data+ 4 , &msgInfo);

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
        	++iMapCtr;

	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"<?xml version=\"1.0\"?>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<BaseMessage Description=\"Cup base message template\">");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgType Value=\"%04d\"/>" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgHeader Value=\"\"/>");
	trace_event(sLine,TRACE);

 for( i = 1 ; i < MAX_CUP_FIELDS ; i++)
 {
    switch (i)
    {
     case CUP_CARD_NBR : 
		if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
			if (debug_option == 1)
			{
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 ,  sWork );
			}
			else
			{        
				TrunCard(sWork, CardTrunc);
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , CardTrunc);
			}
			trace_event(sLine,TRACE);
		}
       break;



     case CUP_ADTNL_DATA_PRIV : 

                if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
                {  
		    if(memcmp(sWork , "NK", 2)== 0)
                        { 
			  sprintf(sLine," <Field Number=\"%03d\" Value=\"NK", i+1);
                          if (debug_option == 1)
                              {

        	      		  HexToAscii(sWork + 2, szKeyZmk, 32);
	                          sprintf(sLine + strlen(sLine),"%s\"/>", szKeyZmk );

				}
                          else
                                sprintf(sLine + strlen(sLine),"*******************\"/>");
                          
			}
		    else
			
		          sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 ,sWork );

			
	           trace_event(sLine,TRACE);

                }
       break;



    case CUP_ICC_DATA :
       if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
       {

          for( j = 0 ; j < nLength; )
          {
             memset(sTag, 0, sizeof(sTag));
             memset(sValue, 0, sizeof(sValue));

             if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
                {
	
                	leftTag=(unsigned char)sWork[j++];
                	rightTag=(unsigned char)sWork[j++];
                	sprintf(sTag, "%02X%02X", leftTag, rightTag);
		}
             else
                {
                        leftTag=(unsigned char)sWork[j++];
                        sprintf(sTag, "%02X", leftTag);
                }

		nLen = (unsigned char)sWork[j++];

             /*nLen = sWork[j++];*/

            HexToAscii(sWork+j,sValue,nLen*2);

			sprintf(sLine2,"%s%s%02X%s",sLine2,sTag ,nLen ,sValue);
			
             j += nLen;
          }
		  
		  
		  
		  
		    sprintf(sTlvField ,"<Field Number=\"%03d\" Value=\"%s\"/>", 55 , sLine2);

			nLoops = (strlen(sTlvField) / 90 ) + ( 0 == strlen(sTlvField) % 90 ? 0 : 1 );
			for ( ; nCounter < nLoops ; nCounter++)
			{
			sprintf(sLine," %.90s", sTlvField + (90 * nCounter));
			trace_event(sLine,TRACE);
			}

       }
       break;

    case CUP_T2_DATA : 
    case CUP_T1_DATA :
	case CUP_PIN_DATA:
        
			if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				
				if (debug_option == 0) 
					memset(sWork,'*',nLength);
				
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork);
				trace_event(sLine,TRACE);

			}

       break;
	   

    default :
        if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
          sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>"  , i+1 ,sWork );
          trace_event(sLine,TRACE);
		}

       break;
    }
 }
	sprintf(sLine,"</MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"</BaseMessage>");
	trace_event(sLine,TRACE);
   trace_event("------------------------------------",TRACE);
   /* PCI */
	memset(sWork,     0, sizeof(sWork));
	memset(sLine,     0, sizeof(sLine));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sTag,      0, sizeof(sTag));
	memset(sValue,    0, sizeof(sValue));
	memset(sLine2, '\0', sizeof(sLine2));
    memset(sTlvField, '\0', sizeof(sTlvField));

   trace_event("End   DumpFileCup()",PROCESSING);
   return;
}



void DumpCup( char *data )
{
        TSCupInfo  msgInfo;
        char       CardTrunc[22 + 1];
        char       sWork[1024];
        char       sLine[MAX_LINE_TRC];
        char       sTag [ 4 + 1], sValue[256];
	char	   szKeyZmk[32 + 1];
        int        nLen;
        int        nLength;
        int        iMapCtr,i,j,k;
        int        nPos = 0;

	unsigned char leftTag,rightTag;


	memset(sWork,     0, sizeof(sWork));
	memset(sLine,     0, sizeof(sLine));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sTag,      0, sizeof(sTag));
	memset(sValue,    0, sizeof(sValue));
        memset(szKeyZmk,  0, sizeof(szKeyZmk));

   

	trace_event("Start DumpCup()",PROCESSING);


	InitCupInfo(&msgInfo);

	AnalyseCup  ( data+ 4 , &msgInfo);

	iMapCtr = 1;
	while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
        	++iMapCtr;

      memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
      for (i=0; i<iMapCtr * 8;i++)
      {
                sprintf(sLine + nPos," %02X",(unsigned char) msgInfo.sBitMap[i]); /*NAB11032020*/
                nPos += 3;
      }
      trace_event(sLine,TRACE);

   trace_event("------------------------------------",TRACE);
   sprintf(sLine,"- M.T.I      : %04d" , msgInfo.nMsgType );
   trace_event(sLine,TRACE);
   trace_event("------------------------------------",TRACE);
   trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
   trace_event("------------------------------------",TRACE);

 for( i = 1 ; i < MAX_CUP_FIELDS ; i++)
 {
    switch (i)
    {
     case CUP_CARD_NBR : /* Mehdi Elyajizi PCI  Card number ne doit pas apparaitre */
		if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
			if (debug_option == 1)
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
			}
			else
			{        
				TrunCard(sWork, CardTrunc);
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s]" , i+1 , nLength, CardTrunc);
			}
			trace_event(sLine,TRACE);
		}
       break;



     case CUP_ADTNL_DATA_PRIV : 

                if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
                {  
		    if(memcmp(sWork , "NK", 2)== 0)
                        { 
			  sprintf(sLine,"- FLD (%03d) : (%03d) : [NK", i+1 , nLength );
                          if (debug_option == 1)
                              {

        	      		  HexToAscii(sWork + 2, szKeyZmk, 32);
	                          sprintf(sLine + strlen(sLine),"%s]", szKeyZmk );

				}
                          else
                                sprintf(sLine + strlen(sLine),"*******************] ");
                          
			}
		    else
			
		          sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );

			
	           trace_event(sLine,TRACE);

                }
       break;



    case CUP_ICC_DATA :
       if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
       {

          sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
          trace_event(sLine,TRACE);
          for( j = 0 ; j < nLength; )
          {
             memset(sTag, 0, sizeof(sTag));
             memset(sValue, 0, sizeof(sValue));

             if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
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

		nLen = (unsigned char)sWork[j++];

             /*nLen = sWork[j++];*/

             HexToAscii(sWork+j,sValue,nLen*2);

			if (debug_option != 1)
			{
				if( memcmp(sTag, "5F34", 4) == 0 || 
					memcmp(sTag, "9F27", 4) == 0 || 
					memcmp(sTag, "57", 2) == 0 || 
					memcmp(sTag, "5A", 2) == 0
					) 
					sprintf(sLine,"      > TAG (%.4s) ....: [***** Len %lu *****]" , sTag , strlen( sValue ));
				else
					sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
			}
			else
				sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
             trace_event(sLine,TRACE);

             j += nLen;
          }
       }
       break;

    case CUP_T2_DATA : /* Mehdi Elyajizi PCI  Pin block ne doit pas apparaitre */
        
			if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
			  if (debug_option == 1)
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
			  else
				sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
			  trace_event(sLine,TRACE);
			}
       break;
	   
    case CUP_T1_DATA : /* Mehdi Elyajizi PCI  Piste 1 ne doit pas apparaitre */
        
			if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				else
					sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
				trace_event(sLine,TRACE);
			}

       break;
	   
    case CUP_PIN_DATA:

			if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option == 1)
					sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				else
					sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
				trace_event(sLine,TRACE);
			}

       break;  /* Mehdi Elyajizi PCI  Pin block ne doit pas apparaitre */

    default :
        if ( GetCupField(i, &msgInfo, sWork, &nLength ) == SUCCES )
		{
          sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
          trace_event(sLine,TRACE);
		}

       break;
    }
	
 }
  if (debug_option == 1)
        DumpFileCup(data);

   trace_event("------------------------------------",TRACE);
   /* PCI */
	memset(sWork,     0, sizeof(sWork));
	memset(sLine,     0, sizeof(sLine));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sTag,      0, sizeof(sTag));
	memset(sValue,    0, sizeof(sValue));
   trace_event("End   DumpCup()",PROCESSING);
   return;
}





