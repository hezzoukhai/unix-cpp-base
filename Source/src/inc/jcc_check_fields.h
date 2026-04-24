#ifndef HPS_JCC_CHECK_FIELDS_H
#define HPS_JCC_CHECK_FIELDS_H

#define FIX    	 1
#define LLVAR  	 2
#define LLLVAR 	 3
#define UNUSED 4
#define N   0
#define M   1
#define O   2

typedef struct SHidFields
{
   int           nPresence; /* N: Not Present M: Mandatory O: Optionnel  */
   int           nLength;   /* Length In Case Of Fix Type */
   int           nType;     /* LLVAR,FIX,LLLVAR           */
}TSHidFields;


TSHidFields JccField_0100[] =
{
   { N,     0,      UNUSED },/* BIT_MAP2                       */
   { M,     0,      LLVAR    },/* CARD_NBR                       */
   { M,     6,      FIX      },/* PROC_CODE                      */
   { M,     12,     FIX      },/* TRANS_AMOUNT                   */
   { N,     12,     FIX      },/* STLMNT_AMOUNT                  */
   { N,     12,     FIX      },/* CARDHOLDER BILLING AMOUNT      */
   { M,     10,     FIX      },/* XMIT_TIME                      */
   { N,     0,      UNUSED },/* CARDHOLDER BILLING FEE         */
   { N,     0,      UNUSED },/* CONVRSN_RATE_STLMNT            */
   { N,     8,      FIX      },/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { M,     6,      FIX      },/* AUDIT_NBR             */
   { M,     6,      FIX      },/* TRANS_TIME            */
   { M,     4,      FIX      },/* TRANS_DATE            */
   { O,     4,      FIX      },/* EXPIRY_DATE           */	/*SMA20140731*/
   { N,     4,      FIX      },/* STLMNT_DATE           */
   { N,     4,      FIX      },/* CNVRSN_DATE           */
   { O,     4,      FIX      },/* CAPTURE_DATE          */
   { M,     4,      FIX      },/* MERCHANT_TYPE         */
   { M,     3,      FIX      },/* ACQR_COUNTRY          */
   { N,     0,      UNUSED },/* PAN_COUNTRY           */

   { N,     3,      FIX      },/* FORWD_COUNTRY                     */
   { M,     3,      FIX      },/* POS_ENTRY_MODE                    */
   { N,     3,      FIX },/* CARD SEQUENCE NUMBER              */ /* LHI 230507 */
   { N,     0,      UNUSED },/* NETWORK INTERNATIONAL IDENTIFIER  */
   { M,     2,      FIX      },/* POS_CONDITION                     */
   { N,     0,      UNUSED },/* POS_PIN_CAPTURE                   */
   { N,     0,      UNUSED },/* AUTHORIZATION ID RESPONSE LENGTH  */
   { N,     0,      UNUSED },/* TRANS_FEE                         */
   { N,     0,      UNUSED },/* AMOUNT SETTLEMENT FEE             */
   { N,     0,      UNUSED },/* AMOUNT TRANSACTION PROCESSING FEE */

   { N,     0,      UNUSED },/* AMOUNT SETLMNT PROCESSING FEE     */
   { M,     3,      UNUSED },/* ACQR_ID                           */
   { N,     3,      UNUSED },/* FORWD_ID                          */
   { N,     0,      UNUSED },/* PAN_EXTENDED                      */
   { N,     37,     LLVAR    },/* T2_DATA                           */
   { N,     0,      UNUSED },/* TRACK 3 DATA                      */
   { M,     12,     FIX      },/* REFERENCE_NBR                     */
   { N,     6,      FIX      },/* AUTHOR_ID                         */
   { N,     2,      FIX      },/* RESPONSE_CODE                     */
   { N,     0,      UNUSED },/* SERVICE RESTRICTION CODE          */

   { M,     8,      FIX      },/* TERMINAL_NBR                      */
   { M,     15,     FIX      },/* OUTLET_NBR                        */
   { M,     40,     FIX    },/* TERMINAL_ADR                      */
   { N,     44,     LLVAR    },/* ADTNL_RESP_DATA                   */
   { N,     76,     LLVAR    },/* T1_DATA                           */
   { N,     0,      UNUSED },/* ADDITRIONAL DATA ISO              */
   { N,     0,      UNUSED },/* ADDITIONAL DATA NATIONAL          */
   { N,     999,    LLLVAR   },/* ADTNL_DATA_PRIV                   */
   { M,     3,      FIX      },/* TRANS_CRNCY                       */
   { N,     3,      FIX      },/* STLMNT_CRNCY                      */

   { N,     3,      FIX      },/* BILLING_CRNCY         */
   { N,     16,     FIX      },/* PIN_DATA              */
   { N,     16,     FIX      },/* SECURITY_DATA         */
   { N,     120,    LLLVAR   },/* ADTNL_AMOUNTS         */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* GEOGR_DATA            */
   { N,     999,    LLLVAR   },/* ADTNL_POS_INFO        */

   { N,     999,    LLLVAR   },/* POS DATA              */
   { N,     0,      UNUSED },/* CPS_FIELDS            */
   { N,     0,      UNUSED },/* PRIV_USE_FIELDS       */
   { N,     0,      UNUSED },/* MAC1                  */
   { N,     0,      UNUSED },/*                       */
   { N,     0,      UNUSED },/* STLMNT_CODE           */
   { N,     0,      UNUSED },/* Extended Payment Code */
   { N,     0,      UNUSED },/* RECEIVER_COUNTRY      */
   { N,     0,      UNUSED },/* STLMNT_INST_COUNTRY   */
   { N,     0,      UNUSED },/* NW_MNGMT_INFO         */

   { N,     0,      UNUSED },/* Message Number              */
   { N,     0,      UNUSED },/* Message number Last         */
   { N,     0,      UNUSED },/* ACTION_DATE                 */
   { N,     0,      UNUSED },/* CR_NBR                      */
   { N,     0,      UNUSED },/* CR_REV_NBR                  */
   { N,     0,      UNUSED },/* DB_NBR                      */
   { N,     0,      UNUSED },/* DB_REV_NBR                  */
   { N,     0,      UNUSED },/* Transfer Number             */
   { N,     0,      UNUSED },/* Transfer Reversal Number    */
   { N,     0,      UNUSED },/* Inquiries Number            */

   { N,     0,      UNUSED },/* Authorizations Number          */
   { N,     0,      UNUSED },/* Credit, Processing Fee Amount  */
   { N,     0,      UNUSED },/* Credit, Transaction Fee Amount */
   { N,     0,      UNUSED },/* Debit, Processing Fee Amount   */
   { N,     0,      UNUSED },/* Debit, Transaction Fee Amount  */
   { N,     0,      UNUSED },/* CR_AMOUNT                      */
   { N,     0,      UNUSED },/* CR_REV_AMOUNT                  */
   { N,     0,      UNUSED },/* DB_AMOUNT                      */
   { N,     0,      UNUSED },/* DB_REV_AMOUNT                  */
   { N,     0,      UNUSED },/* ORIG_DATA                      */

   { N,     0,      UNUSED },/* FILE_UPDATE           */
   { N,     0,      UNUSED },/* FILE_SECURITY         */
   { N,     0,      UNUSED },/* Response Indicator    */
   { N,     0,      UNUSED },/* Service Indicator     */
   { N,     0,      UNUSED },/* REPLACE_AMOUNTS       */
   { N,     0,      UNUSED },/* MSG_SECURITY          */
   { N,     0,      UNUSED },/* NET_AMOUNT            */
   { N,     0,      UNUSED },/* Payee                 */
   { N,     0,      UNUSED },/* STLMNT_INSTITN        */
   { N,     0,      UNUSED },/* RECEIVER_ID           */

   { N,     0,      UNUSED },/* FILE_NAME             */
   { N,     0,      UNUSED },/* ACCOUNT_ID1           */
   { N,     0,      UNUSED },/* ACCOUNT_ID2           */
   { N,     0,      LLLVAR },/* UNUSED }, ENH15.1 SWI24022015*//* Transaction Description   */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */

   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* ADTNL_TRACE_DATA      */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Intra Country Data    */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Original Message Type Id  */

   { N,     0,      UNUSED },/* Issuing Institution Id        */
   { N,     0,      UNUSED },/* Remaining Open to Use         */
   { N,     0,      UNUSED },/* ADR_VERIF_DATA                */
   { N,     0,      UNUSED },/* Free Form Text - Japan        */
   { N,     0,      UNUSED },/* SUPPORT_INFO                  */
   { N,     0,      UNUSED },/* Reserved Private              */
   { N,     0,      UNUSED },/* File Record Action and Data   */
   { N,     0,      UNUSED },/* MAC2                          */
   { N,     0,      UNUSED },/* Field 129                     */
   { N,     0,      UNUSED },/* Terminal Capability Profile   */

   { N,     0,      UNUSED },/* Terminal Verification Results */
   { N,     0,      UNUSED },/* Unpredictable Number          */
   { N,     0,      UNUSED },/* Terminal Serial Number        */
   { N,     0,      UNUSED },/* Visa Discretionary Data       */
   { N,     0,      UNUSED },/* Issuer Discretionary Data     */
   { N,     0,      UNUSED },/* Issuer Discretionary Data     */
   { N,     0,      UNUSED },/* Application Trans. Counter    */
   { N,     0,      UNUSED },/* Application Inter. Profile    */
   { N,     0,      UNUSED },/* ARPC + Response Code          */
   { N,     0,      UNUSED },/* Field 140                     */

   { N,     0,      UNUSED },/* Field 141                     */
   { N,     0,      UNUSED },/* Issuer Script                 */
   { N,     0,      UNUSED },/* Issuer Script Results         */
   { N,     0,      UNUSED },/* Cryptogram Trans Type         */
   { N,     0,      UNUSED },/* Terminal Country Code         */
   { N,     0,      UNUSED },/* Terminal Trans  Date          */
   { N,     0,      UNUSED },/* Cryptogram Amount             */
   { N,     0,      UNUSED },/* Cryptogram Currency Code      */
   { N,     0,      UNUSED },/* Cryptogram Cash Back          */
   { N,     0,      UNUSED },/* Cardholder Card Scheme Crypto.*/

   { N,     0,      UNUSED },/* Field 151             */
   { N,     0,      UNUSED },/* Field 152             */
   { N,     0,      UNUSED },/* Field 153             */
   { N,     0,      UNUSED },/* Field 154             */
   { N,     0,      UNUSED },/* Field 155             */
   { N,     0,      UNUSED },/* Field 156             */
   { N,     0,      UNUSED },/* Field 157             */
   { N,     0,      UNUSED },/* Field 158             */
   { N,     0,      UNUSED },/* Field 159             */
   { N,     0,      UNUSED },/* Field 160             */

   { N,     0,      UNUSED },/* Field 161             */
   { N,     0,      UNUSED },/* Field 162             */
   { N,     0,      UNUSED },/* Field 163             */
   { N,     0,      UNUSED },/* Field 164             */
   { N,     0,      UNUSED },/* Field 165             */
   { N,     0,      UNUSED },/* Field 166             */
   { N,     0,      UNUSED },/* Field 167             */
   { N,     0,      UNUSED },/* Field 168             */
   { N,     0,      UNUSED },/* Field 169             */
   { N,     0,      UNUSED },/* Field 170             */

   { N,     0,      UNUSED },/* Field 171             */
   { N,     0,      UNUSED },/* Field 172             */
   { N,     0,      UNUSED },/* Field 173             */
   { N,     0,      UNUSED },/* Field 174             */
   { N,     0,      UNUSED },/* Field 175             */
   { N,     0,      UNUSED },/* Field 176             */
   { N,     0,      UNUSED },/* Field 177             */
   { N,     0,      UNUSED },/* Field 178             */
   { N,     0,      UNUSED },/* Field 179             */
   { N,     0,      UNUSED },/* Field 180             */

   { N,     0,      UNUSED },/* Field 181             */
   { N,     0,      UNUSED },/* Field 182             */
   { N,     0,      UNUSED },/* Field 183             */
   { N,     0,      UNUSED },/* Field 184             */
   { N,     0,      UNUSED },/* Field 185             */
   { N,     0,      UNUSED },/* Field 186             */
   { N,     0,      UNUSED },/* Field 187             */
   { N,     0,      UNUSED },/* Field 188             */
   { N,     0,      UNUSED },/* Field 189             */
   { N,      UNUSED },/* Field 190             */

   { N,     0,      UNUSED },/* Field 191             */
   { N,     0,      UNUSED },/* Message Authentication Code   */
};

