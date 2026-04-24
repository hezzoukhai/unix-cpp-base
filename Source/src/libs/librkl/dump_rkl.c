/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*                                                                             */
/*                                                                             */
/* Description                                                                 */
/*                                                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_rkl.h>
#include <iso_cps.h>
#include <iso_puf.h>
/*#include <rkl_puf.h>*/
#include <rkl_param.h>

#define SUCCES  0
#define ECHEC  -1

/*void GetRejectCode(reject_rkl,RejectCode)
TSRklHeader * reject_rkl;
char * RejectCode;
{
   RejectCode[0] = ((reject_rkl->abRejectCode[0] >> 4) & 0x0F) + 0x30  ;
   RejectCode[1] = (reject_rkl->abRejectCode[0] & 0x0F) + 0x30  ;
   RejectCode[2] = ((reject_rkl->abRejectCode[1] >> 4) & 0x0F) + 0x30  ;
   RejectCode[3] = (reject_rkl->abRejectCode[1] & 0x0F) + 0x30  ;
}*/

void DumpRklData( char *data )
{
   char sPufData[LG_MAX];
   char sCpsData[LG_MAX];
   char sWork[LG_MAX];
   char sLine[MAX_LINE_TRC];
   char TI[2];
   int nLength;
   int iMapCtr,i,j,k,nPos=0;
   TSRklInfo msgInfo;
   TSCpsInfo cpsInfo;
   TSPufInfo pufInfo;
   /*TSRklPufInfo RklpufInfo;*/
   

	InitPufInfo(&pufInfo);
	/*InitRklPufInfo(&RklpufInfo);*/
	InitRklInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);


      AnalyseRkl  ( data , &msgInfo);
      nPos=0;

      iMapCtr = 1;
      while ( ( msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
      	++iMapCtr;

      memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
      for (i=0; i<iMapCtr * 8;i++)
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
   switch (i)
    {
/*    case 61 :
       if ( GetRklField(i, &msgInfo, sWork, &nLength ) == SUCCES )
       {
          sprintf(sLine,"- FLD (%03d) (%03d)" , i+1, nLength );
	  trace_event(sLine,TRACE);
	  AnalyseCps    (  sWork  , &cpsInfo);
	  memset(sCpsData, 0, sizeof(sCpsData));
          for( j = 0 ; j < 18 ; j++)
        	if ( GetCpsSubField(j, &cpsInfo, sCpsData, &nLength ) == SUCCES )
		{
                	sprintf(sLine,"      > SUBFIELD (%02d) ....: [%s]" 
                        							   , j+1 , sCpsData    );	
			trace_event(sLine,TRACE);
		}
       }
       break;
*/
    case 61 :
        if ( GetRklField(i, &msgInfo, sWork, &nLength ) == SUCCES )
            {
			char sLenG [LG_MAX];
			char sLenNameZmk [LG_MAX];
			char sNameZmk [LG_MAX];
			char sLenTMK [LG_MAX];
			char sTMK [LG_MAX];
			char sTMP [LG_MAX];
			char sLenCvTMK [LG_MAX];
			char sCvTMK [LG_MAX];

	  		memset(sLenG, 0, sizeof(sLenG));
	  		memset(sLenNameZmk, 0, sizeof(sLenNameZmk));
	  		memset(sNameZmk, 0, sizeof(sNameZmk));
	  		memset(sLenTMK, 0, sizeof(sLenTMK));
	  		memset(sTMK, 0, sizeof(sTMK));
	  		memset(sTMP, 0, sizeof(sTMP));
	  		memset(sLenCvTMK, 0, sizeof(sLenCvTMK));
	  		memset(sCvTMK, 0, sizeof(sCvTMK));

                        trace_event("Dump 62",TRACE);

			memcpy(sLenG,sWork,3);
			memcpy(sLenNameZmk,sWork+3,2);
			memcpy(sNameZmk,sWork+3+2,atoi(sLenNameZmk));
			memcpy(sLenTMK,sWork+3+2+atoi(sLenNameZmk),2);
			memcpy(sTMK,sWork+3+2+atoi(sLenNameZmk)+2,atoi(sLenTMK)*2);
			memcpy(sLenCvTMK,sWork+3+2+atoi(sLenNameZmk)+2+(atoi(sLenTMK)*2),2);
			memcpy(sCvTMK,sWork+3+2+atoi(sLenNameZmk)+2+(atoi(sLenTMK)*2)+2,atoi(sLenCvTMK)*2);

                        sprintf(sLine,"- Length of data  [%s]" , sLenG );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- Length Zmk Name [%s]" , sLenNameZmk );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- Zmk Name        [%s]" , sNameZmk );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- Length Tmk      [%s]" , sLenTMK );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- New Tmk         [%s]" , sTMK );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- Length KCV Tmk  [%s]" , sLenCvTMK );
                        trace_event(sLine,TRACE);
                        sprintf(sLine,"- KCV Tmk         [%s]" , sCvTMK );
                        trace_event(sLine,TRACE);
        }
       break;
   /*case 125 :
        if ( GetRklField(i, &msgInfo, sWork, &nLength ) == SUCCES )
         {
          sprintf(sLine,"- FLD (%03d) (%03d)" , i+1, nLength );
	  	  trace_event(sLine,TRACE);*/
         /* AnalyseRklPuf    (  sWork  , &RklpufInfo);*/
	  	/*  memset(sPufData, 0, sizeof(sPufData));
           for( j = 0 ; j < 12 ; j++)
           if ( GetRklPufSubField(j, &RklpufInfo, sPufData, &nLength ) == SUCCES )
	   {
                sprintf(sLine,"      > SUBFIELD (%02d) ....: [%d|%s]" 
                        							, j+1 , nLength,sPufData    );
                trace_event(sLine,TRACE);
				memset(sPufData, 0, sizeof(sPufData));*/
/*
	  			nPos = 31 + strlen(sPufData);

                for(k=0; k<nLength; k++)
                {
                        sprintf(sLine + nPos ,"%02X|" , sPufData[k] );
                        nPos += 3;
                }
                sprintf(sLine + nPos,"]" );
*/

	/*   }
        }
       break;*/
    default :
       	if ( GetRklField(i, &msgInfo, sWork, &nLength ) == SUCCES )
	    {
          		sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
	      		trace_event(sLine,TRACE);
        }
       break;
    }
 }
	
}

