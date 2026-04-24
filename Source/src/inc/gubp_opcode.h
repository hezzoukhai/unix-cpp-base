  /***********************************************************************
  ** nom	: ubp_opcode.h						**
  ** desc	: definition of constants specific to UBP		**
  **		  using Dubai bank Webservices			**
  **				(c) 1998 Hightech Payment Systems	**
  ***********************************************************************/



#ifndef HPS_GUBPOPCODE_H
#define HPS_GUBPOPCODE_H

/* - code operation demandee (OR) ---------------------------------- */
/* - Divers -------------------- */

extern int FCT_UBP_LOAD_ENQUIRY();
extern int FCT_UBP_LOAD_PAYMENT();


#define OR_GEN_UBP_ENQ			9000

/*
#define OR_GEN_UBP_ENQ_M1_U1_S1			9101 
#define OR_GEN_UBP_ENQ_M1_U1_S2			9102 
#define OR_GEN_UBP_ENQ_M1_U1_S3			9103 
#define OR_GEN_UBP_ENQ_M1_U1_S4			9104 
#define OR_GEN_UBP_ENQ_M1_U1_S5			9106 
#define OR_GEN_UBP_ENQ_M1_U1_S6			9107 
#define OR_GEN_UBP_ENQ_M1_U1_S7			9108 
#define OR_GEN_UBP_ENQ_M1_U1_S8			9109 

#define OR_GEN_UBP_ENQ_M1_U2_S1			9201 
#define OR_GEN_UBP_ENQ_M1_U2_S2			9202 
#define OR_GEN_UBP_ENQ_M1_U2_S3			9203 
#define OR_GEN_UBP_ENQ_M1_U2_S4			9204 
#define OR_GEN_UBP_ENQ_M1_U2_S5			9206 
#define OR_GEN_UBP_ENQ_M1_U2_S6			9207 
#define OR_GEN_UBP_ENQ_M1_U2_S7			9208 
#define OR_GEN_UBP_ENQ_M1_U2_S8			9209 

#define OR_GEN_UBP_ENQ_M1_U3_S1			9301 
#define OR_GEN_UBP_ENQ_M1_U3_S2			9302 
#define OR_GEN_UBP_ENQ_M1_U3_S3			9303 
#define OR_GEN_UBP_ENQ_M1_U3_S4			9304 
#define OR_GEN_UBP_ENQ_M1_U3_S5			9306 
#define OR_GEN_UBP_ENQ_M1_U3_S6			9307 
#define OR_GEN_UBP_ENQ_M1_U3_S7			9308 
#define OR_GEN_UBP_ENQ_M1_U3_S8			9309 

#define OR_GEN_UBP_ENQ_M1_U6_S1			9501 
#define OR_GEN_UBP_ENQ_M1_U6_S2			9502 
#define OR_GEN_UBP_ENQ_M1_U6_S3			9503 
#define OR_GEN_UBP_ENQ_M1_U6_S4			9504 
#define OR_GEN_UBP_ENQ_M1_U6_S5			9506 
#define OR_GEN_UBP_ENQ_M1_U6_S6			9507 
#define OR_GEN_UBP_ENQ_M1_U6_S7			9508 
#define OR_GEN_UBP_ENQ_M1_U6_S8			9509 


#define OR_GEN_UBP_ENQ_M1_U7_S1			9701 
#define OR_GEN_UBP_ENQ_M1_U7_S2			9702 
#define OR_GEN_UBP_ENQ_M1_U7_S3			9703 
#define OR_GEN_UBP_ENQ_M1_U7_S4			9704 
#define OR_GEN_UBP_ENQ_M1_U7_S5			9706 
#define OR_GEN_UBP_ENQ_M1_U7_S6			9707 
#define OR_GEN_UBP_ENQ_M1_U7_S7			9708 
#define OR_GEN_UBP_ENQ_M1_U7_S8			9709 

#define OR_GEN_UBP_ENQ_M1_U8_S1			9801 
#define OR_GEN_UBP_ENQ_M1_U8_S2			9802 
#define OR_GEN_UBP_ENQ_M1_U8_S3			9803 
#define OR_GEN_UBP_ENQ_M1_U8_S4			9804 
#define OR_GEN_UBP_ENQ_M1_U8_S5			9806 
#define OR_GEN_UBP_ENQ_M1_U8_S6			9807 
#define OR_GEN_UBP_ENQ_M1_U8_S7			9808 
#define OR_GEN_UBP_ENQ_M1_U8_S8			9809 

#define OR_GEN_UBP_ENQ_M1_U9_S1			9901 
#define OR_GEN_UBP_ENQ_M1_U9_S2			9902 
#define OR_GEN_UBP_ENQ_M1_U9_S3			9903 
#define OR_GEN_UBP_ENQ_M1_U9_S4			9904 
#define OR_GEN_UBP_ENQ_M1_U9_S5			9906 
#define OR_GEN_UBP_ENQ_M1_U9_S6			9907 
#define OR_GEN_UBP_ENQ_M1_U9_S7			9908 
#define OR_GEN_UBP_ENQ_M1_U9_S8			9909 
*/

#define OR_GEN_UBP_PAY_AMT			9611 
#define OR_GEN_UBP_PAY_SUMMARY			9612 
#define OR_GEN_UBP_PAYMENT			9613 




#define NP_UBP_TICKET_ENG 	    "850"
#define NP_UBP_DEC_TICKET_ENG	  "851"
#define NP_UBP_TICKET_AR 	  		"852"
#define NP_UBP_DEC_TICKET_AR 		"853"
#define NP_UBP_JOURNAL_ENG   		"854"
#define NP_UBP_TIMED_OUT_ENG    "855"

typedef struct SNdcGUbp
{
   char             gPnr        [256];
   char             gTran_Flag  [5];
   char             gStan 	[256];      
   char             gName       [256];
   char             gTerm_id    [22];
   char             gAccount    [256];
   char             gMobileNbr  [256];
   char             gAmount     [256];
   char             gTotAmount  [256];
   char             gFee     	[256];
   char             gError_id   [256];
   char             gTrans_ID   [256];
   char             gError_desc [256];
   char             gRecord_data[512];
   char             gOpcode   	[10];
   char             gCoord	[10];
   int              Account_flag;     
} TSNdcGUbp;

extern TSNdcGUbp  tab_NdcGUbp [  ];

#endif /** HPS_ATMOPCODE_H **/

