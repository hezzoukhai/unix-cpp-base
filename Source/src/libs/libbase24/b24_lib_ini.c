/*#include <base24.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <base24/iso_base24.h>
#include <base24/base24_define.h>
#include <define.h>
#include <iso_com.h>

static fld_info_t    tab_Base24ATMFldInfo[] =
{
	{ BASE24_FIX_BITMAP,		0},			/* B24_ATM_BIT_MAP2				*/
	{ BASE24_LL_ALPHA,			19},		/* B24_ATM_CARD_NBR 			*/
	{ BASE24_FIX_ALPHA,			6},			/* B24_ATM_PROC_CODE			*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_ATM_TRANS_AMOUNT			*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_ATM_STLMNT_AMOUNT		*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_ATM_TRANS_AMOUNT			*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_BILLING_AMOUNT		*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ BASE24_FIX_ALPHA,			6},			/* B24_ATM_AUDIT_NBR			*/
	{ BASE24_FIX_ALPHA,			6},			/* B24_ATM_TRANS_TIME			*/
	{ BASE24_FIX_ALPHA,			4},			/* B24_ATM_TRANS_DATE			*/
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			4},			/* B24_ATM_STLMNT_DATE			*/
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			4},			/* B24_ATM_CAPTURE_DATE			*/
	{ BASE24_FIX_ALPHA,			4},			/* B24_ATM_MERCHANT_TYPE		*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			3},			/* B24_ATM_ENTRY_MODE			*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			2},			/* B24_ATM_CONDITION_CODE		*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,		    9},			/* B24_ATM_TRANS_FEE			*/
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},

	{ NOT_USED,		            0},
	{ BASE24_LL_ALPHA,			11},		/* B24_ATM_ACQR_ID				*/
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ BASE24_LL_ALPHA,			37},		/* B24_ATM_T2_DATA				*/
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,		    12},		/* B24_ATM_REFERENCE_NBR		*/
	{ BASE24_FIX_ALPHA,		    6},			/* B24_ATM_AUTHOR_ID			*/
	{ BASE24_FIX_ALPHA,		    2},			/* B24_ATM_RESPONSE_CODE		*/
	{ NOT_USED,					0},

	{ BASE24_FIX_ALPHA,		    16},		/* B24_ATM_TERMINAL_ID			*/
	{ BASE24_FIX_ALPHA,		    15},		/* B24_ATM_OUTLET_NBR			*/
	{ BASE24_FIX_ALPHA,		    40},		/* B24_ATM_TERMINAL_LOCATION	*/
	{ BASE24_LL_ALPHA,		    25},		/* B24_ATM_ADTNL_RESP_DATA		*/
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ BASE24_LLL_ALPHA,		    44},		/* B24_ATM_ADTNL_DATA_PRIV		*/
	{ BASE24_FIX_ALPHA,		    3},			/* B24_ATM_TRANS_CRNCY			*/
	{ BASE24_FIX_ALPHA,			3},			/* B24_ATM_STLMNT_CRNCY:NCCC	*/

	{ BASE24_FIX_ALPHA,			3},			/* B24_ATM_BILLING_CRNCY		*/
	{ BASE24_FIX_ALPHA,		    16},		/* B24_ATM_PIN_DATA				*/
	{ NOT_USED,			        0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ BASE24_LLL_ALPHA,		    12},		/* B24_ATM_TERMINAL_DATA        */


	{ BASE24_LLL_ALPHA,		    /*13: NCCC is 19*/ 19},        /* B24_ATM_CARD_ISS_AUTH_DATA   */
	{ BASE24_FIX_ALPHA,		    13},		/* B24_ATM_POSTAL_CODE	*/
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,         1},			/* B24_ATM_STLMNT_CODE */
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,					0},

    { NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_CR_NBR			*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_CR_REV_NBR		*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_DB_NBR			*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_DB_REV_NBR		*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_TRANSFER_NBR		*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_TRANSFER_REV_NBR */
	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_INQ_NBR			*/

	{ BASE24_FIX_ALPHA,			10},		/* B24_ATM_AUT_NBR			*/
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,			16},		/* B24_ATM_CR_AMOUNT		*/
	{ BASE24_FIX_ALPHA,			16},		/* B24_ATM_CR_REV_AMOUNT	*/
	{ BASE24_FIX_ALPHA,			16},		/* B24_ATM_DB_AMOUNT		*/
	{ BASE24_FIX_ALPHA,			16},		/* B24_ATM_DB_REV_AMOUNT	*/
	{ BASE24_FIX_ALPHA,         42},		/* B24_ATM_ORIG_DATA		*/


   { NOT_USED,					0},
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { BASE24_FIX_ALPHA,		    42},		/* B24_ATM_REPLACEMENT_AMOUNT	*/
   { NOT_USED,			        0},
   { BASE24_FIX_ALPHA,			17},		/* B24_ATM_NET_AMOUNT			*/
   { NOT_USED,					0},
   { BASE24_LL_ALPHA,			11},		/* B24_ATM_STLMNT_INSTI_ID		*/
   { BASE24_LL_ALPHA,			11},		/* B24_ATM_RECEIVER_ID			*/ /* 100 */

   { NOT_USED,					0},
   { BASE24_LL_ALPHA,	        26},		/* B24_ATM_ACCOUNT_ID_1			*/
   { BASE24_LL_ALPHA,	        26},		/* B24_ATM_ACCOUNT_ID_2			*/
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},

   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { BASE24_LLL_ALPHA,	        33},		/* B24_ATM_TERMINAL_ADDR*/

   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
   { BASE24_LLL_ALPHA,	        372},		/* B24_ATM_ACCOUNT_INDICATOR,B24_ATM_STATEMENT_PRINT_DATA*/
   { BASE24_LLL_ALPHA,	        597},		/* B24_ATM_ADD_DATA*/
   { NOT_USED,					0},
   { NOT_USED,	                0},
};


