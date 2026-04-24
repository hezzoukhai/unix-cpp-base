#ifndef PWC_DBL_PROTO__H
#define PWC_DBL_PROTO__H

typedef struct t_dbl_general_header
{
	char Message_Class              [1];
	char Response_Flag              [1];
	char FS1_General_Header		[1];
/*
	char Luno                       [9];
*/
	char Luno                       [3];
	char FS2_General_Header		[1];
	char Message_Sequence_Number    [3];
	char FS3_General_Header         [1];
} v_dbl_general_header;


typedef struct t_dbl_config_header
{
	char Message_Subclass           [1];
	char Message_Identifier         [1];
	char FS1_Config_Header          [1];
} v_dbl_config_header;



typedef struct t_dbl_reply_header
{
	char Message_Class              [1];
	char Response_Flag              [1];
	char FS1_Reply_Header		[1];

/*
	char Luno                       [9];
*/
	char Luno                       [3];
	char FS2_Reply_Header		[1];
/*
	char Message_Sequence_Number    [3];
*/
	char FS3_Reply_Header           [1];
} v_dbl_reply_header;

typedef struct t_dbl_reply
{
        v_dbl_reply_header   Reply_Header;
        char                 Next_state_id_data          [3];
        char                 FS1_reply                   [1];
        char                 Nbr_Type1_Notes             [2];
        char                 Nbr_Type2_Notes             [2];
        char                 Nbr_Type3_Notes             [2];
        char                 Nbr_Type4_Notes             [2];
        char                 FS2_reply                   [1];
/*
        char                 Coinge_Amount_To_Dispence   [2];
        char                 FS3_reply                   [1];
*/
        char                 Transaction_Serial_Nbr      [4];
        char                 Function_Id                 [1];
        char                 Screen_Number               [3];
/*
        char                 Update_Screen_Number        [3];
        char                 Screen_Data                 [24];
*/
        char                 FS4_reply                   [1];
        char                 Message_Coordination_Nbr    [1];
        char                 Card_Return_Retain          [1];
        char                 Printer_Flag_Field          [1];
        char                 Printer_Data_Field          [660];
        char                 FS5_reply                   [1];
        char                 Buffer_Identifier           [1];
        char                 Track3_Data                 [107];
} v_dbl_reply;


typedef struct t_dbl_config 
{
        v_dbl_general_header General_Header;
        v_dbl_config_header  Config_Header;
        char                 Camera_Control_Option       [1];
        char                 Card_Reader_Error_Threshold [3];
        char                 Reserved_j                  [3];
        char                 Reserved_k                  [3]; 
        char                 Track3_Write_Error_Threshold[3];
        char                 Supply_Ready_Amount_Buffer  [3];
        char                 Reserved_n                  [9];
        char                 FS1_config                  [1];
        char                 LUNO_config                 [3];
        char                 FS2_config                  [1];
        char                 Keyboard_Timer_Id           [2];
        char                 Keyboard_Timer              [3];
        char                 Time_Out_Screen_Id          [2];
        char                 Time_Out_Screen             [3];
        char                 Close_State_Timer_Id        [2];
        char                 Close_State_Timer           [3];
        char                 Comms_Timer_Id              [2];
        char                 Comms_Timer                 [3];
        char                 Insertion_Timer_Id          [2];
        char                 Insertion_Timer             [3];
        char                 Cash_Timer_Id               [2];
        char                 Cash_Timer                  [3];
        char                 Poll_Select_Timer_Id        [2];
        char                 Poll_Select_Timer           [3];
        char                 Present_Time_Out_Id         [2];
        char                 Present_Time_Out            [3];
        char                 Night_Safe_Deposit_Timer_Id [2];
        char                 Night_Safe_Deposit_Timer    [3];
        char                 Card_Removal_Time_Out_Id    [2];
        char                 Card_Removal_Time_Out       [3];
        char                 Additional_present_Timer_Id [2];
        char                 Additional_present_Timer    [3];
        char                 Document_Eject_Timer_Id     [2];
        char                 Document_Eject_Timer        [3];
        char                 Statement_Present_Timer_Id  [2];
        char                 Statement_Present_Timer     [3];
        char                 Door_Access_Open_Timer_Id   [2];
        char                 Door_Access_Open_Timer      [3];
        char                 Form_Insertion_Timer_Id     [2];
        char                 Form_Insertion_Timer        [3];
        char                 Vandal_Guard_Close_Timer_Id [2];
        char                 Vandal_Guard_Close_Timer    [3];

} v_dbl_config;

/******************************************************************************/
/* Structure generique pour toutes les donnees relatives aux parametrages     */
/* du GAB avec le protocole DIE BOLD                                          */
/******************************************************************************/

