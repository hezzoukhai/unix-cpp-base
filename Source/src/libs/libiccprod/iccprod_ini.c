/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      iccprod_ini.c						               */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (card production file)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI      25/01/2005 -         V 2.2.0  
/*        SO.ELMAADADI   05/01/2016 -         V 3.1.0    - SOE050116           */
/*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <iso_iccprod.h>
#include <define.h>

tag_info_t  tab_IccProdTagInfo[] = 
{
	{ "5700"	, ICCPROD_TAG_VAR_HEX	,   38},	/* TRACK_2_DATA				*/
	{ "5F20"	, ICCPROD_TAG_VAR_ALPHA	,   26},	/* CARDHOLDER_NAME			*/
	{ "9F1F"	, ICCPROD_TAG_VAR_ALPHA	,   72},	/* TRACK_1_DATA				*/
	{ "5F24"	, ICCPROD_TAG_VAR_BCD	,    6},	/* APPL_EXPIRATION_DATE			*/
	{ "5A00"	, ICCPROD_TAG_VAR_BCD	,   16},	/* APPL_PRIMARY_ACCOUNT			*/
	{ "9F08"	, ICCPROD_TAG_VAR_HEX	,    4},	/* APPL_VERSION_NUMBER			*/
	{ "9F42"	, ICCPROD_TAG_VAR_BCD	,    4},	/* APPL_CURRENCY_CODE_1			*/
	{ "5F25"	, ICCPROD_TAG_VAR_BCD	,    6},	/* APPL_EFFECTIVE_DATE			*/
    { "5F34"        , ICCPROD_TAG_VAR_BCD   ,    2},        /* APPL_SEQ_NBR                         */
	{ "9F07"	, ICCPROD_TAG_VAR_HEX	,    4},	/* APPL_USAGE_CONTROL			*/
	{ "5F28"	, ICCPROD_TAG_VAR_BCD	,    4},	/* ISSUER_COUNTRY_CODE			*/
    { "9F57"        , ICCPROD_TAG_VAR_BCD   ,    4},        /* ISSUER_COUNTRY_CODE1                 */
	{ "5F30"	, ICCPROD_TAG_VAR_BCD	,    4},	/* APPL_SERVICE_CODE			*/
	{ "9F44"	, ICCPROD_TAG_VAR_BCD	,    2},	/* APPL_CURRENCY_EXP			*/
	/* { "9F49"	, ICCPROD_TAG_VAR_BCD	,    6},	 DYNAMIC_DATA_OBJECT_LIST		 SOE050116*/
	{ "9F49"	, ICCPROD_TAG_VAR_HEX	,    6},	/* DYNAMIC_DATA_OBJECT_LIST		 SOE050116*/
	{ "9F11"	, ICCPROD_TAG_VAR_BCD	,    2},	/* ISSUER_CODE_TABLE_INDEX		*/
	{ "5F2D"	, ICCPROD_TAG_VAR_ALPHA	,    2},	/* LANGUAGE_PREFERENCE			*/
	{ "4F00"	, ICCPROD_TAG_VAR_HEX	,   14},	/* APPLICATION_IDENTIFIER		*/
	{ "5000"	, ICCPROD_TAG_VAR_ALPHA	,   16},	/* APPLICATION_LABEL			*/
	{ "8700"	, ICCPROD_TAG_VAR_HEX	,    2},	/* APPL_PRIORITY_INDICATOR		*/
	{ "9F12"	, ICCPROD_TAG_VAR_ALPHA	,   16},	/* APPL_PREFERRED_NAME			*/
    { "8200"        , ICCPROD_TAG_VAR_HEX   ,    4},        /* APPL_INTERCHANGE_PROFILE             */
	{ "9F51"	, ICCPROD_TAG_VAR_BCD	,    4},	/* APPL_CURRENCY_CODE_2			*/
	{ "9F52"	, ICCPROD_TAG_VAR_HEX	,    4},	/* APPL_DEFAULT_ACTION			*/
	{ "9F36"	, ICCPROD_TAG_VAR_BCD	,    4},	/* APPLICATION_TRX_COUNTER		*/
	{ "9F53"	, ICCPROD_TAG_VAR_BCD	,    2},	/* CONSECUTIVE_TRX_LIMIT_INT_T  	*/
	{ "9F72"	, ICCPROD_TAG_VAR_BCD	,    2},	/* CONSECUTIVE_TRX_LIMIT_INT_C  	*/
	{ "9F54"	, ICCPROD_TAG_VAR_BCD	,   12},	/* CUMUL_TRX_AMOUNT_LIMIT_TOT   	*/
    { "9F5C"        , ICCPROD_TAG_VAR_BCD   ,   12},        /* CUMUL_TRX_AMOUNT_UPLIMIT_TOT         */
	{ "9F55"	, ICCPROD_TAG_VAR_HEX	,    2},	/* GEOGRAPHIC_INDICATOR			*/
