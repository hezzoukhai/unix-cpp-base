#ifndef PWC_POS_CBPR__H
#define PWC_POS_CBPR__H


#define	NUL			0x00 

/*   MESSAGES */

#define POS_REF_MESS		"01"
#define TRANSACTION_MESS_02	"02"
#define TRANSACTION_MESS_22	"22" /*YK janv*/
#define STOP_LIST_MESS		"03"
#define AUTHO_MESS_04		"04"
#define AUTHO_MESS_24		"24"
#define AUTHO_MESS_64		"64"
#define AUTHO_MESS_74		"74"
#define AUTHO_MESS_34		"34"
#define AUTHO_MESS_84		"84"

#define MESS_OFFSET		12
#define BYTE12_OFFSET	11
#define BYTE12_LEN		1

/*CBPR*/
#define     POS_CBPR_POS_REF_MESS			11
#define     POS_CBPR_TRANSACTION_MESS_02    12
#define     POS_CBPR_STOP_LIST_MESS         13		
#define     POS_CBPR_AUTHO_MESS_04          14
#define     POS_CBPR_AUTHO_MESS_24          15
#define     POS_CBPR_AUTHO_MESS_64          16
#define     POS_CBPR_BIN_FILE_MESS          17
#define     POS_CBPR_TAC                    18
#define     POS_CBPR_DLE                    19 
#define     POS_CBPR_DCB                    20
#define     POS_CBPR_AUTHO_MESS_74          21
#define     POS_CBPR_AUTHO_MESS_34          22
#define     POS_CBPR_AUTHO_MESS_84          23
#define     POS_CBPR_TRANSACTION_MESS_22    24/*YK janv*/
#define     POS_CBPR_TAC_WAFA               25


/*message 00*/

#define POS_CODE_LEN		10 
#define MESS_LEN		2
#define POS_STOP_LIST_LEN	4
#define REST_LEN		13

typedef struct cbpr_exchange_request_t
{
    char		soh;
    char		pos_code[POS_CODE_LEN];
    char		byte12;
    char		mess[MESS_LEN];
    char		pos_stop_list[POS_STOP_LIST_LEN];
    char		rest[REST_LEN];
} cbpr_exchange_request;

#define BUILDER_CODE_LEN	2
#define MATERIAL_CODE_LEN	2
#define SOFTWARE_VERSION_LEN	2
#define POS_STOP_LIST_SIZE_LEN	2

typedef struct cbpr_param_a_t
{
    char		a;
    char		len[2];
    char		builder_code[BUILDER_CODE_LEN];
    char		material_code[MATERIAL_CODE_LEN];
    char		software_version[SOFTWARE_VERSION_LEN];
    char		pos_stop_list_size[POS_STOP_LIST_SIZE_LEN];
    char		etx;
    char		lrc;
}  cbpr_param_a;

#define MESL_LEN		1
#define BLOCK_NUMBER_LEN	3
#define LAST_STOP_LIST_LEN	4

typedef struct cbpr_param_b_t
{
    char		b;
    char		len[2];
    char		mesl;
    char		block_number[BLOCK_NUMBER_LEN];
    char		last_stop_list[LAST_STOP_LIST_LEN];
    char		etx;
    char		lrc;
} cbpr_param_b;

#define BIN_FILE_NUMBER_LEN	3

typedef struct cbpr_param_c_t
{
    char		c;
    char		len[2];
    char		bin_file_number[BIN_FILE_NUMBER_LEN];
    char		etx;
    char		lrc;
} cbpr_param_c;

typedef struct cbpr_no_param_t
{
    char		etx;
    char		lrc;
} cbpr_no_param;

/*message 01*/


typedef struct cbpr_location_t
{
    char		pos_acronym[15];
    char		lf1;
    char		blancs[7];
    char		lf2;
    char		pos_location[15];
    char		lf3;
} cbpr_location;

typedef struct cbpr_pos_ref_t
{
    char		stx;
    char		pos_code[POS_CODE_LEN];
    cbpr_location	location;
    char		*center_param;
    char		etx;
    char		lrc;
} cbpr_pos_ref;

/*message 03*/

#define MAX_STOPED_NUMBER	20

#define NO_MODIF_MESL		'0'
#define FULL_LIST_MESL		'1'
#define DEL_NUM_MESL		'2'
#define ADD_NUM_MESL		'3'

#define STOP_LIST_NUMBER_LEN	4