void DumpRkl(data)
char *data;
{
   char * texte;
   char   RejectCode[5];
  /* TSRklHeader * header;
   TSRklHeader * reject_rkl;*/
   char sLine[MAX_LINE_TRC];
	trace_event("Start DumpRkl()",PROCESSING);

   /* header = ( TSRklHeader *) data ;        
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
	sprintf(sLine,"- H(Header field) : CONTENT");
	trace_event(sLine,TRACE);
	sprintf(sLine,"-----------------------------------");
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(01) Header Length.........: %02X",header->bHeaderLength );
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(02) Header flag & format..: %02X", header->bHeaderFlag );
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(03) Text Format...........: %02X",header->bTextFormat );
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(04) Total Message Length..: %02X %02X",header->abMsgLength [0], header->abMsgLength [1]);
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(05) Dest Station Id.......: %02X %02X %02X",header->abDestStationId [ 0], 
																	header->abDestStationId  [ 1], header->abDestStationId [ 2]);
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(06) Src  Station Id.......: %02X %02X %02X",header->abSrcStationId [ 0], 
																    header->abSrcStationId  [ 1], header->abSrcStationId [ 2]);
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(07) Round-Trip Ctrl Info..: %02X",header->bEchoInfo);
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(08) Base I Flags..........: %02X %02X",header->abBaseIIFlags[0],  header->abBaseIIFlags[1]);
	trace_event(sLine,TRACE);
    sprintf ( sLine ,"- H(09) Message Status Flags..: %02X %02X %02X",header->abMsgFlags[0], 
                                                                                header->abMsgFlags[1] , header->abMsgFlags[2] );
	trace_event(sLine,TRACE);
    sprintf(sLine ,"- H(10) Batch Number..........: %02X",header->bBatchNumber);
	trace_event(sLine,TRACE);
    sprintf(sLine ,"- H(11) Reserved..............: %02X %02X %02X",header->abReserved [0] , 
                                                                                 header->abReserved [1] , header->abReserved [2]);
	trace_event(sLine,TRACE);
    sprintf(sLine ,"- H(12) User Information......: %02X ",header->bUserInfo);
	trace_event(sLine,TRACE);
   
   if ( header->bHeaderLength >= 26 )     
   {
      reject_rkl = ( TSRklHeader *) data;
      sprintf(sLine,"               MESSAGE WAS REJECTED");
	  trace_event(sLine,TRACE);
      sprintf(sLine,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	  trace_event(sLine,TRACE);
      GetRejectCode(reject_rkl,RejectCode);
      sprintf(sLine,"------> Reject Code :    %.4s                ", RejectCode);
	  trace_event(sLine,TRACE);
      sprintf(sLine,"-++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	  trace_event(sLine,TRACE);

      if ( reject_rkl->abBitMap [0] & 0x80 )
      {
         texte = data + reject_rkl->bHeaderLength ;
         header = ( TSRklHeader *) texte ;
         DumpRklData(data);
      }
   }*/
   /*else*/
         DumpRklData(data);

	trace_event("End   DumpRkl()",PROCESSING);
}
/*void DumpRklStruct(TSRklInfo  *RklInfo)
{
char sWork[LG_MAX];
int  Length;

trace_event("Start DumpRklStruct()",TRACE);
memset(sWork, 0, sizeof(sWork));

   Length = RklBuildMsg (sWork + RKL_HDR_LEN, RklInfo);
   RklBuildHeader(RESPONSE , &(RklInfo->sHeader), NULL, Length,StationId); 
   memcpy(sWork, (char *) &(RklInfo->sHeader), RKL_HDR_LEN); 

DumpRklData (sWork);
trace_event("End   DumpRklStruct()",TRACE);
   
}
*/