static fld_info_t    tab_Base24POSFldInfo[] =
{
	 { BASE24_FIX_BITMAP,		0},			/* B24_POS_BIT_MAP2				*/
	{ BASE24_LL_ALPHA,			19},		/* B24_POS_CARD_NBR 			*/
	{ BASE24_FIX_ALPHA,			6},			/* B24_POS_PROC_CODE			*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_POS_TRANS_AMOUNT			*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_POS_STLMNT_AMOUNT		*/
	{ BASE24_FIX_ALPHA,			12},		/* B24_POS_BILLING_AMOUNT		*/
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_XMIT_TIME			*/
	{ NOT_USED,			        0},         
	{ NOT_USED,			        0},         
	{ NOT_USED,			        0},        
	{ BASE24_FIX_ALPHA,			6},			/* B24_POS_AUDIT_NBR			*/
	{ BASE24_FIX_ALPHA,			6},			/* B24_POS_TRANS_TIME			*/
	{ BASE24_FIX_ALPHA,			4},			/* B24_POS_TRANS_DATE		    */
	{ BASE24_FIX_ALPHA,         4},			/* B24_POS_EXPIRY_DATE			*/
	{ BASE24_FIX_ALPHA,			4},			/* B24_POS_STLMNT_DATE			*/
	{ NOT_USED,			        0},                                          
	{ BASE24_FIX_ALPHA,			4},			/* B24_POS_CAPTURE_DATE			*/
	{ BASE24_FIX_ALPHA,         4},			/* B24_POS_MERCHANT_TYPE		*/
	{ NOT_USED,			        0},                                   
	{ NOT_USED,			        0},                             
	{ NOT_USED,			        0},                             
	{ BASE24_FIX_ALPHA,         3},			/* B24_POS_ENTRY_MODE			*/
	{ NOT_USED,			        0},                
	{ NOT_USED,			        0},                 
	{ BASE24_FIX_ALPHA,         2},			/* B24_POS_CONDITION_CODE		*/
	{ BASE24_FIX_ALPHA,         2},			/* B24_POS_PIN_CAPTURE_CODE		*/
	{ BASE24_FIX_ALPHA,			1},         /*B24_POS_AUTHOR_ID_LENGTH      */ /*NAB08102020 PLUTONL-2288*/
	{ NOT_USED,					0},       
	{ NOT_USED,		            0},      
	{ NOT_USED,		            0},      
	{ NOT_USED,		            0},     
	{ BASE24_LL_ALPHA,			11},	    /* B24_POS_ACQR_ID				*/
	{ NOT_USED,			        0},          
	{ NOT_USED,					0},          
	{ BASE24_LL_ALPHA,			37},		/* B24_POS_T2_DATA				*/
	{ NOT_USED,			        0},          
	{ BASE24_FIX_ALPHA,		    12},		/* B24_POS_REFERENCE_NBR		*/ 
	{ BASE24_FIX_ALPHA,		    6},			/* B24_POS_AUTHOR_ID			*/
	{ BASE24_FIX_ALPHA,		    2},			/* B24_POS_RESPONSE_CODE		*/
	{ NOT_USED,					0},                 
	{ BASE24_FIX_ALPHA,		    16},	    /* B24_POS_TERMINAL_ID			*/
	{ BASE24_FIX_ALPHA,		    15},	    /* B24_POS_OUTLET_NBR			*/
	{ BASE24_FIX_ALPHA,		    40},	    /* B24_POS_TERMINAL_LOCATION	*/
	{ BASE24_LL_ALPHA,	        25},		/* B24_POS_ADTNL_RESP_DATA		*/
	{ NOT_USED,		            0},             
	{ NOT_USED,		            0},           
	{ NOT_USED,		            0},            
	{ BASE24_LLL_ALPHA,		    27},		/* B24_POS_RETAILER_DATA		*/
	{ BASE24_FIX_ALPHA,		    3},			/* B24_POS_TRANS_CRNCY			*/
	{ BASE24_FIX_ALPHA,			3},			/* B24_POS_STLMNT_CRNCY			*/
	{ BASE24_FIX_ALPHA,			3},			/* B24_POS_BILLING_CRNCY		*/
	{ BASE24_FIX_ALPHA,		    16},		/* B24_POS_PIN_DATA				*/
	{ NOT_USED,			        0},          
	{ BASE24_LLL_ALPHA,		    12},        /* B24_POS_ADTNL_AMOUNTS		*/
	{ NOT_USED,			        0}, 
	{ NOT_USED,			        0},
	{ NOT_USED,		            0}, 
	{ NOT_USED,		            0}, 
	{ NOT_USED,			        0}, 	
	{ BASE24_LLL_ALPHA,		    16},	    /* B24_POS_TERMINAL_DATA		*/
	{ BASE24_LLL_ALPHA,		    19},        /* B24_POS_ISS_AUTH_DATA		*/
	{ BASE24_FIX_ALPHA,		    13},		/* B24_POS_POSTAL_CODE			*/
	{ BASE24_LLL_ALPHA,		    600},       /* B24_POS_ADD_DATA				*/
	{ NOT_USED,			        0},          
	{ NOT_USED,			        0},          
	{ BASE24_FIX_ALPHA,         1},			/* B24_POS_STLMNT_CODE 			*/
	{ NOT_USED,                 0},       
	{ NOT_USED,                 0},   
	{ NOT_USED,                 0},  
	{ NOT_USED,					0},  
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_CR_NBR				*/  
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_CR_REV_NBR			*/  
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_DB_NBR				*/ 
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_DB_REV_NBR			*/ 
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_TRANSFER_NBR			*/ 
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_TRANSFER_REV_NBR 	*/ 
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_INQ_NBR				*/ 
	{ BASE24_FIX_ALPHA,			10},		/* B24_POS_AUT_NBR				*/ 
	{ NOT_USED,                 0}, 
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,			16},		/* B24_POS_CR_AMOUNT			*/ 
	{ BASE24_FIX_ALPHA,			16},		/* B24_POS_CR_REV_AMOUNT		*/ 
	{ BASE24_FIX_ALPHA,			16},		/* B24_POS_DB_AMOUNT			*/ 
	{ BASE24_FIX_ALPHA,			16},		/* B24_POS_DB_REV_AMOUNT		*/ 
	{ BASE24_FIX_ALPHA,         42},		/* B24_POS_ORIG_DATA			*/ 
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ BASE24_FIX_ALPHA,		    42},		/* B24_POS_REPLACEMENT_AMOUNT	*/ 
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			17},		/* B24_POS_NET_AMOUNT  			*/
	{ NOT_USED,					0},     
	{ BASE24_LL_ALPHA,			11},		/* B24_POS_STLMNT_INSTI_ID		*/
	{ BASE24_LL_ALPHA,			11},		/* B24_POS_RECEIVER_ID			*/
	{ NOT_USED,	                0}, 
	{ BASE24_LL_ALPHA,	        28 /*0*/},  /*B24_POS_ACCOUNT_ID1			*/ /*NAB08102020 PLUTONL-2288*/
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ NOT_USED,	                0},
	{ BASE24_LLL_ALPHA,	       29},		        /* B24_POS_TERMINAL_ADDR	*/
	{ BASE24_LLL_ALPHA/*NOT_USED*/,	20/*0*/},   /*B24_POS_ADD_POS_IDENTIFICATION*//*NAB08102020 PLUTONL-2288*/
	{ BASE24_LLL_ALPHA/*NOT_USED*/,11/*0*/},    /*B24_POS_CARD_ISS_ID_CODE	*/	/*NAB08102020   PLUTONL-2288*/
	{ BASE24_LLL_ALPHA,	        20},		    /* B24_POS_INVOICE_DATA		 */
	{ BASE24_LLL_ALPHA/*NOT_USED*/,12/*0*/},         /* B24_POS_BATCH_SHIFT_SETT_DATA	 *//*NAB08102020   PLUTONL-2288*/
	{ BASE24_LLL_ALPHA,	        12},		   /* B24_POS_SETTLEMENT_DATA	 */
	{ BASE24_LLL_ALPHA,	        38},		   /* B24_POS_PRE_AUTH_AND_CHARGEBACK */
	{ BASE24_LLL_ALPHA,	        100},		   /* B24_POS_USER_DATA		   			*/
	{ NOT_USED,	                0},
};