/*	{ "9F10"	, ICCPROD_TAG_VAR_HEX	,   14},*/      /* ISSUER_APPLICATION_DATA		*/
	{ "9F56"	, ICCPROD_TAG_VAR_BCD	,    2},	/* ISSUER_AUTHENT_INDICATOR		*/
	{ "9F58"	, ICCPROD_TAG_VAR_BCD	,    2},	/* LOWER_CONSECUTIVE_OFF_LIM    	*/
	{ "9F14"	, ICCPROD_TAG_VAR_BCD	,    2},	/* LOWER_OFFLINE_LIMIT			*/
	{ "9F59"	, ICCPROD_TAG_VAR_BCD	,    2},	/* UPPER_CONSECUTIVE_OFFLIMIT   	*/
    { "9F23"        , ICCPROD_TAG_VAR_BCD   ,    2},        /* UPPER_CONSECUTIVE_OFFLIMIT           */
        { "9F13"        , ICCPROD_TAG_VAR_BCD   ,    4},        /* LAST_ONLINE_ATC_REGISTER             */
        { "9F17"        , ICCPROD_TAG_VAR_BCD   ,    2},        /* PIN_TRY_COUNTER                      */
/*	{ "FF60"	, ICCPROD_TAG_VAR_ALPHA	,  108},*/	/* SPECIFIC DATA*/
        { "FF60"        , ICCPROD_TAG_VAR_ALPHA_EBR3 ,  180},
