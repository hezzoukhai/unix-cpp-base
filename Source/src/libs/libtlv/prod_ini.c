#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tlv_prod.h>
#include <define.h>


static tlv_info_t  tab_TlvProdInfo[] = 
{
	{ "P01"  , PROD_TLV_VAR, 0}, /* PROD_ERROR_CODE_TAG 	 */
	{ "P02"  , PROD_TLV_VAR, 0}, /* PROD_ERROR_TEXT_TAG      */ 
	{ "P03"  , PROD_TLV_VAR, 0}, /* PROD_FLAGS_BUFFER_TAG    */
	{ "P04"  , PROD_TLV_VAR, 0}, /* PROD_PAN_TAG             */
	{ "P05"  , PROD_TLV_VAR, 0}, /* PROD_CVKA_TAG            */ 
	{ "P06"  , PROD_TLV_VAR, 0}, /* PROD_CVKB_TAG            */ 
	{ "P07"  , PROD_TLV_VAR, 0}, /* PROD_SERVICE_CODE_TAG    */ 
	{ "P08"  , PROD_TLV_VAR, 0}, /* PROD_EXPIRY_DATE_TAG     */
	{ "P09"  , PROD_TLV_VAR, 0}, /* PROD_CVV2_TAG    		 */
	{ "P10"  , PROD_TLV_VAR, 0}, /* PROD_CVV1_TAG    		 */
	{ "P11"  , PROD_TLV_VAR, 0}, /* PROD_PVK_INDEX_TAG       */ 
	{ "P12"  , PROD_TLV_VAR, 0}, /* PROD_PVKA_TAG    		 */ 
	{ "P13"  , PROD_TLV_VAR, 0}, /* PROD_PVKB_TAG    		 */ 
	{ "P14"  , PROD_TLV_VAR, 0}, /* PROD_PIN_LMK_TAG    	 */ 
	{ "P15"  , PROD_TLV_VAR, 0}, /* PROD_PVV_TAG    		 */ 
	{ "P16"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_0_TAG   */
	{ "P17"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_1_TAG   */
	{ "P18"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_2_TAG   */
	{ "P19"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_3_TAG   */
	{ "P20"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_4_TAG   */
	{ "P21"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_5_TAG   */
   { "P22"  , PROD_TLV_VAR, 0}, /* PROD_PRINT_FIELD_6_TAG   */ 
   { "P23"  , PROD_TLV_VAR, 0}, /* PROD_OFFSET_TAG          */
   { "P24"  , PROD_TLV_VAR, 0}  /* PROD_PIN_LENGTH          */
};

void  InitTlvProd  (TSTlvProd * tlvInfo)
{
	int	i;
	int	nSize;

	nSize = sizeof(tlvInfo->nPresent)/sizeof(tlvInfo->nPresent[0]);
	for (i = 0; i < nSize; i++)
	{
		tlvInfo->nPosTlv [i] = 0;
		tlvInfo->nPresent[i] = NOT_PRESENT;
	}
	tlvInfo->nLength  = 0;
	memset(tlvInfo->sTlvData,   '\0', sizeof(tlvInfo->sTlvData));
}

void AdjustTlvProd (TSTlvProd * tlvInfo, int nIndice, int nLength)
{
	int i;
	int	nSize;

	nSize = sizeof(tab_TlvProdInfo) /sizeof(tlv_info_t);
	for (i = nIndice; i < nSize; i++)
	{
		if ( tlvInfo->nPresent[i] == PRESENT)
			tlvInfo->nPosTlv [i]  += nLength;
	}
}

int GetTlvProdIndice ( char * szTlvName)
{
	int nIndice, nSize;

	nSize = sizeof(tab_TlvProdInfo) /sizeof(tlv_info_t);
	nIndice = 0;

	do
	{
		if (memcmp(szTlvName, tab_TlvProdInfo[nIndice].sTlvName, 3) == 0)
			return(nIndice);
		nIndice++;
	}while (nIndice < nSize);
	return(-1);
}

int GetTlvProdType ( int nIndice )
{
	return ( tab_TlvProdInfo[nIndice].nType);
}

int GetTlvProdLength ( int nIndice )
{
	return ( tab_TlvProdInfo[nIndice].nLength);
}
