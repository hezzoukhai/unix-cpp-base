  /***********************************************************************
  **  nom	: msgtype.h						**
  **  desc	: definition des constantes type des messages		**
  **  auteur	: Youns OUADI (04 Nov 1998)				**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_MSGTYPE_H
#define HPS_MSGTYPE_H

#define AUTHO_REQ_MSG					"1100"
#define AUTHO_REP_MSG					"1110"
#define AUTHO_CNF_MSG					"1120"

#define TRANS_REQ_MSG					"1200"
#define TRANS_REP_MSG					"1210"
#define TRANS_CNF_MSG					"1220"

#define REV_REQ_MSG					"1420"
#define REV_REP_MSG					"1430"

#define ADMIN_REQ_MSG					"1604"
#define ADMIN_REP_MSG					"1614"
#define ADMIN_CNF_MSG					"1624"

/* ajout V1 DBLi & ISO_SNI */
#define TRANSACTION_MSG                                       "1200"
#define CHEQ_STAT_MSG                                         "1604"
/* ajout V1 DBL*/

#endif  /*** HPS_MSGTYPE_H ***/