static fld_info_t    tab_Base24BaseFldInfo[] =
{
	{ BASE24_FIX_BITMAP,		0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			10},		/* XMIT_TIME			*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ BASE24_FIX_ALPHA,			6},			/* AUDIT_NBR			*/
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			4},			/* B24_BASE_STLMNT_DATE */
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},

	{ NOT_USED,		            0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ BASE24_FIX_ALPHA,		    2},			/* B24_BASE_RESPONSE_CODE		*/
	{ NOT_USED,					0},

	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ BASE24_LLL_ALPHA,		    76},		/* B24_BASE_ADTNL_DATA_PRIV		*/
	{ NOT_USED,					0},
	{ NOT_USED,			        0},

	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ BASE24_FIX_ALPHA,			16}, /*B24_BASE_SECURITY*/
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},


	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,			3}, /*B24_BASE_NW_MNGMT_INFO_CODE*/

    { NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},

	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,					0},


   { NOT_USED,					0},
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { NOT_USED,					0},
   { NOT_USED,			        0},
   { NOT_USED,		            0},
   { NOT_USED,					0},
   { NOT_USED,			        0},
   { NOT_USED,					0},

   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},

   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { BASE24_LLL_ALPHA,			6},		/*B24_BASE_SYS_KEY_MNG*/

   { NOT_USED,					0},
   { NOT_USED,					0},
   { BASE24_LLL_ALPHA,			550},	/*B24_BASE_CRYPTO_SERV_MSG*/
   { NOT_USED,	                0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
};


