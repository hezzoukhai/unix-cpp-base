#include <stdio.h>
#include <signal.h>
#include <hsm_inc.h>
#include <dump.h>
#include <queue.h>
#include <commands.h>
#include <event_auth.h>
#include <p7_define.h>
#include <tlv_privfields.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#define _EXT_VAR_
#include <thr_hsm.h>
#undef _EXT_VAR_
#include <math.h>
#include <openssl/sha.h>
#include <stdint.h>
#include <inttypes.h>


/** Ajout de la fonction HsmQueryBin le 10/10/2003*/

/******************************************************************************/
char 
	decomp_f(w)
	char            w;
{
	if ((w >= 0) && (w <= 9))
		return (w + 0x30);
	return (w + 0x37);
}


/******************************************************************************/
void 
	decomp(x, r)
	char            x;
char           *r;
{
	*(r + 1) = decomp_f((char) (x & 0x0F));
	*r = decomp_f((char) ((x & 0xF0) >> 4));
}


/******************************************************************************/
void 
	decomp_all(A, B)
	char            A[8];
char            B[16];
{
	int             i;

	for (i = 0; i < 8; i++)
		decomp(A[i], B + (2 * i));
}

/******************************************************************************/

void 
	decimalisation(table_decimalisation, cle)
	char            table_decimalisation[17];
char            cle[17];
{
	int             i;
	int             indice_decimal;

	for (i = 0; i < 16; i++) {
		if (cle[i] > 0x39)
			indice_decimal = cle[i] - 0x41 + 10;
		else
			indice_decimal = cle[i] - 0x30;
		cle[i] = table_decimalisation[indice_decimal];
	}
}

/******************************************************************************/

unsigned char 
	comp_g(w)
	unsigned char   w;
{
	if ((w >= '0') && (w <= '9'))
		return (w - 0x30);
	return (w - 0x37);
}


/******************************************************************************/
void 
	comp(x, r)
	unsigned char  *x;
unsigned char  *r;
{
	*r = comp_g(x[1]) + (16 * comp_g(x[0]));
}


/******************************************************************************/
void 
	comp_all(A, B)
	unsigned char   A[16];
unsigned char   B[8];
{
	int             i;

	for (i = 0; i < 8; i++)
		comp(A + (2 * i), (B + i));
}


/******************************************************************************/
void 
	x_or(entre1, entre2, sortie)
	unsigned char   entre1[16];
unsigned char   entre2[16];
unsigned char   sortie[16];
{
	int             i;
	unsigned char   Entree1[8];
	unsigned char   Entree2[8];
	unsigned char   Sortie[8];

	comp_all(entre1, Entree1);
	comp_all(entre2, Entree2);
	for (i = 0; i < 8; i++)
		*(Sortie + i) = *(Entree1 + i) ^ *(Entree2 + i);
	decomp_all(Sortie, sortie);
}


/******************************************************************************/
numeric_extract(entree, sortie)
	char           *entree, *sortie;
{
	int             i, j;

	i = 0;
	j = 0;

	memset(sortie, 0, 17);
	for (i = 0; i < 16; i++) {
		if (!isdigit(entree[i]))
			continue;
		sortie[j] = entree[i];
		j++;
	}
}
/******************************************************************************/
/* expand_pin.                                                                */
/* ----------                                                                 */
/******************************************************************************/
void 
	expand_pin(crypt_pin, pin_block)
	unsigned char  *crypt_pin;
