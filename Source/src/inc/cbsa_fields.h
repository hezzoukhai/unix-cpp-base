/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | cbsa_fields.h                                              |
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

#ifndef PWC_CBSA__H
#define PWC_CBSA__H


#define CBSA_BIT_MAP2				        0
#define CBSA_CARD_NUMBER			        1
#define	CBSA_PROCESSING_CODE			    2
#define CBSA_TRANSACTION_AMOUNT			    3
#define CBSA_TRANSMISSION_DATE_AND_TIME		6
#define CBSA_TRACE_AUDIT_NUMBER			   10
#define	CBSA_TRANSACTION_LOCAL_TIME		   11
#define	CBSA_TRANSACTION_LOCAL_DATE		   12
#define CBSA_END_EXPIRY_DATE			   13
#define CBSA_ACTIVITY_TYPE			       17
#define CBSA_TERMINAL_READ_CAPABILITY	   21
#define CBSA_POS_DATA				       24
#define CBSA_PIN_MAX_LENGHT			       25
#define CBSA_AVQUIRER_INSTITUTION_ID_CODE  31
#define CBSA_TRACKII				       34
#define CBSA_AUTHORIZATION_NUMBER		   37
#define	CBSA_RESPONSE_CODE			       38
#define CBSA_CARD_ACC_TERMINAL_ID		   40
#define CBSA_CARD_ACC_ID_CODE			   41
#define CBSA_ADDITIONAL_RESPONSE_CODE	   43
#define CBSA_ACCES_CODE				       47
#define CBSA_TRANSACTION_CURRENCY		   48
#define CBSA_PIN				           51
#define CBSA_SECURITY_DATA			       52

#endif



