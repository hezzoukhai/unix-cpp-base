#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <list_thr.h>
#include <resources.h>
#include <tlv_autho.h>

#include <tlv_privfields.h>
#include <tlv_private.h>
#include <visa_param.h>
#include <stdlib.h>
#include <globals.h>


int SafManagement( int nIndiceCtx , char *sResource )
{
	int				nRetCode = NOK;
	TSTlvAutho      sTlvInfo;
	char			sDate [ 13]; 
	char			sMti  [ 4 + 1];
	char			sStan [ 6 + 1];
	char			sSrcResource  [ 6 + 1];
	char			sResponseCode [ 4 + 1];
	int				nSendCounter, nMti, nLength;
	char			sBuffer      [ LG_MAX ];
	char			sSendCounter [ 8 + 1 ];
	char			sCardNumber  [ 22 + 1 ];
	char			sAuthoTable  [  3 + 1 ];
	char			sTlvBuffer   [ LG_MAX ];
	char            sRefNbr		 [12 + 1];/* ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	
	

	TSIsoInfo		sMsgIso;
	char			sLine [ MAX_LINE_TRC ];

	memset ( sDate ,         0 , sizeof ( sDate ));
	memset ( sMti  ,         0 , sizeof ( sMti  ));
	memset ( sStan ,         0 , sizeof ( sStan ));
	memset ( sSrcResource  , 0 , sizeof ( sSrcResource ));
	memset ( sResponseCode , 0 , sizeof ( sResponseCode ));
	memset ( sCardNumber ,   0 , sizeof ( sCardNumber ));
	memset ( sSendCounter ,  0 , sizeof ( sSendCounter));
	memset ( sBuffer ,       0 , sizeof ( sBuffer ));
	memset ( sTlvBuffer ,    0 , sizeof ( sTlvBuffer ));
	memset ( sAuthoTable  ,  0 , sizeof ( sAuthoTable ));
	memset ( sRefNbr      , 0, sizeof(sRefNbr));/* ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	

	InitIsoInfo ( &sMsgIso );

	/*ICH20191018 CIS/BASE1 Excp file mngt*/
	/*nRetCode = GetSafData  (	nIndiceCtx,  sResource , 'A',*/ 
	nRetCode = GetBase1SafData  (	nIndiceCtx,  sResource , 'A', 
									GetDeclineAdvicesFlag(), 
									sMti, sStan, sSrcResource, &nSendCounter,
									sResponseCode, sCardNumber , sTlvBuffer  );
	if( nRetCode != OK )
		return nRetCode;

	/************************************************************************/
	trace_event ( "Read   Saf Data", PROCESSING );
	trace_event ( "========= === ======", PROCESSING );

	if(debug_option ==1 )
	{
		sprintf ( sLine ,"sCardNumber ... = [%s] ", sCardNumber );
		trace_event ( sLine , PROCESSING );
	}
	else
	{
		TrunCard(sCardNumber, sBuffer);
		sprintf ( sLine ,"sCardNumber ... = [%s] ", sBuffer );
		trace_event ( sLine , PROCESSING );
	}

	sprintf ( sLine ,"sMti ... .......= [%s] ", sMti );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sStan ... ......= [%s] ", sStan );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sSrcResource ...= [%s] ", sSrcResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sResource ..... =  [%s] ", sResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"nSendCounter  ..= [%d] ", nSendCounter  );
	trace_event ( sLine , PROCESSING );
	/************************************************************************/
	/******************************************************************************/

	InitTlvAutho( &sTlvInfo);
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);
	PrintTlvBuffer (  sTlvBuffer );
	
	PutTlvAutho(AUTO_MSG_TYPE, &sTlvInfo, sMti, 4);

	SafTlvToIso ( nIndiceCtx ,  &sTlvInfo , &sMsgIso );
	dump_p7_struct ( &sMsgIso );


	PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sSrcResource      , 6);
	PutTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sResource         , 6);
	PutTlvAutho ( AUTO_AUTHO_FLAG          , &sTlvInfo,  "S"              , 1);
	PutTlvAutho ( AUTO_AUTHOR_SRC          , &sTlvInfo , "E0"             ,  2);

	sprintf ( sSendCounter , "%08d", nSendCounter  );
	PutTlvAutho ( AUTO_MESSAGE_NUMBER       , &sTlvInfo ,  sSendCounter   ,  8);
	AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	
	

	/************************************************************************/
	/************************************************************************/

	memset ( sBuffer , 0 , sizeof ( sBuffer ));
	nMti = atoi ( sMti );
	if ( nSendCounter > 0 )
	{
		memset ( sDate , 0, sizeof(sDate) );
		current_gmt_date(sDate);
		PutIsoField ( ISO_XMIT_TIME, &sMsgIso, sDate + 2, 10);
		sMsgIso.nMsgType = nMti + 1; 
	}

	switch(nMti)
	{
	case 1420 :
		nRetCode = FCT_REV_REQ_TO_NW(nIndiceCtx,  &sMsgIso, sTlvBuffer);
		if (nRetCode != OK )
		{
			sprintf (sLine, "Could not send SAF message %04d through line.", sMsgIso.nMsgType);
			trace_event ( sLine , PROCESSING );
			/* Start ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
			GetTlvAutho ( AUTO_RRN        ,&sTlvInfo, sRefNbr, &nLength);
			DeleteBase1SafData ( nIndiceCtx,sCardNumber, sMti, 'A' , sStan,
							sSrcResource , sResource , 
							sMti, MALFUNCTION_ACTION,
							sRefNbr);
			/* End ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
			return ( NOK );
		}
        break ;
	/*Start HAL ENHQ2.20 completion advice Art 1.1*/
	case 1120 :
		nRetCode = FCT_AUT_ADVICE_TO_NW(nIndiceCtx,  &sMsgIso, sTlvBuffer);
		if (nRetCode != OK )
		{
			sprintf (sLine, "Could not send SAF message %04d through line.", sMsgIso.nMsgType);
			trace_event ( sLine , PROCESSING );
			return ( NOK );
		}
	    break ;
	/*End HAL ENHQ2.20 completion advice Art 1.1*/		
		/******************************************************************************/
/*ICH20191018 CIS/BASE1 Excp file mngt*/
	case 1304:
		nRetCode = FCT_UPD_REQ_TO_NW(nIndiceCtx, &sMsgIso, sTlvBuffer, "Y");/*to indicate SAF*/
		if (nRetCode != OK) {
			sprintf(sLine, "Could not send SAF message %4d through line.",
				sMsgIso.nMsgType);
			trace_event(sLine, PROCESSING);
			return (NOK);
		}
		break ;

	default : 
		sprintf(sLine,"End SafManagement(UNKNOWN MSG(MALFUNCTION) %d",nMti);
		trace_event(sLine,TRACE);
		return(NOK);
	}

	trace_event ("Sent SAF message.", PROCESSING);
	nRetCode = WaitSafAnswer();
	trace_event ("Response to SAF message received.", PROCESSING);
	return ( nRetCode );


	return(NOK);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static int SafResponseReceived (	int       nIndiceCtx , 
									char      *sPan ,		
									char      *sMti ,
									char      *sStan ,
									char 	  *sRefNbr,/*ICH20191018 CIS/BASE1 Excp file mngt*/
									char      *sSourceResource, 
									char      *sResource,
									char      *sSlfMti ,
									char      *sSlfResponseCode )                    
{
	char	sLine  [ MAX_LINE_TRC ];
	char	sTruncCard[64];
	int		nLength;

	/************************************************************************/
	trace_event ( "Delete Saf Data", PROCESSING );
	trace_event ( "========= === ======", PROCESSING );
	sprintf ( sLine ,"sMti ... ..........= [%s] ", sMti );
	trace_event ( sLine , PROCESSING );
	
	memset(sTruncCard,'\0',sizeof(sTruncCard));


	if(debug_option ==1 )
	{
		sprintf ( sLine ,"PAN ... ..........= [%s] ", sPan );
		trace_event ( sLine , PROCESSING );
	}
	else
	{
		TrunCard(sPan, sTruncCard);
		sprintf ( sLine ,"PAN ... ..........= [%s] ", sTruncCard );
		trace_event ( sLine , PROCESSING );
	}

	sprintf ( sLine ,"sStan ... .........= [%s] ", sStan );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sSourceResource .. = [%s] ", sSourceResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sResource .........= [%s] ", sResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sSlfResponseCode ..= [%s] ", sSlfResponseCode  );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sSlfMti........... = [%s] ", sSlfMti           );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sRefNbr........... = [%s] ", sRefNbr           );
	trace_event ( sLine , PROCESSING );
	/************************************************************************/



	DeleteBase1SafData ( nIndiceCtx,sPan, sMti, 'A' , sStan,
					sSourceResource , sResource , 
					sSlfMti, sSlfResponseCode,
					sRefNbr);/*MJIDOR20191122 CIS/BASE1 Excp file mngt*/
	/* inform SAF controller that a response was received */
	SafRespReceived();

	return OK;
}
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static int SafTimeOutReceived ( int       nIndiceCtx , 
								char      *sPan ,
								char      *sMti , 
								char      *sStan ,
								char 	  *sRefNbr,/*ICH20191018 CIS/BASE1 Excp file mngt*/
								char      *sSourceResource, 
								char      *sResource, 
								char      *sSlfMti , 
								char      *sSlfResponseCode,
								int       nSendCounter ) 
{
	int		nLength;
	char	sLine [ MAX_LINE_TRC ];
	char	sTruncCard[64];

	/************************************************************************/
	trace_event("Start SafTimeOutReceived()",PROCESSING);

	trace_event ( "Update Saf Data", PROCESSING );
	trace_event ( "========= === ======", PROCESSING );
	sprintf ( sLine ,"sMti ... ........= [%s] ", sMti );
	trace_event ( sLine , PROCESSING );

	
	memset(sTruncCard,'\0',sizeof(sTruncCard));
	
	if (debug_option == 1)
	{
		sprintf ( sLine ,"PAN ... ..........= [%s] ", sPan ); 
		trace_event ( sLine , PROCESSING );
	}
	else
	{
		TrunCard(sPan, sTruncCard);
		sprintf ( sLine ,"PAN ... ..........= [%s] ", sTruncCard );
		trace_event ( sLine , PROCESSING );
	}

	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sStan ... .......= [%s] ", sStan );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sRefNbr ... .......= [%s] ", sRefNbr );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sSourceResource.. = [%s] ", sSourceResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"sResource ........= [%s] ", sResource );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"nSendCounter  ... = [%d] ", nSendCounter  );
	trace_event ( sLine , PROCESSING );
	sprintf ( sLine ,"MaxSendValue .. ..= [%d] ", GetMaxSafCounter());
	trace_event ( sLine , PROCESSING );
	/************************************************************************/

	if ( nSendCounter  <  GetMaxSafCounter() )
	{
		UpdateBase1SafData ( nIndiceCtx, sMti, 'A' , sStan,
							sSourceResource, sResource, 
							sRefNbr,/*ICH20191018 CIS/BASE1 Excp file mngt*/
							sPan/*ICH20191018 CIS/BASE1 Excp file mngt*/);
	}
	else
	{
		DeleteBase1SafData ( nIndiceCtx,sPan, sMti, 'A' , sStan,
							sSourceResource , sResource , 
							sSlfMti, sSlfResponseCode,
							sRefNbr);/*ICH20191018 CIS/BASE1 Excp file mngt*/
	}

	SafTimeOutDetected();

	trace_event("End   SafTimeOutReceived(OK)",PROCESSING);

	return OK;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int SafResponseTraitement(	int nIndiceCtx , 
							char *sTlvBuffer , 
							char *sSlfMti,   
							char *sSlfResponseCode , 
							char *sInternalRespCode )
{
	TSTlvAutho      sTlvInfo;
	char       		sMti  [ 4 + 1 ];
	char       		sStan [ 6 + 1 ];
	char       		sResource[ RESOURCE_ID_LEN + 1 ];
	char       		sSrcResource[ RESOURCE_ID_LEN + 1 ];
	int        		nLength;
	
	char			sPan[22+ 1 ];
	char            sRefNbr[12 + 1];/*ICH20191018 CIS/BASE1 Excp file mngt*/

	trace_event ( "Saf Response Received Processing ..", PROCESSING ); 

	memset (  sMti         , 0 , sizeof ( sMti ));
	memset (  sStan        , 0 , sizeof ( sStan ));
	memset (  sResource    , 0 , sizeof ( sResource ));
	memset (  sSrcResource , 0 , sizeof ( sSrcResource ));
	memset (  sPan         , 0 , sizeof ( sPan ));
	memset (  sRefNbr      , 0 , sizeof ( sRefNbr ));

	InitTlvAutho( &sTlvInfo);                                                                                                      
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);

	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sSrcResource, &nLength);
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sResource 	, &nLength);
	GetTlvAutho ( AUTO_MSG_TYPE            , &sTlvInfo, sMti       	, &nLength);
	GetTlvAutho ( AUTO_INTERNAL_STAN       , &sTlvInfo, sStan       , &nLength);

	GetTlvAutho ( AUTO_PAN       , &sTlvInfo, sPan       , &nLength);
	GetTlvAutho ( AUTO_RRN        ,&sTlvInfo, sRefNbr, &nLength);/*ICH20191018 CIS/BASE1 Excp file mngt*/

	SafResponseReceived( nIndiceCtx ,
						sPan,
						sMti , 
						sStan ,
						sRefNbr,/*ICH20191018 CIS/BASE1 Excp file mngt*/
						sSrcResource,  
						sResource,
						sSlfMti ,   
						sSlfResponseCode );
	
	memset (  sPan         , 0 , sizeof ( sPan ));
	trace_event ( "End SafResponseTraitement ..", PROCESSING ); 
	return ( OK );

}

