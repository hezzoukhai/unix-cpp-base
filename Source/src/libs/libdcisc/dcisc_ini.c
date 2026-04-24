#include <stdio.h>
#include <stdlib.h>
#include <iso_dcisc.h>
#include <define.h>
#include <string.h>
#include <iso8583.h>       /* ELKAM20230217 PLUTONL-5914 */
#include <dcisc_define.h>  /* ELKAM20230217 PLUTONL-5914 */

static fld_info_t    tab_DciscFldInfo[] =
{
   { NOT_USED,  	      0},/*	1	*/	/* BIT_MAP1			    */
   { DCISC_LL_ALPHA,	 19},/*	2	*/   /* CARD_NBR 			 */
   { DCISC_FIX_ALPHA,	  6},/*	3	*/	/* PROC_CODE			 */
   { DCISC_FIX_ALPHA,	 12},/*	4	*/	/* TRANS_AMOUNT			 */
   { DCISC_FIX_ALPHA,	 12},/*	5	*/	/* STLMNT_AMOUNT		 */
   { DCISC_FIX_ALPHA,	 12},/*	6	*/	/* Cardholder Billing Amount	 */
   { DCISC_FIX_ALPHA,	 10},/*	7	*/	/* XMIT_TIME			 */
   { DCISC_FIX_ALPHA,	  8},/*	8	*/	/* Cardholder Billing Fee	 */
   { DCISC_FIX_ALPHA,	  8},/*	9	*/	/* CONVRSN_RATE_STLMNT		 */
   { DCISC_FIX_ALPHA,	  8},/*	10	*/	/* CONVRSN_RATE_CARD_HLDR_BILLING */
   
   { DCISC_FIX_ALPHA,	  6},/*	11	*/	/* AUDIT_NBR			 */
   { DCISC_FIX_ALPHA,	 12},/*	12	*/	/* TRANS_DATE_TIME               */
   { DCISC_FIX_ALPHA,	  4},/*	13	*/	/* TRANS_DATE			 */
   { DCISC_FIX_ALPHA,	  4},/*	14	*/	/* EXPIRY_DATE			 */
   { DCISC_FIX_ALPHA,	  6},/*	15	*/	/* STLMNT_DATE			 */
   { DCISC_FIX_ALPHA,	  4},/*	16	*/	/* CNVRSN_DATE			 */
   { DCISC_FIX_ALPHA,	  4},/*	17	*/	/* CAPTURE_DATE			 */
   { DCISC_FIX_ALPHA,	  4},/*	18	*/	/* MERCHANT_TYPE		 */
   { DCISC_FIX_ALPHA,	  3},/*	19	*/	/* ACQR_COUNTRY			 */
   { DCISC_FIX_ALPHA,	  3},/*	20	*/	/* PAN_COUNTRY			 */
   
   { DCISC_FIX_ALPHA,	  3},/*	21	*/	/* FORWD_COUNTRY		 */
   { DCISC_FIX_ALPHA,	 12},/*	22	*/	/* POS_ENTRY_MODE		 */
   { DCISC_FIX_ALPHA,	  3},/*	23	*/	/* Card Sequence Number		 */
   { DCISC_FIX_ALPHA,	  3},/*	24	*/	/* Function Code                 */
   { DCISC_FIX_ALPHA,	  4},/*	25	*/	/* POS_CONDITION		 */
   { DCISC_FIX_ALPHA,	  4},/*	26	*/	/* MERCHANT TYPE  		 */
   { DCISC_FIX_ALPHA,	  1},/*	27	*/	/* Authorization Id Response Lenght */
   { DCISC_FIX_ALPHA,	  6},/*	28	*/	/* TRANS_FEE			 */
   { DCISC_FIX_ALPHA,	  3},/*	29	*/	/* Amount Settlement Fee	 */
   { DCISC_FIX_ALPHA,	 24},/*	30	*/	/* Amount Transaction Processing Fee */
   
   { DCISC_LL_ALPHA,	  9},/*	31	*/	/* Amount Setlmnt Processing Fee */
   { DCISC_LL_ALPHA,	 11},/*	32	*/	/* ACQR_ID			 */
   { DCISC_LL_ALPHA,	 11},/*	33	*/	/* FORWD_ID			 */
   { DCISC_LL_ALPHA,	 28},/*	34	*/	/* PAN_EXTENDED			 */
   { DCISC_LL_ALPHA,	 37},/*	35	*/	/* T2_DATA			 */
   { DCISC_LLL_ALPHA,	  4},/*	36	*/	/* T1_DATA                       */
   { DCISC_FIX_ALPHA,	 12},/*	37	*/	/* REFERENCE_NBR		 */
   { DCISC_FIX_ALPHA,	  6},/*	38	*/	/* AUTHOR_ID			 */
   { DCISC_FIX_ALPHA,	  3},/*	39	*/	/* ACTION CODE  		 */
   { DCISC_FIX_ALPHA,	  3},/*	40	*/	/* Service Restriction Code	 */
   
   { DCISC_FIX_ALPHA,	  8},/*	41	*/	/* TERMINAL_NBR			 */
   { DCISC_FIX_ALPHA,	 15},/*	42	*/	/* OUTLET_NBR			 */
   { DCISC_LL_ALPHA,	 99},/*	43	*/	/* TERMINAL_ADR			 */
   { DCISC_LL_ALPHA,	999},/*	44	*/	/* ADTNL_RESP_DATA		 */
   { DCISC_LL_ALPHA,	 77},/*	45	*/	/* T1_DATA			 */
   { DCISC_LLL_ALPHA,	255},/*	46	*/	/* Additrional Data Iso		 */
   { DCISC_LLL_ALPHA,	255},/*	47	*/	/* Additrional Data National	 */
   { DCISC_LLL_ALPHA,	999},/*	48	*/	/* ADTNL_DATA_PRIV		 */
   { DCISC_FIX_ALPHA,	  3},/*	49	*/	/* TRANS_CRNCY			 */
   { DCISC_FIX_ALPHA,	  3},/*	50	*/	/* STLMNT_CRNCY			 */
   
   { DCISC_FIX_ALPHA,	  3},/*	51	*/	/* BILLING_CRNCY		 */
   { DCISC_FIX_HEX,	     16},/*	52	*/	 /* PIN_DATA			 */   /*AOU15082011 PIN ONLINE VERIF*/
   { DCISC_LL_ALPHA,	 16},/*	53	*/	/* SECURITY_DATA		 */
   { DCISC_LLL_ALPHA,	120},/*	54	*/	/* ADTNL_AMOUNTS		 */
   { DCISC_LLL_BIN,	    299}, /*	55	*/	/* ICC Information               */
   { DCISC_LL_ALPHA,	255},/*	56	*/	/* Orig Data Elements            */
   { DCISC_FIX_ALPHA,	  3},/*	57	*/	/* Reserved National		 */
   { DCISC_LL_ALPHA,	255},/*	58	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	 14},/*	59	*/	/* GEOGR_DATA			 */
   { DCISC_LLL_ALPHA,	  6},/*	60	*/	/* ADTNL_POS_INFO		 */
   
   { DCISC_LLL_ALPHA,	 999}/*	61	*/,	/* NETWORK POS DATA		 */
   { /*DCISC_LLL_ALPHA*/DCISC_LLL_BIN, 	999},/*	62	*/	/* Redemption Number		 */
   { DCISC_LLL_ALPHA, 	999},/*	63	*/	/* Versions       		 */
   { NOT_USED,	          8},/*	64	*/	/* MAC1				 */
   { NOT_USED,	         0}, /*	65	*/	/* 				 */
   { DCISC_LLL_ALPHA,	  1},/*	66	*/	/* STLMNT_CODE			 */
   { DCISC_FIX_ALPHA,	  2},/*	67	*/	/* Extended Payment Code	*/
   { DCISC_FIX_ALPHA,	  3},/*	68	*/	/* RECEIVER_COUNTRY		 */
   { DCISC_FIX_ALPHA,	  3},/*	69	*/	/* STLMNT_INST_COUNTRY		 */
   { DCISC_FIX_ALPHA,	  3},/*	70	*/	/* NW_MNGMT_INFO		 */
   
   { DCISC_FIX_ALPHA,	  8},/*	71	*/	/* Message Number		 */
   { DCISC_LLL_ALPHA,	999},/*	72	*/	/* Data Record        		 */
   { DCISC_FIX_ALPHA,	  6},/*	73	*/	/* ACTION_DATE			 */
   { DCISC_FIX_ALPHA,	 10},/*	74	*/	/* CR_NBR			 */
   { DCISC_FIX_ALPHA,	 10},/*	75	*/	/* CR_REV_NBR			 */
   { DCISC_FIX_ALPHA,	 10},/*	76	*/	/* DB_NBR			 */
   { DCISC_FIX_ALPHA,	 10},/*	77	*/	/* DB_REV_NBR			 */
   { DCISC_FIX_ALPHA,	 10},/*	78	*/	/* Transfer Number		 */
   { DCISC_FIX_ALPHA,	 10},/*	79	*/	/* Transfer Reversal Number	 */
   { DCISC_FIX_ALPHA,	 10},/*	80	*/	/* Inquiries Number		 */
   
   { DCISC_FIX_ALPHA,	 10},/*	81	*/	/* Authorizations Number	 */
   { DCISC_FIX_ALPHA,	 10},/*	82	*/	/* Credit, Processing Fee Amount */
   { DCISC_FIX_ALPHA,	 10},/*	83	*/	/* Payment Number                 */
   { DCISC_FIX_ALPHA,	 10},/*	84	*/	/* Payment Reversal Number     	 */
   { DCISC_FIX_ALPHA,	 10},/*	85	*/	/* Debit, Transaction Fee Amount */
   { DCISC_FIX_ALPHA,	 16},/*	86	*/	/* CR_AMOUNT			 */
   { DCISC_FIX_ALPHA,	 16},/*	87	*/	/* CR_REV_AMOUNT		 */
   { DCISC_FIX_ALPHA,	 16},/*	88	*/	/* DB_AMOUNT			 */
   { DCISC_FIX_ALPHA,	 16},/*	89	*/	/* DB_REV_AMOUNT		 */
   { DCISC_FIX_ALPHA,	 35},/*	90	*/	/* ORIG_DATA			 */
   
   { DCISC_FIX_ALPHA,	  3},/*	91	*/	/* FILE_UPDATE			 */
   { DCISC_FIX_ALPHA,	  3},/*	92	*/	/* FILE_SECURITY		 */
   { DCISC_LL_ALPHA,	 11},/*	93	*/	/* Destination Inst ID Code      */
   { DCISC_LL_ALPHA,	 11},/*	94	*/	/* Source Inst Id Code           */
   { DCISC_LL_ALPHA,	 42},/*	95	*/	/* REPLACE_AMOUNTS		 */
   { DCISC_LLL_ALPHA,	 36},/*	96	*/	/* Additional Key Mngmt data     */ /* CFE the length is 36 instead of 17*/
   { DCISC_FIX_ALPHA,	 17},/*	97	*/	/* NET_AMOUNT			 */
   { DCISC_FIX_ALPHA,	 25},/*	98	*/	/* Payee			 */
   { DCISC_LL_ALPHA,	 11},/*	99	*/	/* STLMNT_INSTITN		 */
   { DCISC_LL_ALPHA,	 11},/*	100	*/	/* RECEIVER_ID			 */
   
   { DCISC_LL_ALPHA,	 17},/*	101	*/	/* FILE_NAME			 */
   { DCISC_LL_ALPHA,	 28},/*	102	*/	/* ACCOUNT_ID1			 */
   { DCISC_LL_ALPHA,	 28},/*	103	*/	/* ACCOUNT_ID2			 */
   { DCISC_LLL_ALPHA,	 16},/*	104	*/	/* Transaction Description	 */
   { DCISC_FIX_ALPHA,	 16},/*	105	*/	/* Reserved Iso			 */
   { DCISC_FIX_ALPHA,	 16},/*	106	*/	/* Reserved Iso			 */
   { DCISC_FIX_ALPHA,	 10},/*	107	*/	/* Reserved Iso			 */
   { DCISC_FIX_ALPHA,	 10},/*	108	*/	/* Reserved Iso			 */
   { DCISC_LL_ALPHA,	  0},/*	109	*/	/* Reserved Iso			 */
   { DCISC_LL_ALPHA,	  0},/*	110	*/	/* Reserved Iso			 */
   
   { DCISC_LLL_ALPHA,	  0},/*	111	*/	/* Reserved Iso			 */
   { DCISC_LLL_ALPHA,	  0},/*	112	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	  0},/*	113	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	  0},/*	114	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	 24},/*	115	*/	/* ADTNL_TRACE_DATA		 */
   { DCISC_LLL_ALPHA,	  0},/*	116	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	  0},/*	117	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	  0},/*	118	*/	/* Intra Country Data		 */
   { DCISC_LLL_ALPHA,	  0},/*	119	*/	/* Reserved National		 */
   { DCISC_LLL_ALPHA,	  0},/*	120	*/	/* Original Message Type Id	 */
   
   { DCISC_LLL_ALPHA,	  0},/*	121	*/	/* Issuing Institution Id	 */
   { DCISC_LLL_ALPHA,	  0},/*	122	*/	/* Remaining Open to Use	 */
   { DCISC_LLL_ALPHA,	 15},/*	123	*/	/* ADR_VERIF_DATA		 */
   { DCISC_LLL_ALPHA,	  0},/*	124	*/	/* Free Form Text - Japan	 */
   { DCISC_LLL_HEX,   	255},/*	125	*/	/* SUPPORT_INFO			 */
   { DCISC_LLL_ALPHA,	255},/*	126	*/	/* Reserved Private		 */
   { DCISC_LLL_ALPHA,	  0},/*	127	*/	/* File Record Action and Data	 */
   { DCISC_FIX_ALPHA,	  8},/*	128	*/	/* MAC2				 */

};

