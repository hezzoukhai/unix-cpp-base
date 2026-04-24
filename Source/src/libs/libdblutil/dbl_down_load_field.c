/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      dbl_down_load_field.c                                                      */
/*                                                                             */
/* Description                                                                 */
/*      Load Tools Management                                                 */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>

#include <define.h>
#include <ascii.h>
#include <dbl_protocol.h>
#include <dblmsgcst.h>
#include <atmopcode.h>
#include <account.h>
#include <langue.h>
#include <dbl_param.h>
#include <thr_dbl_extern.h>

/*********************************************************************
void expand_pin(crypt_pin, pin_block)
unsigned char *crypt_pin;
unsigned char *pin_block;
{
int i;
unsigned char a;
unsigned char b;
char detail[MAX_LINE_TRC];
sprintf(detail, "Start expand_pin(%.8s)", pin_block);
trace_event("Start expand_pin()", PROCESSING);

for(i = 0 ; i < 8 ; i++)
{
a = (*(pin_block + i) >> 4);
b = (*(pin_block + i) & 0x0F);
*(crypt_pin + (2 * i)) = a;
*(crypt_pin + (2 * i + 1)) = b;
}

for (i=0;i<16;i++)
{
if ( ( *(crypt_pin + i) >= 0x00 ) && ( *(crypt_pin + i) <= 0x09) )
*(crypt_pin + i) += 0x30;
else
if ( *(crypt_pin + i) == 0x0A ) *(crypt_pin + i) = 0x41;
else
if ( *(crypt_pin + i) == 0x0B ) *(crypt_pin + i) = 0x42;
else
if ( *(crypt_pin + i) == 0x0C ) *(crypt_pin + i) = 0x43;
else
if ( *(crypt_pin + i) == 0X0D ) *(crypt_pin + i) = 0x44;
else
if ( *(crypt_pin + i) == 0X0E ) *(crypt_pin + i) = 0x45;
else
if ( *(crypt_pin + i) == 0X0F ) *(crypt_pin + i) = 0x46;
else *(crypt_pin + i) = 0x00;
}
sprintf(detail, "End expand_pin(%.16s)", crypt_pin);
trace_event(detail, PROCESSING);
return;
}
********************************************************************/
ExpandPIN(pin, pinExpanded)
unsigned char   pin[8];
unsigned char   pinExpanded[16];
{
   char      sLine[MAX_LINE_TRC];
   int      cpt;
   /*
sprintf(sLine, "Start ExpandPIN(%.8s)", pin);
trace_event(sLine, PROCESSING);
*/
   for(cpt = 0; cpt < 8; cpt ++)
   {
      pinExpanded[cpt*2    ] = 0x30 | (pin[cpt] >> 4);
      pinExpanded[cpt*2 + 1] = 0x30 | (pin[cpt] & 0x0F);
   }
   /*
sprintf(sLine, "End   ExpandPIN(%.16s)", pinExpanded);
trace_event(sLine, PROCESSING);
*/
   return (OK);
}

