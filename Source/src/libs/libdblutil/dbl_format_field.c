/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      format_field.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      Format Field Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <ascii.h>
#include <dbl_paramdef.h>
#include <langue.h>
#include <atmcutoff.h>

#define   PRN_WIDTH   40

int     FormatToDate(char *date,char lang);

/*******************************************************************************
**  nom    : FormatToStatement(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB DIEBOLD.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/

int     FormatToStatement(nIndexCtx,record_data, statement, currency, lang)
int     nIndexCtx;
char    record_data     [512];
char    statement       [512];
char    currency        [  4];
char    lang;
{
   char            detail[MAX_LINE_TRC];
   char            transaction   [1024];
   char            date            [ 256];
   char            amount      [ 41];
   char            balance      [ 41];
   char            mm      [  3];
   char            dd      [  3];
   char            fmt[56], trx_e_label[24], trx_a_label[24], trx_sign[2];
   int             len;
   int             offset;
   int             trans_nbr;
   int             cpt;

   sprintf(detail, "Start FormatToStatement(%.3s,%c)", currency, lang);
   trace_event(detail, PROCESSING);

   memcpy(statement, record_data, strlen(record_data));
   return (OK);

   /* - Raz des variables ------------------------------------------------- */
   memset(statement,           0, sizeof(statement));

   /* - nombre de lignes du releve ---------------------------------------- */
   sscanf(record_data, "%2d", &trans_nbr);

   /* - control de la langueur -------------------------------------------- */
   if ( strlen(record_data) < NP_SHORT_STAT_LEN*trans_nbr + 2)
   {
      trace_event("End   FormatToStatement(LENGTH ERROR)", PROCESSING);
      return (NOK);
   }

   /* - formatage du releve ----------------------------------------------- */
   offset = 0;
   for(cpt = 0; cpt < trans_nbr; cpt ++)
   {
      /* - Raz des variables ----------------------------------------- */
      memset(transaction,   0, sizeof(transaction)   );
      memset(date,      0, sizeof(date)      );
      memset(amount,      0, sizeof(amount)   );
      memset(balance,      0, sizeof(balance)   );

      /* - formatage d'une ligne ------------------------------------ */
      memcpy(transaction, record_data + 2 + NP_SHORT_STAT_LEN*cpt, NP_SHORT_STAT_LEN);
       
          /*trace*/
      sprintf(detail, "<--transaction(cpt = %d)--> %s",cpt, transaction);
      trace_event(detail, PROCESSING);

      if(lang == ARABIC)
      {
	
	/*Ajout d'une  marge de deux espaces pour l'affichage des transactions*/
	memcpy(statement + offset, "     ", 5);
	offset += 5;

         memset(fmt,         0, sizeof(fmt)        );
         memset(trx_e_label, 0, sizeof(trx_e_label));
         memset(trx_a_label, 0, sizeof(trx_a_label));
         memset(trx_sign,    0, sizeof(trx_sign)   );

         /* YOua 13Avr2004 : Spec Ahli */

         /* - montant transaction ------------------- */
        /*BEGIN AHM 26/03/2006*/
        /*memcpy(statement + offset, transaction + NP_AMT_POS + 1, 8);
        offset+=8;
	*/
        memcpy(amount, transaction + NP_AMT_POS + 1, 11);
        len = FormatToAmount(nIndexCtx, amount, currency, NOK, 'R', lang);
        memcpy(statement + offset,   amount, len);
         offset += len;
        /*END AHM*/

         /*statement[offset] = ESC;            offset+=1;
         statement[offset] = 'z';            offset+=1;
         statement[offset] = '1';            offset+=1;
         statement[offset] = '@';            offset+=1;

         statement[offset] = ESC;            offset+=1;
         statement[offset] = 'z';            offset+=1;
         statement[offset] = '1';            offset+=1;
         memcpy(statement + offset, transaction + NP_AMT_POS + 4 + 8 + 1, 2);
         offset+=2;
	 */

         /* - signe de la transaction --------------- */
         statement[offset] = ESC;            offset+=1;
         statement[offset] = 'z';            offset+=1;
         statement[offset] = '0';            offset+=1;

         if(transaction[NP_AMT_SIGN_POS] == 'D')
         {
            memcpy(statement + offset,      "-",  1);   offset+= 1;
         }
         else
         {
            memcpy(statement + offset,      " ",  1);   offset+= 1;
         }

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       "   ", 3);
         offset+= 3;

         /* YOua 13Apr2004 : Arabic Labels Support */
         /* ---- Transaction Label ---------------------*/


         trx_sign[0] = transaction[NP_AMT_SIGN_POS];
         /*memcpy(trx_e_label, transaction + NP_LABEL_POS, NP_LABEL_LEN - 3);*/
         memcpy(trx_e_label, transaction + NP_LABEL_POS, NP_LABEL_LEN);
         if (GetStatementLabel(nIndexCtx, "A", trx_e_label, trx_a_label) != OK)
         {
         	statement[offset] = ESC;            offset+=1;
         	statement[offset] = 'z';            offset+=1;
         	statement[offset] = '0';            offset+=1;
		memcpy(trx_a_label, trx_e_label, NP_LABEL_LEN  + 15);
         	sprintf(fmt, "%%%d.%ds", NP_LABEL_LEN + 15, NP_LABEL_LEN + 15);
         	sprintf(statement + offset, fmt, trx_a_label);
         	offset+=(NP_LABEL_LEN + 15);
         	statement[offset] = ESC;            offset+=1;
         	statement[offset] = 'z';            offset+=1;
         	statement[offset] = '1';            offset+=1;
	 }
	 else
	 {
         	statement[offset] = ESC;            offset+=1;
         	statement[offset] = 'z';            offset+=1;
         	statement[offset] = '1';            offset+=1;
         	/*sprintf(fmt, "%%%d.%ds", NP_LABEL_LEN - 3, NP_LABEL_LEN - 3);*/
         	sprintf(fmt, "%%%d.%ds", NP_LABEL_LEN + 15, NP_LABEL_LEN + 15);
         	sprintf(statement + offset, fmt, trx_a_label);
         	/*offset+=(NP_LABEL_LEN- 3 );*/
         	offset+=(NP_LABEL_LEN + 15);
	 }



         /* - separateur ---------------------------- */
         memcpy(statement + offset,       "   ", 3);
         offset+= 3;
         /* YOua 13Apr2004 : Arabic Labels Support */

         /* - date transaction ---------------------- */
         memcpy(date,   transaction + NP_DATE_POS,  NP_DATE_LEN);
         len = FormatToDate(date, lang);
         memcpy(statement + offset,    date  , len);
         offset+= len;
      }
      else
      {
	/*Ajout d'une  marge de deux espaces pour l'affichage des transactions*/
	memcpy(statement + offset, "  ", 2);
	offset += 2;

         /* - date transaction ---------------------- */
         memcpy(date,   transaction + NP_DATE_POS,  NP_DATE_LEN);
         FormatToDate(date, lang);
         memcpy(statement + offset,      date, 8);
         offset+= 8;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       "   ", 3);
         offset+= 3;

         /* ---- Transaction Label ---------------------*/
         memcpy(trx_e_label, transaction + NP_LABEL_POS, NP_LABEL_LEN);
	 memset(trx_a_label, ' ', 20);
	
         if (GetStatementLabel(nIndexCtx, "E", trx_e_label, trx_a_label) != OK)
             memcpy(trx_a_label, trx_e_label, NP_LABEL_LEN + 15);
            /* memset(trx_a_label, ' ', NP_LABEL_LEN - 3); */


         sprintf(fmt, "%%%d.%ds", NP_LABEL_LEN + 15, NP_LABEL_LEN + 15);
         sprintf(statement + offset, fmt, trx_a_label);
         offset += NP_LABEL_LEN + 15;