typedef struct t_dbl_parameter 
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	char		     Filler         	 [1024];
} v_dbl_parameter;





typedef struct t_dbl_config_id
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	char		     Config_Id_Number		 [4];
} v_dbl_config_id;


typedef struct t_dbl_command
{
	v_dbl_general_header General_Header;
	char		     Command_Code		 [1];
	char		     Command_Modifier		 [1];
} v_dbl_command;



typedef struct t_dbl_diebold_info_load
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	char		     Algorithm_Locator	 	 [3];
	char		     Offset_Index_Data	 	 [21];
} v_dbl_diebold_info_load;


typedef struct t_dbl_mac_selection
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	char		     Transaction_request_Field	 [19];
	char		     Transaction_reply_Field	 [17];
	char 		     FS1_mac_selection 		 [ 1];
	char		     Solicitted_Status_Field	 [ 1];
	char 		     FS2_mac_selection 		 [ 1];
	char		     Other_Messages_Field     	 [ 3];
	char 		     FS3_mac_selection 		 [ 1];
	char		     Track1_Field                [ 6];
	char 		     FS4_mac_selection 		 [ 1];
	char		     Track2_Field                [ 6];
	char 		     FS5_mac_selection 		 [ 1];
	char		     Track3_Field                [11];
	char 		     FS6_mac_selection 		 [ 1];
} v_dbl_mac_selection;



typedef struct t_dbl_date_time
{
	v_dbl_general_header General_Header;
	/*v_dbl_config_header  Config_Header;*/
	char		     Command_Code		 [ 3];
	char		     Action_Code		 [ 2];
	char		     Date_Time                   [12];
} v_dbl_date_time;


typedef struct t_dbl_encryption_key_load
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	/*BEGIN AHM 2007/03/15*/
	/*char		     New_Key_Data                [24];*/
	char		     New_Key_Data                [256];
	/*END AHM*/
} v_dbl_encryption_key_load;


typedef struct t_dbl_init_ekc
{
	v_dbl_general_header General_Header;
	v_dbl_config_header  Config_Header;
	char		     DEA_KEY_EXP_LG              [3];
	char 		     FS1_init_ekc	    	 [1];
	char		     DEA_KEY_EXP                 [216];
	char 		     FS2_init_ekc	    	 [1];
	char		     DEA_KEY_MOD_LG              [3];
	char 		     FS3_init_ekc	    	 [1];
	char		     DEA_KEY_MOD                 [216];
} v_dbl_init_ekc;


typedef struct t_dbl_interactive_response
{
	v_dbl_general_header General_Header;
	char		     Message_Sub_Class           [ 1];
	char 		     Display_Flag     	 	 [ 1];
	char		     Active_Keys		 [10];
	char 		     FS1_interactive_response	 [ 1];
	char		     Screen_Timer_Field		 [ 3];
	char 		     FS2_interactive_response	 [ 1];
	char		     Screen_Data_Field 		 [150];
} v_dbl_interactive_response;


typedef struct t_dbl_terminal_header
{
	char Message_Class              [1];
	char Message_Sub_Class          [1];
	char FS1_Terminal_Header	[1];
/*
	char Luno                       [9];
*/
	char Luno                       [3];
	char FS2_General_Header		[1];
	char FS3_General_Header         [1];
} v_dbl_terminal_header;


typedef struct t_dbl_term_buf
{
	v_dbl_terminal_header Terminal_Header;
/*
	char		      Time_variant_Number        [  8];
	char 		      FS1_Term_Buf     	 	 [  1];
*/
	char		      Top_Receipt		 [  1];
	char		      Message_Coordination	 [  1];
	char 		      FS2_Term_Buf     	 	 [  1];
	char		      Request			 [540];
} v_dbl_term_buf;

typedef  struct t_dbl_cheque_reply
{
        v_dbl_reply_header   Reply_Header;
        char                 Next_state_id_data          [3];
        char                 FS1_reply                   [1];
        char                 FS2_reply                   [1];
/*
        char                 Coinge_Amount_To_Dispence   [2];
        char                 FS3_reply                   [1];
*/
        char                 Transaction_Serial_Nbr      [4];
        char                 Function_Id                 [1];
        char                 Screen_Number               [3];
/*
        char                 Update_Screen_Number        [3];
        char                 Screen_Data                 [36];
*/
        char                 FS4_reply                   [1];
        char                 Message_Coordination_Nbr    [1];
        char                 Card_Return_Retain          [1];
        char                 Printer_Flag_Field          [1];
        char                 Printer_Data_Field          [2024];
/*        char                 FS5_reply                   [1];
        char                 Buffer_Identifier           [1];
        char                 Track3_Data                 [106];
*/
} v_dbl_cheque_reply;


