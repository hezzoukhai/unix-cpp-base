/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | pos_cbsa_thr.h                                             |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme       | shell                                                      |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  youssef KHALOUFI       | Date de Creation  | 30/09/2002   |
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
#ifndef PWC_POS_CBSA_THR__H
#define PWC_POS_CBSA_THR__H


#include <pthread.h>
#include <define.h>
#include <cbpr.h>

typedef struct SPos_cbsaCtx
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
   pthread_mutex_t  PosMutex;
   pthread_cond_t   PosCond ;
   TSCbprMessage    stCbprMessage;
} TSPos_cbsaCtx;

TSPos_cbsaCtx  tab_Pos_cbsaCtx [ MAX_THREAD ];

void  InitTabPos_cbsaCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);
#endif