/*
         memcpy(statement + offset, transaction + NP_LABEL_POS, NP_LABEL_LEN - 3);
         offset+=NP_LABEL_LEN - 3 ;
*/


         /* - separateur ---------------------------- */
         memcpy(statement + offset,       "   ", 3);
         offset+= 3;

         /* - montant transaction ------------------- */
        /*
         memcpy(amount, transaction + NP_AMT_POS, NP_AMT_LEN);
         len = Format2ToAmount(nIndexCtx,amount, currency, NOK, 'R', lang);
         memcpy(statement + offset,   amount + len - 12, 12);
        */
/*
         memcpy(statement + offset, transaction + NP_AMT_POS, NP_AMT_LEN);
         offset+=12;
*/
	/*BEGIN AHM 26/03/2006*/
        /*memcpy(statement + offset, transaction + NP_AMT_POS + 1, 11);*/
	memcpy(amount, transaction + NP_AMT_POS, 12);
	len = FormatToAmount(nIndexCtx, amount, currency, NOK, 'R', lang);
	memcpy(statement + offset,   amount + len - 12, 12);
         offset+=12;
	/*END AHM*/

         /* - signe de la transaction --------------- */
         if(transaction[NP_AMT_SIGN_POS] == 'D')
         {
            memcpy(statement + offset,      "-",  1);
            offset+= 1;
         }
         else
         {
            memcpy(statement + offset,      " ",  1);
            offset+= 1;
         }

         /* YOua 13Avr2004 : Spec Ahli */
         /*
         ** - separateur ---------------------------- **
         memcpy(statement + offset,       " ", 1);
         offset+= 1;

         ** - balance ------------------------------- **
         memcpy(balance,   transaction + NP_BAL_POS, NP_BAL_LEN);
         len = Format2ToAmount(nIndexCtx,balance, currency, NOK, 'R', lang);
         memcpy(statement + offset,   balance+len-12, 12);
         offset+=12;

         ** - signe de la transaction --------------- **
                 if(transaction[NP_BAL_SIGN_POS] == 'D')
         {
            memcpy(statement + offset,      "-",  1);
            offset+= 1;
         }

         else
         {
            memcpy(statement + offset,      " ",  1);
            offset+= 1;
         }
         */
         /* YOua 13Avr2004 : Spec Ahli */
      }

      /* - saut de ligne ------------------------- */
      statement[offset] = LF;
      offset+= 1;
   }

   trace_event("End   FormatToStatement(OK)", PROCESSING);
   return (OK);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToDateTime(date,lang)
char            date[36];
char      lang;
{
   char            detail[MAX_LINE_TRC];
   char            target[36];
   int      offset;

   offset = 0;

   memcpy(target + offset, date + 4,       2);             
   offset+=2;
   memcpy(target + offset, "/",            1);             
   offset+=1;
   memcpy(target + offset, date + 2,       2);             
   offset+=2;
   memcpy(target + offset, "/",            1);             
   offset+=1;
   memcpy(target + offset, date + 0,       2);             
   offset+=2;

   memcpy(target + offset, "  ",           2);             
   offset+=2;

   memcpy(target + offset, date + 6,       2);             
   offset+=2;
   memcpy(target + offset, ":",            1);             
   offset+=1;
   memcpy(target + offset, date + 8,       2);             
   offset+=2;
   memcpy(target + offset, ":",            1);             
   offset+=1;
   memcpy(target + offset, date + 10,      2);             
   offset+=2;

   memcpy(date, target, offset);

   return (offset);
}

