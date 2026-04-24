#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <define.h>
#include <tlv_autho.h>
#include <tlv_private.h>
#include <dump.h>
#include <queue.h>
#include <commands.h>
#include <event_auth.h>
#include <iso_hps.h>
#include <security_data.h>
#include <iso_fields.h>
#include <resources.h>
#include <tlv_private.h>
#include <purge_time.h>


static int flag_alarm = FAUX;
extern int errno;

static void TimeOutOnReceive( int  X )
{
    flag_alarm = TRUE;
    return;
}

int FormatIsoServiceMessage( TSIsoInfo * IsoInfo , char *sFunctionCode , 
				char *sReasonCode  , char *Resource , pid_t nPid )
{
   char             data[256];
   char             date_gmt[10 + 1];
   int              data_Lgth;
   TSTlvPrivate  TlvInfo;
   char          tmpField[256];
   /* YOua 20Jun2004    char          sPid[6+1];  */
   char          sPid[8+1];
   int           tmpFieldLen;
   time_t          nPurgeTime;
   char            sPurgeTime[ 9 + 1];

   IsoInfo->nMsgType = 1606;
   memset ( sPurgeTime , 0 ,  9 + 1 );

   /* CHAMP 7  : XMIT DATE AND TIME */
       memset ( date_gmt , 0 , sizeof ( date_gmt ));
       current_gmt_date(date_gmt);
       AddIsoField( ISO_XMIT_TIME , IsoInfo , date_gmt + 2  , 10 );

   /* CHAMP 24 : FUNCTION CODE */
       AddIsoField( ISO_FUNCTION_CODE , IsoInfo , sFunctionCode  , 3 );

   /* CHAMP 25 : REASON CODE  */
      AddIsoField( ISO_MSG_REASON , IsoInfo , sReasonCode  , 4 );

/* Field 44: Additional Response Data */
   /* 44 in TLV format. */
   InitTlvPrivate (&TlvInfo);
   AddTlvPrivate (BAL_TYPE_TAG , &TlvInfo, "ADM" , 3 );
   tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
   AddIsoField (ISO_ADTNL_RESP_DATA , IsoInfo, tmpField, tmpFieldLen);


   /* CHAMP 53 : SECURITY DATA  */
      PutSecurityData(SECURITY_FORMAT  , tmpField , "00" , 2);
      PutSecurityData(PIN_BLOCK_FORMAT , tmpField , "00" , 2);
      PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , tmpField , "000" , 3);
      PutSecurityData(MAC_KEY_INDEX , tmpField , "000" , 3);

      /* YOua 20Jun2004 Gestion PID 6 a 8 */

      memset( sPid , 0, sizeof ( sPid ));
      sprintf (sPid , "%08d", getpid());
      PutSecurityData(SOURCE_PID , tmpField , sPid  , 8);
      PutSecurityData(SOURCE_RESOURCE , tmpField , "000000"  , 6);

      memset( sPid , 0, sizeof ( sPid ));
      sprintf (sPid , "%08d", nPid );
      PutSecurityData(DESTINATION_PID , tmpField ,  sPid  , 8);
      PutSecurityData(DESTINATION_RESOURCE , tmpField , Resource , 6);
      PutSecurityData(USER , tmpField , "000000000000000"  , 15);
      /* YOua 20Jun2004 */


      nTimerAcq   = 20;
      nIssAcqDelta = 3;
      GetIssuerPurgeTime (&nPurgeTime);


      sprintf ( sPurgeTime , "%09d",  nPurgeTime );
      PutSecurityData( PURGE_TIME, tmpField , sPurgeTime ,9);
      AddIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField, strlen(tmpField));


}

