#define TRUE 1
#define FALSE 0

#include <tools.h>/*LBN20201204 CAPS-2307 V1.6*/

/*#define PROCESSING 0 SWI14022011*/
/*#define TRACE 1 SWI14022011***/

#define UCHAR unsigned char

/* CB2A fields [x=(field number-1)]*/
#define MESS_TYPE             0
#define BIT_MAP2              1
#define CARD_NBR              2
#define PROC_CODE             3
#define TRANS_AMOUNT          4
#define STLMNT_AMOUNT         5
#define XMIT_TIME             7
#define CNVRSN_RATE           9
#define AUDIT_NBR             11
#define TRANS_DATE_TIME       12
#define TRANS_TIME            12
#define TRANS_DATE            13
#define EXPIRY_DATE           14
#define STLMNT_DATE           15
#define CNVRSN_DATE           16
#define MCC                   18
#define ACQR_COUNTRY          19
#define ISSUER_COUNTRY        20
#define POS_CAPABILITY        21
#define POS_ENTRY_MODE        22
#define POS_DATA              22
#define CARD_SEQ              23
#define FUNCTION_CODE         24
#define POS_CONDITION         25
#define MSG_REASON            25
#define POS_PIN_CAPTURE       26
#define BUSINESS_CODE         26
#define UPLOAD_CONTROL        26
#define DOWNLOAD_CONTROL      27
#define AUTHOR_ID_LENGTH      27
#define RECONC_DATE           28
#define RECONC_INDICATOR      29
#define ORIGINAL_AMOUNTS      30
#define PRT_DPY_DATA          31
#define ACQUIRER_ID           32
#define FORWARDER_ID          33
#define PAN_EXTENDED          34
#define T2_DATA               35
#define T3_DATA               36
#define REFERENCE_NBR         37
#define AUTH_ID               38
#define ACTION_CODE           39
#define SERVICE_CODE          40
#define TERMINAL_NBR          41
#define OUTLET_NBR            42
#define TERMINAL_ADR          43
#define ADTNL_RESP_DATA       44
#define T1_DATA               45
#define SYS_CONFIG            46
#define ADTNL_DATA_NATNL      47
#define ADTNL_DATA_PRIV       48
#define TRANS_CRNCY           49
#define STLMNT_CRNCY          50
#define BILLING_CRNCY         51
#define PIN_DATA              52
#define SECURITY_DATA         53
#define ADTNL_AMOUNTS         54
#define CHIP_DATA             55
#define ORIGINAL_DATA         56
#define TRANS_DESCRRP         57
#define RSRVD_NTNL            58
#define NATNL_DATA            59
#define ADTNL_POS_INFO        60
#define OTHER_AMOUNTS         61
#define CPS_FIELDS            62
#define PRIV_USE_FIELDS       63
#define NETWORK_DATA          63
#define MAC1                  64
#define STLMNT_CODE           66
#define XFER_MNGMT            67
#define RECEIVER_COUNTRY      68
#define STLMNT_INST_COUNTRY   69
#define NW_MNGMT_INFO         70
#define XFR_MNG_POS_TO_AQR    70
#define XFR_MNG_AQR_TO_POS    71
#define RECORD_DATA           72
#define ACTION_DATE           73
#define CR_NBR                74
#define CR_REV_NBR            75
#define DB_NBR                76
#define DB_REV_NBR            77
#define XFER_NBR              78
#define XFER_REVERS_NBR       79
#define INFO_NBR              80
#define AUT_REVERS_NBR        81
#define INFO_REVERS_NBR       82
#define PMNT_NBR              83
#define PMNT_REVERS_NBR       84
#define CR_AMOUNT             86
#define CR_REV_AMOUNT         87
#define DB_AMOUNT             88
#define DB_REV_AMOUNT         89
#define ORIG_DATA             90
#define MSG_SECURITY          96
#define NET_AMOUNT            97
#define STLMNT_INSTITN        99
#define RECEIVER_ID           100
#define FILE_NAME             101
#define ACCOUNT_ID1           102
#define ACCOUNT_ID2           103
#define ADTNL_TRACE_DATA      115
#define FLD_116               116
#define FLD_118               118
#define MAC2                  128

#define FIX    1*10
#define VAR    2*10
#define LLVAR  3*10

#define BCD    1/*only numeric*/
#define ASCII  2/*alphanumeric*/
#define BITS   3/*only bits*/
#define BINARY 4/*alphanumeric+bits*/

#define NB_BYTES_LVAR   1
#define NB_BYTES_LLVAR  2
#define NB_BYTES_LLLVAR 3

typedef struct {
   int nNbr;
   int nVarFix;
   int nCode;
   int nLen;
   int nRef;
} CB2A_FIELD;

#ifndef CB2A_DATA_BUF_SIZE
#define CB2A_DATA_BUF_SIZE 4096
   char sCb2aData[CB2A_DATA_BUF_SIZE];
   char sCb2aBitMap[16];
#else
   extern char sCb2aData[CB2A_DATA_BUF_SIZE];
   extern char sCb2aBitMap[16];
#endif

