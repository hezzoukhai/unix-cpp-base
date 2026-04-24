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

#include <define.h>
#include <ascii.h>
#include <paramdef.h>
#include <langue.h>
#include <atmcutoff.h>

#define   PRN_WIDTH   40


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   GetIntegerPart(char   pAmount[],char   pIntegerPart[])
{
   char            detail[MAX_LINE_TRC];
   int   cpt;

   memset(pIntegerPart,   0, 13);

   for(cpt = 0; cpt < strlen(pAmount); cpt ++)
      if(pAmount[cpt] == '.')
         break;

	if( cpt > 0 )
		memcpy(pIntegerPart, pAmount, cpt);

   return (OK);
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   GetDecimalPart(char   pAmount[],char   pDecimalPart[])
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

/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToDate(char            *date,char      lang)
{
   char            detail[MAX_LINE_TRC];
   char            target[24];
   int      offset;

   memset(target,      0, sizeof(target));         
   offset =0;

   if(lang == ARABIC)
   {
      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
      offset+=2;
      memcpy(target + offset,  date + 0, 2);         
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(1",      2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
      offset+=2;
      memcpy(target + offset,   date + 2,   2);      
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(1",      2);      
      offset+=2;
      memcpy(target + offset,   "/",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
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

   return (offset);
}

/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToAmount(
						int	   nIndexCtx,
						char   *amount,
						char   *currency,
						int   currency_flag,
						char   alignement,
						char   lang)
{
   char            detail[MAX_LINE_TRC];
   char            target   [99];
   char            cur_alpha[ 4];
   char      	   lIntegerPart   [13];
   char      	   lDecimalPart   [13];
   char      	   sLocalAmount   [50];
   int             offset;
   int      	   lEscLen;
   int             retour;

   sprintf (detail, "Start FormatToAmount (amount: %s, curr: %s)", amount,currency);
   trace_event(detail, PROCESSING); 
   
   
   memset(target,      0, sizeof(target)   );
   memset(lIntegerPart,   0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,   0, sizeof(lDecimalPart)   );
   memset(sLocalAmount,   0, sizeof(sLocalAmount)   );
   offset = 0;
   lEscLen = 0;

   memcpy (sLocalAmount,amount, strlen(amount)); 
   memset(amount,   0, sizeof(amount)   );
   memset(amount,  ' ', 15  );  

   retour = adjust_amount_float(nIndexCtx,sLocalAmount, currency);
   if(retour == OK)
   {
      /* - formatage ------------------------------------------------------ */
      if (currency_flag == OK)
      {
         retour = GetCurrencyAlpha(nIndexCtx,currency, cur_alpha);
         if (retour == OK)
         {
            if(lang == ARABIC)
            {

               target[offset] = ESC;
               offset+=1;
               target[offset] = '(';
               offset+=1;
               target[offset] = '7';
               offset+=1;

               if(memcmp(cur_alpha, "KWD", 3) == 0)
               {
                  memcpy(target + offset,   " P>", 3);   
                  offset+=3;
               }
               else if(memcmp(cur_alpha, "TND", 3) == 0)
               {
                  memcpy(target + offset,   " (>", 3);   
                  offset+=3;
               }
               else if(memcmp(cur_alpha, "USD", 3) == 0)
               {
                  memcpy(target + offset,   "@V[>  ", 6);   
                  offset+=6;
               }
               else if(memcmp(cur_alpha, "AED", 3) == 0)
               {
                  memcpy(target + offset,   "W]@>", 4);   
                  offset+=4;
               }
               else if(memcmp(cur_alpha, "JOD", 3) == 0)
               {
                  memcpy(target + offset,   "@#Z_> ", 6);   
                  offset+=6;
               }
               else if(memcmp(cur_alpha, "ILS", 3) == 0)
               {
                  memcpy(target + offset,   "TQ_D  ", 6);   
                  offset+=6;
               }
               else if(memcmp(cur_alpha, "EUR", 3) == 0)
               {
                  memcpy(target + offset,   "[@[_  ", 6);
                   offset+=6    ;
               }
               else if(memcmp(cur_alpha, "GBP", 3) == 0)
               {
                  memcpy(target + offset,   "C\"*   ", strlen ("C\"*   "));
                  offset+=strlen ("C\"*   ");
               }
               else if(memcmp(cur_alpha, "QAR", 3)==0)
               {
                   memcpy(target + offset,   "N@ ", 3);
                   offset+=3;
                }

               /*START MMJ29042009*/
               else if(memcmp(cur_alpha, "MAD", 3) == 0)
               {
                  memcpy(target + offset,   "MAD  ", 6);   
                  offset+=6;
               }
               /*END MMJ29042009*/               
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
         GetIntegerPart(sLocalAmount, lIntegerPart);
         GetDecimalPart(sLocalAmount, lDecimalPart);

         /* TBO160509 Avant le changement de caractere */
         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);


         target[offset] = ESC;            
         offset+=1;
         target[offset] = '(';            
         offset+=1;
         target[offset] = '1';            
         offset+=1;

         target[offset] = ',';            
         offset+=1;

   
         target[offset] = ESC;            
         offset+=1;
         target[offset] = '(';            
         offset+=1;
         target[offset] = '7';            
         offset+=1;
      

         /* TBO160509 Apres le changement de caractere d impression */ 
         memcpy(target + offset, lDecimalPart, strlen(lDecimalPart));
         offset += strlen(lDecimalPart);

         lEscLen += 6;


      }
      else
      {
         sprintf(target + offset, "%.16s", sLocalAmount);
      }
   }
   else
   {
      sprintf(target + offset, "%012d", atof(sLocalAmount));
   }

   /* - alignement ----------------------------------------------------- */
   /* memset(amount, 0, 50); */
   if(alignement == 'R' || alignement == 'r')
   {
      if(currency_flag == OK)
      {
         memset(amount, ' ', 20 + lEscLen);
	     memcpy(amount + 20 + lEscLen - strlen(target),	target,	strlen(target));
      }
      else
      {
         memset(amount, ' ', 15 + lEscLen);
         memcpy(amount + 15  + lEscLen - strlen(target),
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

   sprintf (detail, "End FormatToAmount (OK)(amount: %s, target: %s)", amount,target);
   trace_event(detail, PROCESSING); 
   
   return (offset);
}

/*******************************************************************************
**  nom    : FormatToStatement(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB NDC.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToStatement(
							int 	nIndexCtx,
							char    *record_data,
							char    *statement,
							char    *currency,
							char   	lang)
{
   char            detail[MAX_LINE_TRC];
   char            transaction   [256];
   char            date            [ 21];
   char            amount      [ 41];
   char            balance      [ 41];
   char            mm      [  3];
   char            dd      [  3];
   int      	   len;
   int             offset;
   int             trans_nbr;
   int             cpt;

   sprintf(detail, "Start FormatToStatement(%.3s,%c)", currency, lang);
   trace_event(detail, PROCESSING);

   /* - Raz des variables ------------------------------------------------- */
   memset(statement,           0, sizeof(statement));

   /* - nombre de lignes du releve ---------------------------------------- */
   sscanf(record_data, "%2d", &trans_nbr);

   /**MK 060606**/
   if ( trans_nbr > 10 )
	trans_nbr = 10;
   /*************/

/* Insure1212
   sprintf(detail, " Record Data Len : %d Len Calc : %d", strlen(record_data), NP_SHORT_STAT_LEN*trans_nbr + 2);
*/
   sprintf(detail, " Record Data Len : %lu Len Calc : %d, trans %d", strlen(record_data), NP_SHORT_STAT_LEN*trans_nbr + 2, trans_nbr);
   trace_event(detail, PROCESSING);
   dump_buffer(record_data,strlen(record_data),'X','X','X');

   if (memcmp (record_data, "00" , 2) == 0)
   {
      trace_event("End   FormatToStatement(LENGTH 00)", PROCESSING);
      return (NOK);
   }
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
         /* - balance ------------------------------- */
/*
         memcpy(balance,   transaction + NP_BAL_POS, NP_BAL_LEN);
         len = FormatToAmount(nIndexCtx,balance, currency, NOK, 'R', lang);
         memcpy(statement + offset,   balance, len);   
         offset+=len;
*/
         /* - signe de la balance ------------------- */
/*
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '1';            
         offset+=1;

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
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '7';            
         offset+=1;

         /* - separateur ---------------------------- */
/*
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;
*/
         /* - montant transaction ------------------- */
         memcpy(amount,   transaction + NP_AMT_POS, NP_AMT_LEN);
         len = FormatToAmount(nIndexCtx,amount, currency, NOK, 'R', lang);
         memcpy(statement + offset,   amount+len-18, 18);   
         offset+=18;

         /* - signe de la transaction --------------- */
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '1';            
         offset+=1;

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

         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '7';            
         offset+=1;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

         /* ---- Operation Code ------------------------*/
         memcpy(statement + offset,   transaction + NP_LABELE_POS ,NP_LABEL_LEN);
         offset+=NP_LABEL_LEN ;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);
         offset+= 1;

         /* - date transaction ---------------------- */
         memcpy(date,   transaction + NP_DATE_POS,  NP_DATE_LEN);
         len = FormatToDate(date, lang);
	 memcpy(dd, date + 3  , 2);
         memcpy(mm, date + 12 , 2);
         memcpy(date + 3,  mm, 2);
         memcpy(date + 12, dd, 2);
/*
         memcpy(statement + offset,    date  , len);       
         offset+= len;
*/
         memcpy(statement + offset,    date  , 14);       
         offset+= 14;
      }
      else
      {
         /* - date transaction ---------------------- */
         memcpy(date,   transaction + NP_DATE_POS,  NP_DATE_LEN);
         FormatToDate(date, lang);
	 memcpy(statement + offset,  date + 3, 5);   offset+= 5;
/*
         memcpy(statement + offset,      date, 8);      
         offset+= 8;
*/
         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

         /* ---- Operation Code ------------------------*/
         memcpy(statement + offset,   transaction + NP_LABELE_POS ,NP_LABEL_LEN);
         offset+=NP_LABEL_LEN ;
         /*memcpy(statement + offset,   transaction + NP_LABELE_POS ,11);
         offset+=11 ;*/

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);
         offset+= 1;

         /* - montant transaction ------------------- */
         memcpy(amount,      transaction + NP_AMT_POS, NP_AMT_LEN);
         len = FormatToAmount(nIndexCtx,amount, currency, NOK, 'R', lang);
         memcpy(statement + offset,   amount + len - 12, 12);   
         offset+=12;

         /* - signe de la transaction --------------- */
         if(transaction[NP_AMT_SIGN_POS] == 'D')
         {
            memcpy(statement + offset,      "-",  1);   
            offset+= 1;
         }
         else
         {
            memcpy(statement + offset,      "+",  1);   
            offset+= 1;
         }

         /* - separateur ---------------------------- */
/*
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;
*/

         /* - balance ------------------------------- */
         /*YK 17/02/03*/
		 /*
		 memcpy(balance,   transaction + NP_BAL_POS, NP_BAL_LEN);
         len = FormatToAmount(nIndexCtx,balance, currency, NOK, 'R', lang);
         memcpy(statement + offset,   balance+len-12, 12);   
         offset+=12;
         */
         /* - signe de la transaction --------------- */
         /*YK 17/02/03*/
		 /*
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
int     FormatToDateTime(char      *date,char      lang)
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
int     FormatToHour(char      *hour,char      lang)
{
   char            sLine[MAX_LINE_TRC];
   char            target[24];
   int      offset;

   memset(target,      0, sizeof(target));         
   offset =0;

   if(lang == ARABIC)
   {
      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
      offset+=2;
      memcpy(target + offset,  hour + 0, 2);         
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(1",      2);      
      offset+=2;
      memcpy(target + offset,   ":",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
      offset+=2;
      memcpy(target + offset,   hour + 2,   2);      
      offset+=2;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(1",      2);      
      offset+=2;
      memcpy(target + offset,   ":",      1);      
      offset+=1;

      target[offset] = ESC;               
      offset+=1;
      memcpy(target + offset,   "(7",      2);      
      offset+=2;
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

   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToCardNbr(char      *card_nbr,char      align,char      lang)
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
int     FormatToAccountNbr(char   *account_nbr,char   alignement,char   lang)
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
int     Format2ToAmount(
						int	   nIndexCtx,
						char   *amount,
						char   *currency,
						int   currency_flag,
						int   alignement,
						char   lang)
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
   memset(cur_alpha,      0, sizeof(cur_alpha)   ); /**MK 060606*/
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
         /*retour = GetCurrencyAlpha(currency, cur_alpha);** MK 060606*/
         retour = GetCurrencyAlpha(nIndexCtx,currency, cur_alpha);
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

         memcpy(target + offset, lIntegerPart, strlen(lIntegerPart));
         offset += strlen(lIntegerPart);

         target[offset] = ESC;            
         offset+=1;
         target[offset] = '(';            
         offset+=1;
         target[offset] = '1';            
         offset+=1;

         target[offset] = '.';            
         offset+=1;

         target[offset] = ESC;            
         offset+=1;
         target[offset] = '(';            
         offset+=1;
         target[offset] = '7';            
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
      sprintf(target + offset, "%01.0f", (double)atof(amount));
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
int     FormatToRate(
char            *rate,
char            alignement,
char            lang)
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
      target[len] = '7';               
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
int     FormatToBalance(
						int			nIndexCtx,
						char        *balance,
						char       alignement,
						char      lang)
{
   char            detail[MAX_LINE_TRC];
   char            target          [128];
   char            currency        [  4];
   char            sign;
   int             len;

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   len = FormatToAmount(nIndexCtx,target, currency, OK, alignement, lang);

   memset(balance,     0, 128);
   memcpy(balance,     target, strlen(target));
   if(sign == 'D')
   {
      if(lang == ARABIC)
      {
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = '(';
         balance[strlen(balance)] = '1';
         balance[strlen(balance)] = '-';
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = '(';
         balance[strlen(balance)] = '7';
      }
      else
      {
         balance[strlen(balance)] = '-';
      }
   }

   return (strlen(balance));
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToBalancePlus(
							int 	 nIndexCtx,
							char     *balance,
							char   alignement,
							char      lang)
{
   char            detail[MAX_LINE_TRC];
   char            target          [128];
   char            currency        [  4];
   char            sign;
   int             len;

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   len = FormatToAmount(nIndexCtx,target, currency, OK, alignement, lang);

   memset(balance,     0, 128);
   memcpy(balance,     target, strlen(target));
   if(sign == 'D')
   {
      if(lang == ARABIC)
      {
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = '(';
         balance[strlen(balance)] = '1';
         balance[strlen(balance)] = '-';
         balance[strlen(balance)] = ESC;
         balance[strlen(balance)] = '(';
         balance[strlen(balance)] = '7';
      }
      else
      {
         balance[strlen(balance)] = '-';
      }
   }

   return (strlen(balance));
}



/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   FormatToCutoffData(int nIndexCtx,
			 SCutOffData      *pCutOffData,
			 char         pLang
			)
{
   char            sLine[MAX_LINE_TRC];
   char   lTarget      [2024];
   char   lCurrencyCode   [   4];
   char   lCurrencyAlpha   [   4];
   char   lAmt      [  56];
   char	  date_heure_jour[13];
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
   memcpy(lTarget + lTargetOff, "ARRETE NO.  : ", 14);      
   lTargetOff+=14;
   memcpy(lTarget + lTargetOff, pCutOffData->fCutOffId,sizeof(pCutOffData->fCutOffId));
   lTargetOff+=sizeof(pCutOffData->fCutOffId);

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;               
   lTargetOff+=1;


   /* - Date de l'arrete --------------------------------------------------- */
   memcpy(lTarget + lTargetOff, "ARRETE DATE : ", 14);      
   lTargetOff+=14;
   memcpy(lTarget + lTargetOff,pCutOffData->fCutOffDate,sizeof(pCutOffData->fCutOffDate));
   lTargetOff+=sizeof(pCutOffData->fCutOffDate);

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;               
   lTargetOff+=1;
   /* - nombre de cartes capturees ----------------------------------------- */
    memcpy(lTarget + lTargetOff, "CAPTURE NBR.: ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fPickUpNbr , sizeof(pCutOffData->fPickUpNbr));
    lTargetOff      += sizeof(pCutOffData->fPickUpNbr);

   /* - Retour a la ligne -------------------------------------------------- */
   lTarget[lTargetOff]  = LF;               lTargetOff+=1;


   /* - tranche des transactions ------------------------------------------- */
    memcpy(lTarget + lTargetOff, "TRANCHE     : ", 14);      lTargetOff+=14;
    memcpy(lTarget + lTargetOff, pCutOffData->fLowTrnNbr , sizeof(pCutOffData->fLowTrnNbr));
    lTargetOff      += sizeof(pCutOffData->fLowTrnNbr);
    lTarget[lTargetOff]  = '-';               lTargetOff+=1;
    memcpy(lTarget + lTargetOff, pCutOffData->fHighTrnNbr , sizeof(pCutOffData->fHighTrnNbr));
    lTargetOff      += sizeof(pCutOffData->fHighTrnNbr);
    lTarget[lTargetOff]  = LF;               lTargetOff+=1;

   lCpt      = 0;
   while(lCutOffDataLen >= sizeof(SCutOffData) - sizeof(pCutOffData->fCurrData) + sizeof(SCurrData)*(lCpt + 1))
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

      memcpy(lTarget + lTargetOff, "MONNAIE     : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff, lCurrencyAlpha, 3);   
      lTargetOff+=3;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* nbr of transactions approved before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "APP TRN NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fBCOAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCOAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fBCOAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* amounts of transactions approved before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "APP TRN AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fBCOAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCOAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /*TBO021009 insure */

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* nbr of transactions reversed before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "REV TRN NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fBCORevTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCORevTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fBCORevTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* amounts of transactions reversed before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "REV TRN AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fBCORevTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fBCORevTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /* TBO021009 insure */

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* nbr of transactions suspects before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "APPSUSP NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fSAFAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* amounts of transactions suspects before last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "APPSUSP AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fSAFAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /*TBO021009 insure */

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;
      
	  /****
	  memcpy(lTarget + lTargetOff, "---------------------------- ", 28);lTargetOff+=28;
	  lTarget[lTargetOff++]  = LF;***/

      /* nbr of transactions approved after last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "CDAPPTRN NBR: ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fPRGAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fPRGAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* nbr of transactions approved after last cutoff ---------- */
      memcpy(lTarget + lTargetOff, "CDAPPTRN AMT: ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fPRGAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /*TBO021009 insure */

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Nombre de transactions reverse  cash deposit ------- */
      memcpy(lTarget + lTargetOff, "CDREVTRN NBR: ", 14);
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fPRGRevTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGRevTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fPRGRevTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /* - Monttant de transactions reverse  cash deposit ----- */
      memcpy(lTarget + lTargetOff, "CDREVTRN AMT: ", 14);
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fPRGRevTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGRevTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /* TBO021009 insure */

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;
      lTargetOff+=1;

      /**********End MKB290408***********/

      /* =====================================================================*/
      /* ==========            APRES L'ARRETE DES CAISSE           ===========*/
      /* =====================================================================*/
      memcpy(lTarget + lTargetOff, "-------------------- ", 20);   
      lTargetOff+=20;
      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;
      memcpy(lTarget + lTargetOff, "ACTUEL DATE : ", 14);   
      lTargetOff+=14;

	   memcpy(lTarget + lTargetOff,pCutOffData->fTermCurrDate,sizeof(pCutOffData->fTermCurrDate));
	   lTargetOff+=sizeof(pCutOffData->fTermCurrDate);
	/*EBE160107
      current_gmt_date(date_heure_jour);
      memcpy(lTarget + lTargetOff, date_heure_jour, 12);   
      lTargetOff+=12;*/

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
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang);/*TBO021009 insure */

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
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang); /*TBO021009 insure */

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
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, (char)'L', pLang);/*TBO021009 insure */

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

	
   sprintf(sLine, "lTargetOff %d   pCutOffData %lu", lTargetOff, strlen((char *)pCutOffData));
   trace_event(sLine, PROCESSING);

   
   

   return (strlen((char *)pCutOffData));
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   FormatToMessage(char   pMsg[],char   pLang,int   pWidth)
{
   char            detail[MAX_LINE_TRC];
   char            lTarget[501];
   int             lTargetLen;
   char            lLine[2*PRN_WIDTH];
   int             lLineLen;
   int      lMsgLen;
   int             lOff;

   if(debug_option == 1 )
	sprintf(detail, "Start FormatToMessage(%.20s,%c,%d)", pMsg, pLang, pWidth);
   else
    sprintf(detail, "Start FormatToMessage(%c,%d)", pLang, pWidth);
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

   if(debug_option == 1 )
	sprintf(detail, "End   FormatToMessage(OK,%d,%.60s)", lMsgLen, pMsg);
   else
	sprintf(detail, "End   FormatToMessage(OK,%d)", lMsgLen);
   trace_event(detail, PROCESSING);
   return (lMsgLen);
}



/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   AlignField(char   pTransName[],char   pAlign,int   pWidth)
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
**  Sortie : statement -> chaine formatee a envoyer au GAB NDC.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToBillPayment(int		nIndexCtx,char    *record_data,char    *statement,char   lang)
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
         statement[offset] = '7';            
         offset+=1;

         /* - separateur ---------------------------- */
         memcpy(statement + offset,       " ", 1);      
         offset+= 1;

         /* - montant ------------------- */
         len = FormatToAmount(nIndexCtx,amount, currency, NOK, (char)'R', lang); /*TBO021009 insure */
         memcpy(statement + offset,   amount, len);   
         offset+=len;

         /* - signe de la transaction --------------- */
         statement[offset] = ESC;            
         offset+=1;
         statement[offset] = '(';            
         offset+=1;
         statement[offset] = '7';            
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
         len = FormatToAmount(nIndexCtx,amount, currency, NOK, (char)'R', lang); /*TBO021009 insure */
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
**  Sortie : statement -> chaine formatee a envoyer au GAB NDC.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToCompany(char    *record_data,char    *Company,char   lang)
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

int Download_Info(char *Buffer,int  Field_Number,char *Field,int  Lg)
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

/* TBO160509 Add FormatToMajuscule */
/*
int FormatToMajuscule(Buffer)
char Buffer[MAX_LINE_TRC];

{
  int index;
  for (index= 0; Buffer[index] != '\0'; index++)
    if ( Buffer[index] >= 'a' && Buffer[index] <= 'z')
      Buffer[index] = Buffer[index] -32;

 return(OK);
}
*/
/*******************************************************************************
**  nom    : FormatToDepNotes(TreminalNumber, notes_record, notes_formatted) -> renvoit la          **
**         chaine formater des notes deposer.                                         **
**  Entree : notes_record -> chaine de donnees de type NNCCC   NN notes type,  CCC count  **
**  Sortie : notes_formatted -> chaine formatee a envoyer au GAB NDC.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
/*
int     FormatToDepNotes(nIndexCtx, 
                         TreminalNumber,
			 char   *AcquirerBank,
                         notes_record, 
                         notes_formatted, 
                         lang)
int    nIndexCtx;
char   *TreminalNumber;
char   notes_record     [1024];     
char   notes_formatted  [1024];
char   lang;

{
   char            sLine[MAX_LINE_TRC];
   char            ligne            [256];
   char            note_type        [2+1];
   char            amount_note      [ 13];
   char            amount_note1     [ 13];
   char            count_note       [  5];
   char	           formatedAmount   [ 25];
   char 		   total_amount     [ 20];
   char      	   lIntegerPart   [13];
   char      	   lDecimalPart   [13];
   float           lNoteValue       = 0.0;
   double      	   amount;
   int             offset;
   int             trans_nbr;
   int             cpt   = 0;
   int             record_len; 	
   int             amountLen =0;
   int             Len =0;
   
   sprintf(sLine, "Start FormatToDepNotes(%.16s,%.3c,%s)",TreminalNumber, lang,notes_record);
   trace_event(sLine, PROCESSING);

   / * - Raz des variables ------------------------------------------------- * /
   memset(notes_formatted,           0, sizeof(notes_formatted));
   memset(note_type,          		 0, sizeof(note_type));
   memset(amount_note,           	 0, sizeof(amount_note));
   memset(amount_note1,           	 0, sizeof(amount_note1));
   memset(count_note,           	 0, sizeof(count_note));
   memset(total_amount,           	 0, sizeof(total_amount));
   memset(formatedAmount,            0, sizeof(formatedAmount));
   memset(lIntegerPart,  			 0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,  			 0, sizeof(lDecimalPart)   ); 
   record_len = strlen (notes_record);
 

   / * - control de la langueur -------------------------------------------- * /
   if ( record_len == 0)
   {
      trace_event("End   FormatToDepNotes(LENGTH ERROR)", PROCESSING);
      return (NOK);
   }
   / * - formatage du releve ----------------------------------------------- * /
   offset = 0;
	


 while (cpt < record_len)
	{
   sprintf(sLine, "Start Formating(cpt(%d))",cpt);
   trace_event(sLine, PROCESSING);	
   lNoteValue = 0.0;
   amount = 0.0;
   
      if(lang == ARABIC)
      {
     sprintf(sLine, "Start Formating(arab)");
   trace_event(sLine, PROCESSING);	
   
   
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;

		 / * Separateur ---------------------------------* /
         memcpy(notes_formatted + offset,      " ",  1);   
         offset+= 1;
			
         / * - init ---------------------------- * /
		   memset(note_type,          		 0, sizeof(note_type));
		   memset(amount_note,           	 0, sizeof(amount_note));
		   memset(amount_note1,           	 0, sizeof(amount_note1));
		   memset(lIntegerPart,  			 0, sizeof(lIntegerPart)   );
		   memset(lDecimalPart,  			 0, sizeof(lDecimalPart)   ); 		   
		   memset(count_note,           	 0, sizeof(count_note));
		   memset(total_amount,           	 0, sizeof(total_amount));
         / * - montant ligne ------------------- * /
		 memcpy (note_type, notes_record + cpt , 2);
		 memcpy (count_note, notes_record + 2 + cpt , 3);
		 
		GetDenominationInfo(nIndexCtx, TreminalNumber, AcquirerBank, note_type, &lNoteValue);
	
		 if (lNoteValue > 0)
			amount = lNoteValue * atoi(count_note );
		
		else
		{   / * en cas d'echec de la fct GetDenominationInfo * /
			if (atoi(note_type) == 1)
			 {
			   amount = 1 * atoi(count_note );
			   memcpy (amount_note, "  1", 3);
			 }
			 else if (atoi(note_type) == 2)
			 {
			   amount = 5 * atoi(count_note );
			   memcpy (amount_note, "  5", 3);
			 }		 
			 else if (atoi(note_type) == 3)
			 {
			   amount = 10 * atoi(count_note );
			   memcpy (amount_note, " 10", 3);
			 }	
			 else if (atoi(note_type) == 4)
			 {
			   amount = 20 * atoi(count_note );
			   memcpy (amount_note, " 20", 3);
			 }
			 else if (atoi(note_type) == 5)
			 {
			   amount = 0.5 * atoi(count_note );
			   memcpy (amount_note, "0 5", 3);
			 }				 
			 else
			 {
				amount = 0.0;
				memcpy (count_note,  "XXX", 3);
				memcpy (amount_note, "XXX", 3);
			 }
		 }
		 
		 / * Total amount * /	
		 amountLen = 0;
		 sprintf(amount_note1, "%.3f", amount); 		 
		 GetIntegerPart(amount_note1, lIntegerPart); 
		 GetDecimalPart(amount_note1, lDecimalPart);		
				 
		 memcpy(total_amount,      lIntegerPart,  strlen(lIntegerPart));   
		 amountLen+=  strlen(lIntegerPart);					 
		
		
         total_amount[amountLen] = ESC;            
         amountLen+=1;
         total_amount[amountLen] = '(';            
         amountLen+=1;
         total_amount[amountLen] = '1';            
         amountLen+=1;		 

		 memcpy(total_amount + amountLen,      ",",  1);   
		 amountLen+= 1;			 
		 
		 total_amount[amountLen] = ESC;            
         amountLen+=1;
         total_amount[amountLen] = '(';            
         amountLen+=1;
         total_amount[amountLen] = '7';            
         amountLen+=1;

		 memcpy(total_amount + amountLen,      lDecimalPart,  strlen(lDecimalPart));   
		 amountLen+= strlen(lDecimalPart);		 
		 
		 / * formatage * /
		 memset ( formatedAmount, ' ', 10);
		 memcpy (formatedAmount + 17 - amountLen  , total_amount, amountLen);
		 
		 memcpy (notes_formatted + offset, formatedAmount, strlen( formatedAmount));
		 offset+= strlen( formatedAmount);

		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	
		 
		 
		 
         / * - signe de la ligne --------------- * /
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '1';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      "=",  1);   
		 offset+= 1;		 

         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	
		
		 memcpy (notes_formatted + offset, count_note, 3);
		 offset+= 3;		 
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 
		 offset+= 1;			 
         / * - signe de la ligne --------------- * /
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '1';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      "*",  1);   
		 offset+= 1;		 

         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	
		 
		 memset(lIntegerPart,  			 0, sizeof(lIntegerPart)   );
		 memset(lDecimalPart,  			 0, sizeof(lDecimalPart)   ); 	
		   
		 if (lNoteValue < 1)
		    {
				 sprintf(amount_note, "%.3f", lNoteValue);		
				 GetIntegerPart(amount_note, lIntegerPart); 
				 GetDecimalPart(amount_note, lDecimalPart);				 
			 }
		 else
			{
			sprintf(lIntegerPart, "%.0f", lNoteValue);	
			memcpy(lDecimalPart, "000", 3);
			}
		 
		 memcpy (notes_formatted + offset, lIntegerPart, strlen(lIntegerPart));
		 offset+= strlen(lIntegerPart);		

         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '1';            
         offset+=1;		
         notes_formatted[offset] = ',';            
         offset+=1;		 
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;		 
		 memcpy (notes_formatted + offset, lDecimalPart, strlen(lDecimalPart));
		 offset+= strlen(lDecimalPart);				 
     sprintf(sLine, "line : %s", notes_formatted);
     trace_event(sLine, PROCESSING);
	 
		 }
      else
      {
     sprintf(sLine, "Start Formating(ENG)");
     trace_event(sLine, PROCESSING);	
		 / * Separateur ---------------------------------* /
         memcpy(notes_formatted + offset,      " ",  1);   
         offset+= 1;
			
         / * - init ---------------------------- * /
		   memset(note_type,          		 0, sizeof(note_type));
		   memset(amount_note,           	 0, sizeof(amount_note));
		   memset(count_note,           	 0, sizeof(count_note));
		   memset(total_amount,           	 0, sizeof(total_amount));
         / * - montant ligne ------------------- * /
		 memcpy (note_type, notes_record + cpt , 2);
		 memcpy (count_note, notes_record + 2 + cpt , 3);	 
		 
		GetDenominationInfo(nIndexCtx, TreminalNumber, AcquirerBank, note_type, &lNoteValue);

		 if (lNoteValue > 0)
		{

			sprintf( amount_note , "%.3f", lNoteValue);
			
			amount = lNoteValue * atoi(count_note );
		}
		else
		{	/ * en cas d'echec de la fct GetDenominationInfo * /	
			
			if (atoi(note_type) == 1)
			 {
			   amount = 1 * atoi(count_note );
			   memcpy (amount_note, "1.000", 5);
			 }
			 else if (atoi(note_type) == 2)
			 {
			   amount = 5 * atoi(count_note );
			   memcpy (amount_note, "5.000", 5);
			 }		 
			 else if (atoi(note_type) == 3)
			 {
			   amount = 10 * atoi(count_note );
			   memcpy (amount_note, "10.000", 6);
			 }	
			 else if (atoi(note_type) == 4)
			 {
			   amount = 20 * atoi(count_note );
			   memcpy (amount_note, " 20.000", 6);
			 }		
			 else if (atoi(note_type) == 5)
			 {
			   amount = 0.5 * atoi(count_note );
			   memcpy (amount_note, "0.500", 5);
			 }				 
			 else
			 {
				amount = 0.0;
				memcpy (count_note,  "XX.XXX", 6);
				memcpy (amount_note, "XX.XXX", 6);
			 }

		 }
			
		 Len = strlen(amount_note);
		 memcpy (notes_formatted + offset, amount_note, Len);
		 offset+= Len;	
		 
		 if (Len < 6)
		 {
		  		 memcpy(notes_formatted + offset,      " ",  1);   
				 offset+= 1;	
		 }
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      "*",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;			 
		 
		 memcpy (notes_formatted + offset, count_note, 3);
		 offset+= 3;		

		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      "= ",  2);   
		 offset+= 2;	
		 
		 sprintf(total_amount, "%.3f", amount);
		 memcpy (notes_formatted + offset, total_amount, strlen( total_amount));
		 offset+= strlen( total_amount);
		 
      }

      / * - saut de ligne ------------------------- * /
      notes_formatted[offset] = LF;               
      offset+= 1;
	  cpt = cpt + 5;
   }

   	   
   
   trace_event("End   FormatToDepNotes(OK)", PROCESSING);
   return (OK);
}
*/
/*RJE13072014 the function FormatToDepNotes is changed for QATAR region*/

int     FormatToDepNotes(
							int    nIndexCtx,
							char   *TreminalNumber,
							char   *AcquirerBank, /*DDA1201017*/
							char   *notes_record,
							char   *notes_formatted,
							char   lang)
{
   char            sLine[MAX_LINE_TRC];
   char            ligne            [256];
   char            note_type        [2+1];
   char            amount_note      [  5];
   char            count_note       [  5];
   char	           formatedAmount   [ 25];
   char 		   total_amount     [ 20];
   float           lNoteValue       = 0.0;
   double      	   amount;
   int             offset;
   int             trans_nbr;
   int             cpt   = 0;
   int             record_len; 	
   int             amountLen =0;
   
   sprintf(sLine, "Start FormatToDepNotes(%.16s,%.3c,%s)",TreminalNumber, lang,notes_record);
   trace_event(sLine, PROCESSING);

   /* - Raz des variables ------------------------------------------------- */
   memset(notes_formatted,           0, sizeof(notes_formatted));
   memset(note_type,          		 0, sizeof(note_type));
   memset(amount_note,           	 0, sizeof(amount_note));
   memset(count_note,           	 0, sizeof(count_note));
   memset(total_amount,           	 0, sizeof(total_amount));
   memset(formatedAmount,            0, sizeof(formatedAmount));
 
   record_len = strlen (notes_record);
 

   /* - control de la langueur -------------------------------------------- */
   if ( record_len == 0)
   {
      trace_event("End   FormatToDepNotes(LENGTH ERROR)", PROCESSING);
      return (NOK);
   }
   /* - formatage du releve ----------------------------------------------- */
   offset = 0;
	


 while (cpt < record_len)
	{
   sprintf(sLine, "Start Formating(cpt(%d))",cpt);
   trace_event(sLine, PROCESSING);	
   lNoteValue = 0.0;
   amount = 0.0;
   
      if(lang == ARABIC)
      {
     sprintf(sLine, "Start Formating(arab)");
   trace_event(sLine, PROCESSING);	
   
   
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;

		 /* Separateur ---------------------------------*/
         memcpy(notes_formatted + offset,      " ",  1);   
         offset+= 1;
			
         /* - init ---------------------------- */
		   memset(note_type,          		 0, sizeof(note_type));
		   memset(amount_note,           	 0, sizeof(amount_note));
		   memset(count_note,           	 0, sizeof(count_note));
		   memset(total_amount,           	 0, sizeof(total_amount));
         /* - montant ligne ------------------- */
		 memcpy (note_type, notes_record + cpt , 2);
		 memcpy (count_note, notes_record + 2 + cpt , 3);
		 
		GetDenominationInfo(nIndexCtx, TreminalNumber, AcquirerBank /*DDA12012017*/, note_type, &lNoteValue);
	
		 if (lNoteValue > 0)
		{
			if (lNoteValue < 100)
			    sprintf( amount_note , " %2.0f", lNoteValue);
			else
			    sprintf( amount_note , "%3.0f", lNoteValue);
			
			amount = lNoteValue * atoi(count_note );
		}
		else
		{   /* en cas d'echec de la fct GetDenominationInfo */
			if (atoi(note_type) == 1)
			 {
			   amount = 50 * atoi(count_note );
			   memcpy (amount_note, " 50", 3);
			 }
			 else if (atoi(note_type) == 2)
			 {
			   amount = 100 * atoi(count_note );
			   memcpy (amount_note, "100", 3);
			 }		 
			 else if (atoi(note_type) == 3)
			 {
			   amount = 500 * atoi(count_note );
			   memcpy (amount_note, "500", 3);
			 }	
			 else
			 {
				amount = 0.0;
				memcpy (count_note,  "XXX", 3);
				memcpy (amount_note, "XXX", 3);
			 }
		 }
		 
		 /* Total amount */	 
		 sprintf(total_amount, "%.0f", amount);		 
		 amountLen = strlen (total_amount);

		 /* adding ,00 */
         total_amount[amountLen] = ESC;            
         amountLen+=1;
         total_amount[amountLen] = '(';            
         amountLen+=1;
         total_amount[amountLen] = '1';            
         amountLen+=1;		 

		 memcpy(total_amount + amountLen,      ",",  1);   
		 amountLen+= 1;			 
		 
		 total_amount[amountLen] = ESC;            
         amountLen+=1;
         total_amount[amountLen] = '(';            
         amountLen+=1;
         total_amount[amountLen] = '7';            
         amountLen+=1;

		 memcpy(total_amount + amountLen,      "00",  3);   
		 amountLen+= 3;		 
		 
		 /* formatage */
		 memset ( formatedAmount, ' ', 10);
		 memcpy (formatedAmount + 15 - amountLen  , total_amount, amountLen);
		 
		 memcpy (notes_formatted + offset, formatedAmount, strlen( formatedAmount));
		 offset+= strlen( formatedAmount);

		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	
		 
		 
		 
         /* - signe de la ligne --------------- */
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '1';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      "=",  1);   
		 offset+= 1;		 

         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	
		
		 memcpy (notes_formatted + offset, count_note, 3);
		 offset+= 3;		 
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 
		 offset+= 1;			 
         /* - signe de la ligne --------------- */
         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '1';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      "*",  1);   
		 offset+= 1;		 

         notes_formatted[offset] = ESC;            
         offset+=1;
         notes_formatted[offset] = '(';            
         offset+=1;
         notes_formatted[offset] = '7';            
         offset+=1;		 
		 
		 memcpy(notes_formatted + offset,      " ",  1);   
		 
		 offset+= 1;	
		 
		 memcpy (notes_formatted + offset, amount_note, 3);
		 offset+= 3;			 
		 }
      else
      {
     sprintf(sLine, "Start Formating(ENG)");
     trace_event(sLine, PROCESSING);	
		 /* Separateur ---------------------------------*/
         memcpy(notes_formatted + offset,      " ",  1);   
         offset+= 1;
			
         /* - init ---------------------------- */
		   memset(note_type,          		 0, sizeof(note_type));
		   memset(amount_note,           	 0, sizeof(amount_note));
		   memset(count_note,           	 0, sizeof(count_note));
		   memset(total_amount,           	 0, sizeof(total_amount));
         /* - montant ligne ------------------- */
		 memcpy (note_type, notes_record + cpt , 2);
		 memcpy (count_note, notes_record + 2 + cpt , 3);	 
		 
		GetDenominationInfo(nIndexCtx, TreminalNumber, AcquirerBank /*DDA12012017*/, note_type, &lNoteValue);

		 if (lNoteValue > 0)
		{
			if (lNoteValue < 100)
			    sprintf( amount_note , " %2.0f", lNoteValue);
			else
			    sprintf( amount_note , "%3.0f", lNoteValue);
			
			amount = lNoteValue * atoi(count_note );
		}
		else
		{	/* en cas d'echec de la fct GetDenominationInfo */	
			 if (atoi(note_type) == 1)
			 {
			   amount = 50 * atoi(count_note );
			   memcpy (amount_note, " 50", 3);
			 }
			 else if (atoi(note_type) == 2)
			 {
			   amount = 100 * atoi(count_note );
			   memcpy (amount_note, "100", 3);
			 }		 
			 else if (atoi(note_type) == 3)
			 {
			   amount = 500 * atoi(count_note );
			   memcpy (amount_note, "500", 3);
			 }	
			 else
			 {
				amount = 0.0;
				memcpy (count_note,  "XXX", 3);
				memcpy (amount_note, "XXX", 3);
			 }
		 }

		 memcpy (notes_formatted + offset, amount_note, 3);
		 offset+= 3;	

		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      "*",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;			 
		 
		 memcpy (notes_formatted + offset, count_note, 3);
		 offset+= 3;		

		 memcpy(notes_formatted + offset,      " ",  1);   
		 offset+= 1;	

		 memcpy(notes_formatted + offset,      "= ",  2);   
		 offset+= 2;	
		 
		 sprintf(total_amount, "%.2f", amount);
		 memcpy (notes_formatted + offset, total_amount, strlen( total_amount));
		 offset+= strlen( total_amount);
		 
      }

      /* - saut de ligne ------------------------- */
      notes_formatted[offset] = LF;               
      offset+= 1;
	  cpt = cpt + 5;
   }

   notes_formatted[offset] = LF; 
   	   
   
   trace_event("End   FormatToDepNotes(OK)", PROCESSING);
   return (OK);
}