unsigned char  *pin_block;
{
	int             i;
	unsigned char   a, b;
	char            ligne[MAX_LINE_TRC];
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start expand_pin(%.8s)", pin_block);
	else
		sprintf(ligne, "Start expand_pin()");
	trace_event("Start expand_pin()", PROCESSING);

	for (i = 0; i < 8; i++) {

		a = (*(pin_block + i) >> 4);
		b = (*(pin_block + i) & 0x0F);

		*(crypt_pin + (2 * i)) = a;
		*(crypt_pin + (2 * i + 1)) = b;
	}

	for (i = 0; i < 16; i++) {
		if ((*(crypt_pin + i) >= 0x00) && (*(crypt_pin + i) <= 0x09))
			*(crypt_pin + i) += 0x30;
		else if (*(crypt_pin + i) == 0x0A)
			*(crypt_pin + i) = 0x41;
		else if (*(crypt_pin + i) == 0x0B)
			*(crypt_pin + i) = 0x42;
		else if (*(crypt_pin + i) == 0x0C)
			*(crypt_pin + i) = 0x43;
		else if (*(crypt_pin + i) == 0X0D)
			*(crypt_pin + i) = 0x44;
		else if (*(crypt_pin + i) == 0X0E)
			*(crypt_pin + i) = 0x45;
		else if (*(crypt_pin + i) == 0X0F)
			*(crypt_pin + i) = 0x46;
		else
			*(crypt_pin + i) = 0x00;
	}
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End   expand_pin(%.16s)", crypt_pin);
	else
		sprintf(ligne, "End   expand_pin()");
	trace_event(ligne, PROCESSING);

	return;
}
/******************************************************************************/
/* Fonction compress_pin.                             */
/* ---------------------                                                 */
/******************************************************************************/
void 
	compress_pin(target, source)
	char           *target;
char           *source;
{
	int             i;
	unsigned char   a, b, c;

	trace_event("Start compress_pin()", PROCESSING);

	for (i = 0; i < 8; i++) {
		a = *(source + 2 * i);
		b = *(source + 2 * i + 1);
		c = 0x00;

		if (a == 0x41)
			a = 0x0A;
		else if (a == 0x42)
			a = 0x0B;
		else if (a == 0x43)
			a = 0x0C;
		else if (a == 0x44)
			a = 0x0D;
		else if (a == 0x45)
			a = 0x0E;
		else if (a == 0x46)
			a = 0x0F;

		if (b == 0x41)
			b = 0x0A;
		else if (b == 0x42)
			b = 0x0B;
		else if (b == 0x43)
			b = 0x0C;
		else if (b == 0x44)
			b = 0x0D;
		else if (b == 0x45)
			b = 0x0E;
		else if (b == 0x46)
			b = 0x0F;

		c = (unsigned char) ((a << 4) & 0xF0) | (b & 0x0F);

		*(target + i) = c;
	}

	trace_event("End   compress_pin()", PROCESSING);
	return;
}
/******************************************************************************/
/* calc_pin.                                                                  */
/* ----------                                                                 */
/******************************************************************************/
/*
int 
calc_pin(int nIndiceCtx, char *card, char *pin_key, char *pin)
{
int             i, retour;
char            sHsmResultCode[5];

char            sortie[17], entree[17], Buffer[512], Buffer_hsm_recu[512];
char            table_de_decimalisation[] = {
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5'};

trace_event("Start calc_pin()", PROCESSING);
memset(sHsmResultCode, 0, sizeof(sHsmResultCode));
memset(sortie, 0, sizeof(sortie));
memset(entree, 0, sizeof(entree));
memcpy(entree, "00000000", 8);
memcpy(entree + 8, card + strlen(card) - 8, 8);

command_HE(Buffer, pin_key, entree);
retour = HsmQuery(nIndiceCtx, Buffer, Buffer_hsm_recu);
if (retour == NOT_AVAILABLE) {
trace_event("HSM NOT AVAILABLE", ERROR);
return (NOK);
} else if (retour == HSM_MALFUNCTION) {
trace_event("MALFUNCTION ERROR", ERROR);
return (NOK);
}
retour = command_HF(Buffer_hsm_recu, sortie, sHsmResultCode);
if (retour != OK)
return (NOK);


for (i = 0; i < PINLENGTH; i++) {
if (sortie[i] == 'A')
sortie[i] = '0';
else if (sortie[i] == 'B')
sortie[i] = '1';
else if (sortie[i] == 'C')
sortie[i] = '2';
else if (sortie[i] == 'D')
sortie[i] = '3';
else if (sortie[i] == 'E')
sortie[i] = '4';
else if (sortie[i] == 'F')
sortie[i] = '5';
}

memcpy(pin, sortie, PINLENGTH);

trace_event("End   calc_pin()", PROCESSING);
return(0);
}
*/

