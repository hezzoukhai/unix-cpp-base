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
#ifndef MDS_PARAM_H
#define MDS_PARAM_H

#ifndef HPS_MDS_PARAM_H
#define HPS_MDS_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
/*#include <euro_param.h>*/
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
/*EBE140922: are now declared in resources.h
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char AcquirerBank 	[ 6 + 1 ];
*/

#include <resources.h>

/*
char     AcquirerId     [11 + 1];
char     ForwId    		[11 + 1];
char     IcaId    		[6 + 1];
char     CountryCode    [3 + 1];
char     AlphaCode2    	[2 + 1];
char     AlphaCode3    	[3 + 1];
char     BankLocation	[ 11 + 1 ];

char  ReversalGeneration [ 1 +1];
int   EchoTestTimer ;
int   MaxSendEchoTest ;
*/

extern char     AcquirerId      [];
extern char     ForwId    		[];
extern char     IcaId    		[];
extern char     CountryCode     [];
extern char     AlphaCode2    	[];
extern char     AlphaCode3    	[];
extern char     BankLocation	[];

extern char  ReversalGeneration [ ];
extern int   EchoTestTimer ;
extern int   MaxSendEchoTest ;

void MdsToIsoAcceptorName(int  nIndexCtx,char *MdsAcceptName,char *MdsPosData,char *MdsAcceptorAddr,char *IsoAcceptName);/*PLUTONL-3660*/ /* ELKAM20230606 AN6022 */

#define		MDS_PROCESSOR_ID            	"M01"
#define		MDS_ACQUIRER_ID             	"M02"
#define		MDS_ICA_ID                  	"M03"
#define		MDS_COUNTRY                 	"M04"
#define		MDS_ALPHA_CNTR_3            	"M05"
#define		MDS_ALPHA_CNTR_2            	"M06"
#define		MDS_BANK_LOCATION           	"M07"
/*int Step;*/ /* Les etapes de la telecollecte  0: Batch Header,
										    1: Detail,
											2: Trailler 
		  */

#endif /*** HPS_MDS_PARAM_H ***/

#endif /* MDS_PARAM_H */
