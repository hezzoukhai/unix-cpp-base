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
#include <stdlib.h>
#include <globals.h>

#include <int1_struct.h>
#include <int1.h>



int Int1SafManagement( int nIndiceCtx , char *sResource )
{

	int	  nRetCode = NOK;
	char  sMti  [ 4 + 1];
	char  sStan [ 6 + 1];
	char  sSrcResource  [ 6 + 1];
	char  sResponseCode [ 4 + 1];
	int   nSendCounter;
	char  sCardNumber  [ 22 + 1 ];
	char  sTlvBuffer   [ LG_MAX ];


	memset ( sMti  ,         0 , sizeof ( sMti  ));
	memset ( sStan ,         0 , sizeof ( sStan ));
	memset ( sSrcResource  , 0 , sizeof ( sSrcResource ));
	memset ( sResponseCode , 0 , sizeof ( sResponseCode ));
	memset ( sCardNumber ,   0 , sizeof ( sCardNumber ));
	memset ( sTlvBuffer ,    0 , sizeof ( sTlvBuffer ));
	nRetCode = GetSafData  (	nIndiceCtx,  sResource , 'A', 
								'N', 
								sMti, sStan, sSrcResource, &nSendCounter,
								sResponseCode, sCardNumber , sTlvBuffer  );

	if( nRetCode != OK )
		return nRetCode;


	nRetCode = UserInt1SafProcMsg(nIndiceCtx,sMti,sStan,sSrcResource,nSendCounter,sResponseCode,sCardNumber,sTlvBuffer);

	return nRetCode;
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static int Int1SafResponseReceived (	int       nIndiceCtx , 
										char      *sPan ,		
										char      *sMti ,
										char      *sStan ,
										char      *sSourceResource, 
										char      *sResource,
										char      *sSlfMti ,
										char      *sSlfResponseCode )                    
{
	char	sLine  [ MAX_LINE_TRC ];
	char	sTruncCard[64];
	int  nLength;

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
	/************************************************************************/

	DeleteSafData ( nIndiceCtx,
					sPan,
					sMti, 
					'A' , 
					sStan, 
					sSourceResource , 
					sResource , 
					sSlfMti, 
					sSlfResponseCode);
	
	/* inform SAF controller that a response was received */
	SafRespReceived();

	return OK;
}
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
static int Int1SafTimeOutReceived ( int       nIndiceCtx , 
									char      *sPan ,		  
									char      *sMti , 
									char      *sStan ,
									char      *sSourceResource, 
									char      *sResource, 
									char      *sSlfMti , 
									char      *sSlfResponseCode,
									int       nSendCounter ) 
{
	int  nLength;
	char sLine [ MAX_LINE_TRC ];
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
	

	sprintf ( sLine ,"sStan ... .......= [%s] ", sStan );
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
		UpdateSafData ( nIndiceCtx, sMti, 'A' , sStan,
						sSourceResource, sResource, 
						nSendCounter);
	}
	else
	{
		DeleteSafData ( nIndiceCtx,sPan, sMti, 'A' , sStan,  /*MKB111007*/
		sSourceResource , sResource , 
		sSlfMti, sSlfResponseCode);
	}

	SafTimeOutDetected();

	trace_event("End   SafTimeOutReceived(OK)",PROCESSING);

	return OK;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int Int1SafResponseTraitement(	int nIndiceCtx , 
							char *sTlvBuffer , 
							char *sSlfMti,   
							char *sSlfResponseCode , 
							char *sInternalRespCode )
{
	TSTlvAutho      sTlvInfo;
	char       		sMti  [ 4 + 1 ];
	char       		sStan [ 6 + 1 ];
	char       		sResource[ 6 + 1 ];
	char       		sSrcResource[ 6 + 1 ];
	char            sTlvPrivateBuffer [ LG_MAX + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	char			sStandInResource [ 6 + 1 ];
	int        		nRetCode , nLength;

	char			sPan[22+ 1 ];

	trace_event ( "Saf Response Received Processing ..", PROCESSING ); 
	memset (  sMti         , 0 , sizeof ( sMti ));
	memset (  sStan        , 0 , sizeof ( sStan ));
	memset (  sResource    , 0 , sizeof ( sResource ));
	memset (  sSrcResource , 0 , sizeof ( sSrcResource ));
	memset (  sStandInResource  , 0 ,   sizeof ( sStandInResource ));
	memset (  sPan         , 0 , sizeof ( sPan ));

	InitTlvAutho( &sTlvInfo);
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);
	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sSrcResource, &nLength);
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sResource 	, &nLength);
	GetTlvAutho ( AUTO_MSG_TYPE            , &sTlvInfo, sMti       	, &nLength);
	GetTlvAutho ( AUTO_INTERNAL_STAN       , &sTlvInfo, sStan       , &nLength);

	GetTlvAutho ( AUTO_PAN       , &sTlvInfo, sPan       , &nLength);
	

	Int1SafResponseReceived( nIndiceCtx ,
						sPan,
						sMti , 
						sStan ,
						sSrcResource,  
						sResource,
						sSlfMti ,   
						sSlfResponseCode );
	


	PutTlvBuffer ( AUTO_RESPONSE_CODE         , sTlvBuffer , sInternalRespCode  , 3);
	memset ( sTlvPrivateBuffer , 0 , sizeof ( sTlvPrivateBuffer ));

	InitTlvPrivate(&sTlvPrivateInfo);
	if ( GetTlvBuffer ( AUTO_NETWORK_POS_DATA , sTlvBuffer , sTlvPrivateBuffer , &nLength ) == SUCCES )
		AnalyseTlvPrivate(sTlvPrivateBuffer,nLength,&sTlvPrivateInfo );
		

	PutTlvPrivate(RESPONSE_CODE_TAG ,&sTlvPrivateInfo, sSlfResponseCode , strlen(sSlfResponseCode));
	nLength = PrivateBuildTlv( sTlvPrivateBuffer , &sTlvPrivateInfo);
	PutTlvBuffer ( AUTO_NETWORK_POS_DATA ,sTlvBuffer,sTlvPrivateBuffer,nLength);

	memset ( sStandInResource  + 0 , '0'    , 6 );
	memcpy ( sStandInResource  + 2 ,  sMti  , 4 );
	PutTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer , sStandInResource , 6);


 memset (  sPan         , 0 , sizeof ( sPan ));
 trace_event ( "End SafResponseTraitement ..", PROCESSING ); 
	return ( OK );

}

