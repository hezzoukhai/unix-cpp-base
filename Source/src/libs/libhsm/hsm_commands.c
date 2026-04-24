/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      hsm_commands.c                                                         */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Function of hsm commands                                            */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        The buffer constructed in the command CD was incorrect               */
/*        See HYJCC120502                                                      */
/*        H.YOUSFI   10/10/2003 -         V 2.0.2							   */
/*		  I.HASSANI  10/10/2003 _																	   */
/*        Ajout des commandes de gestion du MAC                               */
/*******************************************************************************/

#include <stdio.h>
#include <hsm_inc.h>

/******************************************************************************/
/* Fonction format_card_from_pan.                                             */
/* -------------------                                                        */
/* -----   Numero de Carte sans le dernier digit Luhn Key                     */
/******************************************************************************/
int format_card_from_pan ( char *card, 
						    char *pan 
						 )
{
   char V_card_number[13];

   memset(V_card_number,  0, sizeof(V_card_number));
   memset(V_card_number,'0',                    12);

   if (strlen(pan) <= 12)
      memcpy(V_card_number +  12 - strlen(pan) + 1, 
          pan, 
          strlen(pan) - 1);
   else
      memcpy(V_card_number, pan + (strlen(pan)-13), 12);

   memcpy( card , V_card_number , 12 );
   return( OK );
}

/******************************************************************************/
/* Tableau de descritions des reponses HSM                                    */
/* -------------------                                                        */
/******************************************************************************/
typedef struct s_response
{
   char *reponse;
   char *description;
} t_response;
/******************************************************************************/

t_response hsm_response[] = {
       { "00" , "Operation Successful.........." },    
       { "01" , "Verification Failure........." },    
       { "04" , "Invalid Key Type Code........" },    
       { "05" , "Invalid Key Length flag......" },    
       { "10" , "Source Key Parity Error......" },    
       { "11" , "Destination Key Parity Error." },    
       { "12" , "Contents Of User Storage Empty." },  
       { "13" , "Master Key Parity Error......" },  
       { "14" , "PIN encrypted under (02,03) Invalid." },  
       { "15" , "Invalid Input Data..........." },  
       { "16" , "Console Or Printer Not Ready." },  
       { "17" , "HSM Not In Authorized State.." },  
       { "18" , "Document Format Not Loaded..." },  
       { "19" , "Specified Diebold Table is Invalid."},
       { "20" , "PIN Block Does Not Contain Valid Values."},
       { "21" , "Invalid Indexd Value, or overflow condition."},  
       { "22" , "Invalid Account Number......." },  
       { "23" , "Invalid Pin Block Format Code."},  
       { "24" , "PIN is Fewer than 4 or more than 12"}, 
       { "25" , "Decimalization Table Error...." }, 
       { "30" , "Invalid Reference Number......" }, 
       { "31" , "Insufficient Solicitation Entries for Batch"},
       { "33" , "LMK Key Change Storage Corrupted."},  
       { "40" , "Invalid Firmware Checksum...."},  
       { "41" , "Internal Hardware/Software Error."}, 
       { "42" , "DES Failure ..................."}, 
       { "80" , "Data Length Error, Less or Greather than Expected"},  
       { "90" , "Data Parity Error In Request Message"}
};

/******************************************************************************/
/******************************************************************************/
int TextError (char *err_code, char *sMsgErr){
   int i;
   char ligne[MAX_LINE_TRC];

   for (i=0; i<(sizeof(hsm_response)/sizeof(hsm_response[0])); i++)
      if (!memcmp(hsm_response[i].reponse, err_code, 2) ){
         sprintf ( ligne , "=> %s <=", hsm_response [i].description );
         trace_event ( ligne , PROCESSING );
		 sprintf(sMsgErr,"%s",hsm_response [i].description ); 
		 return(OK);
      }
         sprintf ( sMsgErr , "=>UNKNOWN ERROR CODE %s <=", err_code);
		 return(OK);
}


