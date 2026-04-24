/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | gb_param.h                                           |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme        | template                                              |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  Nom et Prenom Adil Attari  | Date de Creation  | 19/09/02 |
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
#ifndef GB_PARAM_H
#define GB_PARAM_H
#ifndef HPS_GB_PARAM_H
#define HPS_GB_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <resources.h>





#define GB_ACQUIRER_ID "G01"
#define GB_COUNTRY "G02"
#define GB_BANK_LOCATION "G03"
#define GB_INC_REQ "G04"
#define GB_OUTG_RESP "G05"
#define GB_OUTG_REQ "G06"
#define GB_INC_RESP "G07"
#endif /*** HPS_GB_PARAM_H ***/

#endif /* GB_PARAM_H */