static tag_info_t  tab_DciscTagInfo[] = 
{
   /*{ "7100",DCISC_TAG_VAR_BCD , 0},/* Issuer Script Template 1              *//*KCH21042014*/
   /*{ "7200",DCISC_TAG_VAR_BCD , 0},/* Issuer Script Template 2              *//*KCH21042014*/
   { "7100",DCISC_TAG_VAR_HEX , 0},/* Issuer Script Template 1              *//*KCH21042014*/
   { "7200",DCISC_TAG_VAR_HEX , 0},/* Issuer Script Template 2              *//*KCH21042014*/
   { "8600",DCISC_TAG_VAR_BCD , 0},/* Issuer Script command                 */
   { "8200",DCISC_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",DCISC_TAG_VAR_HEX , 0},/* Dedicated File Name                   */ /*AOU250711 Format AID HEXA*/
   { "4F00",DCISC_TAG_VAR_BCD , 0},/* ICC Application Id                    */
   { "8A00",DCISC_TAG_VAR_BCD , 0},/* Authorization Response Code           */
   { "9500",DCISC_TAG_VAR_BCD , 0},/* Terminal Verification Results         */
   { "9A00",DCISC_TAG_VAR_BCD , 0},/* Transaction Date                      */
   { "9C00",DCISC_TAG_VAR_BCD , 0},/* Transaction TYPE                      */
   { "5F2A",DCISC_TAG_VAR_BCD , 0},/* Transaction Currency Code             */
   { "9F02",DCISC_TAG_VAR_BCD , 0},/* Transaction Amount                    */
   { "9F03",DCISC_TAG_VAR_BCD , 0},/* Other Amount                          */
   { "9F09",DCISC_TAG_VAR_BCD , 0},/* Terminal Application Version Number   */
   { "9F10",DCISC_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",DCISC_TAG_VAR_BCD , 0},/* Terminal Country Code                 */
   { "9F1E",DCISC_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",DCISC_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",DCISC_TAG_VAR_BCD , 0},/* Cryptogram Information Data           */
   { "9F33",DCISC_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",DCISC_TAG_VAR_HEX , 0},/* Cardholder Verification Method        */ /*AOU250711 Format CVM HEXA*/
   { "9F35",DCISC_TAG_VAR_BCD , 0},/* Terminal Type                         */
   { "9F36",DCISC_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",DCISC_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",DCISC_TAG_VAR_BCD , 0},/* Transaction Sequence Number           */
   { "9F53",DCISC_TAG_VAR_BCD , 0},/* Transaction Category Code             */
   { "9100",DCISC_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9F54",DCISC_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
   { "9F06",DCISC_TAG_VAR_HEX , 0},/* Application Identifier (AID) terminal */
   { "9F07",DCISC_TAG_VAR_HEX , 0},/* ApplicationUsage Control          */
};

/* ELKAM20230217 PLUTONL-5914 START */
static const TlvInfoProperties	DcAddAuthData = {
4,3,TLV_LT_ASCII,TLV_LT_ASCII,DC_F062_NB_FIELDS,
{
{DC_F062_DF01_POS_TERM_TYPE      ,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","POS_TERM_TYPE"      ,"NULL",NOT_REPEATABLE},
{DC_F062_DF03_CARD_PROD_TYPE     ,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","CARD_PROD_TYPE"     ,"NULL",NOT_REPEATABLE},
{DC_F062_DF04_TOK_PROV_VER_REQ   ,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","TOK_PROV_VER_REQ"   ,"NULL",NOT_REPEATABLE},
{DC_F062_DF05_CUS_MER_INIT_TRANS ,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","CUS_MER_INIT_TRANS" ,"NULL",NOT_REPEATABLE},
},
"DcAddAuthData"
};

void InitDcAddAuthDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &DcAddAuthData;
	ResetTlvInfo(pkTlvInfo);
}
/* ELKAM20230217 PLUTONL-5914 END*/

void  InitDciscIcTag  (TSTagDcisc * msgInfo)
{
   int i;

   for (i = 0; i < MAX_DCISC_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', sizeof(msgInfo->sChipData));
}

void  InitDciscInfo(TSDciscInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_DCISC_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < DCISC_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;

   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
}

int GetDciscFieldType(int field_no)
{
   return(tab_DciscFldInfo[field_no].type);
}

int GetDciscFieldLength(int field_no)
{
   return(tab_DciscFldInfo[field_no].length);
}

int GetDciscTagIndice ( char * szTagName)
{
   int nIndice, nSize;
   char sLine[MAX_LINE_TRC];
   

   nSize = sizeof(tab_DciscTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_DciscTagInfo[nIndice].sTagName, 4) == 0)
	  {
         	return(nIndice);
      }
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetDciscTagType ( int nIndice )
{
   return ( tab_DciscTagInfo[nIndice].nType);
}

int GetDciscTagLength ( int nIndice )
{
   return ( tab_DciscTagInfo[nIndice].nLength);
}

