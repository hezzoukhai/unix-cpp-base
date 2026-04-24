/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_sms.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <visa_param.h>

#define SUCCES  0
#define ECHEC  -1

static void GetRejectCode(reject_visa,RejectCode)
TSSmsHeader * reject_visa;
char * RejectCode;
{
   RejectCode[0] = ((reject_visa->abRejectCode[0] >> 4) & 0x0F) + 0x30  ;
   RejectCode[1] = (reject_visa->abRejectCode[0] & 0x0F) + 0x30  ;
   RejectCode[2] = ((reject_visa->abRejectCode[1] >> 4) & 0x0F) + 0x30  ;
   RejectCode[3] = (reject_visa->abRejectCode[1] & 0x0F) + 0x30  ;
}

void affiche_message_iso_VISA( char *data )
{
   char STRING_FIELD[512];
   char SUBFIELD[512];
   char TI[2];
   int nLength;
   int i,j,k;
   TSSmsInfo msgInfo;
   TSCpsInfo cpsInfo;
   TSPufInfo pufInfo;

	InitPufInfo(&pufInfo);
	InitSmsInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);

   AnalyseSms  ( data , &msgInfo);

fprintf(stderr,"\n--------------------------------------------------------------------------------");
fprintf(stderr,"\n               MESSAGE TYPE : %d\n", msgInfo.nMsgType );
fprintf(stderr,"--------------------------------------------------------------------------------");
fprintf(stderr,"\n-FLD (FIELD): CONTENT             \n");
fprintf(stderr,
"--------------------------------------------------------------------------------");
 for( i = 1 ; i < 127 ; i++)
 {
    switch (i)
    {
    case 61 :
       if ( GetSmsField(i, &msgInfo, data, &nLength ) == SUCCES )
       {
          fprintf(stderr,"\n- FLD (%03d) " , i+1 );
		  AnalyseCps    (  data  , &cpsInfo);
          for( j = 0 ; j < 18 ; j++)
        	if ( GetCpsSubField(j, &cpsInfo, data, &nLength ) == SUCCES )
                fprintf(stderr,"\n       > SUBFIELD (%02d) ..............: %s " 
                        , j+1 , data    );
       }
       break;

    case 62 :
        if ( GetSmsField(i, &msgInfo, data, &nLength ) == SUCCES )
         {
           fprintf(stderr,"\n- FLD (%03d) " , i+1 );
		   AnalysePuf    (  data  , &pufInfo);
           for( j = 0 ; j < 18 ; j++)
           if ( GetPufSubField(j, &pufInfo, data, &nLength ) == SUCCES )
                fprintf(stderr,"\n       > SUBFIELD (%02d) ..............: %s " 
                        , j+1 , data    );
        }
       break;


    default :
        if ( GetSmsField(i, &msgInfo, data, &nLength ) == SUCCES )
          fprintf(stderr,"\n- FLD (%03d) : %s" , i+1 , data );
       break;
    }
 }
}

void affiche_message_iso_VISA_V(data)
char *data;
{
   char * texte;
   char * data_snd;
   char   RejectCode[5];
   TSSmsHeader * header;
   TSSmsHeader * reject_visa;

fprintf(stderr,
"\n--------------------------------------------------------------------------------");
   fprintf(stderr,"\n			DEBUT DUMP MESSAGE  ");
fprintf(stderr,
"\n--------------------------------------------------------------------------------");
fprintf(stderr,"\n");
   header = ( TSSmsHeader *) data ;        

    fprintf ( stderr ,"\n H(01) Header Length.........: %02X",header->bHeaderLength );
    fprintf ( stderr ,"\n H(02) Header flag & format..: %02X", header->bHeaderFlag );
    fprintf ( stderr ,"\n H(03) Text Format...........: %02X",header->bTextFormat );
    fprintf ( stderr ,"\n H(04) Total Message Length..: %02X %02X",header->abMsgLength [0], header->abMsgLength [1]);
    fprintf ( stderr ,"\n H(05) Dest Station Id.......: %02X %02X %02X",header->abDestStationId [ 0], header->abDestStationId  [ 1], header->abDestStationId [ 2]);
    fprintf ( stderr ,"\n H(06) Src  Station Id.......: %02X %02X %02X",header->abSrcStationId [ 0], header->abSrcStationId  [ 1], header->abSrcStationId [ 2]);
    fprintf ( stderr ,"\n H(07) Round-Trip Ctrl Info..: %02X",header->bEchoInfo);
    fprintf ( stderr ,"\n H(08) Base I Flags..........: %02X %02X",header->abBaseIIFlags[0],  header->abBaseIIFlags[1]);
    fprintf ( stderr ,"\n H(09) Message Status Flags..: %02X %02X %02X",header->abMsgFlags[0], header->abMsgFlags[1] , header->abMsgFlags[2] );
    fprintf(stderr ,"\n H(10) Batch Number..........: %02X",header->bBatchNumber);
    fprintf(stderr ,"\n H(11) Reserved..............: %02X %02X %02X",header->abReserved [0] , header->abReserved [1] , header->abReserved [2]);
    fprintf(stderr ,"\n H(12) User Information......: %02X ",header->bUserInfo);
    fprintf(stderr ,"\n H(13) Bit Map...............: %02X %02X ",header->abBitMap[0] , header->abBitMap[1] , header->abBitMap[2]);
    fprintf(stderr ,"\n H(14) Bit Map(Rej Data Gp)..: %02X %02X",header->abRejectCode[0], header->abRejectCode[1]);

   
   if ( header->bHeaderLength >= 26 )     
   {

      fprintf ( stderr,"\n Message rejected \n");
      reject_visa = ( TSSmsHeader *) data;
      
      fprintf(stderr,"               MESSAGE WAS REJECTED\n");
      fprintf(stderr,"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      GetRejectCode(reject_visa,RejectCode);
      fprintf(stderr,"------> Reject Code :    %.4s                ", RejectCode);
      fprintf(stderr,"\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

      if ( reject_visa->abBitMap [0] & 0x80 )
      {
         texte = data + reject_visa->bHeaderLength ;
         header = ( TSSmsHeader *) texte ;
         data_snd = texte + header->bHeaderLength  ;
         affiche_message_iso_VISA(data);
      }
   }
   else
   {
         data_snd = data + header->bHeaderLength ;
         affiche_message_iso_VISA(data);
   }
fprintf(stderr,
"\n--------------------------------------------------------------------------------");
   fprintf(stderr,"\n			FIN DUMP MESSAGE  ");
fprintf(stderr,
"\n--------------------------------------------------------------------------------");
fprintf(stderr,"\n");

}
