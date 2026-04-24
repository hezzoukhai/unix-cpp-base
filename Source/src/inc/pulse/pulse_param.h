/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | mds_param.h                                           |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme        | templat                                                   |
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
#ifndef PULSE_PARAM_H
#define PULSE_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <resources.h>

/*
extern char		ResourceId        	[ ];
extern char		ResourceName      	[ ];
extern char		ResourceStatus    	[ ];
extern char		ResourceInternalId	[ ];
extern char		AcquirerBank		[ ];*/
extern char		AcquirerId		[ ];
extern char		ForwId			[ ];
extern char		IcaId			[ ];
extern char		CountryCode 		[ ];
/*extern char		AlphaCode2		[ ];
extern char		AlphaCode3 		[ ];
extern char		BankLocation		[ ];
extern char		ReversalGeneration	[ ]; 
extern int 		EchoTestTimer ;*/
extern int 		MaxSendEchoTest ;
/*extern int		Step; */



#define		PULSE_PROCESSOR_ID            	"M01"
#define		PULSE_ACQUIRER_ID             	"M02"
#define		PULSE_ICA_ID                  	"M03"
#define		PULSE_COUNTRY                 	"M04"
#define		PULSE_ALPHA_CNTR_3            	"M05"
#define		PULSE_ALPHA_CNTR_2            	"M06"
#define		PULSE_BANK_LOCATION           	"M07"


#endif /* PULSE_PARAM_H */
