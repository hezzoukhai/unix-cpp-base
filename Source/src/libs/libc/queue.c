#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <define.h>
#include <queue.h>
#include <dump.h>

extern long atol();

/*EBE140921: Define Bal ids*/
/******************************************************************************/
P7_DEF_SYM int     nBalMsg;
P7_DEF_SYM int     nBalAdm;
P7_DEF_SYM int     nBalHsm;
P7_DEF_SYM int     nBalShl;
P7_DEF_SYM int     nBalHsmVerf;
P7_DEF_SYM int     nBalHsmProd;
P7_DEF_SYM int	   nBalBridge;
P7_DEF_SYM int	   nBalFrd;
P7_DEF_SYM int	   nBalNtwRawMsg;

/*SKARROUMI PLUTONL-4900*/
P7_DEF_SYM int     nBalBrr;
/*SKARROUMI PLUTONL-4900*/
/******************************************************************************/

int OpenBalCom( key_t nBalName)
{
   int nBalId;

   if ( (nBalId = msgget( nBalName, IPC_CREAT|0666)) < 0)
   {
      fprintf(stderr,"Error Openning MSG Message Queue : %d\n", errno);
      return(NOK);
   }
   return(nBalId);
}

void EmptyQueue(int nBalId, long MsgType)
{
   TSBalMessage msg_buffer;
   /*EBE160318: Insure has an issue to understand this statement
   while(msgrcv( nBalId , &msg_buffer, LG_MAX, MsgType , IPC_NOWAIT) != -1);*/
   int result;
   while(( result = msgrcv( nBalId , &msg_buffer, LG_MAX, MsgType , IPC_NOWAIT)) != -1);
}

int ReadBalMsg(int nBalId, long MsgType, int mode, TSBalExchange *struct_bal)
{
   TSBalMessage msg_buffer;
   int retour;

  trace_event("Start ReadBalMsg()",TRACE);

  memset( (char *)&msg_buffer, 0, sizeof(TSBalMessage));

/*
   do
   {
      retour = msgrcv(nBalId, & msg_buffer, LG_MAX, MsgType, mode); 
   } while ((retour < 0)  && (errno == EINTR)  );
*/
      retour = msgrcv(nBalId, & msg_buffer, LG_MAX, MsgType, mode); 

   if (retour < 0)
   {
  	   trace_event("End   ReadBalMsg(NOK)",TRACE);
      fprintf(stderr,"Error Receiving Message : %d\n", errno);
      return(NOK);
   }
   memcpy ((char *)struct_bal, (char *) &(msg_buffer.sText), 
                                                sizeof(TSBalExchange));
   if ( (nBalId == nBalMsg) || (nBalId == nBalAdm) || (nBalId == nBalFrd) || (nBalId == nBalNtwRawMsg) )
      dump_buffer(struct_bal->sBody, struct_bal->sHeader.nLen, 'A', 'B', 'I');

   memset( (char *)&msg_buffer, 0, sizeof(TSBalMessage));
   trace_event("End   ReadBalMsg(OK)",TRACE);
   return(OK);
}

int WriteBalMsg(int nBalId, TSBalExchange *struct_bal)
{
   TSBalMessage msg_buffer;
   int 	nLength;
   int 	retour;
   char	ligne[MAX_LINE_TRC];
   char	sLine[MAX_LINE_TRC];


  sprintf(sLine,"Start WriteBalMsg()");
  trace_event(sLine,TRACE);
   
   nLength = sizeof(TSBalHeader) + struct_bal->sHeader.nLen;
   msg_buffer.lType = struct_bal->sHeader.nPartnerId;
   time( & (struct_bal->sHeader.nTimeStamp ));
   memcpy ((char *) &(msg_buffer.sText), (char *)struct_bal, sizeof(TSBalExchange));

   if ( (nBalId == nBalMsg) || (nBalId == nBalAdm) )
      dump_buffer(struct_bal->sBody, struct_bal->sHeader.nLen, 'A', 'B', 'O');

    trace_event("================",TRACE);
    sprintf(ligne,"Partner Id %d",struct_bal->sHeader.nPartnerId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Source  Id %d",struct_bal->sHeader.nSourceId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Type    Id %d",(int)msg_buffer.lType);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Command    %d",struct_bal->sHeader.nCommand);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Length     %d",struct_bal->sHeader.nLen);
    trace_event(ligne,TRACE);
    trace_event("================",TRACE);

   do
   {
      retour = msgsnd(nBalId, &msg_buffer, nLength, IPC_NOWAIT);
   } while ( (retour < 0) && (errno == EINTR));

   if (retour < 0)
   {
      sprintf(sLine,"Error writing Bal Msg : %d\n", errno);
      trace_event(sLine, FATAL);
  	  trace_event("WriteBalMsg(NOK)",TRACE);
      return(NOK);
   }

  memset( (char *)&msg_buffer, 0, sizeof(TSBalMessage));
  trace_event("WriteBalMsg(OK)",TRACE);
  return(OK);
}

