/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | mds_thr.h                                             |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme        | templat                                                   |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  Nom et Prenom              | Date de Creation  | 19/09/02 |
+-----------------+-----------------------------+-------------------+----------+
| Lst. mod.       |                                                            |
+-----------------+------------------------------------------------------------+
| Public          |                                                            |
| Objects         |                                                            |
| of the source   |                                                            |
|                 |                                                            |
+-----------------+------------------------------------------------------------+
| Dependances     |                                                            |
+-----------------+------------------------------------------------------------+
*/
#ifndef MDS_THR_H
    #define MDS_THR_H
#ifndef  HPS_THR_MDS_H
#define  HPS_THR_MDS_H
#include <pthread.h>
#include <define.h>
#include <mds_tlv.h>

typedef struct SMdsCtx
{
   int              free;
   int              status;
   pthread_t        tid;  
   int              fd;
   int              flag_bal;
   char             szBufferLine[ LG_MAX ];
   int              nLenLine;
   char             szBufferBal [ LG_MAX ];
   int              nLenBal;
   TSTlvMds        sTlvInfo;
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
} TSMdsCtx;

TSMdsCtx  tab_MdsCtx [ MAX_THREAD ];

void  InitTabMdsCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);

#endif /***  HPS_THR_MDS_H ***/

#endif /* MDS_THR_H */
