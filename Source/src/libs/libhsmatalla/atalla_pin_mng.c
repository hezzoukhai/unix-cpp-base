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


int
	ProdPinOffsetOfcustomerPIN(int nIndexCtx,
								char    *sTpk_Zpk,
								char    *sPVK,
								char	*sPinBlk,
								char	*sPinBlkFmt,
								char 	*sPan,
								char 	*sPinOffset,
								int		sPinLength
				)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sDecTable[SIZE_KEY+1];
	char	sPanFormatted[12 + 1];
	char	sFormat[1 + 1];
	char	sPanPadded[16+1];
	char    sHsmResultCode[4 + 1];
	char	sPinLen[1 + 1];

	trace_event("Start ProdPinOffsetOfcustomerPIN()", PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDecTable,   '\0', sizeof(sDecTable));
	memset(sPanFormatted,'\0', sizeof(sPanFormatted));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sPanPadded,  '\0', sizeof(sPanPadded));
	memset(sPinLen,		'\0', sizeof(sPinLen));


	GetHsmDecTable(sDecTable);

	format_card_from_pan(sPanFormatted, sPan);

	if (strlen(sPanFormatted) < 16)
	{
		memset(sPanPadded, 'D',sizeof(sPanPadded) -1);
		memcpy(sPanPadded, sPanFormatted, strlen(sPanFormatted));
	}

	translate_pin_block_format(sFormat,sPinBlkFmt);
	sprintf(sPinLen, "%X",sPinLength);

	nResult = command_ibm_37(BuffSnd ,
							&nLengthSnd,
							sFormat,
							"",
							sTpk_Zpk,
							sDecTable,
							"0000",
							sPanFormatted,
							sPinLen,
							"F",
							sPVK,
							sPinBlk,
							sPanFormatted
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_ibm_37()", ERROR);
		trace_event("End   ProdPinOffsetOfcustomerPIN(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ProdPinOffsetOfcustomerPIN(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ProdPinOffsetOfcustomerPIN(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   ProdPinOffsetOfcustomerPIN(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_ibm_47(BuffRcv,
						nLengthRcv,
						sPinOffset,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   ProdPinOffsetOfcustomerPIN(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End ProdPinOffsetOfcustomerPIN(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   ProdPinOffsetOfcustomerPIN(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	/* PCI */
	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDecTable,   '\0', sizeof(sDecTable));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sPanPadded,  '\0', sizeof(sPanPadded));




	trace_event("End ProdPinOffsetOfcustomerPIN(OK)", PROCESSING);

	return (OK);
}

int	generate_ABA_PVV(
	int		nIndexCtx,
	 char	*pTpk_Zpk,
	 char	*pPvk,
	 char	*pPinBlk,
	 char	*pPinBlkFmt,
	 char	*pCardNbr,
	 char	*pPvkI,
	 char	*pPvv)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sPan[12 + 1];
	char	sFormat[1 + 1];
	char    sHsmResultCode[4 + 1];
	char    sLine[MAX_LINE_TRC];

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sHsmResultCode,   	'\0', sizeof(sHsmResultCode));

	if (debug_option == 1)
	{
		sprintf(sLine, "Start generate_ABA_PVV(%.16s,%.16s,%.2s,%.22s)",
			pTpk_Zpk, pPinBlk, pPinBlkFmt, pCardNbr);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("Start generate_ABA_PVV()", PROCESSING);

	format_card_from_pan(sPan, pCardNbr);

	translate_pin_block_format(sFormat,pPinBlkFmt);

	nResult = command_visa_37(BuffSnd ,
							&nLengthSnd,
							sFormat,
							"",
							pTpk_Zpk,
							pPvk,
							"",
							"0000",
							pPvkI,
							sPan +1,
							pPinBlk,
							sPan
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_visa_37()", ERROR);
		trace_event("End   generate_ABA_PVV(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_ABA_PVV(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_ABA_PVV(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   generate_ABA_PVV(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_visa_47(BuffRcv,
						nLengthRcv,
						pPvv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   generate_ABA_PVV(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End 	generate_ABA_PVV(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   generate_ABA_PVV(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	/* PCI */
	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));

	trace_event("End   generate_ABA_PVV(OK)", PROCESSING);

	return(OK);
}

int     PinFromTpkToLmk(
        int                             nIndexCtx,
        unsigned char   pTpkKey         [],
        unsigned char   pPinBlk         [],
        unsigned char   pPinBlkFmt      [],
        unsigned char   pCardNbr        [],
        unsigned char   pPinLmk         [])
{
        return NOK;

}

int Production(int nIndiceCtx, char *sTlvBuffer)
{
        return NOK;

}
