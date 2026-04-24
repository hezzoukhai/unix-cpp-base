#ifndef HPS_ISO_SNI_DEFINE_H
#define HPS_ISO_SNI_DEFINE_H

#define BLOCK_SIZE			256
/*---------------*/
/*  ACTION CODE  */
/*---------------*/
#define APPROVED			0
#define DECLINED			100

/*function code*/
#define WITHDRAWAL_REQ			"200"
#define BALANCE_REQ			"281"
#define CHEQUE_REQ			"692"
#define STATEMENT_REQ		"691"

#define OPEN_SESSION		"801"
#define ECHANGE_K7			"881"
#define CUT_OFF_ACK			"882"
#define ON_SERVICE			"890"
#define GAB_OUT_SERVICE		"891"
#define START_SUPERVISOR_MODE		"892"
#define END_SUPERVISOR_MODE		"893"




#define PIN_KEY_TRANSFERT		891
#define TPK_KEY_TRANSFERT		892
#define MAC_KEY_TRANSFERT		893

#define OK_NO_ACTION			899
#define END_TRANSFERT			300
#define INSERT_BLOCK			301

#define	TRX_AUT_REP_FROM_NW 		671 
#define	TRX_ANNUL_REP_FROM_NW 		674



#define UPDATE_PAR			882 
#define UPDATE_APP			883
#define UPDATE_SCR			884
#define UPDATE_PRF			885

#define REBOOT_GAB			898


#define STOP_LIST			881
#define SEND_CUT_OFF			888



#define END_PROCESS		6000

#endif /*HPS_ISO_SNI_DEFINE_H*/