/******************************************************************************/
/* Fonction command_AE.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre*/
/*               au HSM pour translater TPK encryptee sous LMK vers TMK       */
/******************************************************************************/
int    command_AE( char *buffer_transmit ,
				   char *tmk_lmk, 
				   char *tpk_lmk 
				  )
{

   trace_event("Debut command_AE()", PROCESSING);

   memcpy(buffer_transmit, "AE", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk)); 
   buffer_transmit +=strlen(tmk_lmk);
   memcpy(buffer_transmit, tpk_lmk, strlen(tpk_lmk));

   trace_event("Fin   command_AE()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_AF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TPK sous TMK ainsi que le code d'erreur.                 */
/* Entree      : Buffer recu du HSM                                           */
/******************************************************************************/
int    command_AF( char *buffer_received , 
				   char *tpk_tmk ,
				   char * err_code 
				 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_AF()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if ( memcmp(Command, "AF", 2) != 0 )  return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(tpk_tmk, buffer_received,  2 * SIZE_KEY);break;
		tpk_tmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(tpk_tmk, buffer_received,  3 * SIZE_KEY);break;
		tpk_tmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(tpk_tmk, buffer_received,  SIZE_KEY);break;
		tpk_tmk [SIZE_KEY] = '\0';
   }

   trace_event("End   command_AF(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_AG.                                                       	*/
/* -------------------                                                        	*/
/* Description : Cette fonction a pour but de preparer le buffer a transmettre	*/
/*               au HSM pour translater TAK encryptee sous LMK vers TMK         */
/******************************************************************************/
int    command_AG(	char *buffer_transmit,
				  	char *tmk_lmk,
				  	char *tak_lmk 
				  )
{

   trace_event("Debut command_AG()", PROCESSING);

   memcpy(buffer_transmit, "AG", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk)); 
   buffer_transmit +=strlen(tmk_lmk);
   memcpy(buffer_transmit, tak_lmk, strlen(tak_lmk));

   trace_event("Fin   command_AG()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_AH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TAK sous TMK ainsi que le code d'erreur.                 */
/******************************************************************************/
int    command_AH( char *buffer_received,
                   char *tak_tmk,
                   char *err_code 
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_AH()",PROCESSING);

   memset(Command , 0, 3);      

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "AH", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(tak_tmk, buffer_received,  2 * SIZE_KEY);break;
		tak_tmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(tak_tmk, buffer_received,  3 * SIZE_KEY);break;
		tak_tmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(tak_tmk, buffer_received,  SIZE_KEY);break;
		tak_tmk [SIZE_KEY] = '\0';
   }


   trace_event("End   command_AH(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_AS.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la paire CVK sous LMK                     */
/*******************************************************************************/
int    command_AS(char *buffer_transmit)
{
   trace_event("Debut command_AS()", PROCESSING);
   memcpy(buffer_transmit, "AS", 2);
   trace_event("Fin   command_AS()", PROCESSING);

   return(OK);
}

/******************************************************************************/
/* Fonction command_AT.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la paire CVK sous LMK, la Check_value ainsi que          */
/*               le code d'erreur.                                            */
/******************************************************************************/
int command_AT( char *buffer_received, 
				char *cvka_lmk, 
				char *cvkb_lmk, 
				char *check_valuea, 
                char *check_valueb , 
				char *err_code )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_AT()",PROCESSING);

   memset(Command,    0, 3);

   memset(cvka_lmk,    0, SIZE_KEY);
   memset(cvkb_lmk,    0, SIZE_KEY);
   memset(check_valuea,0, SIZE_KEY);
   memset(check_valuea,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "AT", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   memcpy(cvka_lmk     , buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(cvkb_lmk     , buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(check_valuea , buffer_received, SIZE_KEY); 
   buffer_received +=6;
   memcpy(check_valueb , buffer_received, SIZE_KEY);

   trace_event("End   command_AT()",PROCESSING);
   return(OK);
}

/*******************************************************************************/
/* Fonction command_AU.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater la paire CVK sous LMK vers une paire   */
/*               sous ZMK   						                           */
/*******************************************************************************/
int    command_AU( char *buffer_transmit, 
				   char *zmk_lmk, 
                   char *cvka_lmk, 
                   char *cvkb_lmk 
                  )
{
   trace_event("Debut command_AU()", PROCESSING);

   memcpy(buffer_transmit, "AU", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, cvka_lmk,SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, cvkb_lmk, SIZE_KEY);

   trace_event("Fin   command_AU()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_AV.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la paire CVK sous ZMK, la Check value de chaque cle de   */
/*               la pair et le code d'erreur.                                 */
/******************************************************************************/
int    command_AV ( char *buffer_received,
					char *cvka_zmk, 
				    char *cvkb_zmk,
					char *check_valuea,
				    char *check_valueb,
					char *err_code )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_AV()",PROCESSING);

   memset(cvka_zmk    ,    0, SIZE_KEY);
   memset(cvkb_zmk    ,    0, SIZE_KEY);
   memset(check_valuea,    0, SIZE_KEY);
   memset(check_valueb,    0, SIZE_KEY);

   memset(Command , 0, 3);      

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "AV", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   memcpy(cvka_zmk    , buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(cvkb_zmk    , buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(check_valuea, buffer_received, SIZE_KEY);
   buffer_received +=SIZE_KEY;
   memcpy(check_valueb, buffer_received, SIZE_KEY);

   trace_event("End   command_AV()",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_AW.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater la paire CVK sous ZMK vers une paire   */
/*               sous LMK                                                      */
/*******************************************************************************/
int    command_AW( char *buffer_transmit,
				   char *zmk_lmk, 
                   char *cvka_zmk, 
                   char *cvkb_zmk 
                  )
{
   trace_event("Start command_AW()", PROCESSING);

   memcpy(buffer_transmit, "AW", 2);            
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk , SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, cvka_zmk, SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, cvkb_zmk, SIZE_KEY);

   trace_event("End   command_AW()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_AX.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la paire CVK sous LMK, la Check value de chaque cle de   */
/*               la paire et le code d'erreur.                                */
/******************************************************************************/
int command_AX(	char *buffer_received, 
				char *cvka_lmk, 
                char *cvkb_lmk,
				char *check_valuea, 
                char *check_valueb,
				char *err_code
              )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_AX()",PROCESSING);

   memset(Command,    0, 3);

   memset(cvka_lmk,    0, SIZE_KEY);
   memset(cvkb_lmk,    0, SIZE_KEY);
   memset(check_valuea,0, SIZE_KEY);
   memset(check_valueb,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "AX", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   memcpy(cvka_lmk, buffer_received, SIZE_KEY);     
   buffer_received +=SIZE_KEY;
   memcpy(cvkb_lmk, buffer_received, SIZE_KEY);     
   buffer_received +=SIZE_KEY;
   memcpy(check_valuea, buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(check_valueb, buffer_received, SIZE_KEY);

   trace_event("End   command_AX()",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_CA.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de translater un PIN encrypté sous */
/*               une cle TPK vers une cle ZPK.                                */
/******************************************************************************/
int command_CA( char *buffer_transmit,
				char *Tpk,
                char *Zpk,
                char *PinTpk,
                char *format_in,
                char *format_out,
                char *Pan 
               )
{
   char Card[ 13];

   trace_event("Start command_CA()", PROCESSING);

   memset (  Card , 0 ,  13 );

   memcpy(buffer_transmit, "CA",       2);       
   buffer_transmit +=2;
   memcpy(buffer_transmit, Tpk, strlen(Tpk));   
   buffer_transmit +=strlen(Tpk);
 
   memcpy(buffer_transmit, Zpk, strlen(Zpk));   
   buffer_transmit +=strlen(Zpk);
   memcpy(buffer_transmit, "12",       2);   
   buffer_transmit +=2;
   memcpy(buffer_transmit, PinTpk,SIZE_KEY);   
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit, format_in, 2);   
   buffer_transmit +=2;
   memcpy(buffer_transmit, format_out, 2);   
   buffer_transmit +=2;

   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);

   trace_event("End   command_CA()", PROCESSING);
   return (OK);
}


/******************************************************************************/
/* Fonction command_CB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PIN translaté d'une*/
/*               cle TPK vers une cle ZPK.                                    */
/******************************************************************************/
int command_CB( char *buffer_received , 
			    char *PinZpk , 
                char *err_code 
              )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_CB()",PROCESSING);

   memset(PinZpk,     0, SIZE_KEY);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "CB", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 6;

   memcpy(PinZpk, buffer_received, SIZE_KEY);    
   buffer_received +=SIZE_KEY;

   trace_event("End   command_CB()",PROCESSING);
   return (OK);
}

/******************************************************************************/
/* Fonction command_CC.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre*/
/*               au HSM pour translater le Pin d'une ZPK vers une autre ZPK   */
/******************************************************************************/
int     command_CC( char *buffer_transmit,
				    char *zpk_in,
                    char *zpk_out,
                    char *pin,
                    char *format_in,
                    char *format_out,
                    char *pan
                  )
{
   char card[13];

   trace_event("Start command_CC()", PROCESSING);

   memset (  card , 0, 13 );
   memcpy(buffer_transmit, "CC"  ,        2);      
   buffer_transmit +=2;
   memcpy(buffer_transmit, zpk_in, strlen(zpk_in));      
   buffer_transmit +=strlen(zpk_in);
   memcpy(buffer_transmit, zpk_out, strlen(zpk_out));     
   buffer_transmit +=strlen(zpk_out);
   memcpy(buffer_transmit, "12", 2);               
   buffer_transmit +=2;
   memcpy(buffer_transmit, pin, SIZE_KEY);         
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit, format_in , 2);         
   buffer_transmit +=2;
   memcpy(buffer_transmit, format_out, 2);         
   buffer_transmit +=2;
   format_card_from_pan ( card , pan );
   memcpy(buffer_transmit, card ,12);

   trace_event("End   command_CC()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_CD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PIN translaté d'une*/
/*               cle ZPK vers une autre cle ZPK.                              */
/******************************************************************************/
int command_CD( char *buffer_received , 
				char *PinZpk , 
                char *err_code 
               )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_CD()",PROCESSING);

   memset(PinZpk,     0, SIZE_KEY);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "CD", 2) != 0) return(ERROR_COMMAND);
   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 6;

   memcpy(PinZpk, buffer_received, SIZE_KEY);

   trace_event("End   command_CD()",PROCESSING);
   return (OK);
}

/*******************************************************************************/
/* Fonction command_CW.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la CVV a encoder sur la carte             */
/*******************************************************************************/
int    command_CW( char *buffer_transmit, 
                   char *cvka_lmk, 
                   char *cvkb_lmk,
                   char *pan, 
                   char *expirat_date, 
                   char *serv_code
                 )
{
   char ligne[MAX_LINE_TRC];

   memset(ligne,  0,      sizeof(ligne));
   /*sprintf(ligne, "Start  command_CW(%s,%s,%s)",pan,expirat_date,serv_code);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start  command_CW()", PROCESSING);

   memcpy(buffer_transmit, "CW", 2);             
   buffer_transmit+=2;
   memcpy(buffer_transmit, cvka_lmk, SIZE_KEY);  
   buffer_transmit+= SIZE_KEY;
   memcpy(buffer_transmit, cvkb_lmk, SIZE_KEY);  
   buffer_transmit+= SIZE_KEY;
   memcpy(buffer_transmit, pan, strlen(pan));    
   buffer_transmit+= strlen(pan);
   memcpy(buffer_transmit, ";", 1);              
   buffer_transmit+= 1;
   memcpy(buffer_transmit, expirat_date, 4);     
   buffer_transmit+= 4;
   memcpy(buffer_transmit, serv_code, 3);

   trace_event("End   command_CW()",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_CX.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la CVV et le code erreur associe                         */
/******************************************************************************/
int    command_CX( char *buffer_received , 
                   char *cvv, 
				   char *err_code 
                  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_CX()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "CX", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) return(CHECK_ERROR );
   buffer_received += 4;

   memcpy(cvv, buffer_received, 3);

   trace_event("End   command_CX()",PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_CY                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a envoyer    */
/*               au HSM pour la verification du CVV.                          */
/******************************************************************************/
int command_CY ( char *buffer_transmit, 
				 char *card_number, 
				 char *service_code, 
				 char *expiry_date, 
				 char *cvk_A, 
				 char *cvk_B, 
				 char *cvv
               )
{
   char ligne[MAX_LINE_TRC];

   /*sprintf(ligne, "Start command_CY(%.22s,%.3s,%.4s,%.3s)",
       card_number, service_code, expiry_date, cvv );
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_CY()", PROCESSING);

   memcpy(buffer_transmit , "CY", 2);         
   buffer_transmit  += 2;
   memcpy(buffer_transmit , cvk_A, SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit , cvk_B, SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit , cvv  , 3);        
   buffer_transmit += 3;

   memcpy(buffer_transmit , card_number, strlen(card_number));
   buffer_transmit += strlen(card_number);

   memcpy(buffer_transmit ,    ";", 1);          
   buffer_transmit += 1;
   memcpy(buffer_transmit , expiry_date + 0, 2); 
   buffer_transmit += 2;
   memcpy(buffer_transmit , expiry_date + 2, 2); 
   buffer_transmit += 2;
   memcpy(buffer_transmit , service_code, 3);    
   buffer_transmit += 3;

   trace_event("End   command_CY(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_CZ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/*               verification CVV                                             */
/******************************************************************************/
int    command_CZ( char *buffer_received , 
                   char *err_code
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_CZ()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "CZ", 2) != 0)
   {
   		trace_event("End   command_CZ(ERROR_COMMAND)",PROCESSING);
	 	return(ERROR_COMMAND);
   }
   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2, "00", 2) == 0) 
   {
   		trace_event("End   command_CZ(OK)",PROCESSING);
	 	return(OK);
   }
   if (memcmp(err_code + 2, "01", 2) == 0) 
   {
   		trace_event("End   command_CZ(VERIFICATION_FAILED)",PROCESSING);
	 	return(VERIFICATION_FAILED);
   }
   trace_event("End   command_CZ(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);

   
}

/*******************************************************************************/
/* Fonction command_EC.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour verifier le PVV                                   */
/*******************************************************************************/
int   command_EC( char *buffer_transmit, 
                  char *zpk , 
                  char *pvka, 
                  char *pvkb, 
                  char *pin_block, 
                  char *format, 
                  char *pan,
                  char *pvv
				)
{
   char card[13];
   char pvki[2];

   trace_event("Start command_EC()", PROCESSING);

   memset ( card , 0, sizeof(card) );
   memset ( pvki , 0,  sizeof(pvki) );
   memcpy ( pvki , pvv, 1 );

   memcpy(buffer_transmit , "EC", 2);              
   buffer_transmit +=2;
   memcpy(buffer_transmit , zpk, strlen(zpk));        
   buffer_transmit +=strlen(zpk);
   memcpy(buffer_transmit , pvka, SIZE_KEY);      
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit , pvkb, SIZE_KEY);       
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit , pin_block, SIZE_KEY);  
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit , format, 2);            
   buffer_transmit +=2;
   format_card_from_pan ( card , pan );
   memcpy(buffer_transmit , card , 12 );     
   buffer_transmit += 12;
   memcpy(buffer_transmit , pvki, 1);        
   buffer_transmit +=1;
   memcpy(buffer_transmit , pvv+1 ,4);       
   buffer_transmit +=4;

   trace_event("Fin   command_EC(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_ED.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/*               resultat de verif PVV                                        */
/******************************************************************************/
int    command_ED( char *buffer_received, 
                   char *err_code
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_ED()",PROCESSING);
	
   memset(Command, 0, sizeof(Command));

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp( Command , "ED", 2) != 0) 
   {
   		trace_event("End   command_ED(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2, "00", 2) == 0)
   {
   		trace_event("End   command_ED(OK)",PROCESSING);
		return ( OK );
   }
   if (memcmp(err_code + 2, "01", 2) == 0) 
   {
   		trace_event("End   command_ED(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
   }
   trace_event("End   command_ED(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/* Fonction command_DC                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a envoyer    */
/*               au HSM pour la verification du PVV.                          */
/******************************************************************************/
int command_DC( char *buffer_transmit, 
                char *tpk_key, 
                char *pvka_key, 
                char *pvkb_key, 
                char *pin_block, 
                char *format, 
                char *pan, 
                char *pvv
               )
{
   char pvki[2];


   trace_event("Start command_DC()", PROCESSING);

   memset ( pvki , 0, 2 );
   memcpy ( pvki , pvv, 1 );
   memcpy( buffer_transmit ,"DC",   2);        
   buffer_transmit += 2;
   memcpy( buffer_transmit ,tpk_key,   strlen(tpk_key)); 
   buffer_transmit += strlen(tpk_key);
   memcpy( buffer_transmit ,pvka_key,   SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy( buffer_transmit ,pvkb_key,   SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy( buffer_transmit ,pin_block,   SIZE_KEY); 
   buffer_transmit += SIZE_KEY;
   memcpy( buffer_transmit ,format,   2);        
   buffer_transmit += 2;
   memcpy( buffer_transmit ,pan + strlen(pan) - 13 ,       12);        
   buffer_transmit += 12;
   memcpy ( buffer_transmit,pvki ,      1);        
   buffer_transmit += 1;
   memcpy( buffer_transmit ,pvv+1,      4);        
   buffer_transmit += 4;

   trace_event("End   command_DC()", PROCESSING);
   return (OK);
}

/******************************************************************************/
/* Fonction command_DD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/*               verification PVV                                             */
/******************************************************************************/
int    command_DD( char *buffer_received , 
                   char *err_code 
                  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event( "Start command_DD()",PROCESSING);

   memset(Command, 0, sizeof(Command));

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "DD", 2) != 0) 
   {
   		trace_event( "End   command_DD(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2, "00", 2) == 0) 
   {
   		trace_event( "End   command_DD(OK)",PROCESSING);
		return ( OK );
   }
   if (memcmp(err_code + 2, "01", 2) == 0)
   {
   		trace_event( "End   command_DD(VERIFICATION_FAILED)",PROCESSING);
		 return ( VERIFICATION_FAILED );
   }
   trace_event( "End   command_DD(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);
}



/******************************************************************************/
/* Fonction command_DE                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de generer un IBM PIN offset       */
/******************************************************************************/

int command_DE( char *buffer_transmit, 
                char *Pvka, 
                char *PinLmk, 
                char *Pan
              )
{

   char Card [ 13];

   trace_event("Start command_DE()", PROCESSING);

   memset (  Card , 0, 13 );
   memcpy( buffer_transmit , "DE",   2);   
   buffer_transmit += 2;
   memcpy( buffer_transmit , Pvka, SIZE_KEY);   
   buffer_transmit += SIZE_KEY;
   memcpy( buffer_transmit , PinLmk, SIZE_KEY);   
   buffer_transmit += SIZE_KEY;
   memcpy( buffer_transmit , "04", 2);      
   buffer_transmit += 2;

   format_card_from_pan ( Card , Pan );
   memcpy( buffer_transmit , Card ,12);           
   buffer_transmit += 12;
   memcpy( buffer_transmit , Validation_data, SIZE_KEY);   
   buffer_transmit += SIZE_KEY;

   trace_event("End   command_DE(OK)", PROCESSING);
   return (OK);
}


/******************************************************************************/
/* Fonction command_DF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM l'offset.                                                  */
/******************************************************************************/
int    command_DF( char *buffer_received , 
                   char *offset , 
                   char *err_code 
                 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_DF()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if ( memcmp ( Command, "DF", 2 ) != 0) 
   {
   		trace_event("End   command_DF(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_DF(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy ( offset, buffer_received, 12 );
   trace_event("End   command_DF(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_DG.                                                       */
/* -------------------                                                        */
/* Description :  formatter un buffer pour demande de generation PVV          */
/******************************************************************************/
int    command_DG( char *buffer_transmit,
                   char *pvka,
                   char *pvkb, 
                   char *pin, 
                   char *Pan, 
                   char *pvki
				  )
{
   char Card[13];
   memset (  Card , 0, 13 );

   trace_event("Start   command_DG()", PROCESSING);

   memcpy(buffer_transmit, "DG", 2);             
   buffer_transmit +=2;
   memcpy(buffer_transmit, pvka, SIZE_KEY);      
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit, pvkb, SIZE_KEY);      
   buffer_transmit +=SIZE_KEY;
   memcpy(buffer_transmit, pin, PIN_LMK_LENGTH );
   buffer_transmit += PIN_LMK_LENGTH;
   format_card_from_pan ( Card , Pan );
   memcpy( buffer_transmit , Card ,12);           
   buffer_transmit += 12;
   memcpy(buffer_transmit, pvki, 1);             
   buffer_transmit +=1;

   trace_event("End    command_DG()", PROCESSING);
   return(OK);
}



/******************************************************************************/
/* Fonction command_DH.                                                       */
/* -------------------                                                        */
/*                     recuperer le PVV                                       */
/******************************************************************************/
int    command_DH(buffer_received,pvv,err_code)
char   *buffer_received;
char   *pvv;
char   *err_code;
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_DH()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "DH", 2) != 0)
   {
   		trace_event("End   command_DH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_DH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy( pvv, buffer_received, 4);

   trace_event("End   command_DH(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_EA                                                         */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de verifier un PIN methode IBM     */
/******************************************************************************/
int command_EA( char *buffer_transmit,
                char *Zpk,
                char *Pvka,
                char *PinZpk,
                char *format,
                char *Pan,
                char *offset
              )
{

   char Card[13];

   trace_event("Start command_EA()" , PROCESSING);

   memset ( Card , 0, 13 );
   memcpy( buffer_transmit, "EA",   2);          
   buffer_transmit +=2;
   memcpy( buffer_transmit, Zpk, strlen(Zpk));     
   buffer_transmit +=strlen(Zpk);
   memcpy( buffer_transmit, Pvka, SIZE_KEY);     
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, "12", 2);        
   buffer_transmit +=2;
   memcpy( buffer_transmit, PinZpk, SIZE_KEY);     
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, format, 2);     
   buffer_transmit +=2;
   memcpy( buffer_transmit, "04", 2);        
   buffer_transmit +=2;

   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);             
   buffer_transmit += 12;

   memcpy( buffer_transmit, Validation_data, SIZE_KEY);
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, Pan, 10);        
   buffer_transmit +=10;
   memcpy( buffer_transmit, "N", 1);        
   buffer_transmit +=1;
   memcpy( buffer_transmit, Pan+(strlen(Pan)-1),1);
   buffer_transmit +=1;
   memcpy( buffer_transmit, offset, 4);     
   buffer_transmit +=4;
   memcpy( buffer_transmit, "FFFFFFFF", 8);     
   buffer_transmit +=8;

   trace_event("Start command_EA(OK)" , PROCESSING);
   return (OK);
}
/******************************************************************************/
/* Fonction command_EB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/*               verification du PIN par la methode IBM                       */
/******************************************************************************/
int    command_EB( char *buffer_received , 
                   char *err_code 
                  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_EB()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "EB", 2) != 0) 
   {
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2, "00", 2) == 0)
   {
         trace_event("End   command_EB(OK)",PROCESSING);
		 return ( OK );
   }
   if (memcmp(err_code + 2, "01", 2) == 0) 
   {
         trace_event("End   command_EB(VERIFICATION_FAILED)",PROCESSING);
		 return ( VERIFICATION_FAILED );
   }
   trace_event("End   command_EB(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);
}

/*******************************************************************************/
/* Fonction command_EE.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer le PIN                                    */
/*******************************************************************************/
int    command_EE( char *buffer_transmit, 
                   char *pvk_lmk, 
                   char *pvv, 
                   char *pan
                  )
{
   char Card[13];
   memset ( Card , 0, 13 );

   trace_event("Start command_EE()",PROCESSING);

   memcpy(buffer_transmit, "EE", 2);                
   buffer_transmit +=2;
   memcpy(buffer_transmit, pvk_lmk, SIZE_KEY);      
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, pvv, 4);                 
   buffer_transmit += 4;
   memcpy(buffer_transmit, "FFFFFFFF04", 10);       
   buffer_transmit += 10;

   format_card_from_pan ( Card , pan );
   memcpy(buffer_transmit, Card ,12);              
   buffer_transmit += 12;

   memcpy(buffer_transmit, Validation_data , SIZE_KEY);
   buffer_transmit += SIZE_KEY;
   memcpy(buffer_transmit, pan, 10);                
   buffer_transmit += 10;
   memcpy(buffer_transmit, "N", 1);                 
   buffer_transmit += 1;
   memcpy(buffer_transmit, pan+(strlen(pan)-1), 1); 
   buffer_transmit += 1;

   trace_event("End   command_EE(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_EF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le PIN et le code erreur associe               */
/******************************************************************************/
int    command_EF( char *buffer_received , 
                   char *pin , 
                   char *err_code
                  )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_EF()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "EF", 2) != 0) 
   {
   		trace_event("End   command_EF(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_EF(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memset(pin      ,0,            PIN_LMK_LENGTH);
   memcpy(pin      ,buffer_received,       PIN_LMK_LENGTH);

   trace_event("End   command_EF(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_PE.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour imprimer le PIN                                   */
/*******************************************************************************/
int    command_PE( char *buffer_transmit, 
                   char *pan, 
                   char *pin , 
                   char *field0, 
                   char *field1 , 
                   char *field2, 
                   char *field3 , 
                   char *field4, 
                   char *field5 , 
                   char *field6 
				 )
{
   char card[13];
   memset( card , 0, 13 );

   trace_event("Start command_PE()",PROCESSING);

   memcpy(buffer_transmit, "PE", 2);            
   buffer_transmit +=2;
   memcpy(buffer_transmit, "C", 1);             
   buffer_transmit +=1;
   format_card_from_pan ( card , pan );
   memcpy(buffer_transmit, card ,12);          
   buffer_transmit +=12;
   memcpy(buffer_transmit, pin, PIN_LMK_LENGTH);
   buffer_transmit +=PIN_LMK_LENGTH;

   memcpy(buffer_transmit,field0,strlen(field0));
   buffer_transmit += strlen(field0);

   memcpy(buffer_transmit, ";", 1);                    
   buffer_transmit += 1;

   memcpy(buffer_transmit,field1,strlen(field1));
   buffer_transmit += strlen(field1);

   memcpy(buffer_transmit, ";", 1);                    
   buffer_transmit += 1;

   memcpy(buffer_transmit, field2, strlen(field2));
   buffer_transmit += strlen(field2);

   memcpy(buffer_transmit, ";", 1);                    
   buffer_transmit += 1;

   memcpy(buffer_transmit, field3, strlen(field3));
   buffer_transmit += strlen(field3);

   memcpy(buffer_transmit, ";", 1);                     
   buffer_transmit += 1;

   memcpy(buffer_transmit, field4, strlen(field4));
   buffer_transmit += strlen(field4);

   memcpy(buffer_transmit, ";", 1);                     
   buffer_transmit += 1;

   memcpy(buffer_transmit, field5, strlen(field5));
   buffer_transmit += strlen(field5);

   memcpy(buffer_transmit, ";", 1);                     
   buffer_transmit += 1;

   memcpy(buffer_transmit, field6, strlen(field6));
   buffer_transmit += strlen(field6);

   trace_event("End   command_PE(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_PZ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de controler le message recu suite */
/*                l'impression du PIN. Le PF n'est pas remonter par le serv.  */
/******************************************************************************/
int    command_PZ( char *buffer_received,
                   char *err_code
                  )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_PZ()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "PZ", 2) != 0)
   {
   		trace_event("End   command_PZ(ERROR_COMMAND)",PROCESSING);
		 return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_PZ(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }

   trace_event("End   command_PZ(OK)",PROCESSING);
   return(OK);
}

/*******************************************************************************/
/* Fonction command_FA.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater ZPK encryptee sous ZMK vers LMK        */
/******************************************************************************/
int     command_FA( char *buffer_transmit,
                    char *zmk_lmk, 
                    char *zpk_zmk
                  )
{
   char ligne[MAX_LINE_TRC];
   /*sprintf(ligne, "Start command_FA(%.48s, .48%s)", zmk_lmk, zpk_zmk);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_FA()", PROCESSING);

   memcpy(buffer_transmit, "FA", 2);                
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));      
   buffer_transmit +=strlen(zmk_lmk);

   
   if( 32 == strlen(zpk_zmk) )
   { memcpy(buffer_transmit,"X" ,1 ); buffer_transmit +=1; }

   if( 48 == strlen(zpk_zmk) )
   { memcpy(buffer_transmit,"T" ,1 ); buffer_transmit +=1; }
   
   memcpy(buffer_transmit, zpk_zmk, strlen(zpk_zmk));      
   buffer_transmit +=strlen(zpk_zmk);

   trace_event("End   command_FA(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_FB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM ZPK sous LMK, la Check_value ainsi que le code d'erreur. */
/******************************************************************************/
int    command_FB( char *buffer_received, 
                   char *zpk_lmk, 
                   char *check_value , 
                   char *err_code 
                  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FB()",PROCESSING);

   memset(zpk_lmk    ,         0, SIZE_KEY * 3);
   memset(Command    ,         0, 3);
   memset(check_value,         0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
/* memcpy(err_code, buffer_received+ 2, 2); */


   if (memcmp(Command, "FB", 2) != 0) 
   {
   		trace_event("End   command_FB(ERROR_COMMAND)",PROCESSING);
  		return(ERROR_COMMAND);
   }

   /* trace_event("WARNING !! : bEFORE CALLING TEXTERROR",PROCESSING); */
   TextError( err_code + 2,sLine);

   if (memcmp(err_code+2  , "01", 2) == 0 )
   {
       trace_event("WARNING !! : ODD PARITY ENFORCED",PROCESSING);
   }
   else
   {
       /* GF */
       if ( (memcmp(err_code + 2 , "00", 2) != 0) && (memcmp(err_code + 2 , "01", 2) != 0)  ) 
       /* GF */
       {
   		trace_event("End   command_FB(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
       }
   }

   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(zpk_lmk, buffer_received,  2 * SIZE_KEY + 1);      
   		buffer_received +=( 2 * SIZE_KEY + 1);
   		memcpy(check_value, buffer_received,  SIZE_KEY);
		zpk_lmk [ 2 * SIZE_KEY + 1] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(zpk_lmk, buffer_received, 3 * SIZE_KEY + 1);      
   		buffer_received +=( 3 * SIZE_KEY + 1);
   		memcpy(check_value, buffer_received,   SIZE_KEY);
		zpk_lmk [ 3 * SIZE_KEY + 1] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(zpk_lmk, buffer_received, SIZE_KEY);      
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zpk_lmk [SIZE_KEY] = '\0';
   }

   trace_event("End   command_FB(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_FC.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater PVK/TPK/TMK encryptee sous ZMK         */
/*       		 vers LMK  							                           */
/*******************************************************************************/
int    command_FC( char *buffer_transmit, 
                   char *zmk_lmk, 
                   char *key_zmk
                 )
{
   trace_event("Start command_FC()", PROCESSING);

   memcpy(buffer_transmit, "FC", 2);              
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));    
   buffer_transmit +=strlen(zmk_lmk);
   memcpy(buffer_transmit, key_zmk, strlen(key_zmk));    
   buffer_transmit +=strlen(key_zmk);

   trace_event("End   command_FC(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_FD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TPK ou PVK sous LMK, la Check_value ainsi que le code    */
/*               d'erreur.                                                    */
/******************************************************************************/
/**
int    command_FD( char *buffer_received , 
				   char *key_lmk, 
				   char *check_value , 
                   char *err_code 
                  )
**/
int    command_FD( char *buffer_received , char *err_code, char *key_lmk) /** MAW20030725 **/
{
   char Command[3];
   char sLine[MAX_LINE_TRC];
   char check_value[16 + 1];

  trace_event("Start command_FD()",PROCESSING);
  /*sprintf(sLine, "buffer_received=[%s]", buffer_received);
  trace_event(sLine, FATAL);*/ /** HM - 09022010 - PA_DSS **/

   memset(Command    ,    0, 3);
   memset(key_lmk    ,    0, SIZE_KEY);
   memset(check_value,    0, SIZE_KEY); 

   memcpy(Command , buffer_received, 2);      
   /** memcpy(err_code, buffer_received, 4);    **/
   memcpy(err_code, buffer_received+ 2, 2);   

   if (memcmp(Command, "FD", 2) != 0) 
   {
   		trace_event("End   command_FD(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   /** if (memcmp(err_code + 2 , "00", 2) != 0)  **/
/*start YK1208*/
  if (memcmp(err_code  , "01", 2)== 0 )
   {
                trace_event("WARNING FORCAGE PARITE",PROCESSING);
          
   }else
/*end YK1208*/
   if (memcmp(err_code  , "00", 2) != 0 )
   {
   		trace_event("End   command_FD(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy( key_lmk, buffer_received,  2 * SIZE_KEY);   
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received,   SIZE_KEY);
		key_lmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy( key_lmk, buffer_received,  3 * SIZE_KEY);   
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received,  SIZE_KEY);
		key_lmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy( key_lmk, buffer_received, SIZE_KEY);   
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		key_lmk [SIZE_KEY] = '\0';
   }

   /*sprintf(sLine, "==> key_lmk=[%s]", key_lmk);
   trace_event(sLine, FATAL);*/ /** HM - 09022010 - PA_DSS **/

   trace_event("End   command_FD(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_FE.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater PVK, TPK ou TMK encryptee sous LMK     */
/*       		 vers ZMK                                                      */
/*******************************************************************************/
int    command_FE( char *buffer_transmit, 
                   char *zmk_lmk, 
                   char *key_lmk
				 )
{
   trace_event("Start command_FE()", PROCESSING);

   memcpy(buffer_transmit, "FE", 2);            
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));  
   buffer_transmit +=strlen(zmk_lmk);
   memcpy(buffer_transmit, key_lmk, strlen(key_lmk));  
   buffer_transmit +=strlen(key_lmk);

   trace_event("End   command_FE()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_FF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TMK, TPK ou PVK sous ZMK, la Check_value ainsi que le    */
/*               code d'erreur. 				                              */
/******************************************************************************/
int    command_FF(  char *buffer_received , 
					char *key_zmk, 
					char *check_value ,
					char *err_code 
				  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FF()",PROCESSING);

   memset(Command, 0, 3);
   memset(check_value, 0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "FF", 2) != 0) 
   {
   		trace_event("End   command_FF(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_FF(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy( key_zmk, buffer_received,  2 * SIZE_KEY);   
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received,   SIZE_KEY);
		key_zmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy( key_zmk, buffer_received,  3 * SIZE_KEY);   
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received,   SIZE_KEY);
		key_zmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy( key_zmk, buffer_received, SIZE_KEY);   
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		key_zmk [SIZE_KEY] = '\0';
   }

   trace_event("End   command_FF(OK)",PROCESSING);
   return(OK);
}

/********************************************************************************/
/* Fonction command_FI.                                                         */
/* -------------------                                                          */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre  */
/*               au HSM pour generer la ZAK a partir d'une ZMK                  */
/* Entree      : ZMK sous LMK                                                   */
/********************************************************************************/
int    command_FI( char *buffer_transmit , 
				   char *zmk_lmk 
                  )
{
   char FLAG='1';

   trace_event("Start command_FI()", PROCESSING);

   memcpy(buffer_transmit, "FI", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, &FLAG,1);           
   buffer_transmit +=1;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
   buffer_transmit +=strlen(zmk_lmk);

   trace_event("End   command_FI(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_FJ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la ZAK sous LMK, ZAK sous ZMK, la Check_value ainsi      */
/*               que le code d'erreur.                                        */
/******************************************************************************/
int    command_FJ( char *buffer_received, 
				   char *zak_zmk,
                   char *zak_lmk,
                   char *check_value , 
                   char *err_code 
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FJ()",PROCESSING);

   memset(Command,    0, 3);
   memset(zak_lmk,    0, SIZE_KEY);
   memset(zak_zmk,    0, SIZE_KEY);
   memset(check_value,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "FJ", 2) != 0) 
   {
   		trace_event("End   command_FJ(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_FJ(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }

   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
   		memcpy(zak_zmk, buffer_received,  2 * SIZE_KEY); 
   		buffer_received +=( 2 * SIZE_KEY);
   		memcpy(zak_lmk, buffer_received, 2 * SIZE_KEY); 
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [ 2 * SIZE_KEY] = '\0'; 
		break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(zak_zmk, buffer_received,  3 * SIZE_KEY); 
   		buffer_received +=( 3 * SIZE_KEY);
   		memcpy(zak_lmk, buffer_received, 3 * SIZE_KEY); 
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [ 3 * SIZE_KEY] = '\0'; 
		break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(zak_zmk, buffer_received, SIZE_KEY); 
   		buffer_received +=SIZE_KEY;
   		memcpy(zak_lmk, buffer_received, SIZE_KEY); 
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [SIZE_KEY] = '\0';
   }

   trace_event("End   command_FJ(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_FK.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre*/
/*               au HSM pour translater ZAK encryptee sous ZMK vers LMK         */
/******************************************************************************/
int    command_FK( char *buffer_transmit,
				   char *zmk_lmk, 
                   char *zak_zmk
                 )
{

   trace_event("Start command_FK()", PROCESSING);

   memcpy(buffer_transmit, "FK", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, "1",1);             
   buffer_transmit+=1;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
   buffer_transmit +=strlen(zmk_lmk);
   memcpy(buffer_transmit, zak_zmk, strlen(zak_zmk)); 
   buffer_transmit +=strlen(zak_zmk);

   trace_event("End   command_FK(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_FL.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM ZAK sous LMK, Check_value ainsi que le code d'erreur.    */
/******************************************************************************/
int    command_FL( char *buffer_received , 
				   char *zak_lmk, 
                   char *check_value, 
                   char *err_code 
				 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FL()",PROCESSING);

   memset(Command,    0, 3);

   memset(zak_lmk,    0, SIZE_KEY);
   memset(check_value,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "FL", 2) != 0) 
   {
   		trace_event("End   command_FL(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_FL(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(zak_lmk, buffer_received, 2 * SIZE_KEY); 
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk [ 2 * SIZE_KEY] = '\0'; 
		break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(zak_lmk, buffer_received, 3 * SIZE_KEY); 
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk [ 3 * SIZE_KEY] = '\0'; 
		break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(zak_lmk, buffer_received, SIZE_KEY); 
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk [SIZE_KEY] = '\0';
   }


   trace_event("End   command_FL(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_FM.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater ZAK encryptee sous LMK vers ZMK        */
/*******************************************************************************/
int    command_FM( char *buffer_transmit,
				   char *zmk_lmk, 
                   char *zak_lmk 
				 )
{
   trace_event("Start command_FM()", PROCESSING);

   memcpy(buffer_transmit, "FM",           2); 
   buffer_transmit +=2;
   memcpy(buffer_transmit, "1" ,           1); 
   buffer_transmit+=1;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
   buffer_transmit +=strlen(zmk_lmk);
   memcpy(buffer_transmit, zak_lmk, strlen(zak_lmk));

   trace_event("End   command_FM()", PROCESSING);

   return(OK);
}

/******************************************************************************/
/* Fonction command_FN.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM ZAK sous ZMK et le code d'erreur.                        */
/******************************************************************************/
int    command_FN( char *buffer_received, 
                   char *zak_zmk , 
				   char *err_code 
				 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FN()",PROCESSING);

   memset(Command,    0, 3);
   memset(zak_zmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "FN", 2) != 0) 
   {
   		trace_event("End   command_FN(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_FN(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
        memcpy(zak_zmk, buffer_received, 2 * SIZE_KEY);
        buffer_received += (2 * SIZE_KEY);
		zak_zmk [ 2 * SIZE_KEY] = '\0'; 
		break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
        memcpy(zak_zmk, buffer_received, 3 * SIZE_KEY);
        buffer_received += (3 * SIZE_KEY);
		zak_zmk [ 3 * SIZE_KEY] = '\0'; 
		break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
        memcpy(zak_zmk, buffer_received, SIZE_KEY);
        buffer_received +=SIZE_KEY;
		zak_zmk [SIZE_KEY] = '\0'; 
   }

   return(OK);
}



/*******************************************************************************/
/* Fonction command_GC.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater ZPK encryptee sous LMK vers ZPK        */
/*               encrypte sous ZMK    						                   */
/*******************************************************************************/
int    command_GC( char *buffer_transmit,
				   char *zmk_lmk, 
                   char *zpk_lmk
                 )
{
   trace_event("Start command_GC()", PROCESSING);

   memcpy(buffer_transmit, "GC", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
   buffer_transmit +=strlen(zmk_lmk);
   memcpy(buffer_transmit, zpk_lmk, strlen(zpk_lmk)); 
   buffer_transmit +=strlen(zpk_lmk);

   trace_event("End   command_GC()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_GD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM ZPK sous ZMK, la Check_value ainsi que le code d'erreur. */
/******************************************************************************/
int    command_GD( char *buffer_received, 
                   char *zpk_zmk, 
                   char *check_value , 
                   char *err_code 
                 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];
   
   trace_event("Start command_GD()",PROCESSING);

   memset(Command,    0, 3);
   memset(zpk_zmk,    0, SIZE_KEY);
   memset(check_value,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "GD", 2) != 0) 
   {
   		trace_event("End   command_GD(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_GD(ERROR_COMMAND)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
        memcpy(zpk_zmk, buffer_received,  2 * SIZE_KEY);     
		zpk_zmk [ 2 * SIZE_KEY] = '\0';
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
        memcpy(zpk_zmk, buffer_received,  3 * SIZE_KEY);     
		zpk_zmk [ 3 * SIZE_KEY] = '\0';
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
        memcpy(zpk_zmk, buffer_received, SIZE_KEY);     
		zpk_zmk [SIZE_KEY] = '\0';
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
   }

   trace_event("End   command_GD(OK)",PROCESSING);
   return(OK);
}

/*******************************************************************************/
/* Fonction command_GG.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la ZMK a partir des 3 composantes ZMK1,   */
/*     			 ZMK2, ZMK3. 							                       */
/******************************************************************************/
int    command_GG( char *buffer_transmit,
				   char *zmk1_lmk,
				   char *zmk2_lmk,
				   char *zmk3_lmk
                  )
{
   trace_event("Start command_GG()", PROCESSING);

   memcpy(buffer_transmit, "GG", 2);            
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk1_lmk, strlen(zmk1_lmk)); 
   buffer_transmit +=strlen(zmk1_lmk);
   memcpy(buffer_transmit, zmk2_lmk, strlen(zmk2_lmk)); 
   buffer_transmit +=strlen(zmk2_lmk);
   memcpy(buffer_transmit, zmk3_lmk, strlen(zmk3_lmk)); 
   buffer_transmit +=strlen(zmk3_lmk);

   trace_event("End   command_GG()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_GH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la ZMK sous LMK ainsi que le code d'erreur.              */
/******************************************************************************/
int    command_GH( char *buffer_received , 
				   char *zmk_lmk , 
                   char *check_value , 
                   char *err_code 
				 )
{


   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_GH()",PROCESSING);

   memset(Command,    0, 3);
   memset(zmk_lmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "GH", 2) != 0) 
   {
		
   		trace_event("End   command_GH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_GH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(zmk_lmk    , buffer_received, 2 * SIZE_KEY); 
		zmk_lmk [2 * SIZE_KEY] = '\0';
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(zmk_lmk    , buffer_received, 3 * SIZE_KEY); 
		zmk_lmk [3 * SIZE_KEY] = '\0';
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);

		if (strlen(buffer_received) > (2 * SIZE_KEY)) /* ZMK used as double length */
		{
   			memcpy(zmk_lmk    , buffer_received, 2 * SIZE_KEY); 
			zmk_lmk [2 * SIZE_KEY] = '\0';
   			buffer_received += (2 * SIZE_KEY);
   			memcpy(check_value, buffer_received, SIZE_KEY);
		}
		else
		{
   			memcpy(zmk_lmk    , buffer_received, SIZE_KEY); 
			zmk_lmk [SIZE_KEY] = '\0';
   			buffer_received += SIZE_KEY;
   			memcpy(check_value, buffer_received, SIZE_KEY);
		}
   }

   trace_event("End   command_GH(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_HA.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la TAK a partir d'une TMK                 */
/*******************************************************************************/
int    command_HA( char *buffer_transmit, 
				   char *tmk_lmk 
				 )
{
   trace_event("Start command_HA()", PROCESSING);

   memcpy(buffer_transmit, "HA", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk)); 
   buffer_transmit +=strlen(tmk_lmk);

   trace_event("End   command_HA(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_HB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la TAK sous TMK, TAK sous LMK, la Check_value ainsi que  */
/*               le code d'erreur.                                            */
/******************************************************************************/
int    command_HB( char *buffer_received, 
				   char *tak_tmk,
				   char *tak_lmk , 
				   char *err_code 
				 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_HB()",PROCESSING);

   memset(Command,    0, 3);
   memset(tak_lmk,    0, SIZE_KEY);
   memset(tak_tmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "HB", 2) != 0) 
   {
   		trace_event("End   command_HB(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_HB(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(tak_tmk, buffer_received, 2 * SIZE_KEY); 
		tak_tmk [2 * SIZE_KEY] = '\0';
   		buffer_received +=(2 * SIZE_KEY);
   		memcpy(tak_lmk, buffer_received, 2 * SIZE_KEY); 
   		buffer_received += (2 * SIZE_KEY);
		tak_lmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(tak_tmk, buffer_received, 3 * SIZE_KEY); 
		tak_tmk [3 * SIZE_KEY] = '\0';
   		buffer_received +=(3 * SIZE_KEY);
   		memcpy(tak_lmk, buffer_received, 3 * SIZE_KEY); 
   		buffer_received += (3 * SIZE_KEY);
		tak_lmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(tak_tmk, buffer_received, SIZE_KEY); 
		tak_tmk [SIZE_KEY] = '\0';
   		buffer_received +=SIZE_KEY;
   		memcpy(tak_lmk, buffer_received, SIZE_KEY); 
   		buffer_received +=SIZE_KEY;
		tak_lmk [SIZE_KEY] = '\0';
   }


   trace_event("End   command_HB(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_IA.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la ZPK a partir d'une ZMK                 */
/*******************************************************************************/
int    command_IA( char *buffer_transmit , 
                   char *zmk_lmk 
				 )
{
   trace_event("Start command_IA()", PROCESSING);

   memcpy(buffer_transmit, "IA", 2);           
   buffer_transmit +=2;
   memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
   buffer_transmit +=strlen(zmk_lmk);

   trace_event("End   command_IA()", PROCESSING);

   return(OK);
}

/******************************************************************************/
/* Fonction command_IB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la ZPK sous ZMK,ZPK sous LMK, la Check_value ainsi que   */
/*       		 le code d'erreur.                                            */
/******************************************************************************/
int    command_IB( char *buffer_received, 
				   char *zpk_zmk, 
				   char *zpk_lmk, 
				   char *check_value , 
				   char *err_code 
				 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_IB()",PROCESSING);

   memset(Command,    0, 3);
   memset(zpk_lmk,    0, SIZE_KEY);
   memset(zpk_zmk,    0, SIZE_KEY);
   memset(check_value,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "IB", 2) != 0) 
   {
   		trace_event("End   command_IB(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_IB(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(zpk_zmk, buffer_received, 2 * SIZE_KEY); 
		zpk_zmk [2 * SIZE_KEY] = '\0';
   		buffer_received +=(2 * SIZE_KEY);
   		memcpy(zpk_lmk, buffer_received, 2 * SIZE_KEY); 
   		buffer_received +=(2 * SIZE_KEY);
		zpk_lmk [2 * SIZE_KEY] = '\0';
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(zpk_zmk, buffer_received, 3 * SIZE_KEY); 
		zpk_zmk [3 * SIZE_KEY] = '\0';
   		buffer_received +=(3 * SIZE_KEY);
   		memcpy(zpk_lmk, buffer_received, 3 * SIZE_KEY); 
   		buffer_received +=(3 * SIZE_KEY);
		zpk_lmk [3 * SIZE_KEY] = '\0';
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(zpk_zmk, buffer_received, SIZE_KEY); 
		zpk_zmk [SIZE_KEY] = '\0';
   		buffer_received +=SIZE_KEY;
   		memcpy(zpk_lmk, buffer_received, SIZE_KEY); 
   		buffer_received +=SIZE_KEY;
		zpk_lmk [SIZE_KEY] = '\0';
   		memcpy(check_value, buffer_received, SIZE_KEY);
   }


   trace_event("End   command_IB(OK)",PROCESSING);
   return(OK);
}



/******************************************************************************/
/* Fonction command_JC.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de translater un PIN sous TPK vers */
/*               PIN sous LMK.                                                */
/******************************************************************************/
int command_JC(	char *buffer_transmit, 
				char *Tpk, 
				char *PinBlock, 
				char *format, 
				char *Pan
			  )
{
   char Card[13];
   char ligne[MAX_LINE_TRC];
   memset (  Card , 0, 13 );

   /*sprintf(ligne, "Start command_JC(%.16s,%.2s,%.22s)", PinBlock, format, Card);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_JC()", PROCESSING);

   memcpy(buffer_transmit , "JC",           2);      
   buffer_transmit+=2;
   memcpy(buffer_transmit , Tpk ,     strlen(Tpk));     
   buffer_transmit+=strlen(Tpk);
   memcpy(buffer_transmit , PinBlock, SIZE_KEY);     
   buffer_transmit+=SIZE_KEY;
   memcpy(buffer_transmit , format,          2);     
   buffer_transmit+=2;

   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);

   /*sprintf(ligne, "End   command_JC(%.60s)",  buffer_transmit);
   trace_event(ligne,  PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("End   command_JC(OK)",  PROCESSING);

   return (OK);
}


/******************************************************************************/
/* Fonction command_JD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but d'extraire un PIN sous LMK apres   */
/*               translation.                                                */
/******************************************************************************/
int command_JD( char *buffer_received, 
				char *PinLmk, 
				char *err_code 
			  )
{
   char ligne[MAX_LINE_TRC];
   char Command[3];
   char sLine[MAX_LINE_TRC];

   memset(Command,    0, 3);

   /*sprintf(ligne, "Start command_JD(%.40s)", buffer_received);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_JD()", PROCESSING);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received+2, 2);   
   if (memcmp(Command, "JD", 2) != 0) 
   {
   		trace_event("End   command_JD(ERROR_COMMAND)", PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code ,sLine);
   if (memcmp(err_code  , "00", 2) != 0) 
   {
   		trace_event("End   command_JD(CHECK_ERROR)", PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(PinLmk ,   buffer_received, SIZE_KEY);
   /*sprintf(ligne, "End   command_JD(%.2s,%.16s)", err_code, PinLmk);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("End   command_JD(OK)", PROCESSING);

   return (OK);
}

/******************************************************************************/
/* Fonction command_JE.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer pour         */
/*               translater un PIN sous ZPK vers un PIN sous LMK              */
/******************************************************************************/
int command_JE( char *buffer_transmit, 
			    char *Zpk, 
				char *PinBlock, 
				char *format, 
				char *Pan
			  )
{
   char Card[13];
   char ligne[MAX_LINE_TRC];

   memset (  Card , 0, 13 );
   /*sprintf(ligne, "Start command_JE(%.16s,%.2s,%.22s)", PinBlock, format, Card);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_JE()", PROCESSING);

   memcpy( buffer_transmit , "JE",           2);  
   buffer_transmit +=2;
   memcpy( buffer_transmit , Zpk     ,strlen(Zpk));  
   buffer_transmit +=strlen(Zpk);
   memcpy( buffer_transmit , PinBlock,SIZE_KEY);  
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit , format  ,       2);  
   buffer_transmit +=2;

   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);

   sprintf(ligne, "End   command_JE(OK)"); /** HM - 09022010 - PA_DSS **/
   trace_event(ligne,  PROCESSING);
   return (OK);
}


/******************************************************************************/
/* Fonction command_JF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PIN sous LMK       */
/******************************************************************************/
int    command_JF( char *buffer_received , 
				   char *pin_lmk , 
				   char *err_code 
				  )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_JF()",PROCESSING);

   memset(Command,    0, 3);
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "JF", 2) != 0) 
   {
   		trace_event("End   command_JF(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_JF(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(pin_lmk    , buffer_received, SIZE_KEY);

   trace_event("End   command_JF(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_JG.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer pour         */
/*               translater un PIN sous LMK vers un PIN sous ZPK              */
/******************************************************************************/
int command_JG( char *buffer_transmit, 
			    char *Zpk, 
				char *PinLmk, 
				char *format, 
				char *Pan
			  )
{
   char Card[13];
   char ligne[MAX_LINE_TRC];

   memset ( Card , 0, 13 );
   /*sprintf(ligne, "Start command_JG(%.16s,%.2s,%.22s)",  PinLmk, format, Card);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_JG()", PROCESSING);

   memcpy( buffer_transmit , "JG",           2);  
   buffer_transmit +=2;
   memcpy( buffer_transmit , Zpk     ,strlen(Zpk));  
   buffer_transmit +=strlen(Zpk);
   memcpy( buffer_transmit , format  ,       2);  
   buffer_transmit +=2;
   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);              
   buffer_transmit +=12;
   memcpy( buffer_transmit , PinLmk  ,SIZE_KEY);  
   buffer_transmit +=SIZE_KEY;

   sprintf(ligne, "End   command_JE()");
   trace_event(ligne,  PROCESSING);
   return (OK);
}


/******************************************************************************/
/* Fonction command_JH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PINBlock sous ZMK  */
/******************************************************************************/
int    command_JH( char *buffer_received , 
				   char *PinBlock , 
				   char *err_code 
				  )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_JH()",PROCESSING);

   memset(Command,    0, 3);
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "JH", 2) != 0) 
   {
   		trace_event("End   command_JH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_JH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(PinBlock    , buffer_received, SIZE_KEY);

   trace_event("End   command_JH(OK)",PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_OE.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la TMK et la TPK d'une maniere aleatoire  */
/*******************************************************************************/
int    command_OE(char *buffer_transmit)
{
 
  trace_event("Start command_OE()", PROCESSING);

   memcpy(buffer_transmit, "OE", 2);
   buffer_transmit +=2;

   trace_event("End   command_OE()", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_OF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la TMK ou la TPK sous LMK, la Check_value ainsi que      */
/*       		 le code d'erreur.                                            */
/******************************************************************************/
int    command_OF( char *buffer_received, 
				   char *key_lmk,
				   char *check_value,
				   char *err_code)
{


   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_OF()", PROCESSING);

   memset(Command,    0, 3);
   memset(key_lmk,     0, SIZE_KEY);
   memset(check_value, 0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "OE", 2) != 0)
   {
   		 trace_event("End   command_OF(ERROR_COMMAND)", PROCESSING);
		 return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		 trace_event("End   command_OF(CHECK_ERROR)", PROCESSING);
		 return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(key_lmk, buffer_received, 2 * SIZE_KEY);
		key_lmk [2 * SIZE_KEY] = '\0';
   		buffer_received +=(2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(key_lmk, buffer_received, 3 * SIZE_KEY);
		key_lmk [3 * SIZE_KEY] = '\0';
   		buffer_received +=(3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, SIZE_KEY);
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(key_lmk, buffer_received, SIZE_KEY);
		key_lmk [SIZE_KEY] = '\0';
   		buffer_received +=SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
   }


   trace_event("End   command_OF(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_HE                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le pin block encrypte sous une tak             .         */
/******************************************************************************/
int  command_HE( char *buffer_transmit, 
				 char *cle, 
				 char *data
			   )
{

   trace_event("Start command_HE()", PROCESSING);

   memcpy(buffer_transmit, "HE", 2);        
   buffer_transmit  += 2;
   memcpy(buffer_transmit , cle, strlen(cle)); 
   buffer_transmit += strlen(cle);
   memcpy(buffer_transmit , data, SIZE_KEY);

   trace_event("End   command_HE()", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_HF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le pin block encrypte sous une tak             .         */
/******************************************************************************/
int    command_HF( char *buffer_received, 
				   char *pin, 
				   char *err_code
				  )
{


   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_HF()", PROCESSING);

   memset(Command,    0, 3);
   memset(pin,     0,SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "HF", 2) != 0) 
   {
   		trace_event("End   command_HF(ERROR_COMMND)", PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_HF(CHECK_ERROR)", PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(pin, buffer_received , SIZE_KEY);
   trace_event("End   command_HF(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_HG                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le pin block decrypte sous une tak             .         */
/******************************************************************************/
int command_HG( char *buffer_transmit, 
			    char *cle, 
				char *data
			  )
{
   trace_event("Start command_HG()", PROCESSING);

   memcpy(buffer_transmit, "HG", 2);         
   buffer_transmit += 2;
   memcpy(buffer_transmit , cle, strlen(cle));  
   buffer_transmit += strlen(cle);
   memcpy(buffer_transmit , data, SIZE_KEY);

   trace_event("End   command_HG()", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_HH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le pin block decrypt sous une tak             .         */
/******************************************************************************/
int    command_HH( char *buffer_received, 
				   char *pin, 
				   char *err_code
				  )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_HH()",PROCESSING);

   memset(Command,    0, 3);
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "HH", 2) != 0) 
   {
   		trace_event("Start command_HH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("Start command_HH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(pin, buffer_received , SIZE_KEY);

   trace_event("End   command_HH(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_HC                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de generer randomly une cle et     */
/*               l'encrypter sous LMK paire 14,15                             */
/******************************************************************************/
/*
  int  command_HC( char *buffer_transmit, 
				 char *key 
				)
{
   trace_event("Start command_HC()", PROCESSING);

   memcpy(buffer_transmit, "HC", 2);         
   buffer_transmit += 2;
   memcpy(buffer_transmit , key, strlen(key));  
   buffer_transmit += strlen(key);

   trace_event("End   command_HC(OK)", PROCESSING);
   return(OK);
}
*/
/*   int  command_HC( char *buffer_transmit, 
                                 char *key 
                                ) 
{ 
   trace_event("Start command_HC()", PROCESSING); 

   memcpy(buffer_transmit, "HCU", 3); 
   buffer_transmit += 3; 
   memcpy(buffer_transmit , key, strlen(key)); 
   buffer_transmit += strlen(key); 

   trace_event("End   command_HC(OK)", PROCESSING); 
   return(OK); 
}
*/

int  command_HC( char *buffer_transmit, 
				 char *key 
				)
{
char sLine[MAX_LINE_TRC];

   /*sprintf(sLine,"Start command_HC(%s)", key);
   trace_event(sLine,PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start command_HC()",PROCESSING);

   memcpy(buffer_transmit, "HC", 2);         
   buffer_transmit += 2;
   if ( strlen(key) > 16)
   {
   	/* NBD28112008 Start
	emcpy(buffer_transmit, "U", 1);         
   	buffer_transmit += 1;
	   NBD28112008 End */
	memcpy(buffer_transmit , key, strlen(key));  
   	buffer_transmit += strlen(key);
        memcpy(buffer_transmit , ";XU0", 4);  
   	buffer_transmit += 4;
   }
   else
   {
   	memcpy(buffer_transmit , key, strlen(key));  
   	buffer_transmit += strlen(key);
   }
  

   trace_event("End   command_HC(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_HD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer la cle sous LMK       */
/******************************************************************************/
int    command_HD( char *buffer_received, 
				   char *key_key , 
				   char *key_lmk , 
				   char *err_code 
				 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_HD()",PROCESSING);

   memset(Command,    0, 3);
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "HD", 2) != 0) 
   {
   		trace_event("End   command_HD(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_HD(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(key_key, buffer_received , (2 * SIZE_KEY) + 1); 
		key_key [(2 * SIZE_KEY) + 1] = '\0';
   		buffer_received += (2 * SIZE_KEY);
   		buffer_received += 1;
   		memcpy(key_lmk, buffer_received , (2 * SIZE_KEY) + 1);
		key_lmk [(2 * SIZE_KEY) + 1] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(key_key, buffer_received , (3 * SIZE_KEY) + 1); 
		key_key [(3 * SIZE_KEY) + 1] = '\0';
   		buffer_received += 1;
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(key_lmk, buffer_received , (3 * SIZE_KEY) + 1);
		key_lmk [(3 * SIZE_KEY) + 1] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(key_key, buffer_received , SIZE_KEY); 
		key_key [SIZE_KEY] = '\0';
   		buffer_received += SIZE_KEY;
   		memcpy(key_lmk, buffer_received , SIZE_KEY);
		key_lmk [SIZE_KEY] = '\0';
   }


   trace_event("End   command_HD(OK)", PROCESSING);
   return(OK);
}


/*******************************************************************************/
/* Fonction command_FG.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour generer la paire PVK sous LMK                     */
/*******************************************************************************/
int    command_FG( char *buffer_transmit,
				   char *zmk
                )
{
   trace_event("Debut command_FG()", PROCESSING);

   memcpy(buffer_transmit, "FG", 2); 
   buffer_transmit += 2;
   memcpy(buffer_transmit,  zmk, strlen(zmk));

   trace_event("Fin   command_FG(OK)", PROCESSING);

   return(OK);
}

/******************************************************************************/
/* Fonction command_FH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la paire PVK sous LMK, la Check_value ainsi que          */
/*       		 le code d'erreur.			                                  */
/******************************************************************************/
int    command_FH( char *buffer_received, 
				   char *pvka_lmk, 
				   char *pvkb_lmk, 
				   char *pvka_zmk, 
				   char *pvkb_zmk, 
				   char *check_valuea , 
				   char *check_valueb , 
				   char *err_code 
			    )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_FH()",PROCESSING);

   memset(Command,    0, 3);
   memset(pvka_lmk,    0, SIZE_KEY);
   memset(pvkb_lmk,    0, SIZE_KEY);
   memset(pvka_zmk,    0, SIZE_KEY);
   memset(pvkb_zmk,    0, SIZE_KEY);
   memset(check_valuea,0, SIZE_KEY);
   memset(check_valueb,0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "FH", 2) != 0) 
   {
   		trace_event("End   command_FH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_FH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(pvka_lmk, buffer_received, SIZE_KEY);    
   buffer_received +=SIZE_KEY;
   memcpy(pvkb_lmk, buffer_received, SIZE_KEY);    
   buffer_received +=SIZE_KEY;
   memcpy(pvka_zmk, buffer_received, SIZE_KEY);    
   buffer_received +=SIZE_KEY;
   memcpy(pvkb_zmk, buffer_received, SIZE_KEY);    
   buffer_received +=SIZE_KEY;
   memcpy(check_valuea,buffer_received, SIZE_KEY); 
   buffer_received +=SIZE_KEY;
   memcpy(check_valueb,buffer_received, SIZE_KEY);

   trace_event("End   command_FH(OK)",PROCESSING);
   return(OK);
}

/*******************************************************************************/
/* Fonction command_MI.                                                        */
/* -------------------                                                         */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre */
/*               au HSM pour translater TAK encryptee sous ZMK vers LMK        */
/*******************************************************************************/
int    command_MI( char *buffer_transmit , 
				   char *zmk, 
				   char *tak_zmk 
				  )
{
   trace_event("Start command_MI()", PROCESSING);

   memcpy(buffer_transmit,    "MI",        2); 
   buffer_transmit +=2;
   memcpy(buffer_transmit,     zmk, strlen(zmk)); 
   buffer_transmit +=strlen(zmk);
   memcpy(buffer_transmit, tak_zmk, strlen(tak_zmk));

   trace_event("End   command_MI(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_MJ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TAK sous LMK                                             */
/******************************************************************************/
int    command_MJ( char *buffer_received , 
				   char *tak_lmk , 
				   char *check_value , 
				   char *err_code 
				 )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

  trace_event("Start command_MJ()",PROCESSING);

   memset(Command,    0, 3);
   memset(tak_lmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "MJ", 2) != 0) 
   {
  		trace_event("End   command_MJ(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
  		trace_event("End   command_MJ(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(tak_lmk    , buffer_received, 2 * SIZE_KEY); 
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, 2 * SIZE_KEY);
		tak_lmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(tak_lmk    , buffer_received, 3 * SIZE_KEY); 
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, 3 * SIZE_KEY);
		tak_lmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(tak_lmk    , buffer_received, SIZE_KEY); 
   		buffer_received += SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		tak_lmk [SIZE_KEY] = '\0';
   }

   trace_event("End   command_MJ(OK)",PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_MG.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre*/
/*               au HSM pour translater TAK encryptee sous LMK vers ZMK         */
/******************************************************************************/
int    command_MG( char *buffer_transmit , 
				   char *zmk, 
				   char *tak_lmk 
				 )
{
   trace_event("Start command_MG()", PROCESSING);

   memcpy(buffer_transmit,    "MG",        2); 
   buffer_transmit +=2;
   memcpy(buffer_transmit,     zmk, strlen(zmk)); 
   buffer_transmit +=strlen(zmk);
   memcpy(buffer_transmit, tak_lmk, strlen(tak_lmk));

   trace_event("End   command_MG(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_MH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM TAK sous ZMK                                             */
/******************************************************************************/
int    command_MH( char *buffer_received , 
				   char *tak_zmk , 
                   char *check_value , 
                   char *err_code 
			      )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_MH()",PROCESSING);
   
   memset(Command,    0, 3);
   memset(tak_zmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "MH", 2) != 0) 
   {
   		trace_event("End   command_MH(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_MH(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;
   switch (buffer_received[0])
   {
	 case 'U' :
	 case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
   		memcpy(tak_zmk    , buffer_received, 2 * SIZE_KEY); 
   		buffer_received += (2 * SIZE_KEY);
   		memcpy(check_value, buffer_received, 2 * SIZE_KEY);
		tak_zmk [2 * SIZE_KEY] = '\0';
        break;
	 case 'T' :
	 case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
   		memcpy(tak_zmk    , buffer_received, 3 * SIZE_KEY); 
   		buffer_received += (3 * SIZE_KEY);
   		memcpy(check_value, buffer_received, 3 * SIZE_KEY);
		tak_zmk [3 * SIZE_KEY] = '\0';
        break;
	 default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
   		memcpy(tak_zmk    , buffer_received, SIZE_KEY); 
   		buffer_received += SIZE_KEY;
   		memcpy(check_value, buffer_received, SIZE_KEY);
		tak_zmk [SIZE_KEY] = '\0';
   }


   trace_event("End   command_MH(OK)",PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_BA.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but d'encrypter un PIN clair sous LMK  */
/******************************************************************************/
int    command_BA( char *buffer_transmit , 
			       char *pin, 
                   char *Pan 
				 )
{
   char card[13];
   memset ( card , 0, 13 );

   trace_event("Start command_BA()", PROCESSING);

   memcpy(buffer_transmit, "BA", 2);      
   buffer_transmit +=2;
   memcpy(buffer_transmit,pin, SIZE_KEY); 
   buffer_transmit +=SIZE_KEY;
   format_card_from_pan ( card , Pan );
   memcpy(buffer_transmit, card ,12);

   trace_event("End   command_BA(OK)", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_BB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM PIN sous LMK                                             */
/******************************************************************************/
int    command_BB( char *buffer_received , 
				   char *pin_lmk , 
			       char *err_code 
			     )
{

   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_BB()",PROCESSING);

   memset(Command,    0, 3);
   memset(pin_lmk,    0, SIZE_KEY);

   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "BB", 2) != 0) 
   {
   		trace_event("End   command_BB(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_BB(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(pin_lmk    , buffer_received, SIZE_KEY);

   trace_event("End   command_BB(OK)",PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction command_NC.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de tester le fonctionnament HSM    */
/******************************************************************************/
int    command_NC( char *buffer_transmit )
{
   trace_event("Start command_NC()", PROCESSING);

   memcpy(buffer_transmit, "NC", 2);

   trace_event("End   command_NC()", PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_ND.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM le resultat du test de fonctionnement.                   */
/******************************************************************************/
int    command_ND( char *buffer_received, 
				   char *lmk_check, 
			       char *err_code
				  )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_ND()",PROCESSING);

   memset(Command , 0, 3);      
   memcpy(Command , buffer_received, 2);      
   memcpy(err_code, buffer_received, 4);   
   if (memcmp(Command, "ND", 2) != 0) 
   {
   		trace_event("End   command_ND(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
   }

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2 , "00", 2) != 0) 
   {
   		trace_event("End   command_ND(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
   }
   buffer_received += 4;

   memcpy(lmk_check, buffer_received, SIZE_KEY);
   buffer_received +=SIZE_KEY;

   trace_event("End   command_ND(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction command_DA                                                         */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de verifier un PIN methode IBM     */
/******************************************************************************/
/** int command_DA( buffer_transmit,Zpk,Pvka,PinZpk,format,Pan,offset) **/
int command_DA( buffer_transmit,Zpk,Pvka,PinZpk,format,Pan,offset, PinLength) /** MAW20030525 **/
char   *buffer_transmit;
char   *Zpk, *Pvka, *PinZpk, *format, *Pan, *offset;
int    PinLength;  /** MAW20030525 **/
{

   char Card[13];
   char vPinLen [ 2 + 1 ];  /** MAW20030525 **/

   memset ( Card , 0, 13 );
   trace_event("Start command_DA" , PROCESSING);

   memcpy( buffer_transmit, "DA",   2);
   buffer_transmit +=2;
   memcpy( buffer_transmit, Zpk, SIZE_KEY);
 buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, Pvka, SIZE_KEY);
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, "12", 2);
   buffer_transmit +=2;
   memcpy( buffer_transmit, PinZpk, SIZE_KEY);
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, format, 2);
   buffer_transmit +=2;
   /** START MAW20030525 **/
   /** memcpy( buffer_transmit, "04", 2);         **/
   sprintf(vPinLen, "%02d", PinLength);
   memcpy( buffer_transmit, vPinLen, 2);
   buffer_transmit +=2;
   /** END   MAW20030525 **/

   format_card_from_pan ( Card , Pan );
   memcpy(buffer_transmit, Card ,12);
   buffer_transmit += 12;

   memcpy( buffer_transmit, Validation_data, SIZE_KEY);
   buffer_transmit +=SIZE_KEY;
   memcpy( buffer_transmit, Pan, 10);
   buffer_transmit +=10;
   memcpy( buffer_transmit, "N", 1);
   buffer_transmit +=1;
   memcpy( buffer_transmit, Pan+(strlen(Pan)-1),1);
   buffer_transmit +=1;
   /** START MAW20030525 **/
/**
   memcpy( buffer_transmit, offset, 4);
   buffer_transmit +=4;
**/
   memcpy( buffer_transmit, offset, strlen(offset));
   buffer_transmit +=strlen(offset);

/**
   memcpy( buffer_transmit, "FFFFFFFF", 8);
   buffer_transmit +=8;
**/
   memcpy( buffer_transmit, "FFFFFFFFFFFF", 12-strlen(offset));
   buffer_transmit +=12-strlen(offset);
   /** END   MAW20030525 **/

   trace_event( "End command_DA()",  PROCESSING);
   return (OK);
}
/******************************************************************************/
/* Fonction command_DB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/*               verification du PIN par la methode IBM                       */
/******************************************************************************/
int    command_DB( buffer_received , err_code )
char   *buffer_received;
char   *err_code;
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   memset(Command , 0, 3);
   memcpy(Command , buffer_received, 2);
   memcpy(err_code, buffer_received, 4);
   if (memcmp(Command, "DB", 2) != 0) return(ERROR_COMMAND);

   TextError( err_code + 2,sLine);
   if (memcmp(err_code + 2, "00", 2) == 0) return ( OK );
   if (memcmp(err_code + 2, "01", 2) == 0) return ( VERIFICATION_FAILED );
   return(CRYPTOGRAPHIC_ERROR);
}

/***********************************************************************
**  nom   : command_MW                                                **
**  desc  : demande de verification du  mac d'un messsage             **
**  entree: tak_key   : cle d authentication du message               **
**          mac_data  : donnees du MAC                                **
**  sortie: Buffer-> Message a envoyer  au HSM                        **
**  retour: OK    -> fonction aboutie avec succes                     **
**          NOK   -> fonction aboutie avec echec                      **
**          ERROR -> fonction non aboutie                             **
**  auteur: E. ENNOUINI (06 Fev 2002)                                 **
***********************************************************************/
int command_MW(Buffer, mode,tak_key, sVect, mac_data, message_mac ,lg_message_mac)

unsigned char   Buffer[];
char            mode;
unsigned char   tak_key[];
unsigned char   sVect[];
unsigned char   mac_data[];
unsigned char   message_mac[];
int             lg_message_mac;
{

    int     offset= 0;
    char ligne[MAX_LINE_TRC];

    trace_event("Start command_MW()", PROCESSING);

    memcpy(Buffer + offset, "MW",   2);
    offset += 2;

    Buffer[offset++]=mode;
    memcpy(Buffer + offset, tak_key,    SIZE_KEY);
    offset += SIZE_KEY;
    if (mode != '0' && mode != '1')
    {
    	memcpy(Buffer + offset, sVect,    SIZE_KEY);
    	offset += SIZE_KEY;
    }
  
    memcpy(Buffer + offset, mac_data,   8);
    offset += 8;

	sprintf(ligne,"Inside command_MW the length is %d", lg_message_mac);
	trace_event(ligne,PROCESSING);

    sprintf(Buffer+offset,"%03X",lg_message_mac);
    offset += 3;

    memcpy(Buffer + offset, message_mac, lg_message_mac);
    offset += lg_message_mac;

    /** necessaire ??? Buffer[offset]=0;*/

    /*sprintf(ligne, "End command_MW(%02d)%s", offset, Buffer);
    trace_event(ligne,  PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
    trace_event("End command_MW(OK)",  PROCESSING);
    return (offset);
}

 /***********************************************************************
  **  nom   : command_MX                                                **
  **  desc  : recuperation du reponse du HSM  pour une demande de       ** 
  **          verification du MAC                                       **
  **  entree: Buffer_received-> le buffer recu                          **
  **  sortie: error_code    -> code erreur de la commande               **
  **  retour: OK    -> fonction aboutie avec succes                     **
  **          NOK   -> fonction aboutie avec echec                      **
  **  auteur: E. ENNOUINI (07 Fev 2002)                                 **
  ***********************************************************************/
command_MX(Buffer_received, error_code)
unsigned char   Buffer_received[];
unsigned char   error_code[];
{


    char    Command [3];
    char    sLine [MAX_LINE_TRC];

    trace_event("Start command_MX()", PROCESSING);

/** START MAW20030328 **/
    /*sprintf(sLine, "Buffer_received=[%s]", Buffer_received);
    trace_event(sLine, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
    memset(Command, 0, sizeof(Command));
    memcpy(Command, Buffer_received , 2);
/** END   MAW20030328 **/

    memcpy(error_code, Buffer_received + 2, 2);

   if (memcmp(Command, "MX", 2) != 0) return(ERROR_COMMAND); 

   /** if (memcmp(error_code + 2, "00", 2) == 0) return ( OK ); **/
   if (memcmp(error_code , "00", 2) == 0) return ( OK ); /** MAW20030328 **/

   /** if (memcmp(error_code + 2, "01", 2) == 0) return ( VERIFICATION_FAILED ); **/
   if (memcmp(error_code , "01", 2) == 0) return ( VERIFICATION_FAILED ); /** MAW20030328 **/

   trace_event("End   command_MX(ERROR)", PROCESSING);

   return(CRYPTOGRAPHIC_ERROR);
}



 /***********************************************************************
  **  nom   : command_MU                                                **
  **  desc  : demande de generation du MAC d'un message                 **
  **  entree: tak_key    : cle d authentication du message              **
  **          message_mac  :  message to be MACed                       **
  **          lg_message_mac  :  length of the  message to be MACed     **
  **  sortie: Buffer-> a envoyer au module de securite                  **
  **  retour: OK    -> fonction aboutie avec succes                     **
  **          NOK   -> fonction aboutie avec echec                      **
  **          ERROR -> fonction non aboutie                             **
  **  auteur: M. ENNOUINI (07 Avr 2002)                                 **
  ***********************************************************************/
int command_MU(Buffer, mode, tak_key, sVect, message_mac ,lg_message_mac)

unsigned char   Buffer[];
char            mode;
unsigned char   tak_key[];
unsigned char   sVect[];
unsigned char   message_mac[];
int             lg_message_mac;
{

    int     offset = 0;
    char ligne[MAX_LINE_TRC];

    trace_event("Start command_MU()", PROCESSING);

    memcpy(Buffer + offset, "MU",   2);
    offset += 2;

    Buffer[offset++]=mode;
    memcpy(Buffer + offset, tak_key,    SIZE_KEY);
    offset += SIZE_KEY;
    if (mode != '0' && mode != '1')
    {
    	memcpy(Buffer + offset, sVect,    SIZE_KEY);
    	offset += SIZE_KEY;
    }
    
    sprintf(Buffer+offset,"%03X",lg_message_mac);
    offset += 3;

    memcpy(Buffer + offset, message_mac, lg_message_mac);
    offset += lg_message_mac;

    sprintf(ligne, "End command_MU(%02d)", offset);
    trace_event(ligne,  PROCESSING);
    return (offset);
}


 /***********************************************************************
  **  nom   : command_MV                                                **
  **  desc  : extraction des champs du buffer recu du module de securite**
  **  entree: Buffer_received-> le buffer recu                          **
  **  sortie: error_code    -> code erreur de la commande               **
  **  retour: OK    -> fonction aboutie avec succes                     **
  **          NOK   -> fonction aboutie avec echec                      **
  **          ERROR -> fonction non aboutie                             **
  **  auteur: M. ENNOUINI (06 Avr 2001)                                 **
  ***********************************************************************/
command_MV(Buffer_received, error_code, mac_data)
unsigned char   Buffer_received[];
unsigned char   error_code[];
unsigned char   mac_data [];
{

    char    Command [3];
    int     offset=2;

    trace_event("Start command_MV()", PROCESSING);

    memcpy(Command , Buffer_received, 2);      

    memcpy(error_code, Buffer_received + offset, 2);
    offset += 2;

    if (memcmp(Command, "MV", 2) != 0) return(ERROR_COMMAND);

    if (memcmp(error_code,"00",2) != 0) 
	return(CHECK_ERROR );
    else
    	/* memcpy(mac_data,Buffer_received + offset ,8); */
    	memcpy(mac_data,Buffer_received + offset ,strlen(Buffer_received + offset));

    trace_event("End   command_MV(OK)", PROCESSING);

    return (OK);
}

 /***********************************************************************
  **  NAME  : command_KU
  **  DESC  : 
  **  INPUT : 
  **  OUTPUT: 
  **  AUTHOR: M.A.WAHBI 11/Nov/2003
  ***********************************************************************/
int command_KU( 
   char   pModeFlag,        /** Mode Flag. 0=Integrity. 1=Intgr/Conf Issuer MK. 2=Intgr/Conf Diff MK **/
   char   pSchemeFlag,      /** Scheme-ID. 0=VISA. 1=Europay/MasterCard **/
   char   *pMK_SMI,         /** MK for secure messaging with Integrity **/
   char   *pPan,            /** PAN/PAN Seq Nbr. Pre-Formated PAN/PAN Sequence number **/
   char   *pIntgrSsnData,   /** Integrity Session Data. Data used for Integrity Session Key generation **/
   int    pPlainTxtLen,     /** Length of Plain Text Message Data **/
   char   *pPlainTxtData,   /** Plain Text Message Data **/
   char   *pMK_SMC,         /** The MK for Secure Messaging with Confidentiality **/
   char   *pTK,             /** The Transport Key undeer which the Supplied Message is incripted **/
   char   *pConfSsnData,    /** Confidetial Session Data.  Used for confidentiality session key generation **/
   int    pCyphDataOffset,  /** Offset.  Position within Plain Text data to insert Ciphertext data **/
   int    pCyphMsgLen,      /** Cipher text message data length **/
   char   *pCyphMsgData,    /** Cipher text message supplied encrypted using a TK **/
   char   *pOutBuffer       /** Output Buffer containing the actual KU command to be sent to the HSM **/
   )
{

    int     offset = 0;
    char    sLine[MAX_LINE_TRC];
    char    vString [ LG_MAX ];

    trace_event("Start command_KU()", PROCESSING);

    memset( vString, 0, sizeof(vString));

    memcpy(pOutBuffer + offset, "KU",   2);
    offset += 2;

    sprintf(pOutBuffer + offset , "%c", pModeFlag);
    offset += 1;

    sprintf(pOutBuffer + offset , "%c", pSchemeFlag);
    offset += 1;

    memcpy(pOutBuffer + offset, pMK_SMI, strlen(pMK_SMI));
    offset += strlen(pMK_SMI);

    memcpy(pOutBuffer + offset, pPan, 8);
    offset += 8;

    memcpy(pOutBuffer + offset, pIntgrSsnData, 8);
    offset += 8;

    sprintf(vString, "%04X", pPlainTxtLen);
    memcpy(pOutBuffer + offset, vString, 4);
    offset += 4;

    memcpy(pOutBuffer + offset, pPlainTxtData, pPlainTxtLen);
    offset += pPlainTxtLen;
    
    memcpy(pOutBuffer + offset, ";", 1);
    offset += 1;

    if ( pModeFlag == '2' )
    {
       memcpy(pOutBuffer + offset, pMK_SMC, strlen(pMK_SMC));
       offset += strlen(pMK_SMC);
    }
    
    if ( pModeFlag == '1' || pModeFlag == '2' )
    {
       memcpy(pOutBuffer + offset, pTK, strlen(pTK));
       offset += strlen(pTK);

       memcpy(pOutBuffer + offset, pConfSsnData, 8);
       offset += 8;

       sprintf(vString, "%04X", pCyphDataOffset);
       memcpy(pOutBuffer + offset, vString, 4);
       offset += 4;

       sprintf(vString, "%04X", pCyphMsgLen);
       memcpy(pOutBuffer + offset, vString, 4);
       offset += 4;

       memcpy(pOutBuffer + offset, pCyphMsgData, pCyphMsgLen);
       offset += pCyphMsgLen;
    }


    /*sprintf(sLine, "==> offset=[%d]", offset);
    trace_event(sLine,  PROCESSING);*/ /** HM - 09022010 - PA_DSS **/

    sprintf(sLine, "End command_KU(%d)", offset);
    trace_event(sLine,  PROCESSING);
    return (offset);
}


 /***********************************************************************
  **  NAME  : command_KV
  **  DESC  : 
  **  INPUT : 
  **  OUTPUT: 
  **  AUTHOR: M.A.WAHBI 11/Nov/2003
  ***********************************************************************/
command_KV(
   char   *pInBuffer,
   char   pModeFlag,
   char   *pErrCode,
   char   *pMAC,
   int    *pSecMsgLen,
   char   *pSecMsgData
   )
{

    char    sLine [ MAX_LINE_TRC ];
    char    vCommand [ 2 + 1 ];
    char    vString [ LG_MAX ];
    int     offset = 0;


    trace_event("Start command_KV()", PROCESSING);

    memset(vCommand, 0, sizeof(vCommand));
    memset( vString, 0, sizeof(vString));

    memcpy(vCommand , pInBuffer, 2);
    offset += 2;

    memcpy(pErrCode, pInBuffer + offset, 2);
    offset += 2;

    if (memcmp(vCommand, "KV", 2) != 0) return(ERROR_COMMAND);

    if (memcmp(pErrCode,"00",2) != 0)
        return(CHECK_ERROR );
    else
        memcpy(pMAC, pInBuffer + offset, 8);

    offset += 8;

    if ( pModeFlag != '0' )
    {
        memcpy(vString, pInBuffer + offset, 4);
        HexToDec ( vString, *pSecMsgLen);
        offset += 4;

        memcpy(pSecMsgData, pInBuffer + offset, *pSecMsgLen);
        offset += *pSecMsgLen;
    }

    trace_event("End   command_KV(OK)", PROCESSING);

    return (OK);
}

 /***********************************************************************
  **  NAME  : command_KV
  **  DESC  :
  **  INPUT :
  **  OUTPUT:
  **  AUTHOR: M.A.WAHBI 11/Nov/2003
  ***********************************************************************/
int HexToDec ( char *InHexBuff, int *OutDec )
{
   char v1[2+1], v2[2+1], v3[2+1], v4[2+1];

   memset(v1, 0, sizeof(v1));
   memset(v2, 0, sizeof(v2));
   memset(v3, 0, sizeof(v3));
   memset(v4, 0, sizeof(v4));

   memcpy(v1, InHexBuff+0, 1);
   memcpy(v2, InHexBuff+1, 1);
   memcpy(v3, InHexBuff+2, 1);
   memcpy(v4, InHexBuff+3, 1);

   if(v1[0] >= 'A') sprintf(v1,"%02d", 10+ (v1[0]-'A'));
   if(v2[0] >= 'A') sprintf(v2,"%02d", 10+ (v2[0]-'A'));
   if(v3[0] >= 'A') sprintf(v3,"%02d", 10+ (v3[0]-'A'));
   if(v4[0] >= 'A') sprintf(v4,"%02d", 10+ (v4[0]-'A'));

   *OutDec = (4096 * atoi(v1)) + (256 * atoi(v2)) + (16* atoi(v3)) + atoi(v4);

   return(OK);

}


 /***********************************************************************
  **  NAME  : command_KQ
  **  DESC  : ARQC Verification and ARPC Generation
  **  INPUT : 
  **  OUTPUT: 
  **  AUTHOR: H.YOUSFI 11/Nov/2003
  ***********************************************************************/
int command_KQ  (  char *sHsmBuffer,
		   char cSchemeId,
		   char *sDmk,
		   char *sPanAndSeq,
		   char *sAtc,
		   char *sUn,
		   int  nDataLen,
		   char *sData,
		   char *sArqc,
		   char *sArc
   		)
{

    int     offset = 0;
    char    sLine[MAX_LINE_TRC];

    trace_event("Start command_KQ()", PROCESSING);


    memcpy(sHsmBuffer + offset, "KQ",   2);
    offset += 2;

    sHsmBuffer[offset] = '1'; /** Mode Flag: 1 = Perform ARQC Verification and ARPC generation **/
    
    offset += 1;

    sHsmBuffer[offset] = cSchemeId ;
    offset += 1;


    memcpy(sHsmBuffer + offset, sDmk,   strlen(sDmk));
    offset += strlen(sDmk);

    memcpy(sHsmBuffer + offset, sPanAndSeq,   8);
    offset += 8;

    memcpy(sHsmBuffer + offset, sAtc,   2);
    offset += 2;

    memcpy(sHsmBuffer + offset, sUn,   4);
    offset += 4;

    sprintf(sHsmBuffer + offset , "%02X", nDataLen);
    offset += 2;

    memcpy(sHsmBuffer + offset, sData,   nDataLen);
    offset += nDataLen;

    memcpy(sHsmBuffer + offset, ";",   1);
    offset += 1;

    memcpy(sHsmBuffer + offset, sArqc,   8);
    offset += 8;

/** MAW20060107
    memcpy(sHsmBuffer + offset, ";",   1);
    offset += 1;
**/

    memcpy(sHsmBuffer + offset, sArc,   2);
    offset += 2;

    sprintf(sLine, "End command_KQ(%d)", offset);
    trace_event(sLine,  PROCESSING);
    return (offset);
}
/******************************************************************************/
/* Fonction command_KR.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/*               resultat de verif ARQC                                       */
/******************************************************************************/
int    command_KR( char *sBuffRcv, 
                   char *sErrCode,
                   char *sArpc
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_KR()",PROCESSING);
	
   memset(Command, 0, sizeof(Command));

   memcpy(Command , sBuffRcv, 2);      
   memcpy(sErrCode, sBuffRcv, 4);   
   if (memcmp( Command , "KR", 2) != 0) 
   {
   		trace_event("End   command_ED(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
   }

   TextError( sErrCode + 2,sLine);
   if (memcmp(sErrCode + 2, "00", 2) == 0)
   {
		memcpy(sArpc, sBuffRcv + 4, 8);

   		trace_event("End   command_KR(OK)",PROCESSING);
		return ( OK );
   }
   if (memcmp(sErrCode + 2, "01", 2) == 0) 
   {
		memcpy(sArpc, sBuffRcv + 4, 8);
   		trace_event("End   command_KR(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
   }
   trace_event("End   command_KR(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);
}


/******************************************************************************/
/* Function command_KW.                                                       */
/* -------------------                                                        */
/* ARQC (or TC/ACC) Verification and/or ARPC Generation (EMV2000)              */
/******************************************************************************/
int command_KW  (  char *sHsmBuffer,
                   char cSchemeId,
                   char *sDmk,
                   char *sPanAndSeq,
                   char *sAtc,
                   char *sUn,
                   int  nDataLen,
                   char *sData,
                   char *sArqc,
                   char *sArc
                )
{

    int     offset = 0;
    char    sLine[MAX_LINE_TRC];

    trace_event("Start command_KW()", PROCESSING);


    memcpy(sHsmBuffer + offset, "KW",   2);
    offset += 2;

    sHsmBuffer[offset] = '1'; /** Mode Flag: 1 = Perform ARQC Verification and ARPC generation **/

    offset += 1;

    sHsmBuffer[offset] = '0' ; /** Identifier for the Scheme: 0 = VIS 1.4.0 or M/Chip 4 **/
    offset += 1;


    memcpy(sHsmBuffer + offset, sDmk,   strlen(sDmk));
    offset += strlen(sDmk);

    memcpy(sHsmBuffer + offset, sPanAndSeq,   8);
    offset += 8;

    sHsmBuffer[offset] = '1' ; /** Branch/Height parameters: 0 = Branch factor 4; Tree Height 8 **/
    offset += 1;

    memcpy(sHsmBuffer + offset, sAtc,   2);
    offset += 2;

    sprintf(sHsmBuffer + offset , "%02X", nDataLen);
    offset += 2;

    memcpy(sHsmBuffer + offset, sData,   nDataLen);
    offset += nDataLen;

    memcpy(sHsmBuffer + offset, ";",   1);
    offset += 1;

    memcpy(sHsmBuffer + offset, sArqc,   8);
    offset += 8;

    memcpy(sHsmBuffer + offset, sArc,   2);
    offset += 2;

    sprintf(sLine, "End command_KW(%d)", offset);
    trace_event(sLine,  PROCESSING);
    return (offset);
}
/******************************************************************************/
/* Fonction command_KX.                                                       */
/* -------------------                                                        */
/******************************************************************************/
int    command_KX( char *sBuffRcv,
                   char *sErrCode,
                   char *sArpc
                 )
{
   char Command[3];
   char sLine[MAX_LINE_TRC];

   trace_event("Start command_KX()",PROCESSING);
       
   memset(Command, 0, sizeof(Command));

   memcpy(Command , sBuffRcv, 2);
   memcpy(sErrCode, sBuffRcv, 4);
   if (memcmp( Command , "KX", 2) != 0)
   {
                trace_event("End   command_KX(ERROR_COMMAND)",PROCESSING);
                return ( ERROR_COMMAND );
   }

   TextError( sErrCode + 2,sLine);
   if (memcmp(sErrCode + 2, "00", 2) == 0)
   {
                memcpy(sArpc, sBuffRcv + 4, 8);

                trace_event("End   command_KX(OK)",PROCESSING);
                return ( OK );
   }
   if (memcmp(sErrCode + 2, "01", 2) == 0)
   {
                memcpy(sArpc, sBuffRcv + 4, 8);
                trace_event("End   command_KX(VERIFICATION_FAILED)",PROCESSING);
                return ( VERIFICATION_FAILED );
   }
   trace_event("End   command_KX(CRYPTOGRAPHIC_ERROR)",PROCESSING);
   return(CRYPTOGRAPHIC_ERROR);
}