int FormatArguments ( char argc , char **argv , char *Resource , 
			char *FunctionCode , char *ReasonCode )
{

  int i;

  if (  ( argc  < 2 ) || 
        ( strlen (argv[1]) != 6 )
      )
      {
          fprintf ( stderr,"\nUse %s Resource -FfunctionCode -RreasonCode -Hhelp\n\n\n");
          exit(2);
      }
  
     memcpy ( Resource , argv[1] , 6 );
     
  for ( i =2 ; i < argc ; i++ )
      {
         if (
               ( memcmp ( argv[i] , "-F"  , 2 ) != 0 )  &&
               ( memcmp ( argv[i] , "-f"  , 2 ) != 0 ) &&

               ( memcmp ( argv[i] , "-R"  , 2 ) != 0 ) &&
               ( memcmp ( argv[i] , "-r"  , 2 ) != 0 ) &&

               ( memcmp ( argv[i] , "-H"  , 2 ) != 0 ) &&
               ( memcmp ( argv[i] , "-h"  , 2 ) != 0 ) 
              )
         { 
         	fprintf ( stderr,"\n Invalid Option \n");
          	fprintf ( stderr,"\nUse %s Resource -FfunctionCode -RreasonCode -Hhelp\n\n\n");
          	exit(2);
         }
         else
         {

              if (( memcmp ( argv[i] , "-F"  , 2 ) == 0 ) ||
                  ( memcmp ( argv[i] , "-f"  , 2 ) == 0 ) )
                  {
                     if (( strlen ( argv[i] ) - 2 ) != 3 )
                     {
         			fprintf ( stderr,"\n Function Code Lengtrh has to be equal to 3.\n");
          			exit(2);
         	     }
                        memcpy ( FunctionCode , argv[i] + 2 ,  strlen ( argv[i] ) - 2 );
                   }

              if (( memcmp ( argv[i] , "-R"  , 2 ) == 0 ) ||
                  ( memcmp ( argv[i] , "-r"  , 2 ) == 0 ) )
                  {
                     if (( strlen ( argv[i] ) - 2 ) != 4 )
                     {
         			fprintf ( stderr,"\n Reason Code Lengtrh has to be equal to 4.\n");
          			exit(2);
         	     }
                        memcpy ( ReasonCode , argv[i] + 2 ,  strlen ( argv[i] ) - 2 );
                   }

              if (( memcmp ( argv[i] , "-H"  , 2 ) == 0 ) ||
                  ( memcmp ( argv[i] , "-h"  , 2 ) == 0 ) )
                  {
                     fprintf ( stderr,"\n===========================================================================");
          	     fprintf ( stderr,"\nCommad %s Resource -FfunctionCode -RreasonCode -Hhelp");
                     fprintf ( stderr,"\n===========================================================================\n");
                     fprintf ( stderr,"\nFuntion Code Values  : ");       
                     fprintf ( stderr,"\n\t\t\t600 : Stop Request.");
                     fprintf ( stderr,"\n\t\t\t601 : Refresh Parameters.");
                     fprintf ( stderr,"\n\t\t\t602 : Resource Status Inquiry.");
                     fprintf ( stderr,"\n\t\t\t603 : sign-on by (BIN).");
                     fprintf ( stderr,"\n\t\t\t604 : sign-off by (BIN).");
                     fprintf ( stderr,"\n\t\t\t605 : sign-on by Group.");
                     fprintf ( stderr,"\n\t\t\t606 : sign-off by Group.");
                     fprintf ( stderr,"\n\t\t\t607 : Enter SI Mode.");
                     fprintf ( stderr,"\n\t\t\t608 : Stop SI Mode.");
                     fprintf ( stderr,"\n\t\t\t609 : echo test.");
                     fprintf ( stderr,"\n\t\t\t620 : Request For Statistics.");
                     fprintf ( stderr,"\n");
                     fprintf ( stderr,"\nReason Code Values  : ");       
                     fprintf ( stderr,"\n\t\t6600: Normal Stopping Procedure.");
                     fprintf ( stderr,"\n\t\t6601 : System malfunction.");
                     fprintf ( stderr,"\n\t\t6602 : maintenance.");
                     fprintf ( stderr,"\n\t\t6603 : stand in Traitement.");
                     fprintf ( stderr,"\n\t\t6604 : Issuer Problem.");
                     fprintf ( stderr,"\n\t\t6605 : Communication Problem.");
                     fprintf ( stderr,"\n\t\t6699 : Unspecified.");
                     fprintf ( stderr,"\n\n");
                     exit(2);
                   } 
           } 
        } 

  return ( 0 );
}


