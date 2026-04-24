
#ifndef HPS_FUNCTION_H
#define HPS_FUNCTION_H

#include  <gabd_struct_cr.h>


void * thread_proc_GABD(void * arg);
void DumpGabDCRRSReq(crretrait2_t * psCRRetrait2);
int ReverseRequest(	int nIndexCtx,char * szCardNbr,char	* szTransAmnt,char * szAuditNbr,char * szRefNbr);

#endif /*** HPS_FUNCTION_H ***/
