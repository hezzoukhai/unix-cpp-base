#ifndef HPS_MAIN_THR_H
#define HPS_MAIN_THR_H

void * thread_wait_LINE(void * arg);
void * thread_wait_BAL (void * arg);
void * thread_SIG      (void * arg);
void * thread_CTL      (void * arg);
void * thread_ADMIN    (void * arg);
void * thread_TIMEOUT  (void * arg);
void * thread_SAF      (void * arg);
void * thread_PRIS     (void * arg);
void * thread_UFS      (void * arg);
void * thread_IPC      (void * arg);
void * thread_DBTS     (void * arg);
void * thread_adm      (void * arg);
void * thread_signin   (void * arg);
void * thread_STLMNT   (void * arg);
void * thread_proc_POS (void * arg);
void * thread_proc_NDC (void * arg);
void * thread_proc_POS_dyn (void * arg);
void * thread_RNL (void * arg);

/** Ajout G.F le 04/03/03 pour le traitement SMS Outgoing **/
void * thread_OUTG     (void * arg);
/** Fin   G.F le 04/03/03 pour le traitement SMS Outgoing **/
void * pollingThread (void * arg); /* Polling KS 20060915 */


void * thread_child_SIG       (void * arg);
void * thread_child_wait_BAL  (void * arg);
void * thread_child_ADMIN     (void * arg);
void * thread_child_CTL       (void * arg);
void * thread_child_wait_LINE (void * arg);
void * thread_CUTOFF   (void * arg);
void * thread_FRAUD   (void * arg);
void * thread_proc_LAUNCH (void * arg); /*MSA23022009 : Added for supervisor resource to launch the resources*/
void * thread_NOTIF   (void * arg); /**MKB/YAS Notification Module 22112011*/


#endif /* HPS_MAIN_THR_H */
