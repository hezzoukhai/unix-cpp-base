#include <stdio.h>
#include <hsm_inc.h>

/****************************************************************************/
/* generate_cvv :  generation cvc1 & cvc2                     */
/****************************************************************************/
int generate_cvv(t_fabrication *f)
{
   int  retour;
   char exp[5];
   char BuffSnd[128],BuffRcv[128];
   char sHsmResultCode[5];
   char ligne[MAX_LINE_TRC];

   memset (  BuffSnd , 0 , sizeof ( BuffSnd ));
   memset (  BuffRcv , 0 , sizeof ( BuffRcv ));
   memset (  sHsmResultCode , 0 , 5 );

   command_CW(BuffSnd, f->cvka,f->cvkb,f->pan,f->expiry_date,f->service_code);

   retour = HsmQuery ( BuffSnd , BuffRcv );
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      return(HSM_MALFUNCTION);
   }

   retour = command_CX(BuffRcv,f->cvc1,sHsmResultCode);
   if (retour != OK)
   {
      trace_event("Error in command_CX",ERROR);
      return(HSM_MALFUNCTION);
   }

   memset(ligne,  0,      sizeof(ligne));
   sprintf(ligne, "Value CVC1 %.3s",f->cvc1);
   trace_event(ligne, TRACE);

   memset ( exp , 0  , 5 );
   memcpy ( exp , f->expiry_date   , 4 );
   memcpy ( f->expiry_date + 0 , exp + 2 , 2 );
   memcpy ( f->expiry_date + 2 , exp + 0 , 2 );

   command_CW ( BuffSnd , f->cvka,  f->cvkb , f->pan , f->expiry_date , "000");

   HsmQuery ( BuffSnd , BuffRcv );
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      return(HSM_MALFUNCTION);
   }

   retour = command_CX(BuffRcv,f->cvc2,sHsmResultCode);
   if (retour != OK)
   {
      trace_event("Error in command_CX",ERROR);
      return(CRYPTOGRAPHIC_ERROR);
   }

   memset(ligne,  0,      sizeof(ligne));
   sprintf(ligne, "Value CVC2 %.3s",f->cvc2);
   trace_event(ligne, TRACE);

   return(OK);
}

/****************************************************************************/
/* generate_pin :  generation pin                        */
/****************************************************************************/
int    generate_pin(t_fabrication *f)
{
   int retour;
   char BuffSnd[128],BuffRcv[128];
   char pin_lmk[PIN_LMK_LENGTH];
   char sHsmResultCode[5];

   memset (  sHsmResultCode , 0 , 5 );
   memset ( BuffSnd , 0 , sizeof ( BuffSnd ));
   memset ( BuffRcv , 0 , sizeof ( BuffRcv));

   command_EE(BuffSnd, f->pvka, f->pvv, f->pan);

   retour = HsmQuery(BuffSnd , BuffRcv);
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      trace_event("End pvv generation(PIN_VERIFICATION_NOT_POSSIBLE)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      trace_event("End   pvv generation(HSM_MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }


   retour = command_EF( BuffRcv, pin_lmk , sHsmResultCode );
   if (retour != OK)
   {
      trace_event(" Error when generating PIN  command EF !",TRACE);
      return(CRYPTOGRAPHIC_ERROR);
   }

   command_PE (  BuffSnd, f->pan , pin_lmk , 
       f->pin_form.field0 , 
       f->pin_form.field1 , 
       f->pin_form.field2 , 
       f->pin_form.field3 , 
       f->pin_form.field4 , 
       f->pin_form.field5 , 
       f->pin_form.field6 );

   retour = HsmQuery(BuffSnd , BuffRcv);
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      trace_event("End pvv generation(PIN_VERIFICATION_NOT_POSSIBLE)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      trace_event("End   pvv generation(MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }


   retour = command_PZ(BuffRcv,sHsmResultCode);
   if (retour != OK)
   {
      trace_event(" Error when printing PIN  command PZ !",TRACE);
      return(CRYPTOGRAPHIC_ERROR);
   }

   sleep(3);
   return(OK);

}

/****************************************************************************/
/* generate_pvv :  generation pin for pvv        */
/****************************************************************************/
int       generate_pin_for_pvv(t_fabrication *f, char *pin_lmk )
{
   int    retour;
   char BuffSnd[128],BuffRcv[128];
   char sHsmResultCode[5];

   memset (  sHsmResultCode , 0 , 5 );
   memset ( BuffSnd , 0 , sizeof ( BuffSnd ));
   memset ( BuffRcv , 0 , sizeof ( BuffRcv ));

   command_EE(BuffSnd, f->pvka, f->pvv, f->pan);

   retour = HsmQuery(BuffSnd , BuffRcv);
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      trace_event("End pvv generation(PIN_VERIFICATION_NOT_POSSIBLE)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      trace_event("End   pvv generation(MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   retour = command_EF(BuffRcv , pin_lmk , sHsmResultCode );
   if (retour != OK)
   {
      trace_event(" Error when generating PIN for PVV command EF !",TRACE);
      return(CRYPTOGRAPHIC_ERROR);
   }

   return(OK);
}

/****************************************************************************/
/* generate_pvv :  generation pvv                        */
/****************************************************************************/
int   generate_pvv( t_fabrication *f )
{
   int  retour, length;
   char BuffSnd[128],BuffRcv[128];
   char pin_lmk[PIN_LMK_LENGTH];
   char sHsmResultCode[5];

   memset (  sHsmResultCode , 0 , 5 );
   memset( BuffSnd , 0 , sizeof ( BuffSnd));
   memset( BuffRcv , 0 , sizeof ( BuffRcv));

   retour = generate_pin_for_pvv ( f, pin_lmk);
   if (retour != OK)
   {
      trace_event("ERROR IN HSM_PVV_GENERATE WHEN CALL GENERATE_PIN_FOR_PVV()",ERROR);
      return(retour);
   }

   command_DG(BuffSnd,f->pvka, f->pvkb, pin_lmk, f->pan, f->pvki);

   retour = HsmQuery(BuffSnd , BuffRcv);
   if (retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR);
      trace_event("End pvv generation(PIN_VERIFICATION_NOT_POSSIBLE)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   if (retour == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED", ERROR);
      trace_event("End   pvv generation(MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   retour = command_DH( BuffRcv, f->pvv , sHsmResultCode );
   if (retour != OK)
   {
      trace_event("End   pvv generation()",PROCESSING);
      return(CRYPTOGRAPHIC_ERROR);
   }

   trace_event("End   pvv generation(OK)",PROCESSING);
   return(OK);
}