/******************************************************************************/
/* calc_pin_block.                                                            */
/* ----------                                                                 */
/******************************************************************************/
/*
calc_pin_block(int nIndiceCtx, char *card, char *pin_key, char *pin_block)
{
char            pin[5];
char            value[3];
char            entree1[17];
char            entree2[17];

trace_event("Start calc_pin_block()", PROCESSING);

memset(entree1, 0, sizeof(entree1));
memset(entree2, 0, sizeof(entree2));
memset(pin, 0, sizeof(pin));
memset(value, 0, 3);

calc_pin(nIndiceCtx, card, pin_key, pin);

memset(entree1, 'F', 16);
sprintf(value, "%02d", PINLENGTH);
memcpy(entree1, value, 2);
memcpy(entree1 + 2, pin, PINLENGTH);

memcpy(entree2, "0000", 4);
memcpy(entree2 + 4, card + strlen((char *) card) - 13, 12);
x_or((unsigned char *) entree1, (unsigned char *) entree2, (unsigned char *) pin_block);

trace_event("End   calc_pin_block(VOID)", PROCESSING);
return;
}
*/
/******************************************************************************/
/* Fonction FctIncrimentRandBy1()                                             */
/* -------------------                                                        */
/******************************************************************************/
void FctIncrimentRandBy1(char *psRandIn, char *psRandOut) /** The Two Rand (AC) are passed in Bin Mode **/
{
	char  sTmpIn[16 + 1], sTmpOut[16 + 1];
	char  sLine[MAX_LINE_TRC];
	int   i;

	memset(sTmpIn, 0, sizeof(sTmpIn));
	memset(sTmpOut, 0, sizeof(sTmpOut));
	HexToAscii(psRandIn, sTmpIn, 16);

	sprintf(sLine, "START FctIncrimentRandBy1(psRandIn=[%s])", sTmpIn);
	trace_event(sLine, PROCESSING);


	for ( i=7; i>-1; --i)
	{
		if( 0xFF > psRandIn[i] )
		{
			psRandOut[i] = psRandIn[i] + 1;
			break;
		}
		else if( 0xFF == psRandIn[i] && 0 == i )
			memset(psRandOut, 0, 8);
	}

	HexToAscii(psRandOut, sTmpOut, 16);

	sprintf(sLine, "END   FctIncrimentRandBy1(psRandOut=[%s])", sTmpOut);
	trace_event(sLine, PROCESSING);
	return;
}
int
	build_crpt_data_17(
	int		nIndexCtx,
	char	*sTlvBuffer,
	char	*sCrptData,
	int*	pnCryptDataLen,
	char	*sHsmResultCode
	)
{

	unsigned char   sAtc[2 + 1];
	unsigned char   sUn[4 + 1];
	unsigned char   sCrptAmount[6 + 1];
	unsigned char   sCvr[4 + 1];
	char            sIccData[512];
	char            sIAD[128 +1];
	char			szWork[256];
	char            vCvr[32 + 1];
	int				nIccLen;
	int				nLenWork;
	int				nLenData;
	TSTlvPrivate    kTlvInfo;

	trace_event("Start build_crpt_data_17()", PROCESSING);

	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(sCrptAmount, 0, sizeof(sCrptAmount));
	memset(sCvr, 0, sizeof(sCvr));
	memset(sIccData, 0, sizeof(sIccData));
	memset(sIAD, 0, sizeof(sIAD));
	memset(szWork, 0, sizeof(szWork));
	memset(vCvr, 0, sizeof(vCvr));
	InitTlvPrivate(&kTlvInfo);


	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nIccLen) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nLenWork = AnalyseTlvPrivate(sIccData, nIccLen, &kTlvInfo);


	nLenData = 0;



	if (GetTlvPrivate(CHIP_TRANS_AMOUNT_TAG, &kTlvInfo, szWork, &nLenWork) != SUCCESS)
		sprintf(szWork, "%012d", 0);

	AsciiToHex(szWork, sCrptAmount, 12);

	memcpy(sCrptData + nLenData, sCrptAmount, 6);
	nLenData += 6;

	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sUn, 8);

	memcpy(sCrptData + nLenData, sUn, 4);
	nLenData += 4;

	if (GetTlvPrivate(CHIP_ATC_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sAtc, 4);

	memcpy(sCrptData + nLenData, sAtc, 2);
	nLenData += 2;


	if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &kTlvInfo, sIAD, &nLenWork) != SUCCESS)
		sprintf(sIAD, "%32d", 0);


	if (GetTlvPrivate(CHIP_CVR, &kTlvInfo, vCvr, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-CVR", 4);
		trace_event("INVALID CVR ", ERROR);
		trace_event("END   check_crpt_data(KEY_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nLenWork = strlen(vCvr);

	AsciiToHex(vCvr+2, sCvr, 2);
	memcpy(sCrptData + nLenData, sCvr, 1);
	nLenData += 1;


	(*pnCryptDataLen) = nLenData;

	trace_event("End   build_crpt_data_17(OK)", PROCESSING);
	return OK;
}


/******************************************************************************/
int
	build_crpt_data_dflt(
	int		nIndexCtx,
	char	*sTlvBuffer,
	char	*sCVN,
	char	*sCrptData,
	int*	pnCryptDataLen,
	char	*sHsmResultCode
	)
{

	unsigned char   sAtc[2 + 1];
	unsigned char   sUn[4 + 1];
	unsigned char   sCrptAmount[6 + 1];
	unsigned char   sOthrAmount[6 + 1];
	unsigned char   sCountryCode[2 + 1];
	unsigned char   sTvr[5 + 1];
	unsigned char   sCrncyCode[2 + 1];
	unsigned char   sTrxDate[3 + 1];
	unsigned char   sTrxType[1 + 1];
	unsigned char   sAip[2 + 1];
	unsigned char   sCvr[4 + 1];
	char            sIccData[512];
	char            sIAD[128 +1];
	char			szWork[256];
	char            vCvr[32 + 1];
	char			sNetworkCode[2 + 1];
	int				nIccLen;
	int				nLenWork;
	int				nLenData;
	TSTlvPrivate    kTlvInfo;

	trace_event("Start build_crpt_data_dflt()", PROCESSING);

	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(sCrptAmount, 0, sizeof(sCrptAmount));
	memset(sOthrAmount, 0, sizeof(sOthrAmount));
	memset(sCountryCode, 0, sizeof(sCountryCode));
	memset(sTvr, 0, sizeof(sTvr));
	memset(sCrncyCode, 0, sizeof(sCrncyCode));
	memset(sTrxDate, 0, sizeof(sTrxDate));
	memset(sTrxType, 0, sizeof(sTrxType));
	memset(sAip, 0, sizeof(sAip));
	memset(sCvr, 0, sizeof(sCvr));
	memset(sIccData, 0, sizeof(sIccData));
	memset(sIAD, 0, sizeof(sIAD));
	memset(szWork, 0, sizeof(szWork));
	memset(vCvr, 0, sizeof(vCvr));
	memset(sNetworkCode, 0, sizeof(sNetworkCode));
	InitTlvPrivate(&kTlvInfo);


	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nIccLen) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nLenWork = AnalyseTlvPrivate(sIccData, nIccLen, &kTlvInfo);


	GetTlvBuffer(AUTO_NETWORK_CODE, sTlvBuffer, sNetworkCode, &nLenWork);
	nLenData = 0;



	if (GetTlvPrivate(CHIP_TRANS_AMOUNT_TAG, &kTlvInfo, szWork, &nLenWork) != SUCCESS)
		sprintf(szWork, "%012d", 0);

	AsciiToHex(szWork, sCrptAmount, 12);

	memcpy(sCrptData + nLenData, sCrptAmount, 6);
	nLenData += 6;

	if (GetTlvPrivate(CHIP_OTHER_AMOUNT_TAG, &kTlvInfo, szWork, &nLenWork) != SUCCESS)
		sprintf(szWork, "%012d", 0);

	AsciiToHex(szWork, sOthrAmount, 12);

	memcpy(sCrptData + nLenData, sOthrAmount, 6);
	nLenData += 6;


	if (GetTlvPrivate(CHIP_TERM_COUNTRY_CODE_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sCountryCode, 4);
	else
	{
		memcpy(sHsmResultCode, "---1", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TERM_COUNTRY_CODE_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	memcpy(sCrptData + nLenData, sCountryCode, 2);
	nLenData += 2;


	if (GetTlvPrivate(CHIP_TVR_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sTvr, 10);
	else
	{
		memcpy(sHsmResultCode, "---2", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TVR_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	memcpy(sCrptData + nLenData, sTvr, 5);
	nLenData += 5;

	if (GetTlvPrivate(CHIP_TRANS_CUR_CODE_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sCrncyCode, 4);
	else
	{
		memcpy(sHsmResultCode, "---3", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_CUR_CODE_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}


	memcpy(sCrptData + nLenData, sCrncyCode, 2);
	nLenData += 2;

	if (GetTlvPrivate(CHIP_TRANS_DATE_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sTrxDate, 6);
	else
	{
		memcpy(sHsmResultCode, "---4", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_DATE_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	memcpy(sCrptData + nLenData, sTrxDate, 3);
	nLenData += 3;

	if (GetTlvPrivate(CHIP_TRANS_TYPE_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sTrxType, 2);
	else
	{
		memcpy(sHsmResultCode, "---3", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_TYPE_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	memcpy(sCrptData + nLenData, sTrxType, 1);
	nLenData += 1;


	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sUn, 8);

	memcpy(sCrptData + nLenData, sUn, 4);
	nLenData += 4;

	if (GetTlvPrivate(CHIP_AIP_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sAip, 4);
	else
	{
		memcpy(sHsmResultCode, "---4", 4);
		trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_AIP_TAG>> NOT RECEIVED <==", ERROR);
		trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	memcpy(sCrptData + nLenData, sAip, 2);
	nLenData += 2;

	/* ATC */

	sAtc[0] = 0x30;
	sAtc[1] = 0x30;
	if (GetTlvPrivate(CHIP_ATC_TAG, &kTlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sAtc, 4);

	memcpy(sCrptData + nLenData, sAtc, 2);
	nLenData += 2;




	if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &kTlvInfo, sIAD, &nLenWork) != SUCCESS)
		sprintf(sIAD, "%32d", 0);

	/*TODO: CPA as well*/
	if ( memcmp(sNetworkCode, NET_DINERS, NET_LEN) == 0 ||
		(memcmp(sNetworkCode, NET_VISA, NET_LEN) == 0 && memcmp(sCVN,EMV_CVN_18,EMV_CVN_LEN) == 0) )
	{
		AsciiToHex(sIAD, szWork, nLenWork);
		memcpy(sCrptData + nLenData, szWork, nLenWork/2);
		nLenData += nLenWork/2;
	}
	else
	{
		if (GetTlvPrivate(CHIP_CVR, &kTlvInfo, vCvr, &nLenWork) != SUCCESS)
		{
			memcpy(sHsmResultCode, "-CVR", 4);
			trace_event("INVALID CVR ", ERROR);
			trace_event("END   check_crpt_data(KEY_UNAVAILABLE)", PROCESSING);
			return (HSM_MALFUNCTION);
		}

		nLenWork = strlen(vCvr);

		AsciiToHex(vCvr, sCvr, nLenWork);
		memcpy(sCrptData + nLenData, sCvr, nLenWork / 2);
		nLenData += nLenWork / 2;
	}

	(*pnCryptDataLen) = nLenData;

	memset(sAtc, 0, sizeof(sAtc));
	memset(sIccData, 0, sizeof(sIccData));
	memset(szWork, 0, sizeof(szWork));
	InitTlvPrivate(&kTlvInfo);

	trace_event("End   build_crpt_data_dflt(OK)", PROCESSING);
	return OK;
}

/******************************************************************************/
int get_emv_param(	char* sNetworkCode,
							char* sIAD,
							char* sEmvCardInfoData,
							char* sCVN,
							int nPanLength) /*SKA06042023 - PLUTONL-6022*/

{

	trace_event("Start get_emv_param()", PROCESSING);


	if ( memcmp(sNetworkCode, NET_VISA, NET_LEN) == 0)
	{
		if (nPanLength > 16)  /*SKA05042023 - PLUTONL-6022*/
		{
			sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_B;
		}
		else 
		{
			sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		}
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;



		memcpy(sCVN,sIAD+4,EMV_CVN_LEN);	/*IAD format 2 not supported yet*/

		if( memcmp(sCVN,EMV_CVN_18,EMV_CVN_LEN) == 0 )
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_CSK;
			sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_2;	/*VIS 1.6 uses ARPC Method2?*/
		}
		else
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_NONE;
		}
	}
	else if ( memcmp(sNetworkCode, NET_MASTERCARD, NET_LEN) == 0)
	{
		sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;
		sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_MCI_SKD;

		memcpy(sCVN,sIAD+2,EMV_CVN_LEN);
		if( memcmp(sCVN,EMV_CVN_12,EMV_CVN_LEN) == 0 || memcmp(sCVN,EMV_CVN_13,EMV_CVN_LEN) == 0)
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_EMV2000;
		}
		else if( memcmp(sCVN,EMV_CVN_14,EMV_CVN_LEN) == 0 || memcmp(sCVN,EMV_CVN_15,EMV_CVN_LEN) == 0)
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_CSK;
		}
		else
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_MCI_SKD;
		}

	}
	else if ( memcmp(sNetworkCode, NET_CUP, NET_LEN) == 0)
	{
		memcpy(sCVN,sIAD+4,EMV_CVN_LEN);

		sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;
		sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_UPI_SKD;
	}
	else if ( memcmp(sNetworkCode, NET_AMEX, NET_LEN) == 0)
	{
		memcpy(sCVN,sIAD+4,EMV_CVN_LEN);

		sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;
		sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_NONE;
	}
	else if ( memcmp(sNetworkCode, NET_DINERS, NET_LEN) == 0)
	{
		memcpy(sCVN,sIAD+2,EMV_CVN_LEN);

		sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;
		sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_CSK;
	}
	else if ( memcmp(sNetworkCode, NET_JCB, NET_LEN) == 0)
	{
		memcpy(sCVN,sIAD+4,EMV_CVN_LEN);

		sEmvCardInfoData[EMV_OFST_MKD] = EMV_MKD_A;
		sEmvCardInfoData[EMV_OFST_ARPC_MTHD] = EMV_ARPC_MTHD_1;

		if( memcmp(sCVN,EMV_CVN_01,EMV_CVN_LEN) == 0 )
		{
			/*should work with VISA CVN 10 as well? scheme A*/
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_NONE;
		}
		else if( memcmp(sCVN,EMV_CVN_02,EMV_CVN_LEN) == 0 )
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_JCB_SKD;
		}
		else if( memcmp(sCVN,EMV_CVN_04,EMV_CVN_LEN) == 0 )
		{
			sEmvCardInfoData[EMV_OFST_SKD] = EMV_SKD_CSK;
		}
		else
		{
			return NOK;
		}
	}
	

	trace_event("End   get_emv_param(OK)", PROCESSING);
	return OK;
}
/*****************************************************************************/
int CompressAmount(char * TxAmount, unsigned char *sCompressedAmount){

	float 	sRes;
	int 	nAmount = atoi(TxAmount);
	char    szWork[20 + 1];

	memset(szWork               , 0, sizeof(szWork));

	if(nAmount > 140000){

		sRes = log10(atoi(TxAmount) / 1000) * 6553.6;
		sprintf(sCompressedAmount, "%4X", sRes);

	}else{

		sprintf(sCompressedAmount, "%4X", nAmount/10);

	}

	return 0;
}
/*****************************************************************************/
/*NAB10082020 decommented PLUTONL-2170*/
int Sha256_ALgo(char *sData, char * sDataHash){

    SHA256_CTX ctx;
    uint8_t results[SHA256_DIGEST_LENGTH];
    int n;

    n = strlen(sData);
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (uint8_t *)sData, n);
    SHA256_Final(results, &ctx);

    /* Print the digest as one long hex value */

    for (n = 0; n < SHA256_DIGEST_LENGTH; n++)
        sprintf(sDataHash + 2*n, "%02X", results[n]);

	return 0;
}
/*****************************************************************************/
int 
	HsmQuery(int nIndiceCtx, pid_t nHsmPid, char *BuffSnd, char *BuffRcv)
{
	int             BalStatus;
	TSHsmExchange   sBalMessage;
	char            sLine[MAX_LINE_TRC];


	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));
	memset(tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal));
	sBalMessage.sHeader.nIndex = nIndiceCtx;
	sBalMessage.sHeader.tid = pthread_self();
	sBalMessage.sHeader.nCoNumber = GetNextCoNumber(nIndiceCtx);

	sBalMessage.sHeader.nPartnerId = nHsmPid;
	sBalMessage.sHeader.nSourceId = getpid();
	sBalMessage.sHeader.nCommand = HSM_REQUEST;
	memcpy(sBalMessage.sBody, BuffSnd, strlen(BuffSnd));
	sBalMessage.sHeader.nLen = strlen(BuffSnd);

	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		sprintf(sLine, "TO HSM:%d-->%s|", sBalMessage.sHeader.nLen, sBalMessage.sBody);
		trace_event(sLine, TRACE);
	}

	/*EBE140921: Moved from WaitHsmResponse*/
	pthread_mutex_lock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));

	if (WriteBalHsm(nBalHsm, &sBalMessage) != OK) {
		trace_event("Can Not Write Message To Hsm Queue", PROCESSING);
		trace_event("End  HsmQuery ( NOK )", PROCESSING);
		pthread_mutex_unlock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));
		return (HSM_MALFUNCTION);
	}
	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));


	BalStatus = WaitHsmResponse(nIndiceCtx, HSM_TIME_OUT_VALUE, BuffRcv);

	/*EBE140921: Moved from WaitHsmResponse*/
	memset(tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal));
	pthread_mutex_unlock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));


	if (BalStatus != OK) {
		trace_event("Time Out Situation With HSM", PROCESSING);
		trace_event("End HsmQuery ( NOK )", PROCESSING);
		return (NOT_AVAILABLE);
	}

	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		sprintf(sLine, "FROM HSM: <--%s|", BuffRcv);
		trace_event(sLine, TRACE);
	}
	trace_event("End HsmQuery ( OK )", PROCESSING);
	return (OK);
}