int ReadBalHsm(int nBalId, long MsgType, int mode, TSHsmExchange *struct_bal)
{
   TSHsmMessage msg_buffer;
   int retour;

  trace_event("Start ReadBalHsm()", TRACE);

  memset((char *) &msg_buffer, 0, sizeof(TSHsmMessage));

  retour = msgrcv(nBalId, & msg_buffer, LG_MAX, MsgType, mode); 

   if (retour < 0)
   {
  	  trace_event("End   ReadBalHsm(NOK)",TRACE);
      fprintf(stderr,"Error Receiving Message : %d\n", errno);
      return(NOK);
   }
   memcpy ((char *)struct_bal, (char *) &(msg_buffer.sText), 
                                                sizeof(TSHsmExchange));

   memset((char *) &msg_buffer, 0, sizeof(TSHsmMessage));	/*AMER 20160727 PADSS Fix*/
   trace_event("End   ReadBalHsm(OK)",TRACE);
   return(OK);
}

int WriteBalHsm(int nBalId, TSHsmExchange *struct_bal)
{
   TSHsmMessage msg_buffer;
   int 	nLength;
   int 	retour;
   char	ligne[MAX_LINE_TRC];


  sprintf(ligne, "Start WriteBalHsm(nBalId=[%d])",nBalId);
  trace_event(ligne, TRACE);
   
   nLength = sizeof(TSHsmHeader) + struct_bal->sHeader.nLen;
   msg_buffer.lType = struct_bal->sHeader.nPartnerId;
   time( & (struct_bal->sHeader.nTimeStamp ));
   memcpy ((char *) &(msg_buffer.sText), (char *)struct_bal, 
                                                    sizeof(TSHsmExchange));

    trace_event("================",TRACE);
    sprintf(ligne,"Partner Id %d",struct_bal->sHeader.nPartnerId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Source  Id %d",struct_bal->sHeader.nSourceId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Type    Id %d",(int)msg_buffer.lType);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Command    %d",struct_bal->sHeader.nCommand);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Length     %d",struct_bal->sHeader.nLen);
    trace_event(ligne,TRACE);
    trace_event("================",TRACE);
   do
   {
      retour = msgsnd(nBalId, &msg_buffer, nLength, IPC_NOWAIT);
   } while ( (retour < 0) && (errno == EINTR));

   if (retour < 0)
   {
	  sprintf(ligne,"Enable to write in the HSM queue retour %d|errno %d",retour,errno);
	  trace_event(ligne,TRACE);
  	  trace_event("End   WriteBalHsm(NOK)",TRACE);
      return(NOK);
   }
   memset((char *) &msg_buffer, 0, sizeof(TSHsmMessage));       /*AMER 20160727 PADSS Fix*/
   trace_event("End   WriteBalHsm(OK)",TRACE);
   return(OK);
}


int ReadCmdMsg(int nBalId, long MsgType, int mode, TSBridgeExchange *struct_bal)
{
   TSBridgeMessage msg_buffer;
   char				ligne[MAX_LINE_TRC];
   int retour;

  trace_event("Start ReadCmdMsg()", PROCESSING);

  memset((char *) &msg_buffer, 0, sizeof(msg_buffer));

  retour = msgrcv(nBalId, & msg_buffer, LG_MAX, MsgType, mode); 

   if (retour < 0)
   {
  	  trace_event("End   ReadCmdMsg(NOK)",ERROR);
      fprintf(stderr,"Error Receiving Message : %d\n", errno);
      return(NOK);
   }
   memcpy ((char *)struct_bal, (char *) &(msg_buffer.sText), sizeof(TSBridgeExchange));

    trace_event("================",TRACE);
    sprintf(ligne,"Partner Id %d",struct_bal->sHeader.nPartnerId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Source  Id %d",struct_bal->sHeader.nSourceId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Type    Id %d",(int)msg_buffer.lType);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Command    %d",struct_bal->sHeader.nCommand);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Length     %d",struct_bal->sHeader.nLen);
    trace_event(ligne,TRACE);
    trace_event("================",TRACE);

   

   trace_event("End   ReadCmdMsg(OK)",PROCESSING);
   return(OK);
}

int WriteCmdMsg(int nBalId, TSBridgeExchange *struct_bal)
{
   TSBridgeMessage	msg_buffer;
   int 				nLength;
   int 				retour;
   char				ligne[MAX_LINE_TRC];


  sprintf(ligne, "Start WriteCmdMsg(nBalId=[%d])",nBalId);
  trace_event(ligne, PROCESSING);
   
   nLength = sizeof(TSBridgeHeader) + struct_bal->sHeader.nLen;
   msg_buffer.lType = struct_bal->sHeader.nPartnerId;
   /*time( & (struct_bal->sHeader.nTimeStamp ));*/
   memcpy ((char *) &(msg_buffer.sText), (char *)struct_bal, sizeof(TSBridgeExchange));

    trace_event("================",TRACE);
    sprintf(ligne,"Partner Id %d",struct_bal->sHeader.nPartnerId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Source  Id %d",struct_bal->sHeader.nSourceId);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Type    Id %d",(int)msg_buffer.lType);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Command    %d",struct_bal->sHeader.nCommand);
    trace_event(ligne,TRACE);
    sprintf(ligne,"Length     %d",struct_bal->sHeader.nLen);
    trace_event(ligne,TRACE);
    trace_event("================",TRACE);
   do
   {
      retour = msgsnd(nBalId, &msg_buffer, nLength, IPC_NOWAIT);
   } while ( (retour < 0) && (errno == EINTR));

   if (retour < 0)
   {
	  sprintf(ligne,"Enable to write in the CMD queue retour %d|errno %d",retour,errno);
	  trace_event(ligne,ERROR);
  	  trace_event("End   WriteCmdMsg(NOK)",ERROR);
      return(NOK);
   }

   trace_event("End   WriteCmdMsg(OK)",PROCESSING);
   return(OK);
}