#ifdef INIT_FIELDS
   CB2A_FIELD aCb2aFields[] ={
   /*   nNbr nVarFix nCode   nLen    nRef   */
      {  1,   FIX,  BITS,     8,     0},/* BIT_MAP1 */
      {  2,   VAR,   BCD,    19,     0},/* CARD_NBR */
      {  3,   FIX,   BCD,     6,     0},/* PROC_CODE */
      {  4,   FIX,   BCD,    12,     0},/* TRANS_AMOUNT */
      {  5,   FIX,   BCD,    12,     0},/* STLMNT_AMOUNT *//*Non utilise*/
      {  6,   FIX,   BCD,    12,     0},/* Cardholder Billing Amount *//*Non utilise*/
      {  7,   FIX,   BCD,    16,     0},/* XMIT_TIME *//*Non utilise*/
      {  8,   FIX,   BCD,    12,     0},/* Cardholder Billing Fee *//*Non utilise*/
      {  9,   FIX,   BCD,     8,     0},/* CONVRSN_RATE_STLMNT *//*Non utilise*/
      { 10,   FIX,   BCD,     8,     0},/* CONVRSN_RATE_CARD_HLDR_BILLING *//*Non utilise*/
      { 11,   FIX,   BCD,     6,     0},/* AUDIT_NBR */
      { 12,   FIX,   BCD,     6,     0},/* TRANS_TIME */
      { 13,   FIX,   BCD,     4,     0},/* TRANS_DATE */
      { 14,   FIX,   BCD,     4,     0},/* EXPIRY_DATE */
      { 15,   FIX,   BCD,     3,     0},/* STLMNT_DATE *//*Non utilise*/
      { 16,   FIX,   BCD,     4,     0},/* CNVRSN_DATE *//*Non utilise*/
      { 17,   FIX,   BCD,     6,     0},/* CAPTURE_DATE *//*Non utilise*/
      { 18,   FIX,   BCD,     4,     0},/* MERCHANT_TYPE */
      { 19,   FIX,   BCD,     3,     0},/* ACQR_COUNTRY *//*Non utilise*/
      { 20,   FIX,   BCD,     3,     0},/* ISSUER_COUNTRY */
      { 21,   FIX, ASCII,     6,     0},/* POS_CAPABILITY */
      { 22,   FIX,   BCD,     6,     0},/* POS_ENTRY_MODE */
      { 23,   FIX,   BCD,     3,     0},/* CARD_SEQ */
      { 24,   FIX,   BCD,     3,     0},/* FUNCTION_CODE */
      { 25,   FIX,   BCD,     4,     0},/* MSG_REASON */
      { 26,   FIX,   BCD,     6,     0},/* UPLOAD_CONTROL */
      { 27,   FIX,   BCD,     6,     0},/* DOWNLOAD_CONTROL */
      { 28,   FIX,   BCD,     6,     0},/* RECONC_DATE (DATE) *//*Non utilise*/
      { 29,   FIX,   BCD,     2,     0},/* RECONC_INDICATOR *//*Non utilise*/
      { 30,   FIX,   BCD,    12,     0},/* ORIGINAL_AMOUNTS */
      { 31,   VAR, ASCII,   255,     0},/* PRT_DPY_DATA */
      { 32,   VAR,   BCD,    11,     0},/* ACQUIRER_ID */
      { 33,   VAR,   BCD,    11,     0},/* FORWARDER_ID */
      { 34,   VAR, ASCII,    28,     0},/* PAN_EXTENDED *//*Non utilise*/
      { 35,   VAR,   BCD,    37,     0},/* T2_DATA */
      { 36,   VAR,   BCD,   104,     0},/* T3_DATA *//*Non utilise*/
      { 37,   FIX, ASCII,    12,     0},/* REFERENCE_NBR */
      { 38,   FIX, ASCII,     6,     0},/* AUTH_ID */
      { 39,   FIX, ASCII,     4,     0},/* ACTION_CODE */
      { 40,   FIX,   BCD,     3,     0},/* SERVICE_CODE */
      { 41,   FIX, ASCII,     8,     0},/* TERMINAL_NBR */
      { 42,   FIX, ASCII,    15,     0},/* OUTLET_NBR */
      { 43,   FIX, ASCII,    40,     0},/* TERMINAL_ADR *//*Non utilise*/
      { 44,   VAR, ASCII,    99,     0},/* ADTNL_RESP_DATA */
      { 45,   VAR, ASCII,    76,     0},/* T1_DATA *//*Non utilise*/
      { 46,   VAR,BINARY,   255,     0},/* SYS_CONFIG */
      { 47,   VAR, ASCII,   255,     0},/* ADTNL_DATA_NATNL *//*LBN20130703 - #38242*//*DEBUG*/
      { 48,   VAR,BINARY,   255,     0},/* ADTNL_DATA_PRIV *//*Non utilise*/
      { 49,   FIX,   BCD,     3,     0},/* TRANS_CRNCY */
      { 50,   FIX,   BCD,     3,     0},/* STLMNT_CRNCY */
      { 51,   FIX,   BCD,     3,     0},/* BILLING_CRNCY *//*Non utilise*/
      { 52,   FIX,  BITS,     8,     0},/* PIN_DATA *//*Non utilise*/
      { 53,   FIX,   BCD,    16,     0},/* SECURITY_DATA *//*Non utilise*/
      { 54,   VAR, ASCII,   120,     0},/* ADTNL_AMOUNTS */
      { 55,   VAR,BINARY,   255,     0},/* CHIP_DATA */
      { 56, LLVAR,   BCD,    36,     0},/* ORIGINAL_DATA */
      { 57,   VAR,BINARY,   255,     0},/* TRANS_DESCRRP */
      { 58,   VAR,BINARY,   255,     0},/* RSRVD_NTNL */
      { 59, LLVAR,BINARY,  2048,     0},/* NATNL_DATA *//*LBN20160317 - #CEDIC00018320*//*!! max length=65535 !!*/
      { 60,   VAR,BINARY,   255,     0},/* ADTNL_POS_INFO *//*Non utilise*/
      { 61,   VAR,BINARY,   255,     0},/* OTHER_AMOUNTS *//*Non utilise*/
      { 62,   VAR,BINARY,   255,     0},/* CPS_FIELDS *//*Non utilise*/
      { 63,   VAR,BINARY,   255,     0},/* PRIV_USE_FIELDS *//*Non utilise*//*!! max length=65535 !!*/
      { 64,   FIX,  BITS,     8,     0},/* MAC1 *//*Non utilise*/
      { 65,   FIX,  BITS,     8,     0},/* *//*Non utilise*/
      { 66,   FIX,   BCD,     1,     0},/* STLMNT_CODE *//*Non utilise*/
      { 67,   FIX,   BCD,     4,     0},/* XFER_MNGMT *//*Non utilise*/
      { 68,   FIX,   BCD,    18,     0},/* RECEIVER_COUNTRY *//*Non utilise*/
      { 69,   FIX,   BCD,    18,     0},/* STLMNT_INST_COUNTRY *//*Non utilise*/
      { 70,   FIX,   BCD,    14,     0},/* XFR_MNG_POS_TO_AQR */
      { 71,   FIX,   BCD,    14,     0},/* XFR_MNG_AQR_TO_POS */
      { 72, LLVAR,BINARY,   255,     0},/* RECORD_DATA *//*!! max length=65535 !!*/
      { 73,   FIX,   BCD,     6,     0},/* ACTION_DATE *//*Non utilise*/
      { 74,   FIX,   BCD,    10,     0},/* CR_NBR */
      { 75,   FIX,   BCD,    10,     0},/* CR_REV_NBR *//*Non utilise*/
      { 76,   FIX,   BCD,    10,     0},/* DB_NBR */
      { 77,   FIX,   BCD,    10,     0},/* DB_REV_NBR */
      { 78,   FIX,   BCD,    10,     0},/* Transfer Number */
      { 79,   FIX,   BCD,    10,     0},/* Transfer Reversal Number */
      { 80,   FIX,   BCD,    10,     0},/* Inquiries Number *//*Non utilise*/
      { 81,   FIX,   BCD,    10,     0},/* Authorizations Number *//*Non utilise*/
      { 82,   FIX,   BCD,    10,     0},/* Credit, Processing Fee Amount *//*Non utilise*/
      { 83,   FIX,   BCD,    10,     0},/* Credit, Transaction Fee Amount *//*Non utilise*/
      { 84,   FIX,   BCD,    10,     0},/* Debit, Processing Fee Amount *//*Non utilise*/
      { 85,   FIX,   BCD,    12,     0},/* Debit, Transaction Fee Amount *//*Non utilise*/
      { 86,   FIX,   BCD,    16,     0},/* CR_AMOUNT */
      { 87,   FIX,   BCD,    16,     0},/* CR_REV_AMOUNT *//*Non utilise*/
      { 88,   FIX,   BCD,    16,     0},/* DB_AMOUNT */
      { 89,   FIX,   BCD,    16,     0},/* DB_REV_AMOUNT */
      { 90, LLVAR,BINARY,   255,     0},/* ORIG_DATA *//*Non utilise*//*!! max length=65535 !!*/
      { 91,     0,     0,     0,     0},/* FILE_UPDATE *//*Non utilise*/
      { 92,     0,     0,     0,     0},/* FILE_SECURITY *//*Non utilise*/
      { 93,     0,     0,     0,     0},/* Response Indicator *//*Non utilise*/
      { 94,     0,     0,     0,     0},/* Service Indicator *//*Non utilise*/
      { 95,     0,     0,     0,     0},/* REPLACE_AMOUNTS *//*Non utilise*/
      { 96, LLVAR,BINARY,   999,     0},/* MSG_SECURITY *//*Non utilise*/
      { 97,   VAR, ASCII,   255,     0},/* NET_AMOUNT *//*Non utilise*/
      { 98, LLVAR,BINARY,   255,     0},/* Payee *//*Non utilise*//*!! max length=65535 !!*/
      { 99,     0,     0,     0,     0},/* STLMNT_INSTITN *//*Non utilise*/
      {100,   VAR,   BCD,    11,     0},/* RECEIVER_ID *//*Non utilise*/
      {101,   VAR, ASCII,    17,     0},/* FILE_NAME *//*Non utilise*/
      {102,   VAR, ASCII,    28,     0},/* ACCOUNT_ID1 *//*Non utilise*/
      {103,   VAR, ASCII,    28,     0},/* ACCOUNT_ID2 *//*Non utilise*/
      {104,     0,     0,     0,     0},/* Transaction Description *//*Non utilise*/
      {105,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {106,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {107,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {108,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {109,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {110,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {111,     0,     0,     0,     0},/* Reserved Iso *//*Non utilise*/
      {112,     0,     0,     0,     0},/* Reserved National *//*Non utilise*/
      {113,     0,     0,     0,     0},/* Reserved National *//*Non utilise*/
      {114,     0,     0,     0,     0},/* Reserved National *//*Non utilise*/
      {115, LLVAR,BINARY,   255,     0},/* ADTNL_TRACE_DATA *//*Non utilise*//*!! max length=MAX !!*/
      {116, LLVAR, ASCII,   255,     0},/* Reserved National *//*Non utilise*//*!! max length=MAX !!*/
      {117,     0,     0,     0,     0},/* Reserved National *//*Non utilise*/
      {118, LLVAR, ASCII,   999,     0},/* Intra Country Data */
      {119,     0,     0,     0,     0},/* Reserved National *//*Non utilise*/
      {120,     0,     0,     0,     0},/* Original Message Type Id *//*Non utilise*/
      {121,     0,     0,     0,     0},/* Issuing Institution Id *//*Non utilise*/
      {122,     0,     0,     0,     0},/* Remaining Open to Use *//*Non utilise*/
      {123,     0,     0,     0,     0},/* ADR_VERIF_DATA *//*Non utilise*/
      {124,     0,     0,     0,     0},/* Free Form Text - Japan *//*Non utilise*/
      {125,     0,     0,     0,     0},/* SUPPORT_INFO *//*Non utilise*/
      {126,     0,     0,     0,     0},/* Reserved Private *//*Non utilise*/
      {127,   FIX,  BITS,    64,     0},/* File Record Action and Data *//*Non utilise*/
      {128,   FIX,  BITS,     8,     0},/* MAC2 *//*Non utilise*/
   };
#else
   extern CB2A_FIELD aCb2aFields[];
#endif

#define SR(x) &sCb2aData[aCb2aFields[(x)-1].nRef]
#define SL(x) aCb2aFields[(x)-1].nLen
#define SV(x) aCb2aFields[(x)-1].nVarFix/*LBN20160317 - #CEDIC00018320*/
#define SC(x) aCb2aFields[(x)-1].nCode/*LBN20160317 - #CEDIC00018320*/

#define SREF(x) &sCb2aData[aSmsFields[(x)].nRef]/* Relative adress of field in DB*/
#define SLEN(x) aCb2aFields[(x)].nLen/* Field length */

/* NB: TYPES=character structure / TAGS=binary structure */
#ifdef INIT_F31_TYPES
   char f31_01    [] = "01";/*LBN20201204 CAPS-2307*/
   char f31_0f    [] = "0F";/*reserved - unused*/
#else
   extern char f31_01    [];/*LBN20201204 CAPS-2307*/
   extern char f31_0f    [];/*reserved - unused*/
#endif
/*LBN20160317 - #CEDIC00018320*/
#ifdef INIT_F44_TYPES
   char f44_aa    [] = "AA";/*repeatable*/
   char f44_ac    [] = "AC";/*repeatable*/
   char f44_ad    [] = "AD";
   char f44_bc    [] = "BC";
#else
   extern char f44_aa    [];/*repeatable*/
   extern char f44_ac    [];/*repeatable*/
   extern char f44_ad    [];
   extern char f44_bc    [];
#endif
/*LBN20160317 - #CEDIC00018320*/

/* NLY28032008 Start */
#ifdef INIT_F46_TAGS
   unsigned char f46_df50    [] = {0xDF, 0x50};
   unsigned char f46_df51    [] = {0xDF, 0x51};
   unsigned char f46_df52    [] = {0xDF, 0x52};
   unsigned char f46_df53    [] = {0xDF, 0x53};
   unsigned char f46_df54    [] = {0xDF, 0x54};
   unsigned char f46_df55    [] = {0xDF, 0x55};/*V1.3.0 - V1.5.0*/
   unsigned char f46_df56    [] = {0xDF, 0x56};/*V1.3.0 - V1.5.0*/
   unsigned char f46_df58    [] = {0xDF, 0x58};/*V1.3.0*/
   unsigned char f46_df59    [] = {0xDF, 0x59};/*V1.3.0*/
   unsigned char f46_df5a    [] = {0xDF, 0x5A};/*V1.3.0*/
   unsigned char f46_df5b    [] = {0xDF, 0x5B};
   unsigned char f46_df5c    [] = {0xDF, 0x5C};
   unsigned char f46_df5d    [] = {0xDF, 0x5D};
   unsigned char f46_df5e    [] = {0xDF, 0x5E};
   unsigned char f46_df5f    [] = {0xDF, 0x5F};
   unsigned char f46_df60    [] = {0xDF, 0x60};/*V1.3.0*/
   unsigned char f46_df61    [] = {0xDF, 0x61};
   unsigned char f46_df62    [] = {0xDF, 0x62};
   unsigned char f46_df63    [] = {0xDF, 0x63};
   unsigned char f46_df64    [] = {0xDF, 0x64};
   unsigned char f46_df65    [] = {0xDF, 0x65};/*V1.3.0*/
   unsigned char f46_df66    [] = {0xDF, 0x66};
   unsigned char f46_df67    [] = {0xDF, 0x67};
   unsigned char f46_df68    [] = {0xDF, 0x68};/*repeatable*/
   unsigned char f46_df69    [] = {0xDF, 0x69};
   unsigned char f46_df6a    [] = {0xDF, 0x6A};
   unsigned char f46_df6b    [] = {0xDF, 0x6B};
   unsigned char f46_df6c    [] = {0xDF, 0x6C};
   unsigned char f46_df6d    [] = {0xDF, 0x6D};
   unsigned char f46_df6e    [] = {0xDF, 0x6E};
   unsigned char f46_df6f    [] = {0xDF, 0x6F};
   unsigned char f46_df92    [] = {0xDF, 0x92};/*repeatable*//*LBN20160317 - #CEDIC00018320*/
   unsigned char f46_df94    [] = {0xDF, 0x94};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f46_df96    [] = {0xDF, 0x96};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f46_df97    [] = {0xDF, 0x97};/*LBN20160317 - #CEDIC00018320*/
#else
   extern unsigned char f46_df50    [];
   extern unsigned char f46_df51    [];
   extern unsigned char f46_df52    [];
   extern unsigned char f46_df53    [];
   extern unsigned char f46_df54    [];
   extern unsigned char f46_df55    [];/*V1.3.0 - V1.5.0*/
   extern unsigned char f46_df56    [];/*V1.3.0 - V1.5.0*/
   extern unsigned char f46_df58    [];/*V1.3.0*/
   extern unsigned char f46_df59    [];/*V1.3.0*/
   extern unsigned char f46_df5a    [];/*V1.3.0*/
   extern unsigned char f46_df5b    [];
   extern unsigned char f46_df5c    [];
   extern unsigned char f46_df5d    [];
   extern unsigned char f46_df5e    [];
   extern unsigned char f46_df5f    [];
   extern unsigned char f46_df60    [];/*V1.3.0*/
   extern unsigned char f46_df61    [];
   extern unsigned char f46_df62    [];
   extern unsigned char f46_df63    [];
   extern unsigned char f46_df64    [];
   extern unsigned char f46_df65    [];/*V1.3.0*/
   extern unsigned char f46_df66    [];
   extern unsigned char f46_df67    [];
   extern unsigned char f46_df68    [];/*repeatable*/
   extern unsigned char f46_df69    [];
   extern unsigned char f46_df6a    [];
   extern unsigned char f46_df6b    [];
   extern unsigned char f46_df6c    [];
   extern unsigned char f46_df6d    [];
   extern unsigned char f46_df6e    [];
   extern unsigned char f46_df6f    [];
   extern unsigned char f46_df92    [];/*repeatable*//*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f46_df94    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f46_df96    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f46_df97    [];/*LBN20160317 - #CEDIC00018320*/
#endif
/* NLY28032008 End */

#ifdef INIT_F47_TYPES
   char f47_01    [] = "01";/*repeatable*/
   char f47_02    [] = "02";
   char f47_03    [] = "03";
   char f47_04    [] = "04";
   char f47_05    [] = "05";
   char f47_06    [] = "06";
   char f47_07    [] = "07";/*repeatable*/
   char f47_08    [] = "08";/*V1.3.0*/
   char f47_09    [] = "09";/*LBN20160317 - #CEDIC00018320*/
   char f47_10    [] = "10";
   char f47_15    [] = "15";/*V1.3.0 - V1.3.1*/
   char f47_16    [] = "16";/*V1.3.0*/
   char f47_17    [] = "17";/*LBN20160317 - #CEDIC00018320*/
   char f47_18    [] = "18";
   char f47_19    [] = "19";
   char f47_20    [] = "20";
   char f47_21    [] = "21";/*LBN20160317 - #CEDIC00018320*/
   char f47_22    [] = "22";/*LBN20160317 - #CEDIC00018320*/
   char f47_24    [] = "24";
   char f47_25    [] = "25";/*LBN20160317 - #CEDIC00018320*/
   char f47_26    [] = "26";/*LBN20160317 - #CEDIC00018320*/
   char f47_30    [] = "30";/*LBN20160317 - #CEDIC00018320*/
   char f47_32    [] = "32";/*LBN20160317 - #CEDIC00018320*/
   char f47_33    [] = "33";/*V1.6.1*//*LBN20201204 CAPS-2307*/
   char f47_47    [] = "47";
   char f47_48    [] = "48";
   char f47_49    [] = "49";/*LBN20160317 - #CEDIC00018320*/
   char f47_4a    [] = "4A";/*LBN20160317 - #CEDIC00018320*/
   char f47_50    [] = "50";/*V1.3.0*/
   char f47_95    [] = "95";
   char f47_96    [] = "96";
   char f47_99    [] = "99";/*V1.6.1*//*LBN20201204 CAPS-2307*/
#else
   extern char f47_01    [];/*repeatable*/
   extern char f47_02    [];
   extern char f47_03    [];
   extern char f47_04    [];
   extern char f47_05    [];
   extern char f47_06    [];
   extern char f47_07    [];/*repeatable*/
   extern char f47_08    [];/*V1.3.0*/
   extern char f47_09    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_10    [];
   extern char f47_15    [];/*V1.3.0 - V1.3.1*/
   extern char f47_16    [];/*V1.3.0*/
   extern char f47_17    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_18    [];
   extern char f47_19    [];
   extern char f47_20    [];
   extern char f47_21    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_22    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_24    [];
   extern char f47_25    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_26    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_30    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_32    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_33    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern char f47_47    [];
   extern char f47_48    [];
   extern char f47_49    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_4a    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f47_50    [];/*V1.3.0*/
   extern char f47_95    [];
   extern char f47_96    [];
   extern char f47_99    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
#endif

#ifdef INIT_F55_TAGS
/* Chip data */
   char f55_0056    [] = {0x00, 0x56};
   char f55_0082    [] = {0x00, 0x82};
   char f55_008e    [] = {0x00, 0x8E};
   char f55_009a    [] = {0x00, 0x9A};/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   char f55_5f24    [] = {0x5F, 0x24};
   char f55_5f25    [] = {0x5F, 0x25};
   char f55_5f2d    [] = {0x5F, 0x2D};/*repeatable*//*V1.6.1*//*LBN20201204 CAPS-2307*/
   char f55_9f02    [] = {0x9F, 0x02};/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   char f55_9f06    [] = {0x9F, 0x06};
   char f55_9f07    [] = {0x9F, 0x07};
   char f55_9f0a    [] = {0x9F, 0x0A};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   char f55_9f0d    [] = {0x9F, 0x0D};
   char f55_9f0e    [] = {0x9F, 0x0E};
   char f55_9f0f    [] = {0x9F, 0x0F};
   char f55_9f10    [] = {0x9F, 0x10};
   char f55_9f26    [] = {0x9F, 0x26};
   char f55_9f27    [] = {0x9F, 0x27};
   char f55_9f36    [] = {0x9F, 0x36};
   char f55_9f6b    [] = {0x9F, 0x6B};
   char f55_9f7c    [] = {0x9F, 0x7C};/*LBN20160317 - #CEDIC00018320*/
   char f55_df70    [] = {0xDF, 0x70};/*LBN20160317 - #CEDIC00018320*/
   char f55_df71    [] = {0xDF, 0x71};/*V1.3.0 - V1.3.1*/
   char f55_df72    [] = {0xDF, 0x72};/*V1.3.0 - V1.3.1*/
   char f55_df73    [] = {0xDF, 0x73};
   char f55_df74    [] = {0xDF, 0x74};/*V1.3.0 - V1.3.1*/
   char f55_df75    [] = {0xDF, 0x75};/*V1.3.0 - V1.3.1*/
   char f55_df76    [] = {0xDF, 0x76};/*V1.3.0 - V1.3.1*/
   char f55_df77    [] = {0xDF, 0x77};/*V1.3.0 - V1.3.1*/
   char f55_df78    [] = {0xDF, 0x78};/*V1.3.0 - V1.3.1*/
   char f55_df79    [] = {0xDF, 0x79};/*V1.3.0 - V1.3.1*/
   char f55_dfab    [] = {0xDF, 0xAB};/*V1.3.0 - V1.3.1*/
   char f55_df86    [] = {0xDF, 0x86};
   char f55_ff00    [] = {0xFF, 0x00};/*repeatable*/
#else
   extern char f55_0056    [];
   extern char f55_0082    [];
   extern char f55_008e    [];
   extern char f55_009a    [];/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   extern char f55_5f24    [];
   extern char f55_5f25    [];
   extern char f55_5f2d    [];/*repeatable*//*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern char f55_9f02    [];/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   extern char f55_9f06    [];
   extern char f55_9f07    [];
   extern char f55_9f0a    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern char f55_9f0d    [];
   extern char f55_9f0e    [];
   extern char f55_9f0f    [];
   extern char f55_9f10    [];
   extern char f55_9f26    [];
   extern char f55_9f27    [];
   extern char f55_9f36    [];
   extern char f55_9f6b    [];
   extern char f55_9f7c    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f55_df70    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f55_df71    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df72    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df73    [];
   extern char f55_df74    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df75    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df76    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df77    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df78    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df79    [];/*V1.3.0 - V1.3.1*/
   extern char f55_dfab    [];/*V1.3.0 - V1.3.1*/
   extern char f55_df86    [];
   extern char f55_ff00    [];/*repeatable*/
#endif

#ifdef INIT_F57_TAGS
   unsigned char f57_0210    [] = {0x02, 0x10};/*V1.4.0*/
   unsigned char f57_0211    [] = {0x02, 0x11};/*V1.4.0*/
   unsigned char f57_0212    [] = {0x02, 0x12};/*V1.4.0*/
   unsigned char f57_0214    [] = {0x02, 0x14};/*V1.4.0*/
   unsigned char f57_0215    [] = {0x02, 0x15};/*V1.4.0*/
   unsigned char f57_0216    [] = {0x02, 0x16};/*V1.4.0*/
   unsigned char f57_0217    [] = {0x02, 0x17};/*V1.4.0*/
   unsigned char f57_ff01    [] = {0xFF, 0x01};
   unsigned char f57_ff02    [] = {0xFF, 0x02};
   unsigned char f57_ff0e    [] = {0xFF, 0x0E};/*repeatable*//*LBN20160317 - #CEDIC00018320*/
   unsigned char f57_ff0f    [] = {0xFF, 0x0F};/*repeatable*//*LBN20160317 - #CEDIC00018320*/
#else
   extern unsigned char f57_0210    [];/*V1.4.0*/
   extern unsigned char f57_0211    [];/*V1.4.0*/
   extern unsigned char f57_0212    [];/*V1.4.0*/
   extern unsigned char f57_0214    [];/*V1.4.0*/
   extern unsigned char f57_0215    [];/*V1.4.0*/
   extern unsigned char f57_0216    [];/*V1.4.0*/
   extern unsigned char f57_0217    [];/*V1.4.0*/
   extern unsigned char f57_ff01    [];
   extern unsigned char f57_ff02    [];
   extern unsigned char f57_ff0e    [];/*repeatable*//*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f57_ff0f    [];/*repeatable*//*LBN20160317 - #CEDIC00018320*/
#endif

#ifdef INIT_F58_TAGS
   unsigned char f58_0095    [] = {0x00, 0x95};
   unsigned char f58_009c    [] = {0x00, 0x9C};
   unsigned char f58_0409    [] = {0x04, 0x09};
   unsigned char f58_0410    [] = {0x04, 0x10};
   unsigned char f58_0411    [] = {0x04, 0x11};
   unsigned char f58_0412    [] = {0x04, 0x12};
   unsigned char f58_0413    [] = {0x04, 0x13};
   unsigned char f58_0414    [] = {0x04, 0x14};/*LBN20130912 - #39088*/
   unsigned char f58_0415    [] = {0x04, 0x15};/*LBN20130912 - #39088*/
   unsigned char f58_0416    [] = {0x04, 0x16};/*LBN20130912 - #39088*/
   unsigned char f58_0417    [] = {0x04, 0x17};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f58_0418    [] = {0x04, 0x18};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f58_0419    [] = {0x04, 0x19};/*LBN20190305 CAPS-703*/
   unsigned char f58_0420    [] = {0x04, 0x20};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f58_0603    [] = {0x06, 0x03};/*LBN20130912 - #39088*/
   unsigned char f58_0805    [] = {0x08, 0x05};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f58_0809    [] = {0x08, 0x09};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f58_9f33    [] = {0x9F, 0x33};
   unsigned char f58_9f34    [] = {0x9F, 0x34};
   unsigned char f58_9f35    [] = {0x9F, 0x35};
   unsigned char f58_9f37    [] = {0x9F, 0x37};
   unsigned char f58_9f66    [] = {0x9F, 0x66};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f58_df85    [] = {0xDF, 0x85};
   unsigned char f58_df93    [] = {0xDF, 0x93};/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   unsigned char f58_ff0d    [] = {0xFF, 0x0D};
   unsigned char f58_ff0e    [] = {0xFF, 0x0E};
   unsigned char f58_ff0f    [] = {0xFF, 0x0F};
   unsigned char f58_ff20    [] = {0xFF, 0x20};
   unsigned char f58_ff21    [] = {0xFF, 0x21};
   unsigned char f58_ff22    [] = {0xFF, 0x22};
   unsigned char f58_ff23    [] = {0xFF, 0x23};
   unsigned char f58_ff24    [] = {0xFF, 0x24};
   unsigned char f58_ff25    [] = {0xFF, 0x25};
   unsigned char f58_ff26    [] = {0xFF, 0x26};
   unsigned char f58_ff27    [] = {0xFF, 0x27};
   unsigned char f58_ff28    [] = {0xFF, 0x28};
   unsigned char f58_ff29    [] = {0xFF, 0x29};
   unsigned char f58_ff2a    [] = {0xFF, 0x2A};/*V1.3.0*/
   unsigned char f58_ff2b    [] = {0xFF, 0x2B};
   unsigned char f58_ff2c    [] = {0xFF, 0x2C};
   unsigned char f58_ff2d    [] = {0xFF, 0x2D};
   unsigned char f58_ff30    [] = {0xFF, 0x30};
   unsigned char f58_ff31    [] = {0xFF, 0x31};
   unsigned char f58_ff32    [] = {0xFF, 0x32};/*V1.3.0*/
   unsigned char f58_ff33    [] = {0xFF, 0x33};/*V1.3.0*/
   unsigned char f58_ff34    [] = {0xFF, 0x34};/*V1.3.0*/
   unsigned char f58_ff35    [] = {0xFF, 0x35};
   unsigned char f58_ff36    [] = {0xFF, 0x36};
   unsigned char f58_ff37    [] = {0xFF, 0x37};/*V1.3.0*/
   unsigned char f58_ff38    [] = {0xFF, 0x38};
   unsigned char f58_ff39    [] = {0xFF, 0x39};
   unsigned char f58_ff3a    [] = {0xFF, 0x3A};
   unsigned char f58_ff3b    [] = {0xFF, 0x3B};
   unsigned char f58_ff3c    [] = {0xFF, 0x3C};
   unsigned char f58_ff3d    [] = {0xFF, 0x3D};
   unsigned char f58_ff40    [] = {0xFF, 0x40};
   unsigned char f58_ff41    [] = {0xFF, 0x41};/*V1.3.0*/
   unsigned char f58_ff42    [] = {0xFF, 0x42};
   unsigned char f58_ff43    [] = {0xFF, 0x43};/*V1.3.0*/
   unsigned char f58_ff44    [] = {0xFF, 0x44};/*V1.3.0*/
   unsigned char f58_ff45    [] = {0xFF, 0x45};
   unsigned char f58_ff46    [] = {0xFF, 0x46};
   unsigned char f58_ff47    [] = {0xFF, 0x47};
   unsigned char f58_ff48    [] = {0xFF, 0x48};
   unsigned char f58_ff49    [] = {0xFF, 0x49};
   unsigned char f58_ff50    [] = {0xFF, 0x50};/*repeatable*/
   unsigned char f58_ff51    [] = {0xFF, 0x51};/*V1.3.0*/
   unsigned char f58_ff52    [] = {0xFF, 0x52};
   unsigned char f58_ff53    [] = {0xFF, 0x53};
   unsigned char f58_ff54    [] = {0xFF, 0x54};
   unsigned char f58_ff55    [] = {0xFF, 0x55};
   unsigned char f58_ff56    [] = {0xFF, 0x56};
   unsigned char f58_ff57    [] = {0xFF, 0x57};
   unsigned char f58_ff58    [] = {0xFF, 0x58};
   unsigned char f58_ff59    [] = {0xFF, 0x59};
   unsigned char f58_ff60    [] = {0xFF, 0x60};
   unsigned char f58_ff61    [] = {0xFF, 0x61};
   unsigned char f58_ff63    [] = {0xFF, 0x63};
   unsigned char f58_ff62    [] = {0xFF, 0x62};
   unsigned char f58_ff64    [] = {0xFF, 0x64};
   unsigned char f58_ff65    [] = {0xFF, 0x65};
   unsigned char f58_ff81    [] = {0xFF, 0x81};/*LBN20160317 - #CEDIC00018320*/
#else
   extern unsigned char f58_0095    [];
   extern unsigned char f58_009c    [];
   extern unsigned char f58_0409    [];
   extern unsigned char f58_0410    [];
   extern unsigned char f58_0411    [];
   extern unsigned char f58_0412    [];
   extern unsigned char f58_0413    [];
   extern unsigned char f58_0414    [];/*LBN20130912 - #39088*/
   extern unsigned char f58_0415    [];/*LBN20130912 - #39088*/
   extern unsigned char f58_0416    [];/*LBN20130912 - #39088*/
   extern unsigned char f58_0417    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f58_0418    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f58_0419    [];/*LBN20190305 CAPS-703*/
   extern unsigned char f58_0420    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f58_0603    [];/*LBN20130912 - #39088*/
   extern unsigned char f58_0805    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f58_0809    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f58_9f33    [];
   extern unsigned char f58_9f34    [];
   extern unsigned char f58_9f37    [];
   extern unsigned char f58_9f35    [];
   extern unsigned char f58_9f66    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f58_df85    [];
   extern unsigned char f58_df93    [];/*ABE15042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
   extern unsigned char f58_ff0d    [];
   extern unsigned char f58_ff0e    [];
   extern unsigned char f58_ff0f    [];
   extern unsigned char f58_ff20    [];
   extern unsigned char f58_ff21    [];
   extern unsigned char f58_ff22    [];
   extern unsigned char f58_ff23    [];
   extern unsigned char f58_ff24    [];
   extern unsigned char f58_ff25    [];
   extern unsigned char f58_ff26    [];
   extern unsigned char f58_ff27    [];
   extern unsigned char f58_ff28    [];
   extern unsigned char f58_ff29    [];
   extern unsigned char f58_ff2a    [];/*V1.3.0*/
   extern unsigned char f58_ff2b    [];
   extern unsigned char f58_ff2c    [];
   extern unsigned char f58_ff2d    [];
   extern unsigned char f58_ff30    [];
   extern unsigned char f58_ff31    [];
   extern unsigned char f58_ff32    [];/*V1.3.0*/
   extern unsigned char f58_ff33    [];/*V1.3.0*/
   extern unsigned char f58_ff34    [];/*V1.3.0*/
   extern unsigned char f58_ff35    [];
   extern unsigned char f58_ff36    [];
   extern unsigned char f58_ff37    [];/*V1.3.0*/
   extern unsigned char f58_ff38    [];
   extern unsigned char f58_ff39    [];
   extern unsigned char f58_ff3a    [];
   extern unsigned char f58_ff3b    [];
   extern unsigned char f58_ff3c    [];
   extern unsigned char f58_ff3d    [];
   extern unsigned char f58_ff40    [];
   extern unsigned char f58_ff41    [];/*V1.3.0*/
   extern unsigned char f58_ff42    [];
   extern unsigned char f58_ff43    [];/*V1.3.0*/
   extern unsigned char f58_ff44    [];/*V1.3.0*/
   extern unsigned char f58_ff45    [];
   extern unsigned char f58_ff46    [];
   extern unsigned char f58_ff47    [];
   extern unsigned char f58_ff48    [];
   extern unsigned char f58_ff49    [];
   extern unsigned char f58_ff50    [];/*repeatable*/
   extern unsigned char f58_ff51    [];/*V1.3.0*/
   extern unsigned char f58_ff52    [];
   extern unsigned char f58_ff53    [];
   extern unsigned char f58_ff54    [];
   extern unsigned char f58_ff55    [];
   extern unsigned char f58_ff56    [];
   extern unsigned char f58_ff57    [];
   extern unsigned char f58_ff58    [];
   extern unsigned char f58_ff59    [];
   extern unsigned char f58_ff60    [];
   extern unsigned char f58_ff61    [];
   extern unsigned char f58_ff62    [];
   extern unsigned char f58_ff63    [];
   extern unsigned char f58_ff64    [];
   extern unsigned char f58_ff65    [];
   extern unsigned char f58_ff81    [];/*LBN20160317 - #CEDIC00018320*/
#endif

#ifdef INIT_F59_TAGS
   unsigned char f59_0001    [] = {0x00, 0x01};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f59_0002    [] = {0x00, 0x02};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f59_0003    [] = {0x00, 0x03};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f59_0004    [] = {0x00, 0x04};/*LBN20160317 - #CEDIC00018320*/
   unsigned char f59_0005    [] = {0x00, 0x05};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0012    [] = {0x00, 0x12};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0013    [] = {0x00, 0x13};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0014    [] = {0x00, 0x14};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0020    [] = {0x00, 0x20};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0022    [] = {0x00, 0x22};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0023    [] = {0x00, 0x23};/*repeatable*//*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0024    [] = {0x00, 0x24};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0025    [] = {0x00, 0x25};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0026    [] = {0x00, 0x26};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0027    [] = {0x00, 0x27};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0028    [] = {0x00, 0x28};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0029    [] = {0x00, 0x29};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0033    [] = {0x00, 0x33};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0037    [] = {0x00, 0x37};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0041    [] = {0x00, 0x41};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0042    [] = {0x00, 0x42};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0043    [] = {0x00, 0x43};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0044    [] = {0x00, 0x44};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0046    [] = {0x00, 0x46};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_0047    [] = {0x00, 0x47};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1205    [] = {0x12, 0x05};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1206    [] = {0x12, 0x06};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1207    [] = {0x12, 0x07};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1208    [] = {0x12, 0x08};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1209    [] = {0x12, 0x09};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1210    [] = {0x12, 0x10};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1501    [] = {0x15, 0x01};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1502    [] = {0x15, 0x02};/*V1.6.1*//*LBN20201204 CAPS-2307*/
   unsigned char f59_1503    [] = {0x15, 0x03};/*V1.6.1*//*LBN20201204 CAPS-2307*/
#else
   extern unsigned char f59_0001    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f59_0002    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f59_0003    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f59_0004    [];/*LBN20160317 - #CEDIC00018320*/
   extern unsigned char f59_0005    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0012    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0013    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0014    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0020    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0022    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0023    [];/*repeatable*//*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0024    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0025    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0026    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0027    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0028    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0029    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0033    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0037    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0041    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0042    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0043    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0044    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0046    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_0047    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1205    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1206    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1207    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1208    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1209    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1210    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1501    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1502    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern unsigned char f59_1503    [];/*V1.6.1*//*LBN20201204 CAPS-2307*/
#endif

/*V1.6.1*//*LBN20201204 CAPS-2307*/
#ifdef INIT_F72_TAGS
   unsigned char f72_df01    [] = {0xDF, 0x01};
   unsigned char f72_df02    [] = {0xDF, 0x02};/*repeatable*/
   unsigned char f72_df03    [] = {0xDF, 0x03};/*repeatable*/
   unsigned char f72_df04    [] = {0xDF, 0x04};
   unsigned char f72_df05    [] = {0xDF, 0x05};
   unsigned char f72_df06    [] = {0xDF, 0x06};
   unsigned char f72_df08    [] = {0xDF, 0x08};/*repeatable*/
   unsigned char f72_df09    [] = {0xDF, 0x09};/*repeatable*/
   unsigned char f72_df0b    [] = {0xDF, 0x0B};/*repeatable*/
   unsigned char f72_df0c    [] = {0xDF, 0x0C};
   unsigned char f72_df0d    [] = {0xDF, 0x0D};
   unsigned char f72_df16    [] = {0xDF, 0x16};/*repeatable*/
   unsigned char f72_df17    [] = {0xDF, 0x17};/*repeatable*/
   unsigned char f72_df18    [] = {0xDF, 0x18};/*repeatable*/
   unsigned char f72_df19    [] = {0xDF, 0x19};/*repeatable*/
   unsigned char f72_df1a    [] = {0xDF, 0x1A};/*repeatable*/
   unsigned char f72_df1b    [] = {0xDF, 0x1B};/*repeatable*/
   unsigned char f72_df1d    [] = {0xDF, 0x1D};/*repeatable*/
   unsigned char f72_df1e    [] = {0xDF, 0x1E};/*repeatable*/
   unsigned char f72_df1f    [] = {0xDF, 0x1F};
   unsigned char f72_df20    [] = {0xDF, 0x20};
   unsigned char f72_df21    [] = {0xDF, 0x21};
   unsigned char f72_df22    [] = {0xDF, 0x22};
   unsigned char f72_df23    [] = {0xDF, 0x23};
   unsigned char f72_df24    [] = {0xDF, 0x24};/*repeatable*/
   unsigned char f72_df26    [] = {0xDF, 0x26};
   unsigned char f72_df27    [] = {0xDF, 0x27};
   unsigned char f72_df28    [] = {0xDF, 0x28};
   unsigned char f72_df2a    [] = {0xDF, 0x2A};
   unsigned char f72_df2b    [] = {0xDF, 0x2B};
   unsigned char f72_df2c    [] = {0xDF, 0x2C};
   unsigned char f72_df2d    [] = {0xDF, 0x2D};
   unsigned char f72_df2f    [] = {0xDF, 0x2F};/*repeatable*/
   unsigned char f72_df3a    [] = {0xDF, 0x3A};
   unsigned char f72_df3b    [] = {0xDF, 0x3B};
   unsigned char f72_df3c    [] = {0xDF, 0x3C};
   unsigned char f72_df41    [] = {0xDF, 0x41};/*repeatable*/
   unsigned char f72_df43    [] = {0xDF, 0x43};
   unsigned char f72_df44    [] = {0xDF, 0x44};
   unsigned char f72_df45    [] = {0xDF, 0x45};
   unsigned char f72_df46    [] = {0xDF, 0x46};
   unsigned char f72_df47    [] = {0xDF, 0x47};
   unsigned char f72_df48    [] = {0xDF, 0x48};
   unsigned char f72_df49    [] = {0xDF, 0x49};/*repeatable*/
   unsigned char f72_df87    [] = {0xDF, 0x87};/*repeatable*/
   unsigned char f72_df88    [] = {0xDF, 0x88};/*repeatable*/
   unsigned char f72_df89    [] = {0xDF, 0x89};
   unsigned char f72_df8a    [] = {0xDF, 0x8A};
   unsigned char f72_df8b    [] = {0xDF, 0x8B};/*repeatable*/
   unsigned char f72_df98    [] = {0xDF, 0x98};/*repeatable*/
   unsigned char f72_df99    [] = {0xDF, 0x99};
   unsigned char f72_dfa0    [] = {0xDF, 0xA0};
   unsigned char f72_dfa1    [] = {0xDF, 0xA1};/*repeatable*/
   unsigned char f72_dfa2    [] = {0xDF, 0xA2};
   unsigned char f72_dfa3    [] = {0xDF, 0xA3};
   unsigned char f72_dfa4    [] = {0xDF, 0xA4};
   unsigned char f72_dfa5    [] = {0xDF, 0xA5};
   unsigned char f72_dfa6    [] = {0xDF, 0xA6};
   unsigned char f72_dfa7    [] = {0xDF, 0xA7};
   unsigned char f72_dfa8    [] = {0xDF, 0xA8};/*repeatable*/
   unsigned char f72_dfa9    [] = {0xDF, 0xA9};
   unsigned char f72_dfaa    [] = {0xDF, 0xAA};/*repeatable*/
   unsigned char f72_dfab    [] = {0xDF, 0xAB};
   unsigned char f72_dfad    [] = {0xDF, 0xAD};/*repeatable*/
   unsigned char f72_dfae    [] = {0xDF, 0xAE};
   unsigned char f72_dfaf    [] = {0xDF, 0xAF};
   unsigned char f72_dfb0    [] = {0xDF, 0xB0};
   unsigned char f72_dfb1    [] = {0xDF, 0xB1};
   unsigned char f72_dfb2    [] = {0xDF, 0xB2};
   unsigned char f72_dfb3    [] = {0xDF, 0xB3};/*repeatable*/
   unsigned char f72_dfb4    [] = {0xDF, 0xB4};/*repeatable*/
   unsigned char f72_dfb5    [] = {0xDF, 0xB5};/*repeatable*/
#else
   extern unsigned char f72_df01    [];
   extern unsigned char f72_df02    [];/*repeatable*/
   extern unsigned char f72_df03    [];/*repeatable*/
   extern unsigned char f72_df04    [];
   extern unsigned char f72_df05    [];
   extern unsigned char f72_df06    [];
   extern unsigned char f72_df08    [];/*repeatable*/
   extern unsigned char f72_df09    [];/*repeatable*/
   extern unsigned char f72_df0b    [];/*repeatable*/
   extern unsigned char f72_df0c    [];
   extern unsigned char f72_df0d    [];
   extern unsigned char f72_df16    [];/*repeatable*/
   extern unsigned char f72_df17    [];/*repeatable*/
   extern unsigned char f72_df18    [];/*repeatable*/
   extern unsigned char f72_df19    [];/*repeatable*/
   extern unsigned char f72_df1a    [];/*repeatable*/
   extern unsigned char f72_df1b    [];/*repeatable*/
   extern unsigned char f72_df1d    [];/*repeatable*/
   extern unsigned char f72_df1e    [];/*repeatable*/
   extern unsigned char f72_df1f    [];
   extern unsigned char f72_df20    [];
   extern unsigned char f72_df21    [];
   extern unsigned char f72_df22    [];
   extern unsigned char f72_df23    [];
   extern unsigned char f72_df24    [];/*repeatable*/
   extern unsigned char f72_df26    [];
   extern unsigned char f72_df27    [];
   extern unsigned char f72_df28    [];
   extern unsigned char f72_df2a    [];
   extern unsigned char f72_df2b    [];
   extern unsigned char f72_df2c    [];
   extern unsigned char f72_df2d    [];
   extern unsigned char f72_df2f    [];/*repeatable*/
   extern unsigned char f72_df3a    [];
   extern unsigned char f72_df3b    [];
   extern unsigned char f72_df3c    [];
   extern unsigned char f72_df41    [];/*repeatable*/
   extern unsigned char f72_df43    [];
   extern unsigned char f72_df44    [];
   extern unsigned char f72_df45    [];
   extern unsigned char f72_df46    [];
   extern unsigned char f72_df47    [];
   extern unsigned char f72_df48    [];
   extern unsigned char f72_df49    [];/*repeatable*/
   extern unsigned char f72_df87    [];/*repeatable*/
   extern unsigned char f72_df88    [];/*repeatable*/
   extern unsigned char f72_df89    [];
   extern unsigned char f72_df8a    [];
   extern unsigned char f72_df8b    [];/*repeatable*/
   extern unsigned char f72_df98    [];/*repeatable*/
   extern unsigned char f72_df99    [];
   extern unsigned char f72_dfa0    [];
   extern unsigned char f72_dfa1    [];/*repeatable*/
   extern unsigned char f72_dfa2    [];
   extern unsigned char f72_dfa3    [];
   extern unsigned char f72_dfa4    [];
   extern unsigned char f72_dfa5    [];
   extern unsigned char f72_dfa6    [];
   extern unsigned char f72_dfa7    [];
   extern unsigned char f72_dfa8    [];/*repeatable*/
   extern unsigned char f72_dfa9    [];
   extern unsigned char f72_dfaa    [];/*repeatable*/
   extern unsigned char f72_dfab    [];
   extern unsigned char f72_dfad    [];/*repeatable*/
   extern unsigned char f72_dfae    [];
   extern unsigned char f72_dfaf    [];
   extern unsigned char f72_dfb0    [];
   extern unsigned char f72_dfb1    [];
   extern unsigned char f72_dfb2    [];
   extern unsigned char f72_dfb3    [];/*repeatable*/
   extern unsigned char f72_dfb4    [];/*repeatable*/
   extern unsigned char f72_dfb5    [];/*repeatable*/
#endif

#ifdef INIT_F90_TAGS
   unsigned char f90_0001    [] = {0x00, 0x01};/*repeatable*/
#else
   extern unsigned char f90_0001    [];/*repeatable*/
#endif
/*V1.6.1*//*LBN20201204 CAPS-2307 END*/

#ifdef INIT_F118_TYPES
/*V1.6.1*//*LBN20201204 CAPS-2307 f118 is a TLV "character" (not "binary") structure */
/*char f118_01                  [] = {0x00, 0x01};*//*V1.3.0*/
/*char f118_02                  [] = {0x00, 0x02};*//*V1.3.0*/
/*char f118_03                  [] = {0x00, 0x03};*//*V1.3.0*/
/*char f118_04                  [] = {0x00, 0x04};*//*V1.3.0*/
/*char f118_05                  [] = {0x00, 0x05};*//*V1.3.0*/
/*char f118_06                  [] = {0x00, 0x06};*//*LBN20160317 - #CEDIC00018320*/
/*char f118_07                  [] = {0x00, 0x07};*//*LBN20160317 - #CEDIC00018320*/
/*char f118_08                  [] = {0x00, 0x08};*//*LBN20160317 - #CEDIC00018320*/
/*char f118_0a                  [] = {0x00, 0x0A};*/
/*char f118_0b                  [] = {0x00, 0x0B};*/
/*char f118_0c                  [] = {0x00, 0x0C};*/
   char f118_01    [] = "01";/*V1.3.0*/
   char f118_02    [] = "02";/*V1.3.0*/
   char f118_03    [] = "03";/*V1.3.0*/
   char f118_04    [] = "04";/*V1.3.0*/
   char f118_05    [] = "05";/*V1.3.0*/
   char f118_06    [] = "06";/*LBN20160317 - #CEDIC00018320*/
   char f118_07    [] = "07";/*LBN20160317 - #CEDIC00018320*/
   char f118_08    [] = "08";/*LBN20160317 - #CEDIC00018320*/
   char f118_0a    [] = "0A";
   char f118_0b    [] = "0B";
   char f118_0c    [] = "0C";
/*V1.6.1*//*LBN20201204 CAPS-2307 END*/              
#else                            
   extern char f118_01    [];/*V1.3.0*/
   extern char f118_02    [];/*V1.3.0*/
   extern char f118_03    [];/*V1.3.0*/
   extern char f118_04    [];/*V1.3.0*/
   extern char f118_05    [];/*V1.3.0*/
   extern char f118_06    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f118_07    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f118_08    [];/*LBN20160317 - #CEDIC00018320*/
   extern char f118_0a    [];
   extern char f118_0b    [];
   extern char f118_0c    [];
#endif

/*LBN20171018 - MERGE*/
#ifdef INIT_CB2APROTOCOL_VARS
	/* LBN20120417 */
	/*char protocol_version[4+1];*/
	char protocol_v130[4+1] = "1300";
	char protocol_v131[4+1] = "1310";
	char protocol_v140[4+1] = "1400";
	/*LBN20130703 - #38242*/
	char protocol_v150[4+1] = "1500";/*LBN20160317 - #CEDIC00018320*/
   char protocol_v160[4+1] = "1600";/*V1.6.1*//*LBN20201204 CAPS-2307*/
   char protocol_v161[4+1] = "1610";/*V1.6.1*//*LBN20201204 CAPS-2307*/
	char format_ascii [2+1] = "01";
	char format_ebcdic[2+1] = "02";
	int is_ebcdic = 0;
#else
	/*extern char protocol_version[];*/
	extern char protocol_v130[];
	extern char protocol_v131[];
	extern char protocol_v140[];
	extern char protocol_v150[];
   extern char protocol_v160[];/*V1.6.1*//*LBN20201204 CAPS-2307*/
   extern char protocol_v161[];/*V1.6.1*//*LBN20201204 CAPS-2307*/
	extern char format_ascii[];
	extern char format_ebcdic[];
	extern int is_ebcdic;
#endif /** INIT_CB2APROTOCOL_VARS **/
/*LBN20171018 - MERGE*/

/**** REMITTANCE file format ****/
/*Msg 0844*/
typedef struct {
 char remittance_agent_id[11];
 char file_number[6]; /*Field 11 du msg 0844 - 861*/ /*LBN20150119 - #44968*/
 char remittance_number[6]; /*A vide*/ /*LBN20150119 - #44968*/
 char remittance_seq[12]; /*A vide*/ /*LBN20150119 - #44968*/
 char record_type[4]; /*Msg Type*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f11[6];
 char f12[6];/*HHMMSS*/
 char f13[4];/*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f24[3];
 char f25[4];/* Start MOU CB2A EVLUTION : Reçyclage */
 char f33[11];
 char f47_07[2];/*YY*/
 char f47_33[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_47[9];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f56[36];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff52[2];
 char f58_ff53[8];
 char f58_ff54[2];/*LBN20160317 - #CEDIC00018320 - Recyclage*/
 char f58_ff60[6];/*LBN20160317 - #CEDIC00018320 - Recyclage*/
 char f58_ff61[22];
 char f58_ff62[22];
 char f58_ff63[16];
 char f58_ff64[16];
 char newline;
} FILE_HEADER;

/* Msg 0346 */
typedef struct {
 char remittance_agent_id[11];
 char file_number[6]; /*Field 11 du msg 0844 - 861*/ /*LBN20150119 - #44968*/
 char remittance_number[6]; /*Field 11 du msg 0346*/ /*LBN20150119 - #44968*/
 char remittance_seq[12]; /*Incrementation +1*/ /*LBN20150119 - #44968*/
 char record_type[4]; /*Msg Type*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f11[6];
 char f12[6];/*HHMMSS*/
 char f13[4];/*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f18[4];
 char f21[6];/*1.3.1*/
 char f25[4];/* Start MOU CB2A EVLUTION : Reçyclage & Paramétrage*/
 char f26[6];
 char f32[11];
 char f41[8];
 char f42[15];
 char f43[40];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f46_df50[12];
 char f46_df51[3];
 char f46_df52[2];
 char f46_df54[1];
 char f46_df5d[14];
 char f46_df5e[8];
 char f46_df5f[7];
 char f46_df61[7];/*LBN20160317 - #CEDIC00018320*/
 char f47_06[8]; /*MOU CB2A : Add FIELD 47 Subfield 06 TYPE DE SITE */
 char f47_07[2];
 char f47_18[3];
 char f47_19[34];
 char f47_20[11];
 char f47_48[32];
 char f49[3];
 char f58_9f33[6];/* LBN20120801 - #30045 */
 char f58_ff52[2];/* Start MOU CB2A EVLUTION : Reçyclage & Paramétrage*/
 char f58_ff55[12];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff56[6];
 char f58_ff57[6];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff61[22];/* Start MOU CB2A EVLUTION : Reçyclage & Paramétrage*/
 char f58_ff62[22];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff63[16];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff64[16];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff65[22];/* Start MOU CB2A EVLUTION : Reçyclage & Paramétrage*/ 
 char f59_1502[71];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1503[35];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f70[14];
 char newline;
} REMITTANCE_HEADER;

/* Msg 0146 - 0246 */
typedef struct {
 char remittance_agent_id[11];
 char file_number[6]; /*Field 11 du msg 0844 - 861*/ /*LBN20150119 - #44968*/
 char remittance_number[6]; /*Field 11 du msg 0346*/ /*LBN20150119 - #44968*/
 char remittance_seq[12]; /*Valeur du msg 0346*/ /*LBN20150119 - #44968*/
 char record_type[4]; /*Msg Type*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47 - MMJJDD*/
 char f02[19];
 char f03[6];
 char f04[12];
 char f11[6];
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f14[4]; /*MOU CB2A :Date d'expiration de la carte (Date, expiration): obligatoire si traitement piste ISO2 ou VAD, absent sinon */
 char f21[6];/*LBN20120417 - CB2A Fichier 1 3 1*/
 char f22[6];
 char f23[3];
 char f26[6];
 char f35[37];
 char f37[12];
 char f38[6];
 char f39[4];/*LBN20120417 - CB2A Fichier 1 3 1*/
 char f40[3];
 char f44_aa[7];/*LBN20160317 - #CEDIC00018320*/
 char f44_ac[21];/*LBN20160317 - #CEDIC00018320*/
 char f44_bc[21];/*LBN20160317 - #CEDIC00018320*/
 char f46_df5b[3];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f46_df5c[8];/*LBN20160317 - #CEDIC00018320*//* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f46_df96[1];/*LBN20160317 - #CEDIC00018320*/
 char f46_df97[3];/*LBN20160317 - #CEDIC00018320*/
 char f47_01[4]; /*MOU CB2A : Add FIELD 47 Subfield 01 Raison de la demande d'autorisation */
 char f47_02[2];
 char f47_03[1]; /*MOU CB2A : Add FIELD 47 Subfield 03 Type de facture /procédure */
 char f47_04[12];/*LBN20160317 - #CEDIC00018320*/
 char f47_05[1]; /*MOU CB2A : Add FIELD 47 Subfield 05 CODE FORÇAGE D'UNE TRANSACTION EN AUTORISATION */
 char f47_07[2];
 char f47_10[6];
 char f47_15[4];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/
 char f47_24[12];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f47_26[3];
 char f47_30[1];/*LBN20160317 - #CEDIC00018320*/
 char f47_32[12];/*LBN20160317 - #CEDIC00018320*/
 char f47_50[32];/*CB2A 1.3.0*//*LBN20160317 - #CEDIC00018320*/
 char f47_95[50];/*LBN20160317 - #CEDIC00018320*/
 char f47_96[4];/*LBN20160317 - #CEDIC00018320*/
 char f47_99[50];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f54[120];/*LBN20160317 - #CEDIC00018320*/
 char f55_0056[76];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_0082[4];
 char f55_008e[24]; 
 char f55_009a[6];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
 char f55_5f24[6];
 char f55_5f25[6];
 char f55_5f2d[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f02[12];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
 char f55_9f06[32];/*LBN20160317 - #CEDIC00018320*/
 char f55_9f07[4];
 char f55_9f0a[64];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f0d[10];
 char f55_9f0e[10];
 char f55_9f0f[10];
 char f55_9f10[64];/*LBN20160317 - #CEDIC00018320*/
 char f55_9f26[16];
 char f55_9f27[2];
 char f55_9f36[4];
 char f55_9f6b[38];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f7c[64];/*LBN20160317 - #CEDIC00018320*/
 char f55_df70[2];/*LBN20160317 - #CEDIC00018320*/ 
 char f55_df71[7];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df72[8];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df73[1]; /*MOU CB2A : Add FIELD 55 Subfield DF73 Type d’application carte */
 char f55_df74[3];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df75[16];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df76[12];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df77[16];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df78[36];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df86[70];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 *//*LBN20201204 CAPS-2307*/
 char f55_dfab[2];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_ff00[10];/*LBN20160317 - #CEDIC00018320*/
 char f56[36];/*Mandatory in a 0446 message, otherwise absent*//*LBN20160317 - #CEDIC00018320*//*TODO: suppress this field in 0146/0246 format*/
 char f57_0210[22];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0211[22];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0212[22];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0214[17];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0215[17];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0216[20];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_0217[200];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_ff01[32];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_ff02[32];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_ff0e[3];/*LBN20160317 - #CEDIC00018320*/
 char f57_ff0f[13];/*LBN20160317 - #CEDIC00018320*/
 char f58_0095[10];
 char f58_009c[2];
 char f58_0409[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f58_0410[2];/*LBN20140717 - #43286*/
 char f58_0411[1];/*LBN20160317 - #CEDIC00018320*/
 char f58_0412[6];/*LBN20140512 - #42512*//*LBN20160317 - #CEDIC00018320*/ 
 char f58_0413[2];/*LBN20160317 - #CEDIC00018320*/
 char f58_0414[40];/*LBN20130912 - #39088*/
 char f58_0415[2];/*LBN20130912 - #39088*/
 char f58_0416[2];/*LBN20130912 - #39088*/
 char f58_0417[24];/*LBN20160317 - #CEDIC00018320*/
 char f58_0418[6];/*LBN20160317 - #CEDIC00018320*/
 char f58_0419[13];/*LBN20190305 CAPS-703*/
 char f58_0420[113];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f58_0603[5];/*LBN20130912 - #39088*/
 char f58_0809[2];/*LBN20160317 - #CEDIC00018320*/
 char f58_9f33[6];
 char f58_9f34[6];
 char f58_9f37[8];
 char f58_9f66[8];/*LBN20160317 - #CEDIC00018320*/
 char f58_df85[10];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
 char f58_df93[16];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 *//*LBN20201204 CAPS-2307*/
 char f58_ff0d[10];
 char f58_ff0e[10];
 char f58_ff0f[10];
 char f58_ff31[8];/*LBN20171004 MERGE*/
 char f58_ff45[12];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff46[32];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff47[2]; /*MOU CB2A : Add FIELD 58 Subfield FF47 Type de sécurisation de transaction de commerce électronique */ 
 char f58_ff48[80];/*LBN20160317 - #CEDIC00018320*//*LBN20201204 CAPS-2307*/
 char f58_ff49[80];/*LBN20160317 - #CEDIC00018320*//*LBN20201204 CAPS-2307*/
 char f58_ff50[2];
 char f58_ff51[32];/*CB2A 1.3.0*//*LBN20160317 - #CEDIC00018320*/ 
 char f58_ff81[6];/*LBN20160317 - #CEDIC00018320*/
 char f59_0001[39];/*LBN20160317 - #CEDIC00018320*/
 char f59_0002[2];/*LBN20160317 - #CEDIC00018320*/
 char f59_0003[2];/*LBN20160317 - #CEDIC00018320*/
 char f59_0004[40];/*LBN20160317 - #CEDIC00018320*/
 char f59_0005[3];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0012[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0013[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0014[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0020[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0022[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0023[37];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0024[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0025[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0026[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0027[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0028[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0029[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0033[6];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0037[14];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0041[8];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0042[255];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0043[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0044[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0046[140];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0047[50];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1205[35];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1206[19];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1207[30];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1208[40];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1209[11];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1210[34];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1501[107];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} TRX_DETAIL;

/* Msg 0446*/
typedef struct {
 char remittance_agent_id[11];
 char file_number[6]; /*Field 11 du msg 0844 - 861*/ /*LBN20150119 - #44968*/
 char remittance_number[6]; /*Field 11 du msg 0346*/ /*LBN20150119 - #44968*/
 char remittance_seq[12]; /*Valeur du msg 0346*/ /*LBN20150119 - #44968*/
 char record_type[4]; /*Msg Type*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47 - MMJJDD*/
 char f02[19];
 char f03[6];
 char f04[12];
 char f11[6];
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f14[4]; /*MOU CB2A :Date d'expiration de la carte (Date, expiration): obligatoire si traitement piste ISO2 ou VAD, absent sinon */
 char f21[6];/*LBN20120417 - CB2A Fichier 1 3 1*/
 char f22[6];
 char f23[3];
 char f26[6];
 char f35[37];
 char f37[12];
 char f38[6];
 char f39[4];/*LBN20120417 - CB2A Fichier 1 3 1*/
 char f40[3];
 char f44_aa[7];/*LBN20160317 - #CEDIC00018320*/
 char f44_ac[21];/*LBN20160317 - #CEDIC00018320*/
 char f44_bc[21];/*LBN20160317 - #CEDIC00018320*/
 char f46_df5b[3];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f46_df5c[8];/*LBN20160317 - #CEDIC00018320*//* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f46_df96[1];/*LBN20160317 - #CEDIC00018320*/
 char f46_df97[3];/*LBN20160317 - #CEDIC00018320*/
 char f47_01[4]; /*MOU CB2A : Add FIELD 47 Subfield 01 Raison de la demande d'autorisation */
 char f47_02[2];
 char f47_03[1]; /*MOU CB2A : Add FIELD 47 Subfield 03 Type de facture /procédure */
 char f47_04[12];/*LBN20160317 - #CEDIC00018320*/
 char f47_05[1]; /*MOU CB2A : Add FIELD 47 Subfield 05 CODE FORÇAGE D'UNE TRANSACTION EN AUTORISATION */
 char f47_07[2];
 char f47_10[6];
 char f47_15[4];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/
 char f47_24[12];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f47_26[3];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_30[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_32[12];/*LBN20160317 - #CEDIC00018320*/
 char f47_50[32];/*CB2A 1.3.0*//*LBN20160317 - #CEDIC00018320*/
 char f47_99[50];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f54[120];/*LBN20160317 - #CEDIC00018320*/
 char f55_0056[76];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_0082[4];
 char f55_008e[24];
 char f55_009a[6];
 char f55_5f24[6];
 char f55_5f25[6];
 char f55_5f2d[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f02[12];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f06[32];/*LBN20160317 - #CEDIC00018320*/
 char f55_9f07[4];
 char f55_9f0a[64];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f0d[10];
 char f55_9f0e[10];
 char f55_9f0f[10];
 char f55_9f10[64];/*LBN20160317 - #CEDIC00018320*/
 char f55_9f26[16];
 char f55_9f27[2];
 char f55_9f36[4];
 char f55_9f6b[38];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f55_9f7c[64];/*LBN20160317 - #CEDIC00018320*/
 char f55_df70[2];/*LBN20160317 - #CEDIC00018320*/ 
 char f55_df71[7];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df72[8];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df73[1]; /*MOU CB2A : Add FIELD 55 Subfield DF73 Type d’application carte */
 char f55_df74[3];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df75[16];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df76[12];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df77[16];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df78[36];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_df86[70];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 *//*LBN20201204 CAPS-2307*/
 char f55_dfab[2];/*CB2A 1.3.1*//*LBN20160317 - #CEDIC00018320*/ 
 char f55_ff00[10];/*LBN20160317 - #CEDIC00018320*/
 char f56[36];/*Mandatory in a 0446 message, otherwise absent*//*LBN20160317 - #CEDIC00018320*/
 char f57_ff01[32];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_ff02[32];/* MOU CB2A : ADD NEW FIELD SEE SEE BSD-CED-BOA-20110426-GAM-TablesDonnees_V13.doc*/
 char f57_ff0e[3];/*LBN20160317 - #CEDIC00018320*/
 char f57_ff0f[13];/*LBN20160317 - #CEDIC00018320*/
 char f58_0095[10];
 char f58_009c[2];
 char f58_0409[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f58_0410[2];/*LBN20140717 - #43286*/
 char f58_0411[1];/*LBN20160317 - #CEDIC00018320*/
 char f58_0412[6];/*LBN20140512 - #42512*//*LBN20160317 - #CEDIC00018320*/ 
 char f58_0413[2];/*LBN20160317 - #CEDIC00018320*/
 char f58_0414[40];/*LBN20130912 - #39088*/
 char f58_0415[2];/*LBN20130912 - #39088*/
 char f58_0416[2];/*LBN20130912 - #39088*/
 char f58_0417[24];/*LBN20160317 - #CEDIC00018320*/
 char f58_0418[6];/*LBN20160317 - #CEDIC00018320*/
 char f58_0419[13];/*LBN20190305 CAPS-703*/
 char f58_0420[113];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f58_0603[5];/*LBN20130912 - #39088*/
 char f58_0809[2];/*LBN20160317 - #CEDIC00018320*/
 char f58_9f33[6];
 char f58_9f34[6];
 char f58_9f37[8];
 char f58_9f66[8];/*LBN20160317 - #CEDIC00018320*/
 char f58_df85[10];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 */
 char f58_df93[16];/*Start ABE04042013 CB2A : SEE NT-CED-BOA-20120912_Enhancement_CB_V01 *//*LBN20201204 CAPS-2307*/
 char f58_ff0d[10];
 char f58_ff0e[10];
 char f58_ff0f[10];
 char f58_ff45[12];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff46[32];/*LBN20160317 - #CEDIC00018320*/
 char f58_ff47[2]; /*MOU CB2A : Add FIELD 58 Subfield FF47 Type de sécurisation de transaction de commerce électronique */ 
 char f58_ff48[80];/*LBN20160317 - #CEDIC00018320*//*LBN20201204 CAPS-2307*/
 char f58_ff49[80];/*LBN20160317 - #CEDIC00018320*//*LBN20201204 CAPS-2307*/
 char f58_ff51[32];/*CB2A 1.3.0*//*LBN20160317 - #CEDIC00018320*/ 
 char f58_ff81[6];/*LBN20160317 - #CEDIC00018320*/
 char f59_0001[39];/*LBN20160317 - #CEDIC00018320*/
 char f59_0002[2];/*LBN20160317 - #CEDIC00018320*/
 char f59_0003[2];/*LBN20160317 - #CEDIC00018320*/
 char f59_0004[40];/*LBN20160317 - #CEDIC00018320*/
 char f59_0005[3];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0012[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0013[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0014[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0020[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0022[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0023[37];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0024[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0025[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0026[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0027[15];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0028[2];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0029[1];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0033[6];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0037[14];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0041[8];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0042[255];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0043[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0044[4];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0046[140];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_0047[50];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1205[35];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1206[19];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1207[30];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1208[40];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1209[11];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1210[34];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f59_1501[107];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} TRX_REVERSAL;

/* Msg 0546*/
typedef struct {
 char remittance_agent_id[11];
 char file_number[6]; /*Field 11 du msg 0844 - 861*/ /*LBN20150119 - #44968*/
 char remittance_number[6]; /*Field 11 du msg 0346*/ /*LBN20150119 - #44968*/
 char remittance_seq[12]; /*Valeur du msg 0346*/ /*LBN20150119 - #44968*/
 char record_type[4]; /*Msg Type*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f11[6];
 char f12[6];/*HHMMSS*/
 char f13[4];/*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f43[40];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_07[2];
 char f47_09[6];/*LBN20160317 - #CEDIC00018320*/
 char f50[3];
 char f58_ff35[10];/* Start Evolution CB2A : Parametrage */
 char f58_ff36[16];/* Start Evolution CB2A : Parametrage */
 char f58_ff58[10];/* Start Evolution CB2A : Parametrage */
 char f58_ff59[16];/* Start Evolution CB2A : Parametrage */
 char f74[10];
 char f76[10];
 char f77[10];
 char f86[16];
 char f88[16];
 char f89[16];
 char newline;
} REMITTANCE_SUMMARY;

/**** REMITTANCE REPORT (CRR) file format ****/
/* Msg 0840*/
typedef struct {
 char record_type[4]; /*Msg Type*/
 char f11[6]; /*Field 11*/
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f24[3]; /*Field 24*/
 char f25[4]; /*Field 25*/
 char f31_01[250];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f33[11];/*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_07[2]; /*YY*//*Field 47 Subfield 07*/
 char f47_47[9]; /*Field 47 Subfield 47*/
 char f58_ff52[1]; /*Field 58 Subfield FF52*/
 char f58_ff53[8]; /*Field 58 Subfield FF53*/
 char f58_ff54[1]; /*Field 58 Subfield FF54*/
 char f58_ff60[6]; /*Field 58 Subfield FF60*//*YYMMDD*/
 char f58_ff61[22]; /*Field 58 Subfield FF61*/
 char f58_ff62[22]; /*Field 58 Subfield FF62*/
 char f58_ff63[16]; /*Field 58 Subfield FF63*/
 char f58_ff64[16]; /*Field 58 Subfield FF64*/
 char f118_0a[1]; /*Field 118 Subfield 0A*/
 char f118_0b[3]; /*Field 118 Subfield 0B*/
 /*char filler[131];*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} FILE_MSG_840;

/* Msg 0340*/
typedef struct {
 char record_type[4]; /*Msg Type*/
 char f11[6]; /*Field 11*/
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f25[4]; /*Field 25*/
 char f32[11]; /*Field 32*/
 char f46_df5f[7]; /*Field 46 Subfield DF5F*/
 char f47_07[2]; /*Field 47 Subfield 07*/
 char f58_ff31[6]; /*Field 58 Subfield FF31*/
 char f58_ff54[1]; /*Field 58 Subfield FF54*/
 char f58_ff55[6]; /*Field 58 Subfield FF55*/
 char f58_ff56[3]; /*Field 58 Subfield FF56*/
 char f58_ff60[6]; /*Field 58 Subfield FF60*//*AAMMJJ*/
 char f58_ff62[22]; /*Field 58 Subfield FF62*/
 char f58_ff65[22]; /*Field 58 Subfield FF65*/
 char f118_0a[1]; /*Field 118 Subfield 0A*/
 char f118_0b[3]; /*Field 118 Subfield 0B*/
 /*char filler[131];*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} FILE_MSG_340;

/* Msg 0640*/
typedef struct {
 char record_type[4]; /*Msg Type*/
 char f11[6]; /*Field 11*/
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char msg_date[6]; /*Field 13 + Subfiel 07 of 47*/ /*MMJJYY*/
 char f25[4]; /*Field 25*/
 char f27[6]; /*Field 27*/
 char f31_01[250]; /*Field 31*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char f47_07[2]; /*Field 47 Subfield 02*/
 char f47_47[9]; /*Field 47 Subfield 47*/
 char f56[36]; /*Field 56*/
 /*char filler[131];*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} FILE_MSG_640;

/* Msg 0540*/

typedef struct {
 char record_type[4]; /*Msg Type*/
 char f11[6]; /*Field 11*/
 char f12[6]; /*HHMMSS*/
 char f13[4]; /*MMDD*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char msg_date[6]; /*Field 13 + Subfield 07 of 47*/
 char f47_07[2]; /*Field 47 Subfield 07*/
 char f118_0c[6]; /*Field 118 Subfield 0C*/
 /*char filler[131];*//*V1.6.1*//*LBN20201204 CAPS-2307*/
 char newline;
} FILE_MSG_540;

/*LBN20160317 - #CEDIC00018320
int Cb2aInit (void);
int Cb2aAnalyse (char*);
int CheckBit (char*, int);
LBN20160317 - #CEDIC00018320 END*/

/*int BcdToAscii (const UCHAR *sSrc, UCHAR *sDst, int nLng);*//*DEBUG*/
void MyHexToAscii(UCHAR sHex[], char sAscii[], int nBytes);
void hex2ascii_alpha(UCHAR sHex[], char sAscii[], int nBytes); /* LBN20131203 - #40204 */

char *getValueByType (char *sField, char *sType);
/** char *getValueByTag (char *sField, char *sTag); **/
char *getValueByTag (char *sField, int nLen, char *sTag); /** 20080929MLO **/
char *getValueByTag2 (char *sField, char *sTag); /* NLY08042008 */
char *getValueByTag_alpha (char *sField, int nLen, char *sTag); /* LBN20131203 - #40204 */
char *getValueByTag_dyn_TLVtype(int nFieldNbr, char *sField, int nLen, char *sTag, int nBytes, int nTLVType);/*LBN20160317 - #CEDIC00018320*//*LBN20160628 - #CEDIC00018619 - refonte*/
char *getValueByTagAndLength (char *sField, int iFieldLen, int iFieldNbBytes, char *sTag, int iTagNbBytes, int iTagDatatype, int iValueByteStart, int iValuePartLen);/*LBN20201204 CAPS-2307*/

/*int CheckBit( char *cPtr, int nBitNbr );*//*LBN20160317 - #CEDIC00018320*/
/*int FieldPresent( char *cPtr, int nFldNbr);*//*DEBUG*/
void trace_event(char* szBuf, int nLevel);
void HexDump(char* sBuffer, int nLen);
/*void szcpy (char *sDst, const char *sSrc, int nLen);*//*DEBUG*/

int Cb2aInit(void);
int Cb2aAnalyse(char *sBuf);
int Cb2aDump(char *sBuf);
void Cb2aTrace(void);

int ProcessRecord(char *remittance_agent_id);
int RemittanceHeader(char *remittance_agent_id);
int TrxDetail( char *remittance_agent_id);
int RemittanceSummary(char *remittance_agent_id);
int FileHeader(char *remittance_agent_id);
int ReversalRemittance(char *remittance_agent_id);