/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToDate(char *date,char lang)
{
   char            detail[MAX_LINE_TRC];
   char            sLine[MAX_LINE_TRC];
   char            target[256];
   int      offset;


   sprintf(sLine,"Start FormatToDate(%s,%c)", date,lang);
   trace_event(sLine,PROCESSING);

   memset(target,      0, sizeof(target));         
   offset =0;

   if(lang == ARABIC)
   {
      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z1",      2);      
      offset+=2;
      memcpy(target + offset,  date + 0, 2);         
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z0",      2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z1",      2);      
      offset+=2;
      memcpy(target + offset,   date + 2,   2);      
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z0",      2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z1",      2);      
      offset+=2;
      memcpy(target + offset,   date + 4,   2);      
      offset+=2;
   }
   else
   {
      /* Debut S.A. Statement JJ/MM  */
      memcpy(target + offset,   date + 4,   2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;
      memcpy(target + offset,   date + 2,   2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;
      memcpy(target + offset,   date + 0,   2);      
      offset+=2;
      /* Fin Modif S.A. Statement Date */
   }

   memcpy(date, target, offset);

   sprintf(sLine,"End   FormatToDate(%d)", offset);
   trace_event(sLine,PROCESSING);
   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToHour(hour,lang)
char            hour[12];
char      lang;
{
   char            sLine[MAX_LINE_TRC];
   char            target[24];
   int      offset;

   sprintf(sLine, "Start FormatToHour(%.6s,%c)", hour, lang);
   trace_event(sLine, PROCESSING);
   memset(target,      0, sizeof(target));         
   offset =0;

   if(lang == ARABIC)
   {
      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "z1",      2);      
      offset+=2;
      memcpy(target + offset,  hour + 0, 2);         
      offset+=2;

      memcpy(target + offset,   "S",      1);      
      offset+=1;

      memcpy(target + offset,   hour + 2,   2);      
      offset+=2;

      memcpy(target + offset,   "S",      1);      
      offset+=1;

      memcpy(target + offset,   hour + 4,   2);      
      offset+=2;
   }
   else
   {
      memcpy(target + offset,   hour + 0,   2);      
      offset+=2;
      memcpy(target + offset,   ":",      1);      
      offset+=1;
      memcpy(target + offset,   hour + 2,   2);      
      offset+=2;
      memcpy(target + offset,   ":",      1);      
      offset+=1;
      memcpy(target + offset,   hour + 4,   2);      
      offset+=2;
   }

   memcpy(hour, target, offset);

   trace_event("End   FormatToHour", PROCESSING);
   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToCardNbr(card_nbr, align, lang)
char            card_nbr[23];
char      align;
char      lang;
{
   char            detail[MAX_LINE_TRC];
   char            target[23];

   memset(target, 0, sizeof(target));


   memset(target, ' ', 22);

   if(align == 'R' || align == 'r')
      memcpy(target + 22 - strlen(card_nbr), card_nbr, strlen(card_nbr));
   else if(align == 'C' || align == 'c')
      memcpy(target + 11 - strlen(card_nbr)/2, card_nbr, strlen(card_nbr));
   else
      memcpy(target, card_nbr, strlen(card_nbr));


   memcpy(card_nbr, target, strlen(target));

   return (22);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToAccountNbr(account_nbr, alignement,lang)
char   account_nbr[21];
char   alignement;
char   lang;
{
   char            detail[MAX_LINE_TRC];
   char            target[21];

   memset(target, 0, sizeof(target));

   /* - formatage ------------------------------------------------------ */
   /* Debut : Younes OUADI le 29 Avril 1999         **
    ** Motif : pas de formatage               **
    memcpy(target + 0,   account_nbr + 0,   2);
    memcpy(target + 2,   " ",         1);
    memcpy(target + 3,   account_nbr + 2,   2);
    memcpy(target + 5,   " ",         1);
    memcpy(target + 6,   account_nbr + 4,   5);
    memcpy(target +11,   " ",         1);
    memcpy(target +12,   account_nbr + 9,   1);
    ** Fin   : Younes OUADI le 29 Avril 1999         */
   memcpy(target, account_nbr, strlen(account_nbr));

   /* - alignement ----------------------------------------------------- */
   memset(account_nbr, 0, 21);
   memset(account_nbr, ' ', 20);
   if(alignement == 'R' || alignement == 'r')
      memcpy(account_nbr + 20 - strlen(target), target, strlen(target));
   else if(alignement == 'C' || alignement == 'c')
      memcpy(account_nbr + 10 - strlen(target)/2, target, strlen(target));
   else
      memcpy(account_nbr, target, strlen(target));

   return (strlen(account_nbr));
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToAmount(nIndexCtx,amount, currency, currency_flag, alignement,lang)
int	   nIndexCtx;
char   amount  [21];
char   currency[ 4];
int   currency_flag;
int   alignement;
char   lang;
{
   char            detail[MAX_LINE_TRC];
   char            target   [99];
   char            cur_alpha[ 4];
   char      	   lIntegerPart   [13];
   char      	   lDecimalPart   [13];
   int             offset;
   int      	   lEscLen;
   int             retour;
   char             sLine                   [MAX_LINE_TRC];

   sprintf(detail, "Start FormatToAmount(%.20s,%.3s,%d,%d,%c)", amount, currency, currency_flag, alignement, lang);
   trace_event(detail, PROCESSING);

   memset(target,      0, sizeof(target)   );
   memset(lIntegerPart,   0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,   0, sizeof(lDecimalPart)   );
   offset = 0;
   lEscLen = 0;

   retour = GetCurrencyAlpha(nIndexCtx,currency, cur_alpha);
   if(retour == OK)
   {
       retour = adjust_amount_float(nIndexCtx,amount, currency);
      /* - formatage ------------------------------------------------------ */
      if (currency_flag == OK)
      {
         if (retour == OK)
         {
            if(lang == ARABIC)
            {
               if(memcmp(cur_alpha, "KWD", 3) == 0)
               {
                  memcpy(target + offset,   " P>", 3);   
                  offset+=3;
               }
               else
                  if(memcmp(cur_alpha, "TND", 3) == 0)
                  {
                     memcpy(target + offset,   " (>", 3);   
                     offset+=3;
                  }
                  else
                     if(memcmp(cur_alpha, "USD", 3) == 0)
                     {
                        /*memcpy(target + offset,   " \">", 3);   
                        offset+=3;*/
                        memcpy(target + offset,   "@V[>", 4);   
                        offset+=4;
                     } else {
			/*BEGIN AHM 30/08/2005: pour afficher le mot dirham en arabe sur le ticket*/
			/*target[offset] = 'g'; offset+=1;				
			target[offset] = '_'; offset+=1;				
			target[offset] = 'Z'; offset+=1;				
			target[offset] = ','; offset+=1;				
			*/
			/*END AHM*/
		     }
            }
            else
            {
               	memcpy(target + offset, cur_alpha, 3);   
               	offset+=3;
            }
         }
         else
         {
            memcpy(target + offset, currency, 3);      
            offset+=3;
         }

         /* - separateur ------------- */
         memcpy(target + offset, " ", 1);         
         offset+=1;
      }

      if(lang == ARABIC)
      {
         GetIntegerPart(amount, lIntegerPart);
         GetDecimalPart(amount, lDecimalPart);

         target[offset] = ESC;          
         offset+=1;
         target[offset] = 'z';
         offset+=1;
         target[offset] = '1';        
         offset+=1;

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = ESC;            
         offset+=1;
         target[offset] = 'z';
         offset+=1;
         target[offset] = '1';            
         offset+=1;
	 

         target[offset] = '@';            
         offset+=1;

         target[offset] = ESC;            
         offset+=1;
         target[offset] = 'z';
         offset+=1;
         target[offset] = '1';            
         offset+=1;

         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);

         lEscLen += 9;
      }
      else
      {
	 /*BEGIN AHM 03/03/2006*/
         /*sprintf(target + offset, "%.16s", amount);*/
         GetIntegerPart(amount, lIntegerPart);
         GetDecimalPart(amount, lDecimalPart);

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = '.';            
         offset+=1;

         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);
	 /*END AHM*/
      }
   }
   else
   {
      sprintf(target + offset, "%012d", atof(amount));
   }

   /* - alignement ----------------------------------------------------- */
   memset(amount, 0, 21);
   if(alignement == 'R' || alignement == 'r')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 18 + lEscLen);
         memcpy(amount + 18 + lEscLen - strlen(target),
             target,
             strlen(target));
      }
      else
      {
         memset(amount, ' ', 14 + lEscLen);
         memcpy(amount + 14  + lEscLen - strlen(target),
             target,
             strlen(target));
      }
   }
   else if(alignement == 'C' || alignement == 'c')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 9);
         memcpy(amount + 9 - strlen(target)/2, target, strlen(target));
      }
      else
      {
         memset(amount, ' ', 7);
         memcpy(amount + 7 - strlen(target)/2, target, strlen(target));
      }
   }
   else if(alignement == 'L' || alignement == 'l')
   {
      memcpy(amount, target, strlen(target));
   }
   else
   {
      memcpy(amount, target, strlen(target));
   }
   offset = strlen(amount);

   sprintf(detail, "End FormatToAmount(%d)", offset);
   trace_event(detail, PROCESSING);
   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     Format2ToAmount(nIndexCtx,amount, currency, currency_flag, alignement,lang)
