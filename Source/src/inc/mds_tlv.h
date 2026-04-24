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
#ifndef MDS_TLV_H
#define MDS_TLV_H

#ifndef HPS_TLV_MDS_H
#define HPS_TLV_MDS_H

#include <tlv_com.h>
#include <mds_fieldstlv.h>

/**************************************/
#define   MDS_TLV_FIX           0
#define   MDS_TLV_VAR           1
/**************************************/
#define MDS_TLV_CVM			2	
#define MDS_TLV_CERT_SERIAL_NBR		3
#define MDS_TLV_SECURITY_IND		4
#define MDS_TLV_CVC2			5


/**************************************/

#define  MAX_MDS_TLV       999
#define  MAX_MDS_TLV_LEN  4098

typedef struct STlvMds
{
   int  nPresent  [ MAX_MDS_TLV ];
   int  nPosTlv   [ MAX_MDS_TLV ];
   int  nLength;
   char sTlvData  [ MAX_MDS_TLV_LEN ];
}
TSTlvMds;

void InitTlvMds(TSTlvMds * tlvInfo);
int  AnalyseTlvMds(char *buffer,TSTlvMds *tlvInfo);
int  GetTlvMds (char *tlv_name, TSTlvMds *tlvInfo,char *data, int *length);
int  AddTlvMds (char *tlv_name, TSTlvMds *tlvInfo, char *data, int length);
int  PutTlvMds (char *tlv_name, TSTlvMds *tlvInfo, char *data, int length);
int  MdsBuildTlv (char * buffer_snd, TSTlvMds *tlvInfo);
int  PutTlvBuffer(char *tlv_name, char *tlvbuffer,char *data, int length);


#endif /** HPS_TLV_MDS_H **/

#endif /* MDS_TLV_H */
