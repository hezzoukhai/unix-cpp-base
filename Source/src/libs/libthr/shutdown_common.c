#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <dump.h>

#include <iso_hps.h>
#include <define.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <struct_time.h>

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void FCT_SHUTDOWN_BAL(int nIndiceCtx)
{
   char sLine [ MAX_LINE_TRC ];

   sprintf ( sLine , "==> ShutDown of Queue Thread %lu In Progress ...", pthread_self());
   trace_event ( sLine , FATAL );
   DeconnectOracle(nIndiceCtx);
   FreeOracleCtx(nIndiceCtx);
   sprintf ( sLine , "==> ShutDown of Queue Thread %lu Done ...", pthread_self());
   trace_event ( sLine , FATAL );
   pthread_exit ( NULL );
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void FCT_SHUTDOWN_TIME(int nIndiceCtx)
{
   char sLine [ MAX_LINE_TRC ];

   sprintf ( sLine , "==> ShutDown of Time Thread %lu In Progress ...", pthread_self());
   trace_event ( sLine , FATAL );
   DeconnectOracle(nIndiceCtx);
   FreeOracleCtx(nIndiceCtx);
   sprintf ( sLine , "==> ShutDown of Time Thread %lu Done ...", pthread_self());
   trace_event ( sLine , FATAL );
   pthread_exit ( NULL );
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
int SendShutDownSignalsToBalThreads()
{

   TSIsoInfo  IsoInfo;
   char        FieldData[256];
   char        sBuffer[1024];
   char        sentRefNbr[13];
   int         i,nLength;
   TSBalExchange   sBalMsg;

   InitIsoInfo ( &IsoInfo );
   IsoInfo.nMsgType = 1700;

   current_gmt_date (FieldData);
   AddIsoField (ISO_XMIT_TIME, &IsoInfo, FieldData, 10);

   current_julian_date_qjjj (sentRefNbr);
   current_hour ( sentRefNbr + 4);
   memcpy( sentRefNbr + 6 , FieldData , 6 );
   AddIsoField (ISO_REFERENCE_NBR, &IsoInfo, sentRefNbr, 12);

   AddIsoField (ISO_NW_MNGMT_INFO, &IsoInfo, "002", 3);

   /* build message */
   dump_p7_struct (  &IsoInfo );

   nLength = IsoBuildMsg(sBuffer , &IsoInfo);
   sBalMsg.sHeader.nLen = nLength;
   memcpy ( sBalMsg.sBody , sBuffer , nLength );

   for ( i = 0; i < max_thread ; i++ )
   {
       PBalWaitEntry();
       PBalPutMsg   (&sBalMsg);
       PBalConsoInfo();
   }

   return ( OK );
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int SendShutDownSignalsToTimeThreads()
{

   TSTimeExchange   sTimeMsg;
   int 		    i;

   sTimeMsg.sens = 'S';
   for ( i = 0; i < max_time_out_thread_thread ; i++ )
   {
       PTimeWaitEntry();
       PTimePutMsg   (&sTimeMsg);
       PTimeConsoInfo();
   }
   return ( OK );
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FCT_SHUTDOWN_LINE(int nIndiceCtx)
{
   char sLine [ MAX_LINE_TRC ];

   sprintf ( sLine , "ShutDown of Thread %lu [%lu] In Progress ...", pthread_self(),p7_thread_id());
   trace_event ( sLine , FATAL );
   DeconnectOracle(nIndiceCtx);
   FreeOracleCtx(nIndiceCtx);
   sprintf ( sLine , "ShutDown of Thread %lu[%lu] Done ...", pthread_self(),p7_thread_id());
   trace_event ( sLine , FATAL );
   pthread_exit ( NULL );
}

