/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | iso_cbsa.h                                                 |
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
#ifndef PWC_POS_ISO_CBSA__H
#define PWC_POS_ISO_CBSA__H


#include <define.h>
#include <iso_com.h>
#include <cbsa_fields.h>

#define LLVAR  -10
#define LLLVAR -20
/***** Types de Champs  ******/

#define CBSA_A          1
#define CBSA_N          2
#define CBSA_S          3
#define CBSA_AN         4
#define CBSA_NS         5
#define CBSA_ANS        6
#define CBSA_B          7
#define CBSA_Z          8

/**** Longueur de champs  ****/
#define CBSA_BITMAP_LEN           16
#define MAX_CBSA_FIELDS          128
#define MAX_CBSA_DATA_LEN       2048


typedef struct SCbsaMsgInfo
{
   int           nFieldPos    [ MAX_CBSA_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ CBSA_BITMAP_LEN   ];
   char          sData        [ MAX_CBSA_DATA_LEN ];

} TSCbsaMsgInfo;



void  InitCbsaInfo(TSCbsaMsgInfo *SpCbsaMsgInfo);

int   AnalyseCbsa    (char * buffer_rec, TSCbsaMsgInfo *SpCbsaMsgInfo);
int   GetCbsaField   (int    field_no,   TSCbsaMsgInfo *SpCbsaMsgInfo, 
                                                  char * data, int *length);
int   AddCbsaField   (int    field_no,   TSCbsaMsgInfo *SpCbsaMsgInfo, 
                                                  char * data, int length);
int   InsertCbsaField   (int    field_no,   TSCbsaMsgInfo *SpCbsaMsgInfo, 
                                                  char * data, int length);
int   PutCbsaField   (int    field_no,   TSCbsaMsgInfo *SpCbsaMsgInfo, 
                                                  char * data, int length);
int   CbsaBuildMsg (char * buffer_snd, TSCbsaMsgInfo *SpCbsaMsgInfo);

#endif