/* ---------------------------------------------------------------------- */
/* - Supply Counters ---------------------------------------------------- */
#define	SS_SUPPLY_CTR			4

/* - Depository Supply Counters ----------------------------------------- */
typedef struct sDPSupplyCtrT {
    char	fDid		[4];
    char	fDeposit	[5];
} sDPSupplyCtr;

/* - Card Reader Supply Status ------------------------------------------ */
typedef struct sCRSupplyCtrT {
    char	fDid		[4];
    char	fRetained	[5];
} sCRSupplyCtr;

/* - Dipenser Sypply Counters ------------------------------------------- */
typedef struct sDISupplyCtrT {
    char	fDid		[4];
    char	fRS1		[1];
    char	fDivertNbr	[5];
    char	fDispensed1	[5];
    char	fDispensed2	[5];
    char	fDispensed3	[5];
    char	fDispensed4	[5];
    char	fDumpNbr	[5];
} sDISupplyCtr;


/* ---------------------------------------------------------------------- */
/* - Enhanced Supply Counters ------------------------------------------- */
#define	SS_ENH_SUPPLY_CTR		4

/* - Enhanced Depository Supply Counters -------------------------------- */
typedef struct sEnhDPSupplyCtrT {
    char	fDid		[4];
    char	fEnvelopeCnt	[5];
    char	fCheckBin1Cnt	[5];
    char	fCheckBin2Cnt	[5];
    char	fCheckBin3Cnt	[5];
    char	fCheckBin4Cnt	[5];
} sEnhDPSupplyCtr;

/* - Enhanced Card Reader Supply Status --------------------------------- */
typedef struct sEnhCRSupplyCtrT {
    char	fDid		[4];
    char	fRetained	[5];
} sEnhCRSupplyCtr;

/* - Enhanced Dispenser Supply Counters --------------------------------- */
typedef struct sEnhDISupplyCtrT {
    char	fDid		[4];
    char	fRS1		[1];
    char	fDenomination1	[1];
    char	fDenomination2	[1];
    char	fDenomination3	[1];
    char	fDenomination4	[1];
    char	fRS2		[1];
    char	fLoadedCnt1	[5];
    char	fLoadedCnt2	[5];
    char	fLoadedCnt3	[5];
    char	fLoadedCnt4	[5];
    char	fRS3		[1];
    char	fDispensed1	[5];
    char	fDispensed2	[5];
    char	fDispensed3	[5];
    char	fDispensed4	[5];
    char	fRS4		[1];
    char	fDivertCnt11	[5];
    char	fDivertCnt12	[5];
    char	fDivertCnt13	[5];
    char	fDivertCnt14	[5];
    char	fRS5		[1];
    char	fDivertCnt21	[5];
    char	fDivertCnt22	[5];
    char	fDivertCnt23	[5];
    char	fDivertCnt24	[5];
    char	fRS6		[1];
    char	fRetractCnt1	[5];
    char	fRetractCnt2	[5];
    char	fRetractCnt3	[5];
    char	fRetractCnt4	[5];
    char	fDumpNbr	[5];
} sEnhDISupplyCtr;

/* - Enhanced Coin Dispenser Supply Counters --------------------------- */
typedef struct sEnhCNSupplyCtrT {
    char	fDid		[4];
    char	fRS1		[1];
    char	fDenomination1	[1];
    char	fDenomination2	[1];
    char	fDenomination3	[1];
    char	fDenomination4	[1];
    char	fRS2		[1];
    char	fLoadedCnt1	[5];
    char	fLoadedCnt2	[5];
    char	fLoadedCnt3	[5];
    char	fLoadedCnt4	[5];
    char	fRS3		[1];
    char	fDispensed1	[5];
    char	fDispensed2	[5];
    char	fDispensed3	[5];
    char	fDispensed4	[5];
} sEnhCNSupplyCtr;

/* - Enhanced Supplimental Dispenser Supply Counters ------------------- */
typedef struct sEnhSDSupplyCtrT{
    char	fDid		[4];
    char	fRS1		[1];
} sEnhSDSupplyCtr;

/* --------------------------------------------------------------------- */
#define ID_LEN				3
#define	DEPOSITORY_ID			"DP01"
#define	CARD_READER_ID			"CR01"
#define	DISPENSER_ID			"DI01"
#define	COIN_DISPENSER_ID		"CN01"
#define	SUPPLEMENTAL_DISPENSER_ID	"SD01"


#define	LOCAL		22
#define	CONFRERE	23
#define	INTERNATIONAL	24

#endif


