#include <stdio.h>

#include <define.h>
#include <ascii.h>
#include <paramdef.h>
#include <math.h>
#include <string.h>
#include <langue.h>
#include <atmcutoff.h>
#include <thr_gabd_extern.h>

#define   PRN_WIDTH   40


/*******************************************************************************
**  nom    : FormatToStatement(record_data, statement) -> renvoit la          **
**         chaine formater du releve.                                         **
**  Entree : record_data -> chaine de donnees pour le releve.                 **
**  Sortie : statement -> chaine formatee a envoyer au GAB NDC.               **
**  Return : OK -> fonctionnement normal.                                     **
*******************************************************************************/
int     FormatToStatement(nIndexCtx,record_data, statement, currency)
int		nIndexCtx;
char    record_data     [512];
char    statement       [512];
char    currency        [  4];
{
   char            sLine			[MAX_LINE_TRC];
   char            transaction   	[LG_MAX];
   char            date            	[ 21];
   char            amount      		[ 41];
   char            balance     		[ 41];
   char            mm      			[  3];
   char            dd      			[  3];
   int      	   len;
   int             offset;
   int             trans_nbr;
   int             cpt;

   sprintf(sLine, "Start FormatToStatement(%.3s)", currency);
   trace_event(sLine, PROCESSING);

   /* - Raz des variables ------------------------------------------------- */
   memset(statement,           0, sizeof(statement));

   /* - nombre de lignes du releve ---------------------------------------- */
   sscanf(record_data, "%2d", &trans_nbr);
   
   sprintf(sLine, "Longeur : [%.4d] Nombre Trx :  [%.3d]",(int)strlen(record_data),trans_nbr);
   trace_event(sLine, PROCESSING);

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
      memset(date,      	0, sizeof(date)      );
      memset(amount,      	0, sizeof(amount)   );
      memset(balance,      	0, sizeof(balance)   );

      /* - formatage d'une ligne ------------------------------------ */
      memcpy(transaction,
          record_data + 2 + NP_SHORT_STAT_LEN*cpt,
          NP_SHORT_STAT_LEN);
      trace_event(transaction, DATA);
      /* - date transaction ---------------------- */
       memcpy(date,   transaction + NP_DATE_POS,  NP_DATE_LEN);
       FormatToDate(date);
       memcpy(statement + offset,      date, 8);      
       offset+= 8;

       /* - separateur ---------------------------- */
       memcpy(statement + offset,       " ", 1);      
       offset+= 1;

       /* - montant transaction ------------------- */
       memcpy(amount,      transaction + NP_AMT_POS, NP_AMT_LEN);
       len = FormatToAmount(nIndexCtx,amount, currency, NOK, 'R');
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
          memcpy(statement + offset,      " ",  1);   
          offset+= 1;
       }

       /* - separateur ---------------------------- */
       memcpy(statement + offset,       " ", 1);      
       offset+= 1;

       /* - balance ------------------------------- */
       memcpy(balance,   transaction + NP_BAL_POS, NP_BAL_LEN);
       len = FormatToAmount(nIndexCtx,balance, currency, NOK, 'R');
       memcpy(statement + offset,   balance+len-12, 12);   
       offset+=12;

       /* - signe de la transaction --------------- */
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

      /* - saut de ligne ------------------------- */
      statement[offset] = '$';     
	  
      offset+= 1;
   }

   trace_event("End   FormatToStatement(OK)", PROCESSING);
   return (OK);
}