int  HsmQueryBin(int nIndiceCtx, pid_t nHsmPid, char *BuffSnd, int nLength, char *BuffRcv)
{
	int             BalStatus;
	TSHsmExchange   sBalMessage;
	char            sLine[MAX_LINE_TRC];


	trace_event("Start HsmQueryBin ()", PROCESSING);

	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));
	memset(tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal));
	sBalMessage.sHeader.nIndex = nIndiceCtx;
	sBalMessage.sHeader.tid = pthread_self();
	sBalMessage.sHeader.nCoNumber = GetNextCoNumber(nIndiceCtx);

	sBalMessage.sHeader.nPartnerId = nHsmPid;
	sBalMessage.sHeader.nSourceId = getpid();
	sBalMessage.sHeader.nCommand = HSM_REQUEST;
	memcpy(sBalMessage.sBody, BuffSnd, nLength);
	sBalMessage.sHeader.nLen = nLength;


	sprintf(sLine, "GLOBAL PartnerID = %d; SourceID = %d", nHsmPid, getpid());
	trace_event(sLine, TRACE);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		/** sprintf(sLine, "TO HSM:%d-->%s|", sBalMessage.sHeader.nLen, sBalMessage.sBody); **/
		sprintf(sLine, "TO HSM: Len=[%d]--> Data=", sBalMessage.sHeader.nLen);
		trace_event(sLine, TRACE);
		dump_buffer(sBalMessage.sBody, sBalMessage.sHeader.nLen, 'A', 'B', 'O');
	}

	/*EBE140921: Moved from WaitHsmResponse*/
	pthread_mutex_lock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));

	if (WriteBalHsm(nBalHsm, &sBalMessage) != OK) {
		trace_event("Can Not Write Message To Hsm Queue", PROCESSING);
		trace_event("End  HsmQuery ( NOK )", PROCESSING);
		pthread_mutex_unlock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));
		return (HSM_MALFUNCTION);
	}
	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));

	BalStatus = WaitHsmResponse(nIndiceCtx, HSM_TIME_OUT_VALUE, BuffRcv);

	/*EBE140921: Moved from WaitHsmResponse*/
	memset(tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal)); /*PADSS2020-2*/
	pthread_mutex_unlock(&(tab_HsmCtx[nIndiceCtx].HsmMutex));

	if (BalStatus != OK) {
		trace_event("Time Out Situation With HSM", PROCESSING);
		trace_event("End HsmQueryBin ( NOK )", PROCESSING);
		return (NOT_AVAILABLE);
	}


	/**
	sprintf(sLine, "FROM HSM: <--%s|", BuffRcv);
	trace_event(sLine, TRACE);
	**/
	/*
	dump_buffer(tab_HsmCtx[nIndiceCtx].szBufferBal, tab_HsmCtx[nIndiceCtx].nLenBal, 'A', 'B', 'I'); */ /*PADSS2020-2*/

	trace_event("End   HsmQueryBin ( OK )", PROCESSING);
	return (OK);
}