static fld_info_t    tab_Base24HostFldInfo[] =
{
	{ BASE24_FIX_BITMAP,		0},
	{ BASE24_LL_ALPHA,			19},		/* B24_HOST_CARD_NBR 			*/
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ BASE24_FIX_ALPHA,			10},		/* B24_HOST_XMIT_TIME			*/
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ BASE24_FIX_ALPHA,			6},			/* B24_HOST_AUDIT_NBR			*/
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},

	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,					9},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},

	{ NOT_USED,		            0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ BASE24_FIX_ALPHA,		    2},			/* B24_HOST_RESPONSE_CODE		*/
	{ NOT_USED,					0},

	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ BASE24_LLL_ALPHA,		    76},		/* B24_HOST_ADTNL_DATA_PRIV		*/
	{ BASE24_FIX_ALPHA,		    3},			/* B24_HOST_TRANS_CRNCY			*/
	{ NOT_USED,			        0},

	{ NOT_USED,			        0},
	{ NOT_USED,					0},
	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ BASE24_LLL_ALPHA,		    58},		/* B24_HOST_TERMINAL_DATA	*/


	{ NOT_USED,					0},
	{ NOT_USED,		            0},
	{ NOT_USED,		            0},
	{ NOT_USED,			        0},
	{ NOT_USED,			        0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,					0},

    { NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ BASE24_FIX_ALPHA,         6},		/* B24_HOST_ACTION_DATE */
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},

	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,                 0},
	{ NOT_USED,					0},


   { BASE24_FIX_ALPHA,			1},		/* B24_HOST_FILE_UPDATE_CODE */
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { NOT_USED,		            0},
   { NOT_USED,					0},
   { NOT_USED,			        0},
   { NOT_USED,		            0},
   { NOT_USED,					0},
   { NOT_USED,			        0},
   { NOT_USED,					0},

   { BASE24_LL_ALPHA,			/*2*/17},		/* B24_HOST_FILE_NAME */
   { NOT_USED,					0},
   { NOT_USED,					0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { NOT_USED,	                0},

   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { BASE24_LLL_ALPHA,                  89},           /* B24_AUT_HC_UPD_INFO          */
   { NOT_USED,	                0},                    
   { BASE24_LLL_ALPHA,			150},		/* B24_HOST_CAF_BASE_USER_INFO		*/
   { BASE24_LLL_ALPHA,			152},		/* B24_HOST_CAF_NON_CRNCY_DISPENSE	*/
   { BASE24_LLL_ALPHA,			20},		/* B24_HOST_CAF_EMV					*/
   { NOT_USED,	                0},
   { NOT_USED,	                0},
   { BASE24_LLL_ALPHA,			/*109*/150},		/*B24_HOST_BASE_SEG_INFO			*/

   { BASE24_LLL_ALPHA,			76},		/* B24_HOST_CAF_ATM_SEG_INFO		*/
   { BASE24_LLL_ALPHA,			117},		/* B24_HOST_CAF_POS_SEG_INFO		*/
   { BASE24_LLL_ALPHA,			223},		/* B24_HOST_CAF_CARD_SEG_INFO		*/
   { NOT_USED,	                0},
   { NOT_USED,					0},
   { BASE24_LLL_ALPHA,			690},		/* B24_HOST_CAF_ACC_SEG_INFO		*/
   { BASE24_LLL_ALPHA,			29},		/* B24_HOST_CAF_ADDR_SEG_INFO		*/
   { NOT_USED,	                0},
};
/******************************************************************************/
int GetBase24Product(char* sProductIndicator)
{

    if( memcmp(sProductIndicator,B24_PRODUCT_IND_BASE,B24_PRODUCT_IND_LEN) == 0 )
        return B24_PRODUCT_IND_BASE_T;
    if( memcmp(sProductIndicator,B24_PRODUCT_IND_ATM,B24_PRODUCT_IND_LEN) == 0 )
        return B24_PRODUCT_IND_ATM_T;
    if( memcmp(sProductIndicator,B24_PRODUCT_IND_POS,B24_PRODUCT_IND_LEN) == 0 )
        return B24_PRODUCT_IND_POS_T;
    if( memcmp(sProductIndicator,B24_PRODUCT_IND_HOST,B24_PRODUCT_IND_LEN) == 0 )
        return B24_PRODUCT_IND_HOST_T;

    return B24_PRODUCT_IND_QTY;
}
/******************************************************************************/
void  InitBase24Info(TSBase24Info *msgInfo)
{
	memset((char*)msgInfo->nFieldPos,0,sizeof(msgInfo->nFieldPos));
	memset(msgInfo->sBitMap,0,sizeof(msgInfo->sBitMap));
	msgInfo->nLength = 0;
	msgInfo->nMsgType = 0;
	memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
	memset(( char *) & (msgInfo->sHeader), '\0', sizeof(msgInfo->sHeader));
	msgInfo->nProductIndicator = B24_PRODUCT_IND_QTY;
	InitTlvSwitchHeader(&msgInfo->SwitchHeader);
}