/*********************************************************************/
/*********************************************************************/
CompactPIN(pin, pinCompacted)
unsigned char   pin[8];
unsigned char   pinCompacted[16];
{
   char      sLine[MAX_LINE_TRC];
   int      cpt;

/*sprintf(sLine, "Start CompactPIN(%.16s)", pin);
trace_event(sLine, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("Start CompactPIN()", PROCESSING);

   for(cpt = 0; cpt < 8; cpt ++)
   {
      pinCompacted[cpt] = ((pin[cpt*2   ] & 0x0F) << 4)
          | (pin[cpt*2 + 1] & 0x0F);
   }
/*sprintf(sLine, "End   CompactPIN(%.8s)", pinCompacted);
trace_event(sLine, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("End   CompactPIN(OK)", PROCESSING);
   return (OK);
}


/*********************************************************************/
/*********************************************************************/
int Download_Field(Buffer, Field_Number, Field, Lg)
char *Buffer;
int  Field_Number;
char *Field;
int  Lg;
{
   char      sLine[MAX_LINE_TRC];
   char tmp_buf[MAX_FIELD_LENGTH];
   int  i = 0;
   int  j = 0;
   int  nb_FS = 0;
   int  lg = 0;
   int  len=0;

   sprintf(sLine, "Debut Download_Field(%d)", Field_Number);
   trace_event(sLine, PROCESSING);

   memset(tmp_buf,0,sizeof(tmp_buf));

   while ((nb_FS < Field_Number) && (i < Lg-1))
   {
      if (Buffer[i] == FS) nb_FS += 1;
      i += 1;
   }

   if ((nb_FS < Field_Number) || (i == Lg))
   {
      trace_event("Fin   Download_Field(NOK)", PROCESSING);
      return(NOK);
   }
   /*
DEBUT : MODIFIER PAR Y. OUADI LE 04 JUIN 1998
while ((Buffer[j] != FS)  &&  (j < Lg-1)) j+=1;
FIN   : MODIFIER PAR Y. OUADI LE 04 JUIN 1998
*/
   j = i;
   while ((Buffer[j] != FS)  &&  (j < Lg))
      j+=1;

   memcpy(tmp_buf, Buffer+i, j - i);
   /*len = (int)strlen(tmp_buf);*/
   len = j - i;

   memcpy(Field, tmp_buf, len);

   sprintf(sLine, "Fin   Download_Field(%.30s)", Field);
   trace_event(sLine, PROCESSING);
   return(OK);
}


/******************************************************************************/
/******************************************************************************/
void   Load_Pin_Block(pin_rcv, calcul_pin)
char   *pin_rcv;
char   *calcul_pin;
{
   char      sLine[MAX_LINE_TRC];
   int  i, j;


   trace_event("Debut Load_Pin_Block()", PROCESSING);

   j = 0;
   /*
memcpy(pin_rcv, "473<<>033006643;", 16);
*/

   for(i = 0 ; i < 8 ; i++)
   {
      *(calcul_pin + i) = ((*(pin_rcv+j) & 0x0F)<<4) | ((*(pin_rcv+1+j)&0x0F)&0x0F);
      j += 2;
   }


   trace_event("Fin   Load_Pin_Block()", PROCESSING);
   return;
}

/******************************************************************************/
/* PREPARATION DES CODE REPONSE POUR LE TICKET         */
/******************************************************************************/
int   Load_Response_Label(V_response_code, V_response_label)
int    V_response_code;
char   *V_response_label;
{
   char      sLine[MAX_LINE_TRC];


   trace_event("Fin   Load_Response_Label()", PROCESSING);

   switch(V_response_code)
   {
   case   0   : 
      memcpy(V_response_label, "TRANSACTION BIEN TERMINEE",
          25); 
      break;
   case   1   : 
      memcpy(V_response_label, "SOLDE INSUFFISANT        ",
          25); 
      break;
   case   4   : 
      memcpy(V_response_label, "TRANSACTION BIEN TERMINEE",
          25); 
      break;
   case   100   : 
      memcpy(V_response_label, "TRANSACTION REFUSEE      ",
          25); 
      break;
   case   101   : 
      memcpy(V_response_label, "CARTE EXPIREE            ",
          25); 
      break;
   case   102   : 
      memcpy(V_response_label, "FRAUDE SUSPECTEE         ",
          25); 
      break;
   case   104   : 
      memcpy(V_response_label, "CARTE A USAGE PRIVEE     ",
          25); 
      break;
   case   106   : 
      memcpy(V_response_label, "NOMBRE ERRUR PIN DEPASSE ",
          25); 
      break;
   case   109   : 
      memcpy(V_response_label, "COMMERCANT ERRONE        ",
          25); 
      break;
   case   110   : 
      memcpy(V_response_label, "MONTANT CHOISI ERRONE    ",
          25); 
      break;
   case   111   : 
      memcpy(V_response_label, "NUMERO DE CARTE ERRONE   ",
          25); 
      break;
   case   112   : 
      memcpy(V_response_label, "DONNEES DE PIN REQUISES  ",
          25); 
      break;
   case   116   : 
      memcpy(V_response_label, "FONDS INSUFFISANTS       ",
          25); 
      break;
   case   117   : 
      memcpy(V_response_label, "CODE CONFIDENTIEL ERRONE ",
          25); 
      break;
   case   118   : 
      memcpy(V_response_label, "CARTE NON ENREGISTREE    ",
          25); 
      break;
   case   119   : 
      memcpy(V_response_label, "TRANSACTION NON AUTORISEE",
          25); 
      break;
   case   120   : 
      memcpy(V_response_label, "TRANSACTION NON ADMISE   ",
          25); 
      break;
   case   121   : 
      memcpy(V_response_label, "PLAFOND RETRAIT DEPASSE  ",
          25); 
      break;
   case   122   : 
      memcpy(V_response_label, "VIOLATION DE SECURITE    ",
          25); 
      break;
   case   123   : 
      memcpy(V_response_label, "FREQUENCE RETRAIT DEPASSEE",
          26); 
      break;
   case   125   : 
      memcpy(V_response_label, "CARTE NON EN SERVICE     ",
          25); 
      break;
   case   126   : 
      memcpy(V_response_label, "FORMAT PIN NON VALIDE    ",
          25); 
      break;
   case   127   : 
      memcpy(V_response_label, "LONGUEUR PIN NON VALIDE  ",
          25); 
      break;
   case   128   : 
      memcpy(V_response_label, "ERREUR SYNCHRO DU CLE PIN",
          25); 
      break;
   case   200   : 
      memcpy(V_response_label, "CAPTURE CARTE            ",
          25); 
      break;
   case   201   : 
      memcpy(V_response_label, "CAPTURE CARTE PERIMEE    ",
          25); 
      break;
   case   202   : 
      memcpy(V_response_label, "CAPTURE CARTE MANIPULEE  ",
          25); 
      break;
   case   206   : 
      memcpy(V_response_label, "NOMBRE ERRUR PIN DEPASSE ",
          25); 
      break;
   case   207   : 
      memcpy(V_response_label, "CAPTURE CARTE MANIPULEE  ",
          25); 
      break;
   case   208   : 
      memcpy(V_response_label, "CAPTURE CARTE OPPOSEE    ",
          25); 
      break;
   case   209   : 
      memcpy(V_response_label, "CAPTURE CARTE OPPOSEE    ",
          25); 
      break;
   case   210   : 
      memcpy(V_response_label, "CAPTURE CARTE SOUPCONNEE ",
          25); 
      break;
   case   280   : 
      memcpy(V_response_label, "MONTANT ALTERNATIF ANNULE",
          25); 
      break;
   case   282   : 
      memcpy(V_response_label, "DEMANDE CHEQUIER ANNULEE ",
          25); 
      break;
   case   300   : 
      memcpy(V_response_label, "DONNEES PISTE 3 ERRONEES ",
          25); 
      break;
   case   800   : 
      memcpy(V_response_label, "OPERATION ARRETE EN COURS",
          25); 
      break;
   case   902   : 
      memcpy(V_response_label, "TRANSACTION INVALIDE     ",
          25); 
      break;
   case   909   : 
      memcpy(V_response_label, "ERREUR SYSTEME           ",
          25); 
      break;
   case   912   : 
      memcpy(V_response_label, "EMETTEUR NON DISPONIBLE  ",
          25); 
      break;
   case   994   : 
      memcpy(V_response_label, "TRAITEMENT IMPOSSIBLE    ",
          25); 
      break;
   case   995   : 
      memcpy(V_response_label, "ERREUR TRAITEMENT        ",
          25); 
      break;
   default   : 
      trace_event("Fin   Load_Response_Label()",PROCESSING);
      return(NOK);
   }
   trace_event("Fin   Load_Response_Label()", PROCESSING);
   return(OK);
}


/**************************************************************************/
/**************************************************************************/
void   format_card_number(card_number)
char   *card_number;
{
   char      sLine[MAX_LINE_TRC];

   trace_event("Fin   Format_card_number()", PROCESSING);

   memcpy(card_number, "XXXXXX", 6);
   memcpy(card_number + strlen((char*)card_number) - 1, "X", 1);

   trace_event("Fin   Format_card_number()", PROCESSING);
}

/******************************************************************************/
/******************************************************************************
int   load_response_label(pResponseCode,
                            pCurOp,
                            pNextOp,
                            pLang,
                            pResponseLabel)
int   pResponseCode;
int   pCurOp;
int   pNextOp;
char   pLang;
char   pResponseLabel[];
{
   char      sLine[MAX_LINE_TRC];
int      lResponseLabelLen;
int      cpt;

sprintf(sLine, "Start load_response_label(%d,%d,%d,%c)",
                 pResponseCode, pCurOp, pNextOp, pLang);
trace_event(sLine, PROCESSING);

    lResponseLabelLen = 0;

    for(cpt = 0; cpt < RESPONSE_TAB_SIZE; cpt ++)
    {
        if(response_tab[cpt].response_code   == pResponseCode
        && response_tab[cpt].cur_op      == pCurOp
        && response_tab[cpt].next_op      == pNextOp
          )
        {
            switch(pLang)
            {
            case ENGLISH :
                lResponseLabelLen = strlen(response_tab[cpt].response_label_e);
                memcpy(pResponseLabel,
                       response_tab[cpt].response_label_e,
                       lResponseLabelLen);
                break;
            case ARABIC :
                lResponseLabelLen = strlen(response_tab[cpt].response_label_a);
                memcpy(pResponseLabel,
                       response_tab[cpt].response_label_a,
                       lResponseLabelLen);
                break;
            case FRENSH :
                lResponseLabelLen = strlen(response_tab[cpt].response_label_f);
                memcpy(pResponseLabel,
                       response_tab[cpt].response_label_f,
                       lResponseLabelLen);
                break;
            default :
                lResponseLabelLen = strlen(response_tab[cpt].response_label_e);
                memcpy(pResponseLabel,
                       response_tab[cpt].response_label_e,
                       lResponseLabelLen);
                break;
            }

            break;   ** pour sortir de for **
        }
    }

    if(cpt == RESPONSE_TAB_SIZE)
    {
      switch(pLang)
      {
        case ARABIC   :
            memcpy(pResponseLabel, "QZ&& T#E(V\" ^ZZQX_ V", 20);
            lResponseLabelLen = 20;
            break;
        case ENGLISH   :
            memcpy(pResponseLabel, "WE CAN NOT CONTACT YOUR BANK", 28);
            lResponseLabelLen = 28;
            break;
        case FRENSH   :
            memcpy(pResponseLabel, "IMPOSSIBLE DE CONSULTER VOTRE BANQUE", 36);
            lResponseLabelLen = 36;
            break;
        default      :
            memcpy(pResponseLabel, "WE CAN NOT CONTACT YOUR BANK", 28);
            lResponseLabelLen = 28;
            break;
      }
    }


trace_event(pResponseLabel, DATA);
sprintf(sLine, "End   load_response_label(%d)", lResponseLabelLen);
trace_event(sLine, PROCESSING);
return (lResponseLabelLen);
}
******************************************************************************/

/******************************************************************************/
/******************************************************************************
int load_screen_number(response_code, cur_op, next_op, screen_number)
int   response_code;
int   cur_op;
int   next_op;
char   screen_number[4];
{
int      cpt;

sprintf(sLine, "Start load_screen_number(%d,%d,%d)",
                 response_code, cur_op, next_op);
trace_event(sLine, PROCESSING);

    memcpy(screen_number, "057", 4);

    for(cpt = 0; cpt < RESPONSE_TAB_SIZE; cpt ++)
    {
        if(response_tab[cpt].response_code   == response_code
        && response_tab[cpt].cur_op      == cur_op
        && response_tab[cpt].next_op      == next_op
          )
        {
            memcpy(screen_number, response_tab[cpt].screen_number, 3);
            break;
        }
    }

sprintf(sLine, "End   load_screen_number(OK,%.3s)", screen_number);
trace_event(sLine, PROCESSING);
return (OK);
}
******************************************************************************/

/******************************************************************************/
/******************************************************************************/
OperationApproved(responseCode)
int      responseCode;
{
   char      sLine[MAX_LINE_TRC];
   int      retour;

   switch(responseCode)
   {
   case 0 :
   case 1 :
   case 4 :
   case 680 :
      retour = OK;
      break;
   default :
      retour = NOK;
   }

   return (retour);

}

/******************************************************************************/
/******************************************************************************/
WithdrawalOperation(operationCode)
int      operationCode;
{
   char      sLine[MAX_LINE_TRC];
   int      retour;

   switch(operationCode)
   {
   case OR_FAST_CASH :
   case OR_ONUS_WITHDRAWAL :
   case OR_CASH_US :
   case OR_AUTO_CASH :
      retour = OK;
      break;
   default :
      retour = NOK;
   }

   return (retour);

}

/******************************************************************************/
/******************************************************************************
int     Load_Next_Stat(V_response_code, V_cur_op, V_next_op, V_Next_Stat)
int      V_response_code;
int      V_cur_op;
int      V_next_op;
char    *V_Next_Stat;
{
int      cpt;

sprintf(sLine, "Start Load_Next_Stat(%d,%d,%d)",
                 V_response_code, V_cur_op, V_next_op);
trace_event(sLine, PROCESSING);

    memcpy(V_Next_Stat, "411", 3);

    for(cpt = 0; cpt < RESPONSE_TAB_SIZE; cpt ++)
    {
        if(response_tab[cpt].response_code   == V_response_code
        && response_tab[cpt].cur_op      == V_cur_op
        && response_tab[cpt].next_op      == V_next_op
          )
        {
            memcpy(V_Next_Stat, response_tab[cpt].next_state, 3);
            break;
        }
    }

sprintf(sLine, "End   Load_Next_Stat(%.3s)", V_Next_Stat);
trace_event(sLine, PROCESSING);
return(OK);
}
**************************************************************************/

/**************************************************************************/
/**************************************************************************/
void   Load_Balance_Amount(add_amount, bal_amount, currency)
char   *add_amount;
char   *bal_amount;
char   *currency;
{
   char      sLine[MAX_LINE_TRC];
   char   tmp_solde[15];

   trace_event("Debut Load_Balance_Amount()", PROCESSING);

   memset(tmp_solde, 0, sizeof(tmp_solde));

   memcpy(tmp_solde, add_amount + 8 + 3, 12);

   /* Debut modif : Y. OUADI 26 Aou 1998 */
   /* Motif modif : conversion du solde en monnaie mineur */
   adjust_amount(tmp_solde, currency);
   /* Fin   modif : Y. OUADI 26 Aou 1998 */


   if ( add_amount[7 + 3] == 'D' )
      memcpy(bal_amount, "-", 1);
   else memcpy(bal_amount, "+", 1);

   memcpy(bal_amount + 1, tmp_solde, 11);

   trace_event("Fin   Load_Balance_Amount()", PROCESSING);
   return;
}

int GetAccountWording(nIndexCtx,pAccType, pAccWording)
char     pAccType   [ 3];
char     pAccWording   [21];
{
   char      sLine[MAX_LINE_TRC];
   int      cpt;
   int      retour;

   sprintf(sLine, "Start GetAccountWording(%.2s)", pAccType);
   trace_event(sLine, PROCESSING);
   retour = NOK;

   memset(pAccWording,      0, 21);

   for(cpt = 0; cpt < 8; cpt++)
   {
      if(memcmp(pAccType, tab_DblCtx[nIndexCtx].gAccountList[cpt].accountType, 2) == 0)
      {
         memcpy(pAccWording, tab_DblCtx[nIndexCtx].gAccountList[cpt].accountWording,
                strlen(tab_DblCtx[nIndexCtx].gAccountList[cpt].accountWording));
         retour = OK;
         break;
      }
   }
   sprintf(sLine, "End   GetAccountWording(%d,%.20s)", retour, pAccWording);
   trace_event(sLine, PROCESSING);
   return (retour);
}

