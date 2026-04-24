#include <stdio.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <define.h>
#include <queue.h>
#include <p7_common.h>
#include <iso8583.h>
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>
#include <resources.h>


int GenerateZPK(		int 		nIndexCtx,
	char		sDesOption,
	char*		szZmkLmk,
	char*		szZpkLmk,
	char*		szZpkZmk,
	char*		szCheckValue,
    char*       keySchemeZmk, /* AHA20220928 PLUTONL-4980*/
    char*       KeyBlockVersionId /* AHA20220927 PLUTONL-4980*/
	)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start GenerateZPK()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );


	/*nResult = command_10(BuffSnd,
						&nLengthSnd,
						"1PUNE000",
						szZmkLmk,
						"D");
	if (nResult != OK)
	{
		trace_event("Err. In command_10()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						szZmkLmk,
						"G1PUNN000,16");
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateZPK(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateZPK(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateZPK(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_20(BuffRcv,
						nLengthRcv,
						szZpkLmk,
						szZpkZmk,
						szCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_20()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						szZpkZmk,
						szCheckValue,
						szZpkLmk
						);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   GenerateZPK(OK)",PROCESSING);
	return OK;
}


int GenerateResourceZPK(int nIndexCtx,
		char sDesOption,
		char* sResourceId,
		char* szZpkLmk,
		char* szZpkZmk,
		char* szCheckValue,
       char* keySchemeZmk, /* AHA20220928 PLUTONL-4980*/
       char* KeyBlockVersionId /* AHA20220927 PLUTONL-4980*/)
{
	char    szZmkLmk        [256];

	int		nResult;



	trace_event("Start GenerateResourceZPK()",PROCESSING);

	memset(szZmkLmk,			'\0', sizeof(szZmkLmk));

	nResult = GetZMK(nIndexCtx,sResourceId , szZmkLmk);
	if(nResult != OK)
	{
		trace_event("Err. In GetZMK()", ERROR);
		trace_event("End   GenerateResourceZPK(NOK)", PROCESSING);
		return (nResult);
	}

	/*start  AHA20220929 PLUTONL-4980*/
	nResult = GenerateZPK(	nIndexCtx,
							sDesOption,
							szZmkLmk,
							szZpkLmk,
							szZpkZmk,
							szCheckValue,
                            keySchemeZmk,
                            KeyBlockVersionId);
                  /*end  AHA20220929 PLUTONL-4980*/

	if(nResult != OK)
	{
		trace_event("Err. In GenerateZPK()", ERROR);
		trace_event("End   GenerateResourceZPK(NOK)", PROCESSING);
		return (nResult);
	}

	trace_event("End   GenerateResourceZPK(OK)",PROCESSING);
	return OK;


}

int     GenerateResourceTAK(int nIndexCtx,
							char sDesOption,
							char *pResourceId,
							char *pTakLmk,
							char *pTakZmk,
							char *pCheckValue)
{


	char    szZmkLmk        [256];
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start GenerateResourceZPK()",PROCESSING);

	memset(szZmkLmk,			'\0', sizeof(szZmkLmk));
	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );

	nResult = GetZMK(nIndexCtx,pResourceId , szZmkLmk);
	if(nResult != OK)
	{
		trace_event("Err. In GetZMK()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (nResult);
	}

	/*nResult = command_10(BuffSnd,
						&nLengthSnd,
						"1MDNE000",
						szZmkLmk,
						"D");
	if (nResult != OK)
	{
		trace_event("Err. In command_10()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						szZmkLmk,
						"G1MDNN000,16");
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateResourceTAK(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateResourceTAK(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateResourceTAK(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_20(BuffRcv,
						nLengthRcv,
						pTakLmk,
						pTakZmk,
						pCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_20()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						pTakZmk,
						pCheckValue,
						pTakLmk
						);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   GenerateResourceTAK(OK)",PROCESSING);
	return OK;
}
