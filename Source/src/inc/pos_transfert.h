/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | pos_transfert                                              |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme       |			                                                   |
+-----------------+---------------------------------+---------+----------------+
| Aplication      |		Powercard                   | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |		El-Hadj OUADIE		    | Date de Creation  |12/11/2002|
+-----------------+-----------------------------+-------------------+----------+
| Lst. mod.       |		El-Hadj  El-Hadj OUADIE              12/11/02 11:18:04    
+-----------------+------------------------------------------------------------+
| Public          |                                                            |
| Objects         |                                                            |
| of the source   |                                                            |
|                 |                                                            |
+-----------------+------------------------------------------------------------+
| Dependances     |                                                            |
+-----------------+------------------------------------------------------------+
*/
#ifndef PWC_POS_TRANS__H
#define PWC_POS_TRANS__H

#include<iso_hps.h> 
#include<resources.h>
#include<queue.h>


char	compute_LRC_1	(	char	*buffer,	int				length);
int		check_LRC		(	char 	*Message,	int   			Message_Length	);
int	    Send_Message(int nIndiceCtx,char *szmessage, int nMessagelength);
int		Receive_Message	(	int		nIndiceCtx,	SPos_cbsaMsg	*szmessage);

#endif