/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToDateTime(date)
char            date[36];
{
   char            	sLine	[MAX_LINE_TRC];
   char            	target	[36];
   int      		offset;

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
int     FormatToDate(date)
char            date[24];
{
   char            	sLine	[MAX_LINE_TRC];
   char            	target	[24];
   int      		offset;

   memset(target,      0, sizeof(target));         
   offset =0;
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
   memcpy(date, target, offset);

   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToHour(hour)
char            hour[9];
{
   char            	sLine	[MAX_LINE_TRC];
   char            	target	[24];
   int      		offset;

   memset(target,      0, sizeof(target));         
   offset =0;

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

   memcpy(hour, target, offset);

   return (offset);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToCardNbr(card_nbr)
char            card_nbr[23];
{
   char            sLine[MAX_LINE_TRC];
   char            target[23];

   memset(target, 0, sizeof(target));
   memset(target, ' ', 22);
   memcpy(target + 11 - strlen(card_nbr)/2, card_nbr, strlen(card_nbr));
   memcpy(card_nbr, target, strlen(target));
   return (22);
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToAccountNbr(account_nbr,alignement)
char   account_nbr[21];
char   alignement;
{
   char            sLine	[MAX_LINE_TRC];
   char            target	[21];

   memset(target, 0, sizeof(target));
   memcpy(target, account_nbr, strlen(account_nbr));
   if(alignement == 'R' || alignement == 'r')
      memcpy(account_nbr + 20 - strlen(target), target, strlen(target));
   else if(alignement == 'C' || alignement == 'c')
      memcpy(account_nbr + 10 - strlen(target)/2, target, strlen(target));
   else
      memcpy(account_nbr, target+2, strlen(target)-2);

   return (strlen(account_nbr));
}


/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/*-=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToAmount(nIndexCtx,amount, currency, currency_flag, alignement)
int	nIndexCtx;
char   	amount  [21];
char   	currency[ 4];
int   	currency_flag;
char   	alignement;
{
   char           	sLine[MAX_LINE_TRC];
   char           	target   [99];
   char           	cur_alpha[ 4];
   char      		lIntegerPart   [13];
   char      		lDecimalPart   [13];
   int             	offset;
   int      		lEscLen;
   int             	retour;

   memset(target,      		0, sizeof(target)   );
   memset(lIntegerPart,   	0, sizeof(lIntegerPart)   );
   memset(lDecimalPart,   	0, sizeof(lDecimalPart)   );
   offset = 0;
   lEscLen = 0;

   retour = adjust_amount_float(nIndexCtx,amount, currency);
   if(retour == OK)
   {
      /* - formatage ------------------------------------------------------ */
      if (currency_flag == OK)
      {
         retour = GetCurrencyAlpha(nIndexCtx,currency, cur_alpha);
         if (retour == OK)
         {
               memcpy(target + offset, cur_alpha, 3);   
               offset+=3;
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

         sprintf(target + offset, "%.12s", amount);
   }
   else
      sprintf(target + offset, "%01.0f", atof(amount));

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
         trace_event(amount, DATA);
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
   return (offset);
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToRate(rate, alignement)
char            rate[128];
char            alignement;
{
   char            	sLine		[MAX_LINE_TRC];
   char            	target      [128];
   int      		exponent;
   int             	len;

   memset(target,      0, sizeof(target)       );
   len = 0;
   sscanf(rate, "%1d", &exponent);
   memcpy(target + len, rate + 1, 7 - exponent);      
   len+=7-exponent;
   memcpy(target + len, ".", 1);            
   len+=1;
   memcpy(target + len, rate + 1 + 7 - exponent, exponent);
   len+=exponent;
   memset(rate, 0, len + 1);
   memcpy(rate, target, len);

   return (len);
}
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToBalanceGABD(nIndexCtx,balance,alignement)
int				nIndexCtx;
char            balance[128];
char			alignement;
{
   char            sLine			[MAX_LINE_TRC];
   char            target          	[128];
   char            currency        	[  4];
   char            sign;
   int             len;

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   memset(balance,     0, 128);
   if(sign == 'D')
         balance[0] = '-';
   else
         balance[0] = '+';
   memcpy(balance+1,     target+1, 11);
   return (strlen(balance));
}
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int     FormatToBalance(nIndexCtx,balance,alignement)
int				nIndexCtx;
char            balance[128];
char			alignement;
{
   char            sLine			[MAX_LINE_TRC];
   char            target          	[128];
   char            currency        	[  4];
   char            sign;
   int             len;

   memset(target,      0, sizeof(target)       );
   memset(currency,    0, sizeof(currency)     );

   sign = balance[7];
   memcpy(currency,    balance +  4,    3);
   memcpy(target,      balance +  8,   12);
   len = FormatToAmount(nIndexCtx,target, currency, NOK, alignement);
   memset(balance,     0, 128);
   memcpy(balance,     target, strlen(target));
   if(sign == 'D')
         balance[strlen(balance)] = '-';
   return (strlen(balance));
}

/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   GetIntegerPart(pAmount, pIntegerPart)
char   pAmount[];
char   pIntegerPart[];
{
   char            	sLine[MAX_LINE_TRC];
   int   			cpt;

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
   char            sLine[MAX_LINE_TRC];
   int   cpt;

   sprintf(sLine, "amount   = %.13s", pAmount);
   trace_event(sLine, PROCESSING);

   memset(pDecimalPart,   0, 13);

   for(cpt = strlen(pAmount) - 1; cpt > 0; cpt --)
      if(pAmount[cpt] == '.')
         break;

   memcpy(pDecimalPart, pAmount + cpt + 1, strlen(pAmount) - cpt);

   sprintf(sLine, "decimal  = %.13s", pDecimalPart);
   trace_event(sLine, PROCESSING);

   return (OK);
}


/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
/* -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- =-= -=- */
int   AlignField(
char   pTransName[],
char   pAlign,
int   pWidth
)
{
   char            sLine[MAX_LINE_TRC];
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
int   FormatToCutoffData(
int				nIndexCtx,
SCutOffData     *pCutOffData,
char         	pLang
)
{
   char            detail[MAX_LINE_TRC];
   char   lTarget      [1024];
   char   lCurrencyCode   [   4];
   char   lCurrencyAlpha   [   4];
   char   lAmt      [  56];
   int   lTargetOff;
   int   lCutOffDataLen;
   int   lLen;
   int   lCpt;
   int   lRet;

   /* - initialisations ---------------------------------------------------- */
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

      lRet = GetCurrencyAlpha(lCurrencyCode, lCurrencyAlpha);
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
      memcpy(lTarget + lTargetOff, "AGL TRN NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fACOAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fACOAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* - Montant de transactions approuvees apres arrete global --------- */
      memcpy(lTarget + lTargetOff, "AGL TRN AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fACOAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fACOAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L');

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* - Nombre de transactions confirmees suspendues dans le SAF ------- */
      memcpy(lTarget + lTargetOff, "BGL SAF NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fSAFAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fSAFAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* - Monttant de transactions confirmees suspendues dans le SAF ----- */
      memcpy(lTarget + lTargetOff, "BGL SAF AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fSAFAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fSAFAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L');

      memcpy(lTarget + lTargetOff, lAmt, lLen);
      lTargetOff += lLen;

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* - Nombre de transactions confirmees purgees du SAF --------------- */
      memcpy(lTarget + lTargetOff, "BGL PRG NBR : ", 14);   
      lTargetOff+=14;
      memcpy(lTarget + lTargetOff,
          pCutOffData->fCurrData.fPRGAppTrnNbr + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGAppTrnNbr));
      lTargetOff += sizeof(pCutOffData->fCurrData.fPRGAppTrnNbr);

      /* - Retour a la ligne ---------------------------------------------- */
      lTarget[lTargetOff]  = LF;            
      lTargetOff+=1;

      /* - Montant de transactions confirmees purgees du SAF -------------- */
      memcpy(lTarget + lTargetOff, "BGL PRG AMT : ", 14);   
      lTargetOff+=14;

      memset(lAmt, 0, sizeof(lAmt));
      memcpy(lAmt,
          pCutOffData->fCurrData.fPRGAppTrnAmt + lCpt*sizeof(SCurrData),
          sizeof(pCutOffData->fCurrData.fPRGAppTrnAmt));
      lLen = FormatToAmount(nIndexCtx,lAmt, lCurrencyCode, OK, 'L');

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

int CopyP7Buffer(pP7BuffIn, pP7BuffOut)
TSIsoInfo *pP7BuffIn;
TSIsoInfo *pP7BuffOut;
{
int     lCpt;
int     lgField;
char    tmpField[LG_MAX];


trace_event("Start CopyP7Buffer()", PROCESSING);

    for(lCpt = 0; lCpt < 128; lCpt ++)
        if (GetIsoField( lCpt, pP7BuffIn, tmpField, &lgField) == SUCCES )
            PutIsoField(lCpt, pP7BuffOut, tmpField, lgField);

trace_event("End   CopyP7Buffer(OK)", PROCESSING);
return (OK);
}