typedef struct cbpr_update_type_t
{
    char		stx;
    char		stop_list_number[STOP_LIST_NUMBER_LEN];
    char		nul;
    char		mesl;
    char		etx;
    char		lrc;
} cbpr_update_type;

#define NUMBER_LEN		20

typedef struct cbpr_stoped_number_t
{
    char		number[NUMBER_LEN];
    char		cr;
} cbpr_stoped_number;

typedef struct cbpr_stop_list_block_t
{
    char		stx;
    char		block_number[BLOCK_NUMBER_LEN];
    char		cr1;
    cbpr_stoped_number	stoped_number[MAX_STOPED_NUMBER];
    char		etb_etx;
    char		lrc;
} cbpr_stop_list_block;



#define NO_STOP_LIST		"\0"

/*message X2*/
#define NB_TRANS_02		16
#define NB_TRANS_22		14
#define NB_TRANS_42		11

#define SIZE_TRANS_02		39
#define SIZE_TRANS_22		45
#define SIZE_TRANS_42		56

#define FILE_NUMBER_LEN		2

typedef struct cbpr_init_trans_t
{
    char		stx;
    char		file_number[FILE_NUMBER_LEN];
    char		cr1;
    char		block_number[BLOCK_NUMBER_LEN];
    char		cr2;
    char		etx;
    char		lrc;
} cbpr_init_trans;


#define BANK_CODE_LEN		5
#define OPENING_FILE_DATE_LEN	6

typedef struct cbpr_header_file_t
{
    char		stx;
    char		pos_code[POS_CODE_LEN];
    char		cr1;
    char		bank_code[BANK_CODE_LEN];
    char		cr2;
    char		file_number[FILE_NUMBER_LEN];
    char		cr3;
    char		opening_file_date[OPENING_FILE_DATE_LEN];
    char		etx;
    char		lrc;
} cbpr_header_file;

#define MAX_TRANS		631
#define BLOCK_AMOUNT_LEN	10

typedef struct cbpr_remittance_block_t
{
    char		stx;
    char		block_number[BLOCK_NUMBER_LEN];
    char		block_amount[BLOCK_AMOUNT_LEN];
    char		cr1;
    char		trans_data[MAX_TRANS + 2];
} cbpr_remittance_block;

#define NOT_NULL_TRANS_NUMBER_LEN	4
#define DEBIT_TRANS_AMOUNT_LEN		12
#define CREDIT_TRANS_AMOUNT_LEN		12

typedef struct cbpr_remittance_t
{
    char		stx;
    char		not_null_trans_number[NOT_NULL_TRANS_NUMBER_LEN];
    char		cr1;
    char		debit_trans_amount[DEBIT_TRANS_AMOUNT_LEN];
    char		cr2;
    char		credit_trans_amount[CREDIT_TRANS_AMOUNT_LEN];
    char		cr3;
    char		etx;
    char		lrc;
} cbpr_remittance;

#define REMITTANCE_NUMBER_LEN		5
#define REMITTANCE_DATE_LEN		6
#define MESSAGE_LEN			19

typedef struct cbpr_remittance_ack_t
{
    char		stx;
    char		remittance_number[REMITTANCE_NUMBER_LEN];
    char		cr1;
    char		remittance_date[REMITTANCE_DATE_LEN];
    char		cr2;
    char		not_null_trans_number[NOT_NULL_TRANS_NUMBER_LEN];
    char		cr3;
    char		debit_trans_amount[DEBIT_TRANS_AMOUNT_LEN];
    char		cr4;
    char		credit_trans_amount[CREDIT_TRANS_AMOUNT_LEN];
    char		cr5;
    char		message[MESSAGE_LEN];
    char		etx;
    char		lrc;
} cbpr_remittance_ack;


#define NO_DEBIT		0
#define NO_CREDIT		0
#define NO_CANCEL		0


/*message X4*/

#define	MSG_04_LEN	32
#define	MSG_24_LEN	50
#define	MSG_64_LEN	67

#define CARD_NUMBER_LEN		19
#define AMOUNT_LEN		8

typedef struct cbpr_autho_request_04_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char		card_type;
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		etx;
    char		lrc;
} cbpr_autho_request_04;

#define AUTHO_04_NUMBER_LEN	4

typedef struct cbpr_positive_reply_04_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		cr2;
    char		reply;
    char		cr3;
    char		autho_number[AUTHO_04_NUMBER_LEN];
    char		etx;
    char		lrc;
} cbpr_positive_reply_04;

