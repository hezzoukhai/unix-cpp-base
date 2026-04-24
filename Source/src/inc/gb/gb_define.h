#ifndef PWC_GB_DEFINE__H
#define PWC_GB_DEFINE__H

#define GB_F024_SIGNON "801"      /* Sign-on (BIN)						*/
#define GB_F024_SIGNOFF "802"     /* Sign-off (BIN)						*/
#define GB_F024_ECHO "803"        /* Echo test							*/
#define GB_F024_SIGNOFF_SAF "805" /* Sign-off (SAF)						*/

#define GB_F039_NW_REQ_ACCEPTED "800" /* Network management message accepted		*/
#define GB_F039_CONN_REFUSED "880"    /* Connection not accepted					*/
#define GB_F039_APPROVAL "000"        /* Approval									*/

#define GB_REQ_TYPE_SIGNOFFSAF "SIGNOFFSAF"
#define GB_REQ_TYPE_ZPKEXCHANGE "ZPKEXCHANGE"
#define GB_REQ_TYPE_SCREEN "SCREEN"
#define GB_REQ_TYPE_TAKEXCHANGE "TAKEXCHANGE"
#define GB_REQ_TYPE_SAF "SAF"

#define GB_F007_LEN 10
#define GB_F011_LEN 12
#define GB_F012_LEN 14
#define GB_F024_LEN 3
#define GB_F037_LEN 12
#define GB_F039_LEN 3

#define GB_F024_N_SIGNON_BIN 603
#define GB_F024_N_SIGNOFF_BIN 604
#define GB_F024_N_ECHO_TEST 609
#define GB_F024_N_SIGNON 801
#define GB_F024_N_SIGNOFF 802
#define GB_F024_N_ECHO 803
#define GB_F024_N_SIGNON_SAF 804
#define GB_F048_NB_FIELDS 19
#endif