/**/
        { "FF22"        , ICCPROD_TAG_VAR_HEX   ,   32},        /* KEK_KEY                              */
        { "FF29"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* KCV_KEK                              */
       
        { "D500"        , ICCPROD_TAG_VAR_HEX   ,    2},        /* APPLICATION_CONTROL                  */
        { "CE00"        , ICCPROD_TAG_VAR_BCD   ,    2},        /* NON_DOMESTIC_CTRL_FACTOT_EXP         */
/**/
        { "CA00"        , ICCPROD_TAG_VAR_BCD   ,   12},        /* LOWER_CUMUL_OFFLINE_TRX_AMNT         */
        { "CB00"        , ICCPROD_TAG_VAR_BCD   ,   12},        /* UPPER_CUMUL_OFFLINE_TRX_AMNT         */
        { "C600"        , ICCPROD_TAG_VAR_HEX   ,   10},        /* CARD_TVR_ACTION_CODE                 */
        { "C400"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_OFFLINE          */
        { "C500"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_ONLINE           */
        { "C300"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_DECLINE          */
        { "FF23"        , ICCPROD_TAG_VAR_HEX   ,   33},        /* PEK_KEY                              */
	
        { "8C00"	, ICCPROD_TAG_VAR_HEX	,   42},	/* CARD_RISK_MANAG_CDOL1		*/
	{ "8D00"	, ICCPROD_TAG_VAR_HEX	,   46},	/* CARD_RISK_MANAG_CDOL2		*/
	{ "8F00"	, ICCPROD_TAG_VAR_BCD	,    2},	/* CA_PUBLIC_KEY_INDEX			*/

/*	{ "9000"	, ICCPROD_TAG_VAR_HEX	,  256},*/	/* ISS_PUBLIC_KEY_CERTIFICATE   	*/
        { "9000"        , ICCPROD_TAG_VAR_EBR2   ,  256},

/*	{ "9F46"	, ICCPROD_TAG_VAR_HEX	,  256},*/	/* PUBLIC_KEY_CERTIFICATE		*/
        { "9F46"        , ICCPROD_TAG_VAR_EBR2  ,  256},
 
	{ "9F32"	, ICCPROD_TAG_VAR_HEX	,    6},	/* ISS_PUBLIC_KEY_EXPONENT		*/
	{ "9F47"	, ICCPROD_TAG_VAR_HEX	,    6},	/* PUBLIC_KEY_EXPONENT			*/
/*	{ "9300"	, ICCPROD_TAG_VAR_HEX	,  256},*/      /* SIGNED_STATIC_APPL_DATA		*/
       /* { "9300"        , ICCPROD_TAG_VAR_EBR2  ,  256},*//*** ADD MOU29Nov2012 EMV Embratec***/
        { "9300"        , ICCPROD_TAG_VAR_HEX   ,  512},
/**/
        { "9F4B"        , ICCPROD_TAG_VAR_EBR2  ,  256},        /* SIGNED_DYNAMIC_DATA                  */
/**/
	{ "9200"	, ICCPROD_TAG_VAR_HEX	,   72},	/* ISS_PUBLIC_KEY_REMAINDER		*/
	{ "9F48"	, ICCPROD_TAG_VAR_HEX	,   84},	/* PUBLIC_KEY_REMAINDER			*/
/**/
        { "9400"        , ICCPROD_TAG_VAR_BCD   ,   32},        /* APPLICATION_FILE_LOCATOR             */
/**/

	{ "9F4A"	, ICCPROD_TAG_VAR_HEX	,    4},	/* STATIC_DATA_AUTHENT_TAG_LIST 	*/
	{ "8E00"	, ICCPROD_TAG_VAR_HEX	,   16},	/* CVM_LIST				*/
	{ "9F0D"	, ICCPROD_TAG_VAR_HEX	,   10},	/* ISSUER_ACTION_CODE_DEFAULT   	*/
	{ "9F0E"	, ICCPROD_TAG_VAR_HEX	,   10},	/* ISSUER_ACTION_CODE_DENIAL    	*/
	{ "9F0F"	, ICCPROD_TAG_VAR_HEX	,   10},	/* ISSUER_ACTION_CODE_ONLINE    	*/
/**/
        { "9F10"        , ICCPROD_TAG_VAR_HEX   ,   38},        /* CRYPTOGRAM_DATA                      */
        { "9F62"        , ICCPROD_TAG_VAR_HEX   ,   44},        /* PIN_BLOCK                            */
/**/
	
	/** START MAZ 01042015 EMV Multi Application  **/
    { "D1", ICCPROD_TAG_VAR_HEX, 2 } ,/*CRYPTOGRAM VERSION NUMBER*/
    { "D2", ICCPROD_TAG_VAR_HEX, 2 } ,/*DERIVATION KEY INDEX */
    { "DF07", ICCPROD_TAG_VAR_HEX, 2 } ,/*PIN TRY LIMIT_TAG */
    { "D7", ICCPROD_TAG_VAR_HEX, 2 } ,/*ONLINE AUTHORIZATION INDICATOR */
    { "DF03", ICCPROD_TAG_VAR_HEX, 16 } ,/*REFERENCE PIN CODE */
	{ "9F60"	, ICCPROD_TAG_VAR_HEX	,  148},	/* KEY BUFFER				*/
    /** END MAZ 01042015  **/

};


tag_info_t  tab_IccProdTagInfo1[] =
{
/**/ 
        { "FF22"        , ICCPROD_TAG_VAR_HEX   ,   32},        /* KEK_KEY                              */
        { "FF29"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* KCV_KEK                              */ 
        { "D500"        , ICCPROD_TAG_VAR_HEX   ,    2},        /* APPLICATION_CONTROL                  */
        { "CE00"        , ICCPROD_TAG_VAR_BCD   ,    2},        /* NON_DOMESTIC_CTRL_FACTOT_EXP         */ 
/**/
        { "CA00"        , ICCPROD_TAG_VAR_BCD   ,   12},        /* LOWER_CUMUL_OFFLINE_TRX_AMNT         */
        { "CB00"        , ICCPROD_TAG_VAR_BCD   ,   12},        /* UPPER_CUMUL_OFFLINE_TRX_AMNT         */
        { "C600"        , ICCPROD_TAG_VAR_HEX   ,   10},        /* CARD_TVR_ACTION_CODE                 */
        { "C400"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_OFFLINE          */
        { "C500"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_ONLINE           */
        { "C300"        , ICCPROD_TAG_VAR_HEX   ,    6},        /* CARDISS_ACTION_CODE_DECLINE          */
        { "FF23"        , ICCPROD_TAG_VAR_HEX   ,   32},        /* PEK_KEY                              */
};

void  InitIccProdIcTag  (TSTagIccProd * msgInfo)
{
   int i;
   for (i = 0; i < MAX_ICCPROD_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_ICCPROD_CHIP_LEN);
}

int GetIccProdTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_IccProdTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_IccProdTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetIccProdTagType ( int nIndice )
{
   return ( tab_IccProdTagInfo[nIndice].nType);
}

int GetIccProdTagLength ( int nIndice )
{
   return ( tab_IccProdTagInfo[nIndice].nLength);
}
int CopyAscii ( char *SrcBf ,int lgin, char *DestBf )
{
int i = 0;
	for(i=0;i<lgin;i++)
		DestBf[i] = SrcBf[i];

	return(OK);
}