void  InitBase24ProductInfo(TSBase24Info *msgInfo, int nProductIndicator)
{
	InitBase24Info(msgInfo);
	msgInfo->nProductIndicator = nProductIndicator;
}

void  InitBase24InfoReplyTo(TSBase24Info *msgInfo, TSBase24Info *OrigMsgInfo)
{
	InitBase24Info(msgInfo);
	memcpy((char*)&msgInfo->sHeader,(char*)&OrigMsgInfo->sHeader,sizeof(msgInfo->sHeader));
	memcpy((char*)&msgInfo->SwitchHeader,(char*)&OrigMsgInfo->SwitchHeader,sizeof(msgInfo->SwitchHeader));
	msgInfo->nProductIndicator = OrigMsgInfo->nProductIndicator;

	msgInfo->sHeader.sResponderCode = B24_H_ENT_CD_DFLT_RESPONDER;
}
/******************************************************************************/
void  InitBase24BaseInfo(TSBase24Info *msgInfo)
{
	InitBase24Info(msgInfo);
	msgInfo->nProductIndicator = B24_PRODUCT_IND_BASE_T;
}

int GetBase24BaseFieldType(int field_no)
{
   return(tab_Base24BaseFldInfo[field_no].type);
}

int GetBase24BaseFieldLength(int field_no)
{
   return(tab_Base24BaseFldInfo[field_no].length);
}
/******************************************************************************/
void  InitBase24ATMInfo(TSBase24Info *msgInfo)
{
	InitBase24Info(msgInfo);
	msgInfo->nProductIndicator = B24_PRODUCT_IND_ATM_T;
}

