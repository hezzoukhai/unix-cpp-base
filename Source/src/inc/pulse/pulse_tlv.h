/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | mds_tlv.h                                             |
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
#ifndef PULSE_TLV_H
#define PULSE_TLV_H

#ifndef HPS_TLV_PULSE_H
#define HPS_TLV_PULSE_H

#include <tlv_com.h>
#include <pulse/pulse_fieldstlv.h>

/**************************************/
#define   PULSE_TLV_FIX           0
#define   PULSE_TLV_VAR           1
/**************************************/
#define PULSE_TLV_CVM			2	
#define PULSE_TLV_CERT_SERIAL_NBR		3
#define PULSE_TLV_SECURITY_IND		4
#define PULSE_TLV_CVC2			5


/**************************************/

#define  MAX_PULSE_TLV       999
#define  MAX_PULSE_TLV_LEN  4098

typedef struct STlvPulse
{
   int  nPresent  [ MAX_PULSE_TLV ];
   int  nPosTlv   [ MAX_PULSE_TLV ];
   int  nLength;
   char sTlvData  [ MAX_PULSE_TLV_LEN ];
}
TSTlvPulse;

void InitTlvPulse(TSTlvPulse * tlvInfo);
int  AnalyseTlvPulse(char *buffer,TSTlvPulse *tlvInfo);
int  GetTlvPulse (char *tlv_name, TSTlvPulse *tlvInfo,char *data, int *length);
int  AddTlvPulse (char *tlv_name, TSTlvPulse *tlvInfo, char *data, int length);
int  PutTlvPulse (char *tlv_name, TSTlvPulse *tlvInfo, char *data, int length);
int  PulseBuildTlv (char * buffer_snd, TSTlvPulse *tlvInfo);
int  PutTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length);


#endif /** HPS_TLV_PULSE_H **/

#endif /* PULSE_TLV_H */