TSHidFields JccField_0110[] =
{
   { N,     0,      UNUSED },/* BIT_MAP2                       */
   { M,     0,      LLVAR    },/* CARD_NBR                       */
   { M,     6,      FIX      },/* PROC_CODE                      */
   { M,     12,     FIX      },/* TRANS_AMOUNT                   */
   { N,     12,     FIX      },/* STLMNT_AMOUNT                  */
   { N,     12,     FIX      },/* CARDHOLDER BILLING AMOUNT      */
   { M,     10,     FIX      },/* XMIT_TIME                      */
   { N,     0,      UNUSED },/* CARDHOLDER BILLING FEE         */
   { N,     0,      UNUSED },/* CONVRSN_RATE_STLMNT            */
   { N,     8,      FIX      },/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { M,     6,      FIX      },/* AUDIT_NBR             */
   { M,     6,      FIX      },/* TRANS_TIME            */
   { M,     4,      FIX      },/* TRANS_DATE            */
   { N,     4,      FIX      },/* EXPIRY_DATE           */
   { N,     4,      FIX      },/* STLMNT_DATE           */
   { N,     4,      FIX      },/* CNVRSN_DATE           */
   { O,     4,      FIX      },/* CAPTURE_DATE          */
   { N,     4,      FIX      },/* MERCHANT_TYPE         */
   { N,     3,      FIX      },/* ACQR_COUNTRY          */
   { N,     0,      UNUSED },/* PAN_COUNTRY           */

   { N,     3,      FIX      },/* FORWD_COUNTRY                     */
   { N,     4,      UNUSED      },/* POS_ENTRY_MODE                    */
   { N,     0,      UNUSED },/* CARD SEQUENCE NUMBER              */
   { N,     0,      UNUSED },/* NETWORK INTERNATIONAL IDENTIFIER  */
   { N,     2,      UNUSED },/* POS_CONDITION                     */
   { N,     0,      UNUSED },/* POS_PIN_CAPTURE                   */
   { N,     0,      UNUSED },/* AUTHORIZATION ID RESPONSE LENGTH  */
   { N,     0,      UNUSED },/* TRANS_FEE                         */
   { N,     0,      UNUSED },/* AMOUNT SETTLEMENT FEE             */
   { N,     0,      UNUSED },/* AMOUNT TRANSACTION PROCESSING FEE */

   { N,     0,      UNUSED },/* AMOUNT SETLMNT PROCESSING FEE     */
   { N,     3,      UNUSED },/* ACQR_ID                           */
   { N,     3,      UNUSED },/* FORWD_ID                          */
   { N,     0,      UNUSED },/* PAN_EXTENDED                      */
   { N,     37,     LLVAR    },/* T2_DATA                           */
   { N,     0,      UNUSED },/* TRACK 3 DATA                      */
   { N,     12,     FIX      },/* REFERENCE_NBR                     */
   { N,     6,      FIX      },/* AUTHOR_ID                         */
   { M,     2,      FIX      },/* RESPONSE_CODE                     */
   { N,     0,      UNUSED },/* SERVICE RESTRICTION CODE          */

   { N,     8,      FIX      },/* TERMINAL_NBR                      */
   { N,     15,     FIX      },/* OUTLET_NBR                        */
   { N,     40,     UNUSED    },/* TERMINAL_ADR                      */
   { N,     44,     LLVAR    },/* ADTNL_RESP_DATA                   */
   { N,     76,     LLVAR    },/* T1_DATA                           */
   { N,     0,      UNUSED },/* ADDITRIONAL DATA ISO              */
   { N,     0,      UNUSED },/* ADDITIONAL DATA NATIONAL          */
   { N,     999,    LLLVAR   },/* ADTNL_DATA_PRIV                   */
   { M,     3,      FIX      },/* TRANS_CRNCY                       */
   { N,     3,      FIX      },/* STLMNT_CRNCY                      */

   { N,     3,      FIX      },/* BILLING_CRNCY         */
   { N,     16,     FIX      },/* PIN_DATA              */
   { N,     16,     FIX      },/* SECURITY_DATA         */
   { N,     120,    LLLVAR   },/* ADTNL_AMOUNTS         */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* GEOGR_DATA            */
   { N,     999,    LLLVAR   },/* ADTNL_POS_INFO        */

   { N,     999,    LLLVAR   },/* POS DATA              */
   { N,     0,      UNUSED },/* CPS_FIELDS            */
   { N,     0,      UNUSED },/* PRIV_USE_FIELDS       */
   { N,     0,      UNUSED },/* MAC1                  */
   { N,     0,      UNUSED },/*                       */
   { N,     0,      UNUSED },/* STLMNT_CODE           */
   { N,     0,      UNUSED },/* Extended Payment Code */
   { N,     0,      UNUSED },/* RECEIVER_COUNTRY      */
   { N,     0,      UNUSED },/* STLMNT_INST_COUNTRY   */
   { N,     0,      UNUSED },/* NW_MNGMT_INFO         */

   { N,     0,      UNUSED },/* Message Number              */
   { N,     0,      UNUSED },/* Message number Last         */
   { N,     0,      UNUSED },/* ACTION_DATE                 */
   { N,     0,      UNUSED },/* CR_NBR                      */
   { N,     0,      UNUSED },/* CR_REV_NBR                  */
   { N,     0,      UNUSED },/* DB_NBR                      */
   { N,     0,      UNUSED },/* DB_REV_NBR                  */
   { N,     0,      UNUSED },/* Transfer Number             */
   { N,     0,      UNUSED },/* Transfer Reversal Number    */
   { N,     0,      UNUSED },/* Inquiries Number            */

   { N,     0,      UNUSED },/* Authorizations Number          */
   { N,     0,      UNUSED },/* Credit, Processing Fee Amount  */
   { N,     0,      UNUSED },/* Credit, Transaction Fee Amount */
   { N,     0,      UNUSED },/* Debit, Processing Fee Amount   */
   { N,     0,      UNUSED },/* Debit, Transaction Fee Amount  */
   { N,     0,      UNUSED },/* CR_AMOUNT                      */
   { N,     0,      UNUSED },/* CR_REV_AMOUNT                  */
   { N,     0,      UNUSED },/* DB_AMOUNT                      */
   { N,     0,      UNUSED },/* DB_REV_AMOUNT                  */
   { N,     0,      UNUSED },/* ORIG_DATA                      */

   { N,     0,      UNUSED },/* FILE_UPDATE           */
   { N,     0,      UNUSED },/* FILE_SECURITY         */
   { N,     0,      UNUSED },/* Response Indicator    */
   { N,     0,      UNUSED },/* Service Indicator     */
   { N,     0,      UNUSED },/* REPLACE_AMOUNTS       */
   { N,     0,      UNUSED },/* MSG_SECURITY          */
   { N,     0,      UNUSED },/* NET_AMOUNT            */
   { N,     0,      UNUSED },/* Payee                 */
   { N,     0,      UNUSED },/* STLMNT_INSTITN        */
   { N,     0,      UNUSED },/* RECEIVER_ID           */

   { N,     0,      UNUSED },/* FILE_NAME             */
   { N,     0,      UNUSED },/* ACCOUNT_ID1           */
   { N,     0,      UNUSED },/* ACCOUNT_ID2           */
   { N,     0,      LLLVAR },/* UNUSED }, ENH15.1 SWI24022015*//* Transaction Description   */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved Iso          */

   { N,     0,      UNUSED },/* Reserved Iso          */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* ADTNL_TRACE_DATA      */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Intra Country Data    */
   { N,     0,      UNUSED },/* Reserved National     */
   { N,     0,      UNUSED },/* Original Message Type Id  */

   { N,     0,      UNUSED },/* Issuing Institution Id        */
   { N,     0,      UNUSED },/* Remaining Open to Use         */
   { N,     0,      UNUSED },/* ADR_VERIF_DATA                */
   { N,     0,      UNUSED },/* Free Form Text - Japan        */
   { N,     0,      UNUSED },/* SUPPORT_INFO                  */
   { N,     0,      UNUSED },/* Reserved Private              */
   { N,     0,      UNUSED },/* File Record Action and Data   */
   { N,     0,      UNUSED },/* MAC2                          */
   { N,     0,      UNUSED },/* Field 129                     */
   { N,     0,      UNUSED },/* Terminal Capability Profile   */

   { N,     0,      UNUSED },/* Terminal Verification Results */
   { N,     0,      UNUSED },/* Unpredictable Number          */
   { N,     0,      UNUSED },/* Terminal Serial Number        */
   { N,     0,      UNUSED },/* Visa Discretionary Data       */
   { N,     0,      UNUSED },/* Issuer Discretionary Data     */
   { N,     0,      UNUSED },/* Issuer Discretionary Data     */
   { N,     0,      UNUSED },/* Application Trans. Counter    */
   { N,     0,      UNUSED },/* Application Inter. Profile    */
   { N,     0,      UNUSED },/* ARPC + Response Code          */
   { N,     0,      UNUSED },/* Field 140                     */

   { N,     0,      UNUSED },/* Field 141                     */
   { N,     0,      UNUSED },/* Issuer Script                 */
   { N,     0,      UNUSED },/* Issuer Script Results         */
   { N,     0,      UNUSED },/* Cryptogram Trans Type         */
   { N,     0,      UNUSED },/* Terminal Country Code         */
   { N,     0,      UNUSED },/* Terminal Trans  Date          */
   { N,     0,      UNUSED },/* Cryptogram Amount             */
   { N,     0,      UNUSED },/* Cryptogram Currency Code      */
   { N,     0,      UNUSED },/* Cryptogram Cash Back          */
   { N,     0,      UNUSED },/* Cardholder Card Scheme Crypto.*/

   { N,     0,      UNUSED },/* Field 151             */
   { N,     0,      UNUSED },/* Field 152             */
   { N,     0,      UNUSED },/* Field 153             */
   { N,     0,      UNUSED },/* Field 154             */
   { N,     0,      UNUSED },/* Field 155             */
   { N,     0,      UNUSED },/* Field 156             */
   { N,     0,      UNUSED },/* Field 157             */
   { N,     0,      UNUSED },/* Field 158             */
   { N,     0,      UNUSED },/* Field 159             */
   { N,     0,      UNUSED },/* Field 160             */

   { N,     0,      UNUSED },/* Field 161             */
   { N,     0,      UNUSED },/* Field 162             */
   { N,     0,      UNUSED },/* Field 163             */
   { N,     0,      UNUSED },/* Field 164             */
   { N,     0,      UNUSED },/* Field 165             */
   { N,     0,      UNUSED },/* Field 166             */
   { N,     0,      UNUSED },/* Field 167             */
   { N,     0,      UNUSED },/* Field 168             */
   { N,     0,      UNUSED },/* Field 169             */
   { N,     0,      UNUSED },/* Field 170             */

   { N,     0,      UNUSED },/* Field 171             */
   { N,     0,      UNUSED },/* Field 172             */
   { N,     0,      UNUSED },/* Field 173             */
   { N,     0,      UNUSED },/* Field 174             */
   { N,     0,      UNUSED },/* Field 175             */
   { N,     0,      UNUSED },/* Field 176             */
   { N,     0,      UNUSED },/* Field 177             */
   { N,     0,      UNUSED },/* Field 178             */
   { N,     0,      UNUSED },/* Field 179             */
   { N,     0,      UNUSED },/* Field 180             */

   { N,     0,      UNUSED },/* Field 181             */
   { N,     0,      UNUSED },/* Field 182             */
   { N,     0,      UNUSED },/* Field 183             */
   { N,     0,      UNUSED },/* Field 184             */
   { N,     0,      UNUSED },/* Field 185             */
   { N,     0,      UNUSED },/* Field 186             */
   { N,     0,      UNUSED },/* Field 187             */
   { N,     0,      UNUSED },/* Field 188             */
   { N,     0,      UNUSED },/* Field 189             */
   { N,      UNUSED },/* Field 190             */

   { N,     0,      UNUSED },/* Field 191             */
   { N,     0,      UNUSED },/* Message Authentication Code   */
};


#endif /*  HPS_JCC_CHECK_FIELDS_H */