int 
	HsmRequest(int nIndexCtx, pid_t nHsmPid, int nHsmBalId, char *buffer_in, int nLength_in, char *buffer_out, int* nLength_out)
{
	int             BalStatus;
	TSHsmExchange   sBalMessage;
	char            sLine[MAX_LINE_TRC];


	trace_event("Start HsmRequest ()", PROCESSING);

	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));
	memset(tab_HsmCtx[nIndexCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndexCtx].szBufferBal));
	sBalMessage.sHeader.nIndex = nIndexCtx;
	sBalMessage.sHeader.tid = pthread_self();
	sBalMessage.sHeader.nCoNumber = GetNextCoNumber(nIndexCtx);

	sBalMessage.sHeader.nPartnerId = nHsmPid;
	sBalMessage.sHeader.nSourceId = getpid();
	sBalMessage.sHeader.nCommand = HSM_REQUEST;
	memcpy(sBalMessage.sBody, buffer_in, nLength_in);
	sBalMessage.sHeader.nLen = nLength_in;


	sprintf(sLine, "GLOBAL PartnerID = %d; SourceID = %d", nHsmPid, getpid()); trace_event(sLine, TRACE);

	if (debug_option == 1)
	{
		sprintf(sLine, "TO HSM: Len=[%d]", sBalMessage.sHeader.nLen); trace_event(sLine, TRACE);
		dump_buffer(sBalMessage.sBody, sBalMessage.sHeader.nLen, 'A', 'B', 'O');
	}

	/*EBE140921: Moved from WaitHsmResponse*/
	pthread_mutex_lock(&(tab_HsmCtx[nIndexCtx].HsmMutex));

	if (WriteBalHsm(nHsmBalId, &sBalMessage) != OK) 
	{
		trace_event("End  HsmRequest ( NOK ): Can Not Write Message To Hsm Queue", PROCESSING);
		pthread_mutex_unlock(&(tab_HsmCtx[nIndexCtx].HsmMutex));
		return (HSM_MALFUNCTION);
	}

	memset((char *) &sBalMessage, 0, sizeof(TSHsmExchange));

	BalStatus = WaitHsmResponse(nIndexCtx, HSM_TIME_OUT_VALUE, buffer_out);

	/*EBE140921: Moved from WaitHsmResponse*/
	memset(tab_HsmCtx[nIndexCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndexCtx].szBufferBal)); /*PADSS2020-2*/
	pthread_mutex_unlock(&(tab_HsmCtx[nIndexCtx].HsmMutex));

	if (BalStatus != OK) 
	{
		trace_event("End HsmRequest ( NOK ): Time Out Situation With HSM", PROCESSING);
		return (NOT_AVAILABLE);
	}


	(*nLength_out)= tab_HsmCtx[nIndexCtx].nLenBal;
	/*
	dump_buffer(tab_HsmCtx[nIndexCtx].szBufferBal, tab_HsmCtx[nIndexCtx].nLenBal, 'A', 'B', 'I');*/ /*PADSS2020-2*/

	trace_event("End   HsmRequest ( OK )", PROCESSING);
	return (OK);
}