int GetBase24ATMFieldType(int field_no)
{
   return(tab_Base24ATMFldInfo[field_no].type);
}

int GetBase24ATMFieldLength(int field_no)
{
   return(tab_Base24ATMFldInfo[field_no].length);
}
/******************************************************************************/
void  InitBase24POSInfo(TSBase24Info *msgInfo)
{
	InitBase24Info(msgInfo);
	msgInfo->nProductIndicator = B24_PRODUCT_IND_POS_T;
}

int GetBase24POSFieldType(int field_no)
{
   return(tab_Base24POSFldInfo[field_no].type);
}

int GetBase24POSFieldLength(int field_no)
{
   return(tab_Base24POSFldInfo[field_no].length);
}
/******************************************************************************/
void  InitBase24HostInfo(TSBase24Info *msgInfo)
{
	InitBase24Info(msgInfo);
	msgInfo->nProductIndicator = B24_PRODUCT_IND_HOST_T;
}
int GetBase24HostFieldType(int field_no)
{
   return(tab_Base24HostFldInfo[field_no].type);
}

int GetBase24HostFieldLength(int field_no)
{
   return(tab_Base24HostFldInfo[field_no].length);
}
/******************************************************************************/
/*
int GetBase24DefaultFieldType(int field_no)
{
   return(BASE24_UNDEF);
}

int GetBase24DefaultFieldLength(int field_no)
{
   return(0);
}
*/
/******************************************************************************/
typedef int (*GetFieldType)(int);
typedef int (*GetFieldLength)(int);

static GetFieldType fnGetBase24FieldType[B24_PRODUCT_IND_QTY] = {
&GetBase24BaseFieldType,
&GetBase24ATMFieldType,
&GetBase24POSFieldType,
&GetBase24HostFieldType,
};

static GetFieldLength fnGetBase24FieldLength[B24_PRODUCT_IND_QTY] = {
&GetBase24BaseFieldLength,
&GetBase24ATMFieldLength,
&GetBase24POSFieldLength,
&GetBase24HostFieldLength,
};

/******************************************************************************/
/*inline*/ int GetBase24FieldType(int field_no, int nProductIndicator)
{
   return((*fnGetBase24FieldType[nProductIndicator])(field_no));
}

/*inline*/ int GetBase24FieldLength(int field_no, int nProductIndicator)
{
   return((*fnGetBase24FieldLength[nProductIndicator])(field_no));
}
/******************************************************************************/