int        nIndexCtx;
char   amount  [21];
char   currency[ 4];
int   currency_flag;
int   alignement;
char   lang;
{
   char            detail[MAX_LINE_TRC];
   char            target   [99];
   char            cur_alpha[ 4];
   char            lIntegerPart   [13];
   char            lDecimalPart   [13];
   int             offset;
   int             lEscLen;
   int             retour;
   char             sLine                   [MAX_LINE_TRC];

   sprintf(detail, "Start Format2ToAmount(%.20s,%.3s,%d,%d)", amount, currency, currency_flag, alignement);
   trace_event(detail, PROCESSING);

   memset(target,      0, sizeof(target)   );
   memset(lIntegerPart,   0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,   0, sizeof(lDecimalPart)   );
   offset = 0;
   lEscLen = 0;

  retour = GetCurrencyAlpha(nIndexCtx,currency, cur_alpha);
   retour = adjust_amount_float(nIndexCtx,amount, currency);
   if(retour == OK)
   {
      /* - formatage ------------------------------------------------------ */
      if (currency_flag == OK)
      {
         if (retour == OK)
         {
            if(lang == ARABIC)
            {
               if(memcmp(cur_alpha, "KWD", 3) == 0)
               {
                  memcpy(target + offset,   " P>", 3);
                  offset+=3;
               }
               else
                  if(memcmp(cur_alpha, "TND", 3) == 0)
                  {
                     memcpy(target + offset,   " (>", 3);
                     offset+=3;
                  }

                  else
                     	if(memcmp(cur_alpha, "USD", 3) == 0) {
                        	/*memcpy(target + offset,   " \">", 3);
                        	offset+=3;*/
                        	memcpy(target + offset,   "@V[>", 4);
                        	offset+=4;
                     	} else {
				/*BEGIN AHM 30/08/2005 Pour afficher le mot diraham en arabe sur l'ecran*/
				/*target[offset] = ESC; offset+=1;
				target[offset] = 'G'; offset+=1;
				target[offset] = CHARACTER_SET_ARAB; offset+=1;
				target[offset] = '0'; offset+=1;

				target[offset] = '&'; offset+=1;
				target[offset] = ; offset+=1;
				target[offset] = 'H'; offset+=1;
				target[offset] = VT; offset+=1;
				target[offset] = '\;'; offset+=1;
				target[offset] = VT; offset+=1;
				target[offset] = '-'; offset+=1;
				*/
		     	}
            }
            else
            {
                memcpy(target + offset, cur_alpha, 3);
                offset+=3;
            }
         }
         else
         {
            memcpy(target + offset, currency, 3);
            offset+=3;
         }

         /* - separateur ------------- */
         memcpy(target + offset, " ", 1);
         offset+=1;
      }

      if(lang == ARABIC)
      {
         GetIntegerPart(amount, lIntegerPart);
         GetDecimalPart(amount, lDecimalPart);

         target[offset] = ESC;
         offset+=1;
         target[offset] = 'G';
         offset+=1;
         target[offset] = CHARACTER_SET_ARAB;
         offset+=1;
         target[offset] = '0';
         offset+=1;

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = ESC;
         offset+=1;
         target[offset] = 'G';
         offset+=1;
         target[offset] = '0';
         offset+=1;
         target[offset] = '0';
         offset+=1;


         target[offset] = '.';
         offset+=1;

         target[offset] = ESC;
         offset+=1;
         target[offset] = 'G';
         offset+=1;
         target[offset] = CHARACTER_SET_ARAB;
         offset+=1;
         target[offset] = '0';
         offset+=1;

         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);

         lEscLen += 6;
      }
      else
      {
	/*BEGIN AHM 03/03/2006*/
         /*sprintf(target + offset, "%.16s", amount);*/
         GetIntegerPart(amount, lIntegerPart);
         GetDecimalPart(amount, lDecimalPart);

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = '.';
         offset+=1;

         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);

	/*END AHM*/
      }
   }
   else
   {
      sprintf(target + offset, "%012d", atof(amount));
   }

   /* - alignement ----------------------------------------------------- */
   memset(amount, 0, 21);
   if(alignement == 'R' || alignement == 'r')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 18 + lEscLen);
         memcpy(amount + 18 + lEscLen - strlen(target),
             target,
             strlen(target));
      }
      else
      {
         memset(amount, ' ', 14 + lEscLen);
         memcpy(amount + 14  + lEscLen - strlen(target),
             target,
             strlen(target));
      }
   }

   else if(alignement == 'C' || alignement == 'c')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 9);
         memcpy(amount + 9 - strlen(target)/2, target, strlen(target));
      }
      else
      {
         memset(amount, ' ', 7);
         memcpy(amount + 7 - strlen(target)/2, target, strlen(target));
      }
   }
   else if(alignement == 'L' || alignement == 'l')
   {
      memcpy(amount, target, strlen(target));
   }
   else
   {
      memcpy(amount, target, strlen(target));
   }
   offset = strlen(amount);

   sprintf(detail, "End Format2ToAmount(%d)", offset);
   trace_event(detail, PROCESSING);
   return (offset);
}