main ( char argc , char **argv )
{
   TSResources    sResource;
   TSIsoInfo      IsoInfo;
   TSBalExchange   sBalMessage;
   int            nRetCode;
   int            flag_alarm=NOK;
   char          tmpField[256];
   int           tmpFieldLen;
   int           nLength;
   int           BalStatus;
   char          Resource [ 6 + 1];
   char          FunctionCode  [ 3 + 1];
   char          ReasonCode  [ 4 + 1];


   memset (  Resource  , 0 , sizeof ( Resource ));
   memset (  FunctionCode   , 0 , sizeof (  FunctionCode ));
   memset (  ReasonCode   , 0 , sizeof (  ReasonCode ));


   memcpy ( FunctionCode , "602"  , 3 );
   memcpy ( ReasonCode   , "6699" , 4 );
   FormatArguments ( argc , argv , 
			     Resource , FunctionCode , 
				ReasonCode );


   if (( nBalAdm = OpenBalCom(BAL_ADM) ) < 0)
   {
      fprintf(stderr, "OpenBalCom(BAL_ADM) failed. \n");
      exit(2);
   }


   AllocateOracleCtx( 1);
   ConnectOracle ( 1 );

   memcpy (sResource.resource_id, Resource , 6);
   if ( GetResources ( 1, &sResource) != OK)
   {
      fprintf (stderr,"GetResources() failed.\n");
      DeconnectOracle(1);
      FreeOracleCtx(1);
      exit (2);
   }

    InitIsoInfo ( &IsoInfo );
    FormatIsoServiceMessage( &IsoInfo , FunctionCode , ReasonCode , 
                                 Resource , sResource.pris_pid );
    fprintf ( stderr,"\n==> Request to be sent....\n");

    sBalMessage.sHeader.nPartnerId = sResource.pris_pid,
    sBalMessage.sHeader.nSourceId  = getpid();
    sBalMessage.sHeader.nCommand   = 1600;
    nLength = IsoBuildMsg ( sBalMessage.sBody, &IsoInfo);
    sBalMessage.sHeader.nLen = nLength;

/*
    fprintf(stderr,"\n================",TRACE);
    fprintf(stderr,"\n== Deliver  ====",TRACE);
    fprintf(stderr,"\nPartner Id %d",sBalMessage.sHeader.nPartnerId);
    fprintf(stderr,"\nSource  Id %d",sBalMessage.sHeader.nSourceId);
    fprintf(stderr,"\nCommand    %d",sBalMessage.sHeader.nCommand);
    fprintf(stderr,"\nLength     %d",sBalMessage.sHeader.nLen);
    fprintf(stderr,"\n================\n",TRACE);
*/

    if ( (nRetCode = WriteBalMsg(nBalAdm , &sBalMessage)) != OK)
    {
          fprintf(stderr,"\nCan Not Write Message To Queue (%d)\n" , nRetCode );
          DeconnectOracle(1);
          FreeOracleCtx(1);
          exit (2);
    }

/*****************************************************************************/

   flag_alarm = FALSE;
   memset ( (char *)&sBalMessage , 0, sizeof ( TSBalExchange ));
   memset ( (char *)&IsoInfo     , 0, sizeof ( TSIsoInfo        ));

   install_handler (SIGALRM,TimeOutOnReceive);
   (void)alarm(5);

   do {
        BalStatus = ReadBalMsg(nBalAdm, getpid(), !IPC_NOWAIT, &sBalMessage);
      } while ((BalStatus == -1) && (flag_alarm == FALSE) && (errno != EINTR));
    (void)alarm(0);

    if (
                        ( BalStatus  == -1 )   ||
                        ( flag_alarm == TRUE )
        )
    {
          fprintf(stderr,"\nTime Out Event Situation\n", PROCESSING);
          DeconnectOracle(1);
          FreeOracleCtx(1);
          exit (2);
    }

   flag_alarm = FALSE;

   InitIsoInfo (&IsoInfo);
   AnalyseIso (sBalMessage.sBody, &IsoInfo);

   fprintf ( stderr,"\nMessage Received...\n");
   GetIsoField  (ISO_RESPONSE_CODE, &IsoInfo, tmpField, &tmpFieldLen );
   DeconnectOracle(1);
   FreeOracleCtx(1);
   exit(0);
}

