/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_visa.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <visa_param.h>

#define SUCCES  0
#define ECHEC  -1

static void GetRejectCode(reject_visa,RejectCode)
TSVisaHeader * reject_visa;
char * RejectCode;
{
   RejectCode[0] = ((reject_visa->abRejectCode[0] >> 4) & 0x0F) + 0x30  ;
   RejectCode[1] = (reject_visa->abRejectCode[0] & 0x0F) + 0x30  ;
   RejectCode[2] = ((reject_visa->abRejectCode[1] >> 4) & 0x0F) + 0x30  ;
   RejectCode[3] = (reject_visa->abRejectCode[1] & 0x0F) + 0x30  ;
}

void DumpVisaData( char *data )
{
   char sPufData[LG_MAX];
   char sCpsData[LG_MAX];
   char sWork[LG_MAX];
   char sLine[MAX_LINE_TRC];
   char TI[2];
   int nLength;
   int iMapCtr,i,j,k,nPos=0;
   TSVisaInfo msgInfo;
   TSCpsInfo cpsInfo;
   TSPufInfo pufInfo;
   

	InitPufInfo(&pufInfo);
	InitVisaInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);
	InitVisaPufInfo(&VisapufInfo);	/*AMER 20160727: PADSS Fix*/

      AnalyseVisa  ( data , &msgInfo);
      nPos=0;

      iMapCtr = 1;
      while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
      	++iMapCtr;

      memcpy(sLine,"- BIT MAP  :     ",17); nPos+=17;
      for (i=0; i<iMapCtr * 8;i++)
      {
                sprintf(sLine + nPos," %02X", msgInfo.sBitMap[i]);
		nPos += 3
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
    switch (i)
    {
    case 61 :
       if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
       {
          sprintf(sLine,"- FLD (%03d) (%d)" , i+1, nLength );
	  trace_event(sLine,TRACE);
	  AnalyseCps    (  sWork  , &cpsInfo);
	  memset(sCpsData, 0, sizeof(sCpsData));
          /*for( j = 0 ; j < 18 ; j++)   ENH 11.1 */
	  for( j = 0 ; j <20 ; j++) /*ENH 11.1*/
        	if ( GetCpsSubField(j, &cpsInfo, sCpsData, &nLength ) == SUCCES )
		{
                	sprintf(sLine,"      > SUBFIELD (%02d) ..............: [%s]" 
                        							   , j+1 , sCpsData    );	
			trace_event(sLine,TRACE);
		}
       }
       break;

    case 62 :
        if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
         {
          sprintf(sLine,"- FLD (%03d) (%d)" , i+1, nLength );
	  trace_event(sLine,TRACE);
          AnalysePuf    (  sWork  , &pufInfo);
	  memset(sPufData, 0, sizeof(sPufData));
           for( j = 0 ; j < 18 ; j++)
           if ( GetPufSubField(j, &pufInfo, sPufData, &nLength ) == SUCCES )
	   {
                sprintf(sLine,"      > SUBFIELD (%02d) ..............: [%s]" 
                        							, j+1 , sPufData    );
		trace_event(sLine,TRACE);
	   }
        }
       break;
    default :
        if ( GetVisaField(i, &msgInfo, sWork, &nLength ) == SUCCES )
	if ( i < 129 )
	{
          sprintf(sLine,"- FLD (%03d)   (%d)    [%s]" , i+1, nLength,sWork );
	  trace_event(sLine,TRACE);
	}
	else
	{
	  nPos = 0;
          sprintf(sLine,"- FLD (%03d)   (%03d)[" , i+1, nLength); nPos+=21;
	  for(j=0; j<nLength; j++)		
	  {
          	sprintf(sLine + nPos ,"0x%02X|" , sWork[j] );
		nPos += 5;
	  }
       	  fprintf(sLine + nPos,"]" );
	  trace_event(sLine,TRACE);
	}
       break;
    }
 }
}

void DumpVisa(data)
char *data;
{
   char * texte;
   char   RejectCode[5];
   TSVisaHeader * header;
   TSVisaHeader * reject_visa;

fprintf(stderr,"\n--------------------------------------------------------------------------------");
    header = ( TSVisaHeader *) data ;        
	fprintf(stderr,"\n-----------------------------------");
	fprintf(stderr,"\n- H(Header field) : CONTENT");
	fprintf(stderr,"\n-----------------------------------");
    fprintf ( stderr ,"\n H(01) Header Length.........: %02X",header->bHeaderLength );
    fprintf ( stderr ,"\n H(02) Header flag & format..: %02X", header->bHeaderFlag );
    fprintf ( stderr ,"\n H(03) Text Format...........: %02X",header->bTextFormat );
    fprintf ( stderr ,"\n H(04) Total Message Length..: %02X %02X",header->abMsgLength [0], header->abMsgLength [1]);
    fprintf ( stderr ,"\n H(05) Dest Station Id.......: %02X %02X %02X",header->abDestStationId [ 0], 
																	header->abDestStationId  [ 1], header->abDestStationId [ 2]);
    fprintf ( stderr ,"\n H(06) Src  Station Id.......: %02X %02X %02X",header->abSrcStationId [ 0], 
																    header->abSrcStationId  [ 1], header->abSrcStationId [ 2]);
    fprintf ( stderr ,"\n H(07) Round-Trip Ctrl Info..: %02X",header->bEchoInfo);
    fprintf ( stderr ,"\n H(08) Base I Flags..........: %02X %02X",header->abBaseIIFlags[0],  header->abBaseIIFlags[1]);
    fprintf ( stderr ,"\n H(09) Message Status Flags..: %02X %02X %02X",header->abMsgFlags[0], 
                                                                                header->abMsgFlags[1] , header->abMsgFlags[2] );
    fprintf(stderr ,"\n H(10) Batch Number..........: %02X",header->bBatchNumber);
    fprintf(stderr ,"\n H(11) Reserved..............: %02X %02X %02X",header->abReserved [0] , 
                                                                                 header->abReserved [1] , header->abReserved [2]);
    fprintf(stderr ,"\n H(12) User Information......: %02X ",header->bUserInfo);
   
   if ( header->bHeaderLength >= 26 )     
   {
      fprintf ( stderr,"\n Message rejected \n");
      reject_visa = ( TSVisaHeader *) data;
      
      fprintf(stderr,"               MESSAGE WAS REJECTED\n");
      fprintf(stderr,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      GetRejectCode(reject_visa,RejectCode);
      fprintf(stderr,"------> Reject Code :    %.4s                ", RejectCode);
      fprintf(stderr,"\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

      if ( reject_visa->abBitMap [0] & 0x80 )
      {
         texte = data + reject_visa->bHeaderLength ;
         header = ( TSVisaHeader *) texte ;
         affiche_message_iso_VISA(data);
      }
   }
   else
         DumpVisaData(data);
fprintf(stderr,"\n--------------------------------------------------------------------------------\n");
}
void DumpVisaStruct(TSVisaInfo  *VisaInfo)
{
char sWork[LG_MAX];
trace_event("Start DumpVisaStruct()",TRACE);
memset(sWork, 0, sizeof(sWork));

VisaBuildMsg(sWork, VisaInfo);

DumpVisaData (sWork);
trace_event("End   DumpVisaStruct()",TRACE);
	
}