/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     Format2ToAmount_1(nIndexCtx,amount, currency, currency_flag, alignement,lang)
int	   nIndexCtx;
char   amount  [21];
char   currency[ 4];
int   currency_flag;
int   alignement;
char   lang;
{
   char            detail[MAX_LINE_TRC];
   char            target   [99];
   char            cur_alpha[ 4];
   char      lIntegerPart   [13];
   char      lDecimalPart   [13];
   double      amt;
   int             offset;
   int      lEscLen;
   int             retour;

   memset(target,      0, sizeof(target)   );
   memset(lIntegerPart,   0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,   0, sizeof(lDecimalPart)   );
   offset = 0;
   lEscLen = 0;

   amt = atof(amount);
   amount_to_iso(&amt, currency);
   sprintf(amount, "%012.0f", amt);

   retour = adjust_amount_float(nIndexCtx,amount, currency);
   if(retour == OK)
   {
      /* - formatage ------------------------------------------------------ */
      if (currency_flag == OK)
      {
         retour = GetCurrencyAlpha(currency, cur_alpha);
         if (retour == OK)
         {
            if(lang == ARABIC)
            {
               if(memcmp(cur_alpha, "KWD", 3) == 0)
               {
                  memcpy(target + offset,   " P>", 3);   
                  offset+=3;
               }
               else
                  if(memcmp(cur_alpha, "TND", 3) == 0)
                  {
                     memcpy(target + offset,   " (>", 3);   
                     offset+=3;
                  }
                  else
                     if(memcmp(cur_alpha, "USD", 3) == 0)
                     {
                        memcpy(target + offset,   " \">", 3);   
                        offset+=3;
                     } 
            }
            else
            {
               memcpy(target + offset, cur_alpha, 3);   
               offset+=3;
            }
         }
         else
         {
            memcpy(target + offset, currency, 3);      
            offset+=3;
         }

         /* - separateur ------------- */
         memcpy(target + offset, " ", 1);         
         offset+=1;
      }

      if(lang == ARABIC)
      {
         GetIntegerPart(amount, lIntegerPart);
         GetDecimalPart(amount, lDecimalPart);

         target[offset] = ESC;            
         offset+=1;
         target[offset] = 'G';            
         offset+=1;
         target[offset] = CHARACTER_SET_ARAB;            
         offset+=1;
         target[offset] = '0';            
         offset+=1;

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = ESC;            
         offset+=1;
         target[offset] = 'G';            
         offset+=1;
         target[offset] = '0';            
         offset+=1;
         target[offset] = '0';            
         offset+=1;

         target[offset] = '.';            
         offset+=1;

         target[offset] = ESC;            
         offset+=1;
         target[offset] = 'G';            
         offset+=1;
         target[offset] = CHARACTER_SET_ARAB;            
         offset+=1;
         target[offset] = '0';            
         offset+=1;

         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);

         lEscLen += 6;
      }
      else
      {
         sprintf(target + offset, "%.16s", amount);
      }
   }
   else
   {
      sprintf(target + offset, "%01.0f", atof(amount));
   }

   /* - alignement ----------------------------------------------------- */
   memset(amount, 0, 21);
   if(alignement == 'R' || alignement == 'r')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 20 + lEscLen);
         memcpy(amount + 20 + lEscLen - strlen(target),
             target,
             strlen(target));
      }
      else
      {
         memset(amount, ' ', 16 + lEscLen);
         memcpy(amount + 16  + lEscLen - strlen(target),
             target,
             strlen(target));
      }
   }
   else if(alignement == 'C' || alignement == 'c')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 10);
         memcpy(amount + 10 - strlen(target)/2, target, strlen(target));
      }
      else
      {
         memset(amount, ' ', 8);
         memcpy(amount + 8 - strlen(target)/2, target, strlen(target));
      }
   }
   else if(alignement == 'L' || alignement == 'l')
   {
      memcpy(amount, target, strlen(target));
   }
   else
   {
      memcpy(amount, target, strlen(target));
   }
   offset = strlen(amount);

   return (offset);
}



