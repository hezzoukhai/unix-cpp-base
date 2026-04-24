#ifndef HPS_NDC_PROTOCOL_H
#define HPS_NDC_PROTOCOL_H

typedef struct t_ndc_general_header
{
	char Message_Class              [1];
	char Response_Flag              [1];
	char FS1_General_Header		[1];
	#ifdef NDC_MAC
	char Luno                       [9];
	#else
	char Luno                       [3];	
	#endif
	char FS2_General_Header		[1];
	char Message_Sequence_Number    [3];
	char FS3_General_Header         [1];
} v_ndc_general_header;


typedef struct t_ndc_config_header
{
	char Message_Subclass           [1];
	char Message_Identifier         [1];
	char FS1_Config_Header          [1];
} v_ndc_config_header;

/***************************************************/
/* Ajout de la structure par Bellamine Faycal 990614*/

typedef struct t_ndc_parameter
{
    v_ndc_general_header General_Header;
    v_ndc_config_header  Config_Header;
    char             Filler              [1024];
} v_ndc_parameter;

typedef struct t_ndc_reply_header
{
    char Message_Class              [1];
    char Response_Flag              [1];
    char FS1_Reply_Header       [1];
    #ifdef NDC_MAC
    char Luno                       [9];
    #else
    char Luno                       [3];	
    #endif
    char FS2_Reply_Header       [1];
    #ifdef NDC_MAC
    char Message_Sequence_Number    [3];
    #endif
    char FS3_Reply_Header           [1];
} v_ndc_reply_header;

typedef struct t_ndc_fits
{
        v_ndc_general_header General_Header;
        v_ndc_config_header  Config_Header;
        char                 Fits_Data           [2500];/* [1024];*/
} v_ndc_fits;

/******/

typedef struct t_ndc_state 
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     State_Data     	 [600];
} v_ndc_state;



typedef struct t_ndc_screen
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     Screen_Data     	 [600];
} v_ndc_screen;



typedef struct t_ndc_config 
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
        char 		     Camera_Control_Option       [1];
        char 		     Card_Reader_Error_Threshold [3];
        char 		     Reserved_j                  [3];
        char 		     Reserved_k                  [3]; 
	char 		     Track3_Write_Error_Threshold[3];
        char 		     Supply_Ready_Amount_Buffer  [3];
        char 		     Reserved_n                  [9];
	char 		     FS1_config		    	 [1];
	char 		     LUNO_config		 [3];
	char 		     FS2_config		    	 [1];
	char 		     Keyboard_Timer_Id           [2];
        char 		     Keyboard_Timer              [3];
        char 		     Time_Out_Screen_Id          [2];
        char 		     Time_Out_Screen             [3];
        char 		     Close_State_Timer_Id        [2];
        char 		     Close_State_Timer           [3];
        char 		     Comms_Timer_Id              [2];
        char 		     Comms_Timer                 [3];
        char 		     Insertion_Timer_Id          [2];
        char 		     Insertion_Timer             [3];
        char 		     Cash_Timer_Id               [2];
        char 		     Cash_Timer                  [3];
        char 		     Poll_Select_Timer_Id        [2];
        char 		     Poll_Select_Timer           [3];
        char 		     Present_Time_Out_Id         [2];
        char 		     Present_Time_Out            [3];
        char 		     Night_Safe_Deposit_Timer_Id [2];
        char 		     Night_Safe_Deposit_Timer    [3];
        char 		     Card_Removal_Time_Out_Id    [2];
        char 		     Card_Removal_Time_Out       [3];
	char 		     Additional_present_Timer_Id [2];
	char 		     Additional_present_Timer    [3];
	char 		     Bna_Cash_Accepte_Id 	 [2]; /**MKB050508** Cash Deposit**/
	char 		     Bna_Cash_Accepte   	 [3]; /**MKB050508** Cash Deposit**/
	char 		     Bna_Cash_Reject_Id 	 [2]; /**MKB050508** Cash Deposit**/
	char 		     Bna_Cash_Reject   	 	 [3]; /**MKB050508** Cash Deposit**/
	char 		     Document_Eject_Timer_Id     [2];
	char 		     Document_Eject_Timer        [3];
	char 		     Statement_Present_Timer_Id  [2];
	char 		     Statement_Present_Timer     [3];
	char 		     Door_Access_Open_Timer_Id   [2];
	char 		     Door_Access_Open_Timer      [3];
	char 		     Form_Insertion_Timer_Id     [2];
	char 		     Form_Insertion_Timer        [3];
	char 		     Vandal_Guard_Close_Timer_Id [2];
	char 		     Vandal_Guard_Close_Timer    [3];

} v_ndc_config;


typedef struct t_ndc_enhanced_config 
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
        char 		     Option_number               [2];
        char 		     Option_code                 [3];
        char 		     FS1_enhanced_config         [1];
        char 		     Timer_Number                [2]; 
	char 		     Number_Sec_Timer            [3];

} v_ndc_enhanced_config;


typedef struct t_ndc_fit
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     Fit_Data       	 	 [600];
} v_ndc_fit;


typedef struct t_ndc_config_id
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     Config_Id_Number		 [4];
} v_ndc_config_id;


typedef struct t_ndc_command
{
	v_ndc_general_header General_Header;
	char		     Command_Code		 [1];
	char		     Command_Modifier		 [1];
} v_ndc_command;



