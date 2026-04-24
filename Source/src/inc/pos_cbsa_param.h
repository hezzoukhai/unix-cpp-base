/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | pos_cbsa_param.h                                           |
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
#ifndef PWC_POS_CBSA_PARAM__H
#define PWC_POS_CBSA_PARAM__H


#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char AcquirerBank [ 6 + 1 ];

#endif