/******************************************************************************/
/******************************************************************************/
int int1SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  )
{
	TSTlvAutho      sTlvInfo;
	char       		sMti  [ 4 + 1 ];
	char       		sStan [ 6 + 1 ];
	char       		sResource[ 6 + 1 ];
	char       		sSrcResource[ 6 + 1 ];
	char       		sSendCounter[ 8 + 1 ];
	char       		sPan[ 19 + 1 ];
	int        		nRetCode , nSendCounter , nLength;

	trace_event("Start SafTimeOutTraitement()",PROCESSING);
	PrintTlvBuffer(sTlvBuffer);


	memset (  sMti         , 0 , sizeof ( sMti ));
	memset (  sStan        , 0 , sizeof ( sStan ));
	memset (  sResource    , 0 , sizeof ( sResource ));
	memset (  sSendCounter , 0 , sizeof ( sSendCounter ));
	memset (  sSrcResource , 0 , sizeof ( sSrcResource ));
	memset (  sPan 	   , 0 , sizeof ( sPan ));

	InitTlvAutho( &sTlvInfo);                                                                                                      
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo);
	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sSrcResource, &nLength);
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sResource 	, &nLength);
	GetTlvAutho ( AUTO_MSG_TYPE            , &sTlvInfo, sMti       	, &nLength);
	GetTlvAutho ( AUTO_INTERNAL_STAN       , &sTlvInfo, sStan       , &nLength);
	GetTlvAutho ( AUTO_MESSAGE_NUMBER       ,&sTlvInfo, sSendCounter, &nLength);
	GetTlvAutho ( AUTO_PAN       	   ,&sTlvInfo, sPan, &nLength); 
	nSendCounter = atoi ( sSendCounter );

	Int1SafTimeOutReceived( nIndiceCtx ,
						sPan, 
						sMti,  sStan,  
						sSrcResource , sResource , sMti , 
						ISSUER_UNAVAILABLE_ACTION ,
						nSendCounter );
	memset (  sPan 	   , 0 , sizeof ( sPan ));
	trace_event("End   SafTimeOutTraitement()",PROCESSING);
	return(OK);

	return OK;
}

