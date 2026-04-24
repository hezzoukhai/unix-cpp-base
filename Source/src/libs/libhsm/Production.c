/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      Production.c                                                           */
/*                                                                             */
/* Description                                                                 */
/*      Production Tools Management                                            */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>
#include <dump.h>
#include <string.h>
#include <resources.h>    
#include <tlv_prod.h>
#include <string.h>
#include <prod_var.h>
#include <signal.h>

#define  DATA_DIRECTORY  getenv("DATA")

extern  int     errno;
extern  char    *getenv();
extern int atoi();



/******************************************************************************/
typedef struct s_prod_err
{
   char *error_code;
   char *error_text;
} t_prod_err;
/******************************************************************************/

t_prod_err prod_err[] = {
    {  PROD_HSM_PROD_SUCCES        , "Production Succes" },
    {  PROD_FLAG_BUF_MISS_ERR      , "Production Flags Buffer is Missing" },
    {  PROD_PAN_MISS_ERR           , "Cardholder Number (Pan) is Missing" },
    {  PROD_CVKA_MISS_ERR          , "CVKA  is Missing" },
    {  PROD_CVKB_MISS_ERR          , "CVKB is Missing" },
    {  PROD_SERVICE_CODE_MISS_ERR  , "Service Code Missing" },
    {  PROD_EXPIRY_DATE_MISS_ERR   , "Expiry Date is Missing" },
    {  PROD_HSM_PROD_UNAV_ERR      , "Production HSM Unavailable" },
    {  PROD_HSM_PROD_MALF_ERR      , "Production HSM Malfunction" },
    {  PROD_HSM_PROD_PROC_ERR      , "Production HSM Processing Error" },
    {  PROD_PVKA_MISS_ERR          , "PVKA is Missing" },
    {  PROD_PVKB_MISS_ERR          , "PVKB is Missing" },
    {  PROD_PVK_INDEX_MISS_ERR     , "PVK Index is Missing" }
};

/******************************************************************************/
/******************************************************************************/
char *GetProdErrorText( char *err_code ){
   int i;
   char ligne[MAX_LINE_TRC];

   for (i=0; i<(sizeof(prod_err)/sizeof(prod_err[0])); i++)
      if (!memcmp(prod_err[i].error_code, err_code, 4) )
         return ( prod_err[i].error_text );
   return ( "Unknown Reason" );
}



int ProductionErrorStatus ( int nIndiceCtx, char *sTlvBuffer , char *error_code )
{
    char error_text [ 30 ];
    char sLine      [ MAX_LINE_TRC ];

    PutProdTlvBuffer ( PROD_ERROR_CODE_TAG, sTlvBuffer, error_code , 4 );
    memset ( error_text , 0 , sizeof ( error_text ));
    strcpy ( error_text , (char *)GetProdErrorText( error_code ) );
    PutProdTlvBuffer ( PROD_ERROR_TEXT_TAG, sTlvBuffer, error_text   , strlen ( error_text ));

    sprintf ( sLine , "PRODUCTION <error_code>:<%.4s> <error_text>:<%s>", error_code , error_text );
    trace_event ( sLine , PROCESSING );

    return(OK);
}