typedef struct cbpr_negative_reply_04_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		cr2;
    char		reply;
    char		etx;
    char		lrc;
} cbpr_negative_reply_04;

#define TRACK2_LEN		37

typedef struct cbpr_autho_request_24_t
{
    char		stx;
    char		track2[TRACK2_LEN];
    char		card_type;
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		etx;
    char		lrc;
} cbpr_autho_request_24;

#define TRACK2_AMEX_LEN 30

typedef struct cbpr_autho_request_amex_24_t
{
    char		stx;
    char		track2[TRACK2_AMEX_LEN];    
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		etx;
    char		lrc;
} cbpr_autho_request_amex_24;


#define AUTHO_24_NUMBER_LEN	6

typedef struct cbpr_positive_reply_24_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		cr2;
    char		reply;
    char		cr3;
    char		autho_number[AUTHO_24_NUMBER_LEN];
    char		etx;
    char		lrc;
} cbpr_positive_reply_24;

typedef struct cbpr_negative_reply_24_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char		cr1;
    char		amount[AMOUNT_LEN];
    char		cr2;
    char		reply;
    char		etx;
    char		lrc;
} cbpr_negative_reply_24;

#define VALID_DATE_LEN 4

typedef struct cbpr_autho_request_34_t
{
    char		stx;
    char		card_number[CARD_NUMBER_LEN];
    char        validity_date[VALID_DATE_LEN]; 
	char		card_type;
    char		cr;   
    char		amount[AMOUNT_LEN];
    char		etx;
    char		lrc;
} cbpr_autho_request_34;

#define PIN_BLOCK_LEN		16

typedef struct cbpr_autho_request_64_t
{
    char		stx;
    char		track2[TRACK2_LEN];
    char		card_type;
    char		cr1;
    char		pin_block[PIN_BLOCK_LEN];
    char		cr2;
    char		amount[AMOUNT_LEN];
    char		etx;
    char		lrc;
} cbpr_autho_request_64;

typedef struct cbpr_autho_request_74_t
{
    char		stx;
    char		track2[TRACK2_LEN];
    char		card_type;
    char		cr;
    char		amount[AMOUNT_LEN];
    char		pin_block[PIN_BLOCK_LEN];
	char		encrypte_key[PIN_BLOCK_LEN];    
    char		etx;
    char		lrc;
} cbpr_autho_request_74;

#define AUTHO_64_NUMBER_LEN		6

typedef struct cbpr_positive_reply_64_t
{
    char		stx;
    char		track2[TRACK2_LEN];
    char		card_type;
    char		cr1;
    char		pin_block[PIN_BLOCK_LEN];
    char		cr2;
    char		amount[AMOUNT_LEN];
    char		cr3;
    char		reply;
    char		autho_number[AUTHO_64_NUMBER_LEN];
    char		etx;
    char		lrc;
} cbpr_positive_reply_64;

typedef struct cbpr_negative_reply_64_t
{
    char		stx;
    char		track2[TRACK2_LEN];
    char		card_type;
    char		cr1;
    char		pin_block[PIN_BLOCK_LEN];
    char		cr2;
    char		amount[AMOUNT_LEN];
    char		cr3;
    char		reply;
    char		etx;
    char		lrc;
} cbpr_negative_reply_64;


/*variables globales*/
typedef struct SCbprMessage
{
	char            pos_code		    [POS_CODE_LEN 		+ 1];
	char		    byte12;
	char            mess			    [MESS_LEN 		+ 1];
	char            pos_stop_list		[POS_STOP_LIST_LEN 	+ 1];
	char		    builder_code		[BUILDER_CODE_LEN 	+ 1];
	char		    material_code		[MATERIAL_CODE_LEN 	+ 1];
	char		    software_version	[SOFTWARE_VERSION_LEN 	+ 1];
	char            pos_stop_list_size	[POS_STOP_LIST_SIZE_LEN + 1];
	char            mesl;
	char            block_number		[BLOCK_NUMBER_LEN 	+ 1];
	char            last_stop_list		[LAST_STOP_LIST_LEN 	+ 1];
	char		    bin_file_number		[BIN_FILE_NUMBER_LEN 	+ 1];
	char            new_pos_stop_list   [POS_STOP_LIST_LEN 	+ 1];

}TSCbprMessage;

#endif