/******************************************************************************/
/******************************************************************************/
int SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  )
{
	TSTlvAutho      sTlvInfo;
	char       		sMti  [ 4 + 1 ];
	char       		sStan [ 6 + 1 ];
	char       		sResource[ 6 + 1 ];
	char       		sSrcResource[ 6 + 1 ];
	char       		sSendCounter[ 8 + 1 ];
	char       		sPan[ 19 + 1 ];
	char            sRefNbr[12 + 1];/*ICH20191018 CIS/BASE1 Excp file mngt*/
	int        		nRetCode , nSendCounter , nLength;

	trace_event("Start SafTimeOutTraitement()",PROCESSING);


	PrintTlvBuffer(sTlvBuffer);


	memset (  sMti         , 0 , sizeof ( sMti ));
	memset (  sStan        , 0 , sizeof ( sStan ));
	memset (  sResource    , 0 , sizeof ( sResource ));
	memset (  sSendCounter , 0 , sizeof ( sSendCounter ));
	memset (  sSrcResource , 0 , sizeof ( sSrcResource ));
	memset (  sPan 	   	   , 0 , sizeof ( sPan ));
	memset(	  sRefNbr      , 0, sizeof(sRefNbr));/*ICH20191018 CIS/BASE1 Excp file mngt*/

	InitTlvAutho( &sTlvInfo);                                                                                                      
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);

	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sSrcResource, &nLength);
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sResource 	, &nLength);
	GetTlvAutho ( AUTO_MSG_TYPE            , &sTlvInfo, sMti       	, &nLength);
	GetTlvAutho ( AUTO_INTERNAL_STAN       , &sTlvInfo, sStan       , &nLength);
	GetTlvAutho ( AUTO_MESSAGE_NUMBER       ,&sTlvInfo, sSendCounter, &nLength);
	GetTlvAutho ( AUTO_PAN       	   ,&sTlvInfo, sPan, &nLength); 
	GetTlvAutho ( AUTO_RRN        ,&sTlvInfo, sRefNbr, &nLength);/*ICH20191018 CIS/BASE1 Excp file mngt*/

	nSendCounter = atoi ( sSendCounter );

	SafTimeOutReceived( nIndiceCtx ,
						sPan, 
						sMti,  sStan,sRefNbr,/*ICH20191018 CIS/BASE1 Excp file mngt*/  
						sSrcResource , sResource , sMti , 
						ISSUER_UNAVAILABLE_ACTION ,
						nSendCounter );

	memset (  sPan 	   , 0 , sizeof ( sPan ));
	trace_event("End   SafTimeOutTraitement()",PROCESSING);
	return(OK);

}

