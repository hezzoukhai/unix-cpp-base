/* Id		Version	Auth	Date		Comment									*/
/* 0001MT	V.2.0.1	MT	04072002	Add constant ERR_ROLLBACK_SEGMENT					*/

#ifndef HPS_BATCH_RESPONSE_H
#define HPS_BATCH_RESPONSE_H

/*
Il ne faut pas depasser 4756 pour respecter les valeurs retournees par le proc*
*/

#define SUCCESSFULLY             4501
#define OLD_DATA_FILE            4515
#define DUPLICATE_DATA_FILE      4516
#define WRONG_FILE_SEQUENCE      4517
#define ABSENT_FILE_HEADER       4518
#define ERROR_READ_BUFFER 	 4519
#define EMPTY_FILE		 4590
#define ERROR_FILE_EXIST	 4591

#define ERROR_CONNECT            4520 
#define ERROR_GET_PARAMETERS     4521
#define ERROR_LENGTH_RECORD      4522
#define ERROR_TRUNCATE_TABLES    4523
#define ERROR_OPENING_FILE       4524
#define ERROR_INSERT_BUFFER      4525
#define ERROR_IN_MAIN_FUNCTION   4526
#define ERROR_CALL_MAIN_FUNCTION 4527
#define ERROR_UPDATE_PARAM       4528
#define ERROR_READ_EXTENSION 	 4529
#define ERROR_ARCHIVAGE          5608
#define ERROR_COMMIT 	 	 4530
#define ERROR_ROLLBACK 	 	 4531
#define ECCF_ERROR_INIT_VAR_HOST 4532
#define ECCF_ERROR_TRANS_CODE 	 4533
#define ECCF_ERROR_AFFECT_STRUCT 4534
#define ECCF_ERROR_AFFECT_VAR_HOST  4535
#define ERROR_DELETE_ECCF_OUTGOING  4536
#define ERROR_UPDATE_ECCF_OUTGOING  4537
#define ERROR_PROCESS_BUFFER 	4538
#define ERROR_FETCH_BANK      	4539
#define ERROR_FILE_TYPE       	4540
#define ERROR_SQL_LOADER 	4541
#define ERR_CREDIT_DAILY_CUTOFF 	4542
#define ERR_CREDIT_MONTHLY_CUTOFF 4543
#define ERR_ACCOUNT_ACTIVITY	  4544
#define ERROR_RBS_ON 		4999
#define NO_RECORD_TO_BE_PROC        5400

#define ERROR_INIT_VARIABLES        4545
#define ERROR_WRITING_FILE          4546
#define ERROR_ARCHIVAGE             5608
#define NO_DATA_FILE                5610 /*  BIS13042011*/


/* **  Start MOU 01/07/2011 : CB2A************************ */
#define ERROR_READ_FILE_HEADER      5900
#define ERROR_FORMAT_FILE_HEADER    5901
#define ERROR_READ_IPDU_DE          5902
#define ERROR_FORMAT_IPDU_DE        5903
#define ERROR_CONVERSION_MESSAGE    5904
#define ERROR_ID_REMETTANT          5912
#define ERROR_CONVERT_546           5906
#define ERROR_CONVERT_246           5907
#define ERROR_CONVERT_446           5908
#define ERROR_CONVERT_346           5909
#define ERROR_CONVERT_844           5910
#define ERROR_LENGTH_FILE           5911

/* **  END MOU 01/07/2011 : CB2A ************************* */

#define ERROR_FORMAT_INVALIDE       5817  /* MOU 03082011*/
#define ERROR_PARAM_REPRISE_FLAG    5913 /* MOU 25112011*/


/* START MBH17042012 */
#define ERR_TRUNCATE_TABLES_IPM_IN  5324
#define ERR_TRUNCATE_TABLES_B2_IN   4577
#define PARTIAL_SUCCESSFULLY        4511 /* AEO 02042014 */
#define UNSUCCESSFULLY              4599
/* END MBH17042012 */


/* 0001MT */
#define ERR_ROLLBACK_SEGMENT		8037
/* 0001MT */
/*ATH17072013 START */
#define FILE_NOT_FOUND        		7001
#define SQL_LOADER_FAILURE			7002
/*ATH17072013 END */
#define ERROR_PARAMETERS			49		/*	ATH30082013	*/
#define INVALID_FILE				56		/*	ATH20092013	*/

/*      ATH20092013     START*/
#define ERROR_GET_CERTIFICATE           7003
#define ERROR_ENCRYPT_FILE                      7004
/*      ATH20092013     END*/

/*START	MOU23072013 */
#define CONVERT_CB2A_FILE_ERROR		8059
#define ERROR_GET_REMITTANCE_AGENT  8060
/*END	MOU23072013 */

#endif /* HPS_BATCH_RESPONSE_H */
