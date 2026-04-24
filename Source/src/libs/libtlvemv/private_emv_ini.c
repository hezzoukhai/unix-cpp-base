/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      private_emv_ini.c                                                      */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   24/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <tlv_private_emv.h>
#include <define.h>


tlv_info_t  tab_TlvPrivateEmvInfo[] = 
{
   { "C00", PRIVATE_TLV_VAR, 0 } ,/*CHIP_BUFFER_EMV_HEADER_TAG*/ 
   { "C01", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRACK_2_TAG*/
   { "C02", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CARDHOLDER_NAME_TAG*/
   { "C03", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRACK_1_TAG*/
   { "C04", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_EXPIRATION_DATE_TAG*/
   { "C05", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_PRIMARY_ACCOUNT_TAG*/
   { "C06", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_VERSION_NUMBER_TAG*/
   { "C07", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_CURRENCY_CODE1_TAG*/
   { "C08", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_EFFECTIVE_DATE_TAG*/
   { "C09", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_SEQ_NBR_TAG*/
   { "C10", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_USAGE_CONTROL_TAG*/
   { "C11", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISSUER_COUNTRY_CODE_TAG*/
   { "C12", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISSUER_COUNTRY_CODE1_TAG*/
   { "C13", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_SERVICE_CODE_TAG*/
   { "C14", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_CURRENCY_EXP_TAG*/
   { "C15", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DDOL_TAG*/
   { "C16", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISSUER_CODE_TABLE_INDEX_TAG*/
   { "C17", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LANGUAGE_PREFERENCE_TAG*/
   { "C18", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_IDENTIFIER_TAG*/
   { "C19", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLICATION_LABEL_TAG*/
   { "C20", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_PRIORITY_INDICATOR_TAG*/
   { "C21", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_PREFERRED_NAME_TAG*/
   { "C22", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_INTERCHANGE_PROFILE_TAG*/
   { "C23", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_CURRENCY_CODE2_TAG*/
   { "C24", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPL_DEFAULT_ACTION_TAG*/
   { "C25", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLICATION_TRX_COUNTER_TAG*/
   { "C26", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CONSEC_TRX_LIMIT_INT_T_TAG*/
   { "C27", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CONSEC_TRX_LIMIT_INT_C_TAG*/
   { "C28", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CUMUL_TRX_AMT_LIMIT_TOT_TAG*/
   { "C29", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CUMUL_TRX_AMT_UPLIMIT_TOT_TAG*/
   { "C30", PRIVATE_TLV_VAR, 0 } ,/*CHIP_GEOGRAPHIC_INDICATOR_TAG*/
  /* { "C28", PRIVATE_TLV_VAR, 0 },*//*CHIP_ISS_APPLICATION_DATA_TAG*/
   { "C31", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_AUTHENT_NOT_PERFORM_TAG*/
   { "C32", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LOWER_CONSEC_OFF_LIM_TAG*/
   { "C33", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LOWER_OFFLINE_LIMIT_TAG*/
   { "C34", PRIVATE_TLV_VAR, 0 } ,/*CHIP_UPPER_CONSEC_OFFLIMIT_TAG*/
   { "C35", PRIVATE_TLV_VAR, 0 } ,/*CHIP_UPPER_CONSEC_OFFLIMIT_TAG*/ 
   { "C36", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LAST_ONLINE_RGSTR_TAG*/
   { "C37", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PIN_TRY_COUNTER_TAG*/
   { "C38", PRIVATE_TLV_VAR, 0 } ,/*CHIP_SPECIFIC_DATA_TAG*/
  
/*Start Specific Data */ 
   { "C39", PRIVATE_TLV_VAR, 0 } ,/*CHIP_KEK_KEY_TAG*/
   { "C40", PRIVATE_TLV_VAR, 0 } ,/*CHIP_KCV_KEK_TAG*/

   { "C41", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLICATION_CTRL_TAG*/
   { "C42", PRIVATE_TLV_VAR, 0 } ,/*CHIP_N_DOM_CTRL_FACT_EXP_TAG*/
  
   { "C43", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LOWER_CUM_OFF_TRX_AMT_TAG*/
   { "C44", PRIVATE_TLV_VAR, 0 } ,/*CHIP_UPPER_CUM_OFF_TRX_AMT_TAG*/
   { "C45", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CARD_TVR_ACTION_CODE_TAG*/
   { "C46", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_OFF_TAG*/
   { "C47", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_ONL_TAG*/
   { "C48", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_DEC_TAG*/
   { "C49", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PEK_KEY_TAG*/
/*End Specific Data */

   { "C50", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CARD_RISK_MANAG_CDOL1_TAG*/
   { "C51", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CARD_RISK_MANAG_CDOL2_TAG*/
   { "C52", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CA_PUBLIC_KEY_INDEX_TAG*/
   { "C53", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_PUBLIC_KEY_CERTIF_TAG*/
   { "C54", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PUBLIC_KEY_CERTIF_TAG*/
   { "C55", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_PUBLIC_KEY_EXP_TAG*/
   { "C56", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PUBLIC_KEY_EXP_TAG*/
   { "C57", PRIVATE_TLV_VAR, 0 } ,/*CHIP_STATIC_APPL_DATA_TAG*/

   { "C58", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DYNAMIC_DATA_TAG*/

   { "C59", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_PUBLIC_KEY_RMNDER_TAG*/
   { "C60", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PUBLIC_KEY_RMNDER_TAG*/

   { "C61", PRIVATE_TLV_VAR, 0 } ,/*CHIP_AFL_TAG*/

   { "C62", PRIVATE_TLV_VAR, 0 } ,/*CHIP_SDA_TAG_LIST_TAG*/
   { "C63", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CVM_LIST_TAG*/
   { "C64", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_ACTION_CODE_DFAULT_TAG*/
   { "C65", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_ACTION_CODE_DENIAL_TAG*/
   { "C66", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_ACTION_CODE_ONLINE_TAG*/

   { "C67", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRYPTOGRAM_DATA_TAG*/
   { "C68", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PIN_BLOCK_TAG*/

   /** START MAZ 01042015 EMV Multi Application  **/
   { "C71", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CVN_TAG  D1*/
   { "C72", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DKI_TAG  D2*/
   { "C73", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PIN_TRY_LIMIT_TAG DF07*/
   { "C74", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ONLINE_AUTH_INDICATOR_TAG D7*/
   { "C75", PRIVATE_TLV_VAR, 0 } ,/*CHIP_REFERENCE_PIN_CODE_TAG DF03*/   
   { "C69", PRIVATE_TLV_VAR, 0 } ,/*CHIP_KEY_BUFFER_TAG*/
   /** END MAZ 01042015  **/
   
   { "C70", PRIVATE_TLV_VAR, 0 } ,/*CHIP_END_CODE_TAG*/
   { "C80", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DED_FILE_NAME_TAG */ 
   { "C81", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_USE_CONTROL_TAG */ 
};

tlv_info_t  tab_TlvPrivateEmvInfo1[] =
{
   { "C39", PRIVATE_TLV_VAR, 0 } ,/*CHIP_KEK_KEY_TAG*/
   { "C40", PRIVATE_TLV_VAR, 0 } ,/*CHIP_KCV_KEK_TAG*/

   { "C41", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLICATION_CTRL_TAG*/
   { "C42", PRIVATE_TLV_VAR, 0 } ,/*CHIP_N_DOM_CTRL_FACT_EXP_TAG*/

   { "C43", PRIVATE_TLV_VAR, 0 } ,/*CHIP_LOWER_CUM_OFF_TRX_AMT_TAG*/
   { "C44", PRIVATE_TLV_VAR, 0 } ,/*CHIP_UPPER_CUM_OFF_TRX_AMT_TAG*/
   { "C45", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CARD_TVR_ACTION_CODE_TAG*/
   { "C46", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_OFF_TAG*/
   { "C47", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_ONL_TAG*/
   { "C48", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRDISS_ACTION_CODE_DEC_TAG*/
   { "C49", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PEK_KEY_TAG*/
};

void  InitTlvPrivateEmv  (TSTlvPrivate * tlvInfo)
{
   int i;

   for (i = 0; i < MAX_PRIVATE_TLV; i++)
   {
      tlvInfo->nPosTlv [i] = 0;
      tlvInfo->nPresent[i] = NOT_PRESENT;
   }
   tlvInfo->nLength  = 0;
   memset(tlvInfo->sTlvData,   '\0', MAX_PRIVATE_TLV_LEN);
}

int GetTlvPrivateEmvIndice ( char * szTlvName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_TlvPrivateEmvInfo) /sizeof(tlv_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTlvName, tab_TlvPrivateEmvInfo[nIndice].sTlvName, 3) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetTlvPrivateEmvType ( int nIndice )
{
   return ( tab_TlvPrivateEmvInfo[nIndice].nType);
}

int GetTlvPrivateEmvLength ( int nIndice )
{
   return ( tab_TlvPrivateEmvInfo[nIndice].nLength);
}

void AdjustTlvPrivateEmv (TSTlvPrivate * tlvInfo, int nIndice, int nLength)
{
   int i;

   for (i = nIndice; i < MAX_PRIVATE_TLV; i++)
   {
      if ( tlvInfo->nPresent[i] == PRESENT)
         tlvInfo->nPosTlv [i]  += nLength;
   }
}

