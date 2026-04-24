#include <define.h>
#include <sms_param.h>

/*ICH 20190427 use resource param*/
int  GetSmsParams (int  nIndexCtx, char* pResourceId)
{

	int				nLength;
	char			szParameters[2048 + 1];
   char			sDataField[256];

	memset(szParameters,'\0',sizeof(szParameters));
   memset(sDataField,'\0',sizeof(sDataField));

	if (GetResourceParams(nIndexCtx, pResourceId, szParameters) != OK) 
	{
		fprintf(stderr, "GetResourceParams() failed.\n");
		return(ERROR);
	}

	GetTlvBuffer (SMS_STATION_ID, szParameters, StationId, &nLength );
	GetTlvBuffer (SMS_ACQUIRER_ID, szParameters, AcquirerId, &nLength );	  
	GetTlvBuffer (SMS_PLUS_PMC_ID, szParameters, PlusPmc, &nLength );
	GetTlvBuffer (SMS_COUNTRY, szParameters, CountryCode, &nLength );
	GetTlvBuffer (SMS_ALPHA_CNTR_1, szParameters, AlphaCode3, &nLength );
	GetTlvBuffer (SMS_ALPHA_CNTR_2, szParameters, AlphaCode2, &nLength );
	GetTlvBuffer (SMS_BANK_LOCATION, szParameters, BankLocation, &nLength );
   /*Start IBO20200622 PLUTONL-1967*
	memset(g_sCaptureMsg, '0', 4);
	if (GetTlvBuffer (SMS_INC_REQ, szParameters, sDataField, &nLength ) == SUCCESS)
		g_sCaptureMsg[0]=sDataField[0];
	if (GetTlvBuffer (SMS_OUTG_RESP, szParameters, sDataField, &nLength ) == SUCCESS)
		g_sCaptureMsg[1]=sDataField[0];
	if (GetTlvBuffer (SMS_OUTG_REQ, szParameters, sDataField, &nLength ) == SUCCESS)
		g_sCaptureMsg[2]=sDataField[0];
	if (GetTlvBuffer (SMS_INC_RESP, szParameters, sDataField, &nLength ) == SUCCESS)
		g_sCaptureMsg[3]=sDataField[0];
	/*End IBO20200622 PLUTONL-1967*/

	return(OK);	
}


void DumpSmsGlobals ( )
{
   fprintf (stderr,"\n Comms Id ................. = %.2s ", CommsId );
   fprintf (stderr,"\n trace_file.................= %s ", trace_file  );
   fprintf (stderr,"\n trace_level .............. = %d ", trace_level );
   fprintf (stderr,"\n Acquirer Timer ........... = %d ", nTimerAcq );
   fprintf (stderr,"\n Delta Time Acquirer/Issuer = %d ", nIssAcqDelta);
   fprintf (stderr,"\n Min Troubles Ind ......... = %d ", MinTroublesInd );
   fprintf (stderr,"\n Max Troubles Ind ......... = %d ", MaxTroublesInd );
   fprintf (stderr,"\n Nbr Max Threads  ......... = %d ", max_thread  );
   fprintf (stderr,"\n Nbr User Threads ......... = %d ", nUserThread );
   fprintf (stderr,"\n Max Request .............. = %d ", max_request );
   fprintf (stderr,"\n Protocol ID .............. = %.2s ",  ProtocolId );
   fprintf (stderr,"\n Device Name .............. = %.15s ",  DeviceName );
   fprintf (stderr,"\n Remote Application Name... = %.15s ",  RemoteAppName );
   fprintf (stderr,"\n Remote Application Id..... = %.15s ",  RemoteAppId   );
   fprintf (stderr,"\n Local  Application Name... = %.15s ",  LocalAppName );
   fprintf (stderr,"\n Local  Application Id..... = %.15s ",  LocalAppId );
   fprintf (stderr,"\n Station Id ............... = %.6s ", StationId );
   fprintf (stderr,"\n Plus Pmc ................. = %.6s ", PlusPmc );
   fprintf (stderr,"\n Country Code.............. = %.3s ",  CountryCode );
   fprintf (stderr,"\n Alpha Code2............... = %.2s ", AlphaCode2 );
   fprintf (stderr,"\n Alpha Code3............... = %.3s ",  AlphaCode3 );
   fprintf (stderr,"\n Acq Inst Id............... = %.11s ",  AcquirerId );
   fprintf (stderr,"\n Bank Location............. = %.11s ", BankLocation );
   fprintf (stderr,"\n Acquirer Bank ............ = %.6s ", AcquirerBank );
   fprintf(stderr,"\n Capture Msg......= %.4s ", g_sCaptureMsg );   /*IBO20200622 PLUTONL-1967*/
   fprintf(stderr,"\n Auth Reply Back Period = %d ", g_nAuthReplyBackPeriod );/*ZKO10032022 PLUTONL-4411*/

   fprintf ( stderr,"\n");
}

int GetMaxSafCounter()
{
   return ( 10 );
}

GetAdviceInterval ( char *ResourceId )
{
   return ( 2 );
}

char GetDeclineAdvicesFlag ( char * Resource )
{
  return ( 'N' );
}
