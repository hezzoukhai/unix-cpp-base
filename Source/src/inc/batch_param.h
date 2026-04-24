#ifndef PWC_BATCH_PARAM__H
#define PWC_BATCH_PARAM__H


typedef struct TBatchGetParam
{
		char 	batch_name_str    [60+1];
		int  	batch_name_len;

		char 	group_code_str    [6 + 1]; /* AZA10012018 */
		int  	group_code_len; /* AZA10012018 */

		char  	business_date_str	[8+1];
		int   	business_date_len;

		char	parameter_1_str	    [256+1];
		int 	parameter_1_len;

		char	parameter_2_str	    [256+1];
		int 	parameter_2_len;

		char	parameter_3_str	    [256+1];
		int 	parameter_3_len;

		char	parameter_4_str	    [256+1];
		int 	parameter_4_len;

		char	parameter_5_str	    [256+1];
		int 	parameter_5_len;

		char	parameter_6_str	    [256+1];
		int 	parameter_6_len;

		char	parameter_7_str	    [256+1];
		int 	parameter_7_len;

		char	parameter_8_str	    [256+1];
		int 	parameter_8_len;

		char	parameter_9_str	    [256+1];
		int 	parameter_9_len;

		char	parameter_10_str    [256+1];
		int 	parameter_10_len;

		char 	batch_id_str    [10+1];/**MBH20110512 **/
		int  	batch_id_len;/**MBH20110512 **/

} SBatchGetParam;

typedef struct TBatchUpDateParam
{
      char    batch_name_str    [60+1];
      int     batch_name_len;

	  char    group_code_str    [6 + 1]; /* AZA10012018 */
	  int     group_code_len; /* AZA10012018 */
	  
      char    status            [1+1];
      int     ending_status;

      char    user_message_str  [512+1];
      int     user_message_len;

	  	  /*ATH17072013 START */
	  char  	business_date_str	[8+1];
	  int   	business_date_len;
		
      char 	batch_id_str    [10+1];
	  int  	batch_id_len;
	  /*ATH17072013 END */
	  
      int     ora_err;          
      int     counter_1; 
      int     counter_2;
      int     counter_3;
      int     counter_4;
      int     counter_5;
      float   amount_1; 
      float   amount_2; 
      float   amount_3; 
      float   amount_4; 
      float   amount_5; 

}SBatchUpDateParam;

  /**MBH20110512 **/
typedef struct TBatchGetError
{

      char    batch_id_str    [10+1];
      int     batch_id_len;   
 
      char    parallel_job_number_str	    [4+1];
      int     parallel_job_number_len;

      char    code_error_pwc_str	    [9+1];
      int     code_error_pwc_len;

      char    code_error_ora_str	    [9+1];
      int     code_error_ora_len;
    

}SBatchGetError;
  /**MBH20110512 **/
  
 #endif
 
 