/**************************************************************************************/
/* CVV1 Calculation Procedure                                                         */
/**************************************************************************************/
int Cvv1Calculation ( int nIndiceCtx ,char *sTlvBuffer )
{
 int nLength;
 char  ligne[MAX_LINE_TRC];
 char  sPan[ 22 + 1 ];
 char  sCVKA[ SIZE_KEY + 1 ],  sCVKB[ SIZE_KEY + 1 ];
 char  sCVV1[ 3 + 1 ];
 char  sHsmResultCode[ 4 + 1 ];

 char  sServiceCode[ 3 + 1 ];
 char  sExpiryDate [ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];

     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_CVKA_TAG ,sTlvBuffer, sCVKA ,&nLength) != SUCCES)
     {
         trace_event ( "CVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_CVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_CVKB_TAG ,sTlvBuffer, sCVKB ,&nLength) != SUCCES)
     {
         trace_event ( "CVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_CVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_SERVICE_CODE_TAG ,sTlvBuffer, sServiceCode  ,&nLength) != SUCCES)
     {
         trace_event ( "Service Code is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_SERVICE_CODE_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_EXPIRY_DATE_TAG ,sTlvBuffer, sExpiryDate  ,&nLength) != SUCCES)
     {
         trace_event ( "Expiry Date is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_EXPIRY_DATE_MISS_ERR ));
     }

	 if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }

      command_CW(BuffSnd, sCVKA , sCVKB , sPan ,sExpiryDate , sServiceCode);

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_CX(BuffRcv, sCVV1, sHsmResultCode) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      /*sprintf ( ligne , "sCVV1  : %.3s ", sCVV1 );
      trace_event ( ligne , PROCESSING );*/ /** HM - 09022010 - PA_DSS **/
     
      PutProdTlvBuffer ( PROD_CVV1_TAG, sTlvBuffer, sCVV1 , 3 );
      return ( OK );
}


/**************************************************************************************/
/* CVV2 Calculation Procedure                                                         */
/**************************************************************************************/
int Cvv2Calculation ( int nIndiceCtx ,char *sTlvBuffer )
{
 int nLength;
 char  sPan[ 22 + 1 ];
 char  sCVKA[ SIZE_KEY + 1 ],  sCVKB[ SIZE_KEY + 1 ];
 char  sCVV2[ 3 + 1 ];
 char  sHsmResultCode[ 4 + 1 ];

 char  sServiceCode[ 3 + 1 ];
 char  sExpiryDate [ 4 + 1 ],  sExpSwip[ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];


     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_CVKA_TAG ,sTlvBuffer, sCVKA ,&nLength) != SUCCES)
     {
         trace_event ( "CVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_CVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_CVKB_TAG ,sTlvBuffer, sCVKB ,&nLength) != SUCCES)
     {
         trace_event ( "CVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_CVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_EXPIRY_DATE_TAG ,sTlvBuffer, sExpiryDate  ,&nLength) != SUCCES)
     {
         trace_event ( "Expiry Date is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_EXPIRY_DATE_MISS_ERR ));
     }

     memcpy ( sExpSwip  + 0 , sExpiryDate + 2 , 2 );
     memcpy ( sExpSwip  + 2 , sExpiryDate + 0 , 2 );

	 if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }

      command_CW(BuffSnd, sCVKA , sCVKB , sPan ,sExpiryDate , "000");

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_CX(BuffRcv, sCVV2, sHsmResultCode) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      PutProdTlvBuffer ( PROD_CVV2_TAG, sTlvBuffer, sCVV2 , 3 );
      return ( OK );
}

/**************************************************************************************/
/* PVV  Calculation Procedure                                                         */
/**************************************************************************************/
int PvvCalculation ( int nIndiceCtx ,char *sTlvBuffer )
{
 int   nLength;
 char  sPan[ 22 + 1 ];
 char  sPVKA[ SIZE_KEY + 1 ],  sPVKB[ SIZE_KEY + 1 ];
 char  sHsmResultCode[ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];
 char  sPVKIndex[ 1 + 1 ];
 char  pin_lmk[ PIN_LMK_LENGTH + 1 ];
 char  sPVV[ 4 + 1 ];

     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_PVKA_TAG ,sTlvBuffer, sPVKA ,&nLength) != SUCCES)
     {
         trace_event ( "PVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVKB_TAG ,sTlvBuffer, sPVKB ,&nLength) != SUCCES)
     {
         trace_event ( "PVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVK_INDEX_TAG  ,sTlvBuffer, sPVKIndex ,&nLength) != SUCCES)
     {
         trace_event ( "PVK Index is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVK_INDEX_MISS_ERR ));
     }

      command_EE(BuffSnd, sPVKA, PIN_DATA_OFFSET, sPan);

	 if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_EF( BuffRcv, pin_lmk , sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      PutProdTlvBuffer ( PROD_PIN_LMK_TAG, sTlvBuffer, pin_lmk , PIN_LMK_LENGTH );
     
      command_DG ( BuffSnd, sPVKA, sPVKB, pin_lmk, sPan, sPVKIndex );

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

	 if ( command_DH( BuffRcv, sPVV , sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      trace_event ( "PVV Calculation Done" , PROCESSING );
      PutProdTlvBuffer ( PROD_PVV_TAG , sTlvBuffer, sPVV , 4 );
      return ( OK );
}

/**************************************************************************************/
/* PIN  Calculation Procedure                                                         */
/**************************************************************************************/
int PinCalculation ( int nIndiceCtx ,char *sTlvBuffer )
{
 int   nLength;
 char  sPan[ 22 + 1 ];
 char  sPVKA[ SIZE_KEY + 1 ],  sPVKB[ SIZE_KEY + 1 ];
 char  sHsmResultCode[ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];
 char  sPVKIndex[ 1 + 1 ];
 char  pin_lmk[ PIN_LMK_LENGTH + 1 ];

     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_PVKA_TAG ,sTlvBuffer, sPVKA ,&nLength) != SUCCES)
     {
         trace_event ( "PVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVKB_TAG ,sTlvBuffer, sPVKB ,&nLength) != SUCCES)
     {
         trace_event ( "PVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVK_INDEX_TAG  ,sTlvBuffer, sPVKIndex ,&nLength) != SUCCES)
     {
         trace_event ( "PVK Index is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVK_INDEX_MISS_ERR ));
     }

      command_EE(BuffSnd, sPVKA, PIN_DATA_OFFSET, sPan);

	 if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_EF( BuffRcv, pin_lmk , sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      PutProdTlvBuffer ( PROD_PIN_LMK_TAG, sTlvBuffer, pin_lmk , PIN_LMK_LENGTH );
      trace_event ( "PIN Calculation Done" , PROCESSING );
      return ( OK );
}


/**************************************************************************************/
/* PIN  Calculation and Print Procedure                                               */
/**************************************************************************************/
int PinPrint ( int nIndiceCtx ,char *sTlvBuffer )
{
 int   nLength;
 char  sPan[ 22 + 1 ];
 char  sPVKA[ SIZE_KEY + 1 ],  sPVKB[ SIZE_KEY + 1 ];
 char  sHsmResultCode[ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];
 char  sPVKIndex[ 1 + 1 ];
 char  pin_lmk[ PIN_LMK_LENGTH + 1 ];
 char sPrintField0 [ 24  + 1];
 char sPrintField1 [ 40  + 1];
 char sPrintField2 [ 30  + 1];
 char sPrintField3 [ 90  + 1];
 char sPrintField4 [ 70  + 1];
 char sPrintField5 [  7  + 1];
 char sPrintField6 [  7  + 1];

     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_PVKA_TAG ,sTlvBuffer, sPVKA ,&nLength) != SUCCES)
     {
         trace_event ( "PVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVKB_TAG ,sTlvBuffer, sPVKB ,&nLength) != SUCCES)
     {
         trace_event ( "PVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVK_INDEX_TAG  ,sTlvBuffer, sPVKIndex ,&nLength) != SUCCES)
     {
         trace_event ( "PVK Index is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVK_INDEX_MISS_ERR ));
     }

      command_EE(BuffSnd, sPVKA, PIN_DATA_OFFSET, sPan);

	 if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_EF( BuffRcv, pin_lmk , sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      PutProdTlvBuffer ( PROD_PIN_LMK_TAG, sTlvBuffer, pin_lmk , PIN_LMK_LENGTH );
     
     if( GetProdTlvBuffer( PROD_PRINT_FIELD_0_TAG ,sTlvBuffer, sPrintField0 ,&nLength) != SUCCES)
         memset ( sPrintField0 , 0 , sizeof ( sPrintField0 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_1_TAG ,sTlvBuffer, sPrintField1 ,&nLength) != SUCCES)
         memset ( sPrintField1 , 0 , sizeof ( sPrintField1 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_2_TAG ,sTlvBuffer, sPrintField2 ,&nLength) != SUCCES)
         memset ( sPrintField2 , 0 , sizeof ( sPrintField2 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_3_TAG ,sTlvBuffer, sPrintField3 ,&nLength) != SUCCES)
         memset ( sPrintField3 , 0 , sizeof ( sPrintField3 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_4_TAG ,sTlvBuffer, sPrintField4 ,&nLength) != SUCCES)
         memset ( sPrintField4 , 0 , sizeof ( sPrintField4 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_5_TAG ,sTlvBuffer, sPrintField5 ,&nLength) != SUCCES)
         memset ( sPrintField5 , 0 , sizeof ( sPrintField5 ));

     if( GetProdTlvBuffer( PROD_PRINT_FIELD_6_TAG ,sTlvBuffer, sPrintField6 ,&nLength) != SUCCES)
         memset ( sPrintField6 , 0 , sizeof ( sPrintField6 ));

     command_PE (  BuffSnd, sPan , pin_lmk ,
                   sPrintField0,
                   sPrintField1,
                   sPrintField2,
                   sPrintField3,
                   sPrintField4,
                   sPrintField5,
                   sPrintField6 );

     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

     if ( command_PZ(BuffRcv,sHsmResultCode) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

     sleep ( 3 );
     return ( OK );
}


/**************************************************************************************/
/* PRoduction  Procedure                                                              */
/**************************************************************************************/
int Production( int nIndiceCtx , char *sTlvBuffer )
{
 int    nLength;
 char   sProdFlagsBuffer [ PROD_FLAGS_LENGTH ];
 char   CVV1_PROD_FLAG;
 char   CVV2_PROD_FLAG;
 char    PVV_PROD_FLAG;
 char   PIN_CALC_FLAG;
 char   PIN_PRINT_FLAG;
 char   PVV_GENR_FLAG;
 char   OFFSET_GENR_FLAG;


  trace_event ( "Production() Start ", PROCESSING );
  memset  ( sProdFlagsBuffer , 0 , sizeof ( sProdFlagsBuffer ));

  PrintProdTlvBuffer ( sTlvBuffer );

     if( GetProdTlvBuffer( PROD_FLAGS_BUFFER_TAG ,sTlvBuffer, sProdFlagsBuffer ,&nLength) != SUCCES)
     {
         trace_event ( "Production Flags Buffer is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_FLAG_BUF_MISS_ERR ));
     }
    
   CVV1_PROD_FLAG = sProdFlagsBuffer [ CVV1_PROD_FLAG_POS ];
   CVV2_PROD_FLAG = sProdFlagsBuffer [ CVV2_PROD_FLAG_POS ];
    PVV_PROD_FLAG = sProdFlagsBuffer [  PVV_PROD_FLAG_POS ];
    PIN_CALC_FLAG = sProdFlagsBuffer [ PIN_CALC_FLAG_POS  ];
   PIN_PRINT_FLAG = sProdFlagsBuffer [ PIN_PRINT_FLAG_POS ];
    PVV_GENR_FLAG  = sProdFlagsBuffer    [ PVV_GENR_FLAG_POS    ];
    OFFSET_GENR_FLAG  = sProdFlagsBuffer [ OFFSET_GENR_FLAG_POS ];
  
  if ( CVV1_PROD_FLAG == TO_CALCULATE)
      Cvv1Calculation ( nIndiceCtx , sTlvBuffer );

  if ( CVV2_PROD_FLAG == TO_CALCULATE)
      Cvv2Calculation ( nIndiceCtx , sTlvBuffer );

  if ( PVV_PROD_FLAG == TO_CALCULATE)
      PvvCalculation ( nIndiceCtx , sTlvBuffer );

  if ( PIN_CALC_FLAG == TO_CALCULATE )
      PinCalculation ( nIndiceCtx , sTlvBuffer );
  
  if ( PIN_PRINT_FLAG == TO_CALCULATE )
      PinPrint ( nIndiceCtx , sTlvBuffer );

  if ( PVV_GENR_FLAG == TO_CALCULATE )
      ProdPinPvv ( nIndiceCtx , sTlvBuffer );

  if ( OFFSET_GENR_FLAG == TO_CALCULATE )
      ProdPinOffset ( nIndiceCtx , sTlvBuffer );

  trace_event ( "Production() End ", PROCESSING );
  return ( OK );
}


/**************************************************************************************/
/* PVV  Calculation Procedure                                                         */
/**************************************************************************************/
int ProdPinPvv ( int nIndiceCtx ,char *sTlvBuffer )
{
 int   nLength;
 char  sPan[ 22 + 1 ];
 char  sPVKA[ SIZE_KEY + 1 ],  sPVKB[ SIZE_KEY + 1 ];
 char  sHsmResultCode[ 4 + 1 ];
 pid_t  nPid;

 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];
 char  sPVKIndex[ 1 + 1 ];
 char  pin_lmk [ SIZE_KEY + 1 ]; 
 char  sPVV[ 4 + 1 ];

     trace_event ( " Start  ProdPinPvv()" , PROCESSING );
     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 

     if( GetProdTlvBuffer( PROD_PVKA_TAG ,sTlvBuffer, sPVKA ,&nLength) != SUCCES)
     {
         trace_event ( "PVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKA_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVKB_TAG ,sTlvBuffer, sPVKB ,&nLength) != SUCCES)
     {
         trace_event ( "PVKB is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKB_MISS_ERR ));
     }

     if( GetProdTlvBuffer( PROD_PVK_INDEX_TAG  ,sTlvBuffer, sPVKIndex ,&nLength) != SUCCES)
     {
         trace_event ( "PVK Index is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVK_INDEX_MISS_ERR ));
     }
     if( GetProdTlvBuffer( PROD_PIN_LMK_TAG  ,sTlvBuffer, pin_lmk ,&nLength) != SUCCES)
     {
         trace_event ( "Pin Block is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PIN_BLOCK_MISS_ERR ));
     }

     command_DG ( BuffSnd, sPVKA, sPVKB, pin_lmk, sPan, sPVKIndex );
     if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }
     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }

	 if ( command_DH( BuffRcv, sPVV , sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }

      PutProdTlvBuffer ( PROD_PVV_TAG , sTlvBuffer, sPVV , 4 );
      trace_event ( " End  ProdPinPvv()" , PROCESSING );
      return ( OK );
}

/************************************************************************
  **  nom       : ProdPinOffset                                     **
  **  desc      : Calcul de l'offset du PIN methode IBM                 **
  **  entree    : pvka_key-> Cl\351 pvka                                   **
  **              pPinLmk -> PIN Block sous Lmk                         **
  **              pCardNbr-> Num\351ro de carte                            **
  **  sortie    : Offset du PIN                                         **
  ************************************************************************/
int ProdPinOffset ( int nIndiceCtx ,char *sTlvBuffer )
{
 pid_t  nPid;
 char  sPan[ 22 + 1 ];
 char  sPVKA[ SIZE_KEY + 1 ];
 char  sHsmResultCode[ 4 + 1 ];
 unsigned char  BuffSnd   [LG_MAX];
 unsigned char  BuffRcv   [LG_MAX];
 char  sPVKIndex[ 1 + 1 ];
 char  pin_lmk[ SIZE_KEY + 1 ];
 char  sPinOffset[ 12 + 1 ];
 char  sPinLength[3];
 char  detail [MAX_LINE_TRC];
 int   nLength;

     trace_event("Start ProdPinOffset()",PROCESSING);

     memset (BuffSnd,    0,  sizeof(BuffSnd));
     memset (BuffRcv,    0,  sizeof(BuffRcv));
     memset (pin_lmk   , 0,  sizeof(pin_lmk));
     memset (sPinOffset, 0,  sizeof(sPinOffset));

     if( GetProdTlvBuffer( PROD_PAN_TAG ,sTlvBuffer, sPan ,&nLength) != SUCCES)
     {
         trace_event ( "Pan is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PAN_MISS_ERR ));
      } 
     if( GetProdTlvBuffer( PROD_PVKA_TAG ,sTlvBuffer, sPVKA ,&nLength) != SUCCES)
     {
         trace_event ( "PVKA is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PVKA_MISS_ERR ));
     }
     if( GetProdTlvBuffer( PROD_PIN_LMK_TAG  ,sTlvBuffer, pin_lmk ,&nLength) != SUCCES)
     {
         trace_event ( "Pin Block is Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PIN_BLOCK_MISS_ERR ));
     }
     if( GetProdTlvBuffer( PROD_PIN_LENGTH  ,sTlvBuffer, sPinLength ,&nLength) != SUCCES)
     {
         trace_event ( "Pin Length Missing", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_PIN_LENGTH_MISS_ERR ));
     }

     /* - Construction du tampon ------------------------------------------- */
     command_DE(BuffSnd, sPVKA, pin_lmk, sPan, sPinLength);

     /* - Appel HSM -------------------------------------------------------- */
     if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
     {
         trace_event ( "Resource HSM PROD Unavailable", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_UNAV_ERR ));
     }
     if ( HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv ) != OK )
     {
         trace_event ( "HSM Communication Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_MALF_ERR ));
     }
     if ( command_DF( BuffRcv, sPinOffset,sHsmResultCode ) != OK )
     {
         trace_event ( "HSM Processing Malfunction", ERROR );
         return ( ProductionErrorStatus ( nIndiceCtx , sTlvBuffer , PROD_HSM_PROD_PROC_ERR ));
     }
 	PutProdTlvBuffer ( PROD_OFFSET_TAG , sTlvBuffer, sPinOffset , strlen(sPinOffset) );
	trace_event("End   ProdPinOffset()", PROCESSING);

    return(OK);
}