/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToRate(rate, alignement,lang)
char            rate[128];
char            alignement;
char            lang;
{
   char            detail[MAX_LINE_TRC];
   char            target          [128];
   int      exponent;
   int             len;

   memset(target,      0, sizeof(target)       );
   len = 0;
   sscanf(rate, "%1d", &exponent);

   if ( lang == ARABIC )
   {
      memcpy(target + len, rate + 1, 7 - exponent);      
      len+=7-exponent;
      target[len] = ESC;               
      len+=1;
      target[len] = '(';               
      len+=1;
      target[len] = '1';               
      len+=1;
      target[len] = '.';               
      len+=1;
      target[len] = ESC;               
      len+=1;
      target[len] = '(';               
      len+=1;
      target[len] = CHARACTER_SET_ARAB;               
      len+=1;
      memcpy(target + len, rate + 1 + 7 - exponent, exponent);
      len+=exponent;
   }
   else
   {
      memcpy(target + len, rate + 1, 7 - exponent);      
      len+=7-exponent;
      memcpy(target + len, ".", 1);            
      len+=1;
      memcpy(target + len, rate + 1 + 7 - exponent, exponent);
      len+=exponent;
   }

   memset(rate, 0, len + 1);
   memcpy(rate, target, len);

   return (len);
}
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToBalance(nIndexCtx,balance, alignement,lang)
int	 nIndexCtx;
char            balance[128];
char      alignement;
char      lang;
{
   char            detail[MAX_LINE_TRC];
   char            target          [128];
   char            currency        [  4];
   char            sign;
   int             len;

   sprintf(detail, "Start FormatToBalance(%.127s,%c,%c)", balance, alignement, lang);
   trace_event(detail, PROCESSING);

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   len = FormatToAmount(nIndexCtx,target, currency, OK, alignement, lang);

   memset(balance,     0, 128);
   memcpy(balance,     target, strlen(target));
   if(sign == 'D')
      if(lang == ARABIC)
      {
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = 'z';
         balance[strlen(balance)] = '0';
         balance[strlen(balance)] = '-';
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = 'z';
         balance[strlen(balance)] = '1';
      }
      else
      {
         balance[strlen(balance)] = '-';
      }

   sprintf(detail, "End FormatToBalance(%d)", strlen(balance));
   trace_event(detail, PROCESSING);
   return (strlen(balance));
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToBalancePlus(nIndexCtx,balance, alignement,lang)
int 	 nIndexCtx;
char            balance[128];
char      alignement;
char      lang;
{
   char            detail[MAX_LINE_TRC];
   char            target          [128];
   char            currency        [  4];
   char            sign;
   int             len;


   sprintf(detail, "Start FormatToBalancePlus(%.127s,%c,%c)", balance, alignement, lang);
   trace_event(detail, PROCESSING);

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   len = Format2ToAmount(nIndexCtx,target, currency, NOK, alignement, lang);

   memset(balance,     0, 128);
   memcpy(balance,     target, strlen(target));
   if(sign == 'D')
      if(lang == ARABIC)
      {
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = 'G';
         balance[strlen(balance)] = '0';
         balance[strlen(balance)] = '0';
         balance[strlen(balance)] = '-';
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = 'G';
         balance[strlen(balance)] = CHARACTER_SET_ARAB;
         balance[strlen(balance)] = '0';
      }
      else
      {
         balance[strlen(balance)] = '-';
      }

   sprintf(detail, "End FormatToBalancePlus(%d)", strlen(balance));
   trace_event(detail, PROCESSING);

   return (strlen(balance));
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   GetIntegerPart(pAmount, pIntegerPart)
char   pAmount[];
char   pIntegerPart[];
{
   char            detail[MAX_LINE_TRC];
   int   cpt;

   memset(pIntegerPart,   0, 13);

   for(cpt = 0; cpt < strlen(pAmount); cpt ++)
      if(pAmount[cpt] == '.')
         break;

   memcpy(pIntegerPart, pAmount, cpt);

   return (OK);
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   GetDecimalPart(pAmount, pDecimalPart)
char   pAmount[];
char   pDecimalPart[];
{
   char            detail[MAX_LINE_TRC];
   int   cpt;

   sprintf(detail, "amount   = %.13s", pAmount);
   trace_event(detail, PROCESSING);

   memset(pDecimalPart,   0, 13);

   for(cpt = strlen(pAmount) - 1; cpt > 0; cpt --)
      if(pAmount[cpt] == '.')
         break;

   memcpy(pDecimalPart, pAmount + cpt + 1, strlen(pAmount) - cpt);

   sprintf(detail, "decimal  = %.13s", pDecimalPart);
   trace_event(detail, PROCESSING);

   return (OK);
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   FormatToCutoffData(int nIndexCtx,
                         SCutOffData      *pCutOffData,
                         char         pLang
                        )
{
   char            sLine[MAX_LINE_TRC];
   char   lTarget      [1024];
   char   lCurrencyCode   [   4];
   char   lCurrencyAlpha   [   4];
   char   lAmt      [  56];
   char   date_heure_jour[18];
   int   lTargetOff;
   int   lCutOffDataLen;
   int   lLen;
   int   lCpt;
   int   lRet;


   /* - initialisations ---------------------------------------------------- */
   memset(date_heure_jour ,   0, sizeof(date_heure_jour));
   memset(lTarget,   0, sizeof(lTarget));
   lCutOffDataLen   = strlen((char *)pCutOffData);
   lTargetOff      = 0;

   /* - Numero de l'arrete ------------------------------------------------- */
   memcpy(lTarget + lTargetOff, "CUTOFF NO.  : ", 14);
   lTargetOff+=14;
   memcpy(lTarget + lTargetOff,
       pCutOffData->fCutOffId,
       sizeof(pCutOffData->fCutOffId));
   lTargetOff+=sizeof(pCutOffData->fCutOffId);

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;
   lTargetOff+=1;


   /* - Date de l'arrete --------------------------------------------------- */
   memcpy(lTarget + lTargetOff, "CUTOFF DATE : ", 14);
   lTargetOff+=14;
   memcpy(lTarget + lTargetOff,
       pCutOffData->fCutOffDate,
       sizeof(pCutOffData->fCutOffDate));
   lTargetOff+=sizeof(pCutOffData->fCutOffDate);

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;
   lTargetOff+=1;
   /* - nombre de cartes capturees ----------------------------------------- */
    memcpy(lTarget + lTargetOff, "PICKUP  NBR.: ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fPickUpNbr , 6);
    lTargetOff      += 6;

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;               lTargetOff+=1;


   /* - tranche des transactions ------------------------------------------- */

    memcpy(lTarget + lTargetOff, "RANGE       : ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fLowTrnNbr , 8);
    lTargetOff      += 8;

    lTarget[lTargetOff]  = '-';               lTargetOff+=1;

    memcpy(lTarget + lTargetOff, pCutOffData->fHighTrnNbr , 8);
    lTargetOff      += 8;

   /* - Retour a la ligne -------------------------------------------------- */
   /*lTarget[lTargetOff]  = LF;               lTargetOff+=1;

    memcpy(lTarget + lTargetOff, "CAPTURE NBR.: ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fPickUpNbr , 6);
    lTargetOff      += 6;*/


   /* - Retour a la ligne -------------------------------------------------- */

   /*lTarget[lTargetOff]  = LF;               lTargetOff+=1;*/


   /* - tranche des transactions ------------------------------------------- */
    /*memcpy(lTarget + lTargetOff, "RANGE       : ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fLowTrnNbr , 8);
    lTargetOff      += 8;
    lTarget[lTargetOff]  = '-';               lTargetOff+=1;
    memcpy(lTarget + lTargetOff, pCutOffData->fHighTrnNbr , 8);
    lTargetOff      += 8;
    lTarget[lTargetOff]  = LF;               lTargetOff+=1;
    */

   lCpt      = 0;
   while(lCutOffDataLen >= sizeof(SCutOffData) - sizeof(pCutOffData->fCurrData)
       + sizeof(SCurrData)*(lCpt + 1))
   {
      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - monnaie i ------------------------------------------------------ */
      memset(lCurrencyCode,   0, sizeof(lCurrencyCode));
      memset(lCurrencyAlpha,   0, sizeof(lCurrencyAlpha));

      memcpy(lCurrencyCode,
          pCutOffData->fCurrData.fCurrCode + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fCurrCode));

      lRet = GetCurrencyAlpha(nIndexCtx,lCurrencyCode, lCurrencyAlpha);
      if(lRet != OK)
         memcpy(lCurrencyAlpha, lCurrencyCode, 3);

      memcpy(lTarget + lTargetOff, "CURRENCY    : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff, lCurrencyAlpha, 3);
      lTargetOff+=3;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions approuvees apres arrete global ---------- */
      memcpy(lTarget + lTargetOff, "APP TRN NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fBCOAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCOAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fBCOAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Montant de transactions approuvees apres arrete global --------- */
      memcpy(lTarget + lTargetOff, "APP TRN AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fBCOAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCOAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions Annuler avant arrete global ---------- */
      memcpy(lTarget + lTargetOff, "REV TRN NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fBCORevTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCORevTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fBCORevTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Montant de transactions Annuler avant arrete global --------- */
      memcpy(lTarget + lTargetOff, "REV TRN AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fBCORevTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCORevTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions confirmees suspendues dans le SAF ------- */
      memcpy(lTarget + lTargetOff, "APPSUSP NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fSAFAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Monttant de transactions confirmees suspendues dans le SAF ----- */
      memcpy(lTarget + lTargetOff, "APPSUSP AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fSAFAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* =====================================================================*/
      /* ==========            APRES L'ARRETE DES CAISSE           ===========*/
      /* =====================================================================*/
      memcpy(lTarget + lTargetOff, "-------------------- ", 20);
      lTargetOff+=20;
      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;
      memcpy(lTarget + lTargetOff, "ACTUAL DATE : ", 14);
      lTargetOff+=14;
/*    current_gmt_date(date_heure_jour);
      memcpy(lTarget + lTargetOff, date_heure_jour, 12);
      lTargetOff+=12;
*/
      current_system_date (date_heure_jour);
      memcpy(lTarget + lTargetOff, date_heure_jour, 17);
      lTargetOff+=17;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions approuvees apres arrete global ---------- */
      memcpy(lTarget + lTargetOff, "APP TRN NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fACOAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fACOAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Montant de transactions approuvees apres arrete global --------- */
      memcpy(lTarget + lTargetOff, "APP TRN AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fACOAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions Annuler avant arrete global ---------- */
      memcpy(lTarget + lTargetOff, "REV TRN NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fACORevTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACORevTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fACORevTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Montant de transactions Annuler avant arrete global --------- */
      memcpy(lTarget + lTargetOff, "REV TRN AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fACORevTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACORevTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions confirmees suspendues dans le SAF ------- */
      memcpy(lTarget + lTargetOff, "APPSUSP NBR : ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,pCutOffData->fCurrData.fACOSuspTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOSuspTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fACOSuspTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Monttant de transactions confirmees suspendues dans le SAF ----- */
      memcpy(lTarget + lTargetOff, "APPSUSP AMT : ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fACOSuspTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOSuspTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L', pLang);

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      lCpt ++;
   }

   /* - copie dans pCutOffData --------------------------------------------- */
   memset(pCutOffData,            0, lCutOffDataLen   );
   memcpy(pCutOffData,      lTarget, lTargetOff   );


   return (strlen((char *)pCutOffData));
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   FormatToMessage(pMsg, pLang, pWidth)
char   pMsg[];
char   pLang;
int   pWidth;
{
   char            detail[MAX_LINE_TRC];
   char            lTarget[501];
   int             lTargetLen;
   char            lLine[2*PRN_WIDTH];
   int             lLineLen;
   int      lMsgLen;
   int             lOff;

   sprintf(detail, "Start FormatToMessage(%.20s,%c,%d)", pMsg, pLang, pWidth);
   trace_event(detail, PROCESSING);

   memset(lTarget,      0, sizeof(lTarget));
   lTargetLen = 0;
   lMsgLen = strlen(pMsg);

   if(lMsgLen != 0)
   {
      lOff = 0;
      while(lOff < lMsgLen)
      {
         memset(lLine, 0, pWidth+1);
         memset(lLine, ' ', pWidth/2);

         if ( pMsg[lOff] == LF )
            lOff ++;

         lLineLen = (lMsgLen-lOff>pWidth ? pWidth : lMsgLen-lOff);

         while((lLineLen != 0) && (pMsg[lOff + lLineLen] != LF))
            lLineLen --;

         if (lLineLen == 0)
            lLineLen = (lMsgLen-lOff>pWidth ? pWidth : lMsgLen-lOff);

         if(lLineLen == pWidth)
            while((lLineLen != 0) && (pMsg[lOff + lLineLen] != ' '))
               lLineLen --;

         if (lLineLen == 0)
            lLineLen = (lMsgLen-lOff>pWidth ? pWidth : lMsgLen-lOff);

         memcpy(lLine + (pWidth - lLineLen)/2, pMsg + lOff, lLineLen);
         lOff += lLineLen;

         memcpy(lTarget+lTargetLen,lLine,strlen(lLine));
         lTargetLen+=strlen(lLine);

         lTarget[lTargetLen] = LF;         
         lTargetLen+=1;
      }
   }
   else
   {
      memcpy(lTarget, "   ", 3);
      lTargetLen = 3;
   }

   memset(pMsg, 0, lTargetLen+1);
   memset(pMsg, ' ', lTargetLen);
   memcpy(pMsg, lTarget, lTargetLen);
   lMsgLen = lTargetLen;

   sprintf(detail, "End   FormatToMessage(OK,%d,%.60s)", lMsgLen, pMsg);
   trace_event(detail, PROCESSING);
   return (lMsgLen);
}



/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   AlignField(
char   pTransName[],
char   pAlign,
int   pWidth
)
{
   char            detail[MAX_LINE_TRC];
   char   lTarget   [256];
   int   lLen;

   memset(lTarget, 0, sizeof(lTarget));
   memset(lTarget, ' ', pWidth);
   lLen = strlen(pTransName);

   switch ( pAlign )
   {
   case 'R' :
      memcpy(lTarget + pWidth - lLen, pTransName, lLen);
      break;
   case 'C' :
      memcpy(lTarget + (pWidth - lLen) / 2, pTransName, lLen);
      break;
   case 'L' :
   default  :
      memcpy(lTarget, pTransName, lLen);
   }

   memset(pTransName, 0, lLen+1);
   memcpy(pTransName, lTarget, pWidth);

   return (pWidth);
}



/*******************************************************************************
**  nom    : FormatToBillPayment(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB DIEBOLD.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToBillPayment(nIndexCtx,record_data, statement, lang)
int		nIndexCtx;
char    record_data     [512];
char    statement       [512];
char   lang;
{
   char            sLine		 	[MAX_LINE_TRC];
   char            reference   	    [ 32];
   char            amount      		[ 41];
   char            currency   		[  4];
   int      	   len;
   int      	   Lg;
   int             offset;
   int             trans_nbr;
   int             cpt;
   int             indice = 0;

   sprintf(sLine, "Start FormatToBillPayment(%c)", lang);
   trace_event(sLine, PROCESSING);

   /* - Raz des variables ------------------------------------------------- */
   memset(statement,           0, sizeof(statement));

   /* - nombre de lignes du releve ---------------------------------------- */
   sscanf(record_data, "%3d", &trans_nbr);

   /* - formatage du releve ----------------------------------------------- */
   offset = 0;
   Lg = strlen(record_data);
   for(cpt = 0; cpt < trans_nbr; cpt ++)
   {
      /* - Raz des variables ----------------------------------------- */
      memset(reference,   	0, sizeof(reference)   );
      memset(amount,      	0, sizeof(amount)   );
      memset(currency,      0, sizeof(currency)   );


      /* - formatage d'une ligne ------------------------------------ */
	  while (indice < Lg)
   	  {
      	if (record_data[indice] == '[') { 
				Download_Info(record_data+ indice+1, 0, reference, Lg-indice);
				Download_Info(record_data+ indice, 1, currency, Lg-indice);
				Download_Info(record_data+indice, 2, amount, Lg-indice);
				indice+=strlen(reference)+1;
				indice+=strlen(currency)+1;
				indice+=strlen(amount);
				break;
				}
      	indice += 1;
   	  }
      if(lang == ARABIC)
      {
         /* - reference ------------------------------- */
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '1';            
         offset+=1;
         memcpy(statement + offset,   reference, strlen(reference));   
         offset+=strlen(reference);
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = CHARACTER_SET_ARAB;            
         offset+=1;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

         /* - montant ------------------- */
         len = FormatToAmount(nIndexCtx,amount, currency, OK, 'R', lang);
         memcpy(statement + offset,   amount, len);   
         offset+=len;

         /* - signe de la transaction --------------- */
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = CHARACTER_SET_ARAB;            
         offset+=1;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

      }
      else
      {
         memcpy(statement + offset,   reference, strlen(reference));   
         offset+=strlen(reference);

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

         /* - montant transaction ------------------- */
         len = FormatToAmount(nIndexCtx,amount, currency, NOK, 'R', lang);
         memcpy(statement + offset,   amount + len - 12, 12);   
         offset+=12;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

      }

      /* - saut de ligne ------------------------- */
      statement[offset] = LF;               
      offset+= 1;
   }

   trace_event("End   FormatToBillPayment(OK)", PROCESSING);
   return (OK);
}
/*******************************************************************************
**  nom    : FormatToCompany(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB DIEBOLD.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToCompany(record_data, Company, lang)
char    record_data     [512];
char    Company       	[32];
char   lang;
{
   char            sLine		 	[MAX_LINE_TRC];
   char    		   statement       	[20];
   int      	   Lg;
   int             offset;
   int             cpt = 0;

   sprintf(sLine, "Start FormatToCompany(%c)", lang);
   trace_event(sLine, PROCESSING);

   /* - Raz des variables ------------------------------------------------- */
   memset(Company,           	0, sizeof(Company));
   memset(statement,           	0, sizeof(statement));

   /* - formatage du releve ----------------------------------------------- */
   offset = 0;
   Lg = strlen(record_data);
   /* - formatage d'une ligne ------------------------------------ */
   while (cpt < Lg)
   {
      	if (record_data[cpt] == '[') 
			break;
      	cpt += 1;
    }
   	if ( cpt == Lg ) {
      		trace_event("End   FormatToCompany(LENGTH ERROR)", PROCESSING);
      		return (NOK);
   		}
	else{
      memcpy(statement, record_data + 3 , cpt-3);
      if(lang == ARABIC)
      {
         Company[offset] = ESC;            
         offset+=1;
         Company[offset] = '(';            
         offset+=1;
         Company[offset] = '1';            
         offset+=1;
      }
      memcpy(Company + offset,   statement, strlen(statement));   
      offset+=strlen(statement);
	 }

   trace_event("End   FormatToCompany(OK)", PROCESSING);
   return (OK);
}

int Download_Info(Buffer, Field_Number, Field, Lg)
char *Buffer;
int  Field_Number;
char *Field;
int  Lg;
{
   char     sLine 	[MAX_LINE_TRC];
   char 	tmp_buf	[LG_MAX];
   int  	i = 0;
   int  	j = 0;
   int  	nb_FS = 0;
   int  	lg = 0;
   int  	len=0;

   sprintf(sLine, "Debut Download_Info(%d)", Field_Number);
   trace_event(sLine, PROCESSING);

   memset(tmp_buf,0,sizeof(tmp_buf));

   while ((nb_FS < Field_Number) && (i < Lg-1))
   {
      if (Buffer[i] == '|') nb_FS += 1;
      i += 1;
   }

   if ((nb_FS < Field_Number) || (i == Lg))
   {
      trace_event("Fin   Download_Info(NOK)", PROCESSING);
      return(NOK);
   }
   j = i;
   while ((Buffer[j] != '|') && (Buffer[j] != ']') &&  (j < Lg))
      j+=1;

   memcpy(tmp_buf, Buffer+i, j - i);
   len = j - i;
   if ( Field_Number == 2 )
		len--;
   	memcpy(Field, tmp_buf, len);

   sprintf(sLine, "Fin   Download_Info(%.30s)", Field);
   trace_event(sLine, PROCESSING);
   return(OK);
}


/*******************************************************************************
**  nom    : FormatToBalancing(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB DIEBOLD.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToBalancing(nIndexCtx,record_data, statement, lang)
int     nIndexCtx;
char*    record_data;
char*    statement;
char    lang;
{
   char            sLine[MAX_LINE_TRC];
   int      offset;


      sprintf(sLine,"Start FormatToBalancing()");
      trace_event(sLine,PROCESSING);

      offset = 0;

      memcpy(statement + offset, "(", 1);
      offset += 1;
      memcpy(statement + offset, statement + 3, 12);
      offset += 12;
      memcpy(statement + offset, ")     S     ", 12);
      offset += 12;
      memcpy(statement + offset, statement + 3 + 12 + 12, 12);
      offset += 12;

      return offset;
}
