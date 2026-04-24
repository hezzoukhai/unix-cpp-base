#ifndef HPS_SUPERVISE_PARAM_H
#define HPS_SUPERVISE_PARAM_H

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [ 32 +1];
extern char node_id[];	/*EBE140708*/

/* Resource Specific Parameters read from PCRD_SUPERVISE_PARAM table */
char  pris_verify_by_kill;
char  pris_verify_by_adm_msg;
char  pris_compare_internal_id;
int   pris_polling_time;
/*ICH20190428
int   ipc_polling_time;                                                                                                       
int   ufs_polling_time;                                                                                                       
int   dbts_polling_time;
*/ 

/*ICH20190428*/
#define		SU_PRIS_VERIFY_BY_KILL              "U01"
#define		SU_PRIS_COMPARE_INTERNAL_ID         "U02"
#define		SU_PRIS_COMPARE_ADM_MSG             "U03"
#define		SU_PRIS_POLLING_TIME                "U04"

/*MSA23022009 */
typedef struct SArgParamCmd
{
   int  nIndice;
   /*EBE140724: we are copying here script path and name, may be bigger than 64. See supervise_pris
   char sShl[64];*/
   char sShl[256];
}TSArgParamCmd;
/*END MSA23022009 */                                                                                                    


#endif /*** HPS_SUPERVISE_PARAM_H ***/
