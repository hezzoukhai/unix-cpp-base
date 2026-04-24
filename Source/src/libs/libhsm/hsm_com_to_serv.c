#include <stdio.h>
#include <hsm_inc.h>
#include <sys/ipc.h>
#include <queue.h>



extern  void time_out_on_receive();


void    fct_timeout()
{
        trace_event("HSM TIME OUT IN RECEIVING MESSAGE", PROCESSING);
        return;
}

/*--------------------------------------------------------------------------*/
/* Fonction hsm_com_to_serv .                                               */
/* ----------------------------                                             */
/* Description : Cette fonction a pour but de delivrer une demande          */
/*--------------------------------------------------------------------------*/
int	hsm_com_to_serv(ressource_id,
			buffer_rcv, 
			offset, 
			buffer_out
		       )
char           *ressource_id;
char           *buffer_rcv; 
int            offset; 
char           *buffer_out; 
{
char     lPartner[2];
int             retour;
int             ipc_id_queue;
pid_t           HSM_ID;
unsigned short  command = 1;
int             length = 0;
char sLine [ MAX_LINE_TRC];

sprintf(sLine, "Start hsm_com_to_serv(%.2s)", ressource_id);
trace_event(sLine, PROCESSING);

ipc_id_queue  = get_ipc_id((key_t) BAL_MSG);


retour  =  deliver_message_hsm(ressource_id,
                           getpid(),
                           HSM_VERIF,
                           &HSM_ID,
                           buffer_rcv,
                           ipc_id_queue,
                           command,
                           offset);

if (retour == NOT_AVAILABLE) 
{
  trace_event("HSM INTERFACE NOT AVAILABLE", ERROR);
  trace_event("End   hsm_com_to_serv(HSM NOT AVAILABLE)", PROCESSING);
  return(MALFUNCTION);
}

if (retour != DELIVERED)
{
  trace_event("BUFFER NOT DELIVERED PROPERLY", ERROR);
  trace_event("End   hsm_com_to_serv(SYSTEM_MALFUNCTION)", PROCESSING);
  return(MALFUNCTION);
}

if (retour == DELIVERED)
{
  trace_event("MESSAGE DELIVERED", TRACE);

/*----------------------------------------------------------*/
/* dans le cas d'utilisation d'un timer pour le serveur iso */
/*----------------------------------------------------------*/
  alarm(DELAY_HSM);
memcpy(lPartner,HSM_VERIF,2);
  retour  =  receive_message_hsm(ressource_id,
			     getpid()    ,
			     getpid()    ,
			     buffer_out  ,
                 &length     ,
                 lPartner,
			     &HSM_ID     ,
                 ipc_id_queue,
			     LG_MAX      ,
			     &command
                 ,!IPC_NOWAIT 
			    );
/*----------------------------------------------------------*/
/* dans le cas d'utilisation d'un timer pour le serveur iso */
/*----------------------------------------------------------*/
  alarm(0);
/* Fin Modif. S. AFELLAY le 26-09-99 */ 
/*----------------------------------------------------------*/

if (retour != RECEIVED)
{
    trace_event("ERREUR IN RECEIVING BUFFER", ERROR);
    trace_event("End   hsm_com_to_serv(SYSTEM_MALFUNCTION)", PROCESSING);
    return(MALFUNCTION);
}
  
trace_event("HSM BUFFER OUT" , PROCESSING);
dump_buffer((unsigned char *)buffer_out, length      , 'A','L','I');

trace_event("End   hsm_com_to_serv(OK)",PROCESSING);
return(OK);
}

}