typedef struct t_ndc_diebold_info_load
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     Algorithm_Locator	 	 [3];
	char		     Offset_Index_Data	 	 [21];
} v_ndc_diebold_info_load;


typedef struct t_ndc_mac_selection
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
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
} v_ndc_mac_selection;



typedef struct t_ndc_date_time
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     Date_Time                   [10];
} v_ndc_date_time;


typedef struct t_ndc_encryption_key_load
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     New_Key_Data                [256];
} v_ndc_encryption_key_load;

/*Start DDA15052016 - RKL Management Integration by OBE18072016*/
typedef struct t_ndc_enh_encryption_key_load
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     New_Key_Data                [2048+1];
} v_ndc_enh_encryption_key_load;
/*End   DDA15052016 - RKL Management*/


typedef struct t_ndc_init_ekc
{
	v_ndc_general_header General_Header;
	v_ndc_config_header  Config_Header;
	char		     DEA_KEY_EXP_LG              [3];
	char 		     FS1_init_ekc	    	 [1];
	char		     DEA_KEY_EXP                 [216];
	char 		     FS2_init_ekc	    	 [1];
	char		     DEA_KEY_MOD_LG              [3];
	char 		     FS3_init_ekc	    	 [1];
	char		     DEA_KEY_MOD                 [216];
} v_ndc_init_ekc;


typedef struct t_ndc_reply
{

	v_ndc_reply_header   Reply_Header;
	v_ndc_general_header General_Header;/*FB*/
    char		     Next_state_id_data          [3];
	char 		     FS1_reply        	 	 [1];
	char		     Nbr_Type1_Notes             [2];
	char		     Nbr_Type2_Notes             [2];
	char		     Nbr_Type3_Notes             [2];
	char		     Nbr_Type4_Notes             [2];
	char 		     FS2_reply        	 	 [1];
	char		     Coinge_Amount_To_Dispence   [2];
	char 		     FS3_reply        	 	 [1];
	char		     Transaction_Serial_Nbr      [4];
	char		     Function_Id                 [1];
	char		     Screen_Number               [3];
	char 		     FS4_reply        	 	 [1];
	char		     Message_Coordination_Nbr 	 [1];
	char		     Card_Return_Retain		 [1];
	char 		     Printer_Flag_Field 	 [1];
	char		     Printer_Data_Field		 [500];
	char		     Buffer_Identifier           [1];
	char		     Track3_Data		 [106];
} v_ndc_reply;



typedef struct t_ndc_interactive_response
{
	v_ndc_general_header General_Header;
	char		     Message_Sub_Class           [ 1];
	char 		     Display_Flag     	 	 [ 1];
	char		     Active_Keys		 [10];
	char 		     FS1_interactive_response	 [ 1];
	char		     Screen_Timer_Field		 [ 3];
	char 		     FS2_interactive_response	 [ 1];
	char		     Screen_Data_Field 		 [80];
} v_ndc_interactive_response;


typedef struct t_ndc_terminal_header
{
	char Message_Class              [1];
	char Message_Sub_Class          [1];
	char FS1_Terminal_Header	[1];
	char Luno                       [3];
	char FS2_General_Header		[1];
	char FS3_General_Header         [1];
} v_ndc_terminal_header;


typedef struct t_ndc_term_buf
{
	v_ndc_terminal_header Terminal_Header;
	char		      Time_variant_Number        [  8];
	char 		      FS1_Term_Buf     	 	 [  1];
	char		      Top_Receipt		 [  1];
	char		      Message_Coordination	 [  1];
	char 		      FS2_Term_Buf     	 	 [  1];
	char		      Request			 [LG_MAX];  /*** MB230806 ***/
} v_ndc_term_buf;

/****FB 990615************/
#define LOCAL           22
#define CONFRERE        23
#define INTERNATIONAL   24

/*************************/



/* Function ID Functions Performed */
#define NDC_FN_ID_PRINT_IMMEDIATE 			'4' 		/* Print immediate												*/
#define NDC_FN_ID_SET_NEXT_STATE_PRINT		'5' 		/* Set next state and print										*/
#define NDC_FUN_ID_NIGHT_SAFE_DEP_PRINT		'6' 		/* Night safe deposit and print									*/
#define NDC_FUN_ID_EJ_CRD_DISP_PRINT		'A' 		/* Eject card and dispense and print (card before cash)			*/


/* Card Return/Retain Flag*/
#define NDC_CRRF_CARD_RETURN				'0' 		/* return card during Close state */
#define NDC_CRRF_CARD_RETAIN				'1' 		/* retain card during Close state */


/* Printer Flag. */

#define NDC_PRFLG_NO_PRINT					'0' 		/* Do not print								*/
#define NDC_PRFLG_PRINT_JRNL				'1' 		/* Print on journal printer only			*/
#define NDC_PRFLG_PRINT_RECEIPT				'2' 		/* Print on receipt printer only			*/
#define NDC_PRFLG_PRINT_JRNL_RECEIPT		'3' 		/* Print on receipt and journal printer		*/


#define NDC_CDM_RETURN_NOTES				'*'
#define NDC_CDM_DEP_NOTES_N_PRINT			'-'

#endif /*** HPS_NRC_PROTOCOL_H ***/
