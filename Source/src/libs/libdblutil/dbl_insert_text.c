#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <define.h>
#include <dump.h>
#include <ascii.h>
#include <dbl_paramdef.h>
#include <langue.h>
#include <iso_hps.h>
#include <security_data.h>

#define	PRN_WIDTH	40

/*extern trace_event(char*, int); EBE140515: wrong signature*/

/*******************************************************************************
**  nom    : deplacer(ptr, pos, offset) -> deplace la sous chaine localisee   **
**		par 'pos' de la chaine 'ptr->data' de 'offset' carac          **
**  Entree : ptr -> pointeur sur la structure dbl_param.	              **
**		  pos -> position a partir de la quelle if faut	              **
**		deplacer.					              **
**		  offset -> distance de deplacement.		              **
**  Sortie : Aucune.							      **
**  Return : OK -> fonctionnement normal.			              **
**		  ERROR -> depacement de la langueur de la chaine             **
*******************************************************************************/
int deplacer(ptr,pos,offset)
dbl_param	*ptr;
char		*pos;
int		offset;
{
int counter;
  if (ptr->len + offset <= MAXLEN)
  {
    for(counter = ptr->len; counter >= pos - ptr->data; counter--)
      ptr->data[counter + offset] = ptr->data[counter];
    ptr->len = ptr->len + offset;
    return (OK);
  }
  else
  {
    return (ERROR);
  }
}

/*******************************************************************************
**  no m   : insert_text(c_prints, P7Buffer) -> insert les donnees	      **
**		appropriesde 'P7Buffer' dans 'c_prints->data'.             **
**								              **
**  Entree : c_prints -> structure dbl_param.		                      **
**		  P7Buffer -> buffer contenant les donnees a	              **
**		imprimer.					              **
**								              **
**  Sortie : Aucune.					                      **
**								              **
**  Return : OK -> fonctionnement normal.			              **
**		  ERROR -> code introvable.			              **
**		  ERROR -> deplacement impossible.		              **
**								              **
*******************************************************************************/

int	insert_text(nIndexCtx,c_prints, P7Buffer, lang)
int			nIndexCtx;
dbl_param	*c_prints;
TSIsoInfo	*P7Buffer;
char		lang;
{
char		*searched_pos;
char		*current_pos;
int			current_len;
int			offset;
char		code		[   2];
char		record_data	[1024];
char		statement	[1024];
char		field		[LG_MAX];
char		term_nbr	[  16];
char		Iso_AcqBank	[6+1   ];/*ICH270715*/
char		currency	[   4];
char		tmpField	[LG_MAX];
char		sLine		[MAX_LINE_TRC];
int			lgField;

char             szScrtyData[256];
char             szDestRsrc[20];
char             szIssRespCode[10];
int              ret;

sprintf(sLine, "Start insert_text(%c)", lang);
trace_event(sLine, PROCESSING);

  memset(record_data,   0, sizeof(record_data));
  memset(statement,     0, sizeof(statement));
  memset(currency,	0, sizeof(currency));
  memset(Iso_AcqBank,			0, sizeof(Iso_AcqBank));/*ICH270715*/

  current_pos = c_prints->data;
  current_len = c_prints->len;
  while((searched_pos=(char *)memchr(current_pos, STX, current_len)) != NULL)
  {
    memcpy(code, searched_pos + 1, 2);
    
    sprintf(sLine, "DEBUG CODE=[%.2s]", code);
    trace_event(sLine, PROCESSING);
   

    /*
	- OPERATION_DATE -------------------------------------------------
    */
    if (memcmp(code, NP_OPERATION_DATE, 2) == 0)
    { if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,		0, sizeof(field));
        memcpy(field,tmpField,lgField - 6 );
        offset = FormatToDate(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_OPERATION_DATE_LEN);
        offset = NP_OPERATION_DATE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }
    
    /*
	- OPERATION_DATE_TIME -------------------------------------------------
    */
    else if (memcmp(code, NP_OPERATION_DATE_TIME, 2) == 0)
    { if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,		0, sizeof(field));
        memcpy(field, tmpField, lgField );
        offset = FormatToDateTime(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_OPERATION_DATE_TIME_LEN);
        offset = NP_OPERATION_DATE_TIME_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- BALANCE_DATE -------------------------------------------------
    */
    else if (memcmp(code, NP_BALANCE_DATE, 2) == 0)
    { if ( GetIsoField( ISO_ACTION_DATE , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,		0, sizeof(field));
        memcpy(field, tmpField,lgField); 
        offset = FormatToDate(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_BALANCE_DATE_LEN);
        offset = NP_BALANCE_DATE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- OPERATION_HOUR -------------------------------------------------
    */
    else if (memcmp(code, NP_OPERATION_HOUR, 2) == 0)
    { if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField + 6, lgField - 6);
        offset = FormatToHour(field, lang) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_OPERATION_HOUR_LEN);
        offset = NP_OPERATION_HOUR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- CARD_NBR -------------------------------------------------
    */
    else if (memcmp(code, NP_CARD_NBR, 2) == 0)
    { if ( GetIsoField( ISO_CARD_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField,lgField );
        offset = lgField - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_CARD_NBR_LEN);
        offset = NP_CARD_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- TRUNC_CARD_NBR -------------------------------------------------
    */
    else if (memcmp(code, NP_TRUNC_CARD_NBR, 2) == 0)
    {
	 if ( GetIsoField( ISO_CARD_NBR , P7Buffer, tmpField, &lgField) == SUCCES ) { 
		memset(field, 0, sizeof(field));
        	memcpy(field, tmpField,lgField - 4 );
        	if ( lang == ARABIC )
           		memcpy(field + lgField - 4, "----", 4);
        	else
           		memcpy(field + lgField - 4, "----", 4);

        	offset = lgField - 3;
      	} else { 
		memset(field, 0, sizeof(field));
        	memset(field,   ' ', NP_CARD_NBR_LEN);
        	offset = NP_CARD_NBR_LEN - 3;
      	}

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- SOURCE_ACCOUNT -------------------------------------------------
    */
    else if (memcmp(code, NP_SOURCE_ACCOUNT, 2) == 0)
    { if ( GetIsoField( ISO_ACCOUNT_ID1 , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
	/*BEGIN Adil HMAMI: 10/08/2005*/
        /*memcpy(field,tmpField+2,lgField-2); offset = strlen(field) - 3;*/
        memcpy(field,tmpField, lgField);
	 offset = strlen(field) - 3;
	/*END Adil HMAMI*/
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	'-', NP_SOURCE_ACCOUNT_LEN);
        offset = NP_SOURCE_ACCOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- TARGET_ACCOUNT -------------------------------------------------
    */
    else if (memcmp(code, NP_TARGET_ACCOUNT, 2) == 0)
    { if ( GetIsoField( ISO_ACCOUNT_ID2 , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
	/*BEGIN Adil HMAMI: 10/08/2005*/
        /*memcpy(field, tmpField+2,lgField-2);*/
        memcpy(field, tmpField, lgField);
	/*END Adil HMAMI*/
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_TARGET_ACCOUNT_LEN);
        offset = NP_TARGET_ACCOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- ATM_NUMBER -----------------------------------------------------
    */
    else if (memcmp(code, NP_ATM_NUMBER, 2) == 0)
    {if (GetIsoField( ISO_TERMINAL_NBR, P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField, lgField );
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_ATM_NUMBER_LEN);
        offset = NP_ATM_NUMBER_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- ATM_NAME -----------------------------------------------------
    */
    else if (memcmp(code, NP_ATM_NAME, 2) == 0)
    {if (GetIsoField( ISO_TERMINAL_NBR, P7Buffer, tmpField, &lgField) == SUCCES
 		   && GetIsoField(ISO_ACQR_ID ,P7Buffer, Iso_AcqBank, &lgField)== SUCCES  )
      { memset(field, 0, sizeof(field));
        memset(term_nbr, 0, sizeof(term_nbr));

        memcpy(term_nbr,tmpField,lgField); 
        offset = GetPtServName(nIndexCtx,term_nbr,Iso_AcqBank,/*ICH270715*/ field) - 3;
        if ( offset < 0 )
        { offset = 0;
          memcpy(field, "   ", 3);
        }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_ATM_NAME_LEN);
        offset = NP_ATM_NAME_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- OPERATION_NUMBER -----------------------------------------------
    */
    else if (memcmp(code, NP_OPERATION_NUMBER, 2) == 0)
    {if (GetIsoField( ISO_AUDIT_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field,tmpField,lgField);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field, 0, sizeof(field));
        memset(field,	' ', NP_OPERATION_NUMBER_LEN);
        offset = NP_OPERATION_NUMBER_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- BALANCE --------------------------------------------------------
    */
    /*else if ( ( memcmp(code, NP_BALANCE, 2)		== 0)
          ||  ( memcmp(code, NP_AVAILABLE_BALANCE, 2)	== 0)
          ||  ( memcmp(code, NP_LOAN_BALANCE, 2)	== 0)
            )
    {
	if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES ) { 
		if ( lgField >=23 ) { memset(field,		0, sizeof(field));
        		memcpy(field, tmpField + 3 - 3, 20);
        		offset = FormatToBalance(nIndexCtx,field, 'L', lang) - 3;
        		if (offset < 0) offset = 0;
      		}
	} else { 
		memset(field, 0, sizeof(field));
        	memset(field,	' ', NP_BALANCE_LEN);
        	offset = NP_BALANCE_LEN - 3;
   	}

      	if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        	memcpy(searched_pos, field, strlen(field));
      	else { 
		trace_event("End   insert_text(ERROR)",PROCESSING);
		return (ERROR);
      	}
    }*/

    /*
	- ACCOUNT_BALANCE ------------------------------------------------
    */
    /*else if ( memcmp(code, NP_ACCOUNT_BALANCE, 2)	== 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=43 ) 
      { memset(field,		0, sizeof(field));
        memcpy(field, tmpField + 23 - 3, 20);
        offset = FormatToBalance(nIndexCtx,field, 'L') - 3;
        if (offset < 0) offset = 0;
      }
	  }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_ACCOUNT_BALANCE_LEN);
        offset = NP_BALANCE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }*/

    /*BEGIN AHM 25/02/2006*/
        /*
        - BALANCE --------------------------------------------------------
    */
    else if ( ( memcmp(code, NP_BALANCE, 2)             == 0)
          ||  ( memcmp(code, NP_AVAILABLE_BALANCE, 2)   == 0)
          ||  ( memcmp(code, NP_LOAN_BALANCE, 2)        == 0)
            )
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=20 )
      { memset(field,           0, sizeof(field));
       memset(field, ' ', 18);

        /* YOua 17Oct2003 : il faut baser la recherche sur le AMOUNT_TYPE */
        /*
        memcpy(field, tmpField, 20);
        */
        if (memcmp(code, NP_LOAN_BALANCE, 2) == 0)
           memcpy(field, tmpField, 20);
        else
        {
           offset = 0;
           while(offset < lgField){
               if (memcmp(tmpField + offset + 2, "02", 2) == 0){
                   memcpy(field, tmpField + offset, 20);
                   break;
               }
/** H.YOUSFI If 02 Not Found */
               else
                   memcpy(field, tmpField , 20);

               offset += 20;
           }
        }
        if (strlen(field) == 0){
           memset(field, ' ', NP_BALANCE_LEN);
           offset = NP_BALANCE_LEN - 3;
        }
        /* YOua 17Oct2003 */

        if(lang == ARABIC)
            offset = FormatToBalance(nIndexCtx,field, 'R',lang) - 3;
        else
            offset = FormatToBalance(nIndexCtx,field, 'L',lang) - 3;
        if (offset < 0) offset = 0;
      }
          }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_BALANCE_LEN);
        offset = NP_BALANCE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
        - ACCOUNT_BALANCE ------------------------------------------------
    */
    else if ( memcmp(code, NP_ACCOUNT_BALANCE, 2)       == 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=20 )
      { memset(field,           0, sizeof(field));
       memset(field, ' ', 18);
        /* YOua 17Oct2003 : il faut baser la recherche sur le AMOUNT_TYPE */
        /*
        memcpy(field, tmpField + 20, 20);
        */
        offset = 0;
        while(offset < lgField){
            if (memcmp(tmpField + offset + 2, "01", 2) == 0){
                memcpy(field, tmpField + offset, 20);
                break;
            }
/** H.YOUSFI If 02 Not Found */ 
               else
                   memcpy(field, tmpField , 20);
            offset += 20;
        }
        if (strlen(field) == 0){
           memset(field, ' ', NP_BALANCE_LEN);
           offset = NP_BALANCE_LEN - 3;
        }
        /* YOua 17Oct2003 */

        if(lang == ARABIC)
            offset = FormatToBalance(nIndexCtx,field, 'R',lang) - 3;
        else
            offset = FormatToBalance(nIndexCtx,field, 'L',lang) - 3;
        if (offset < 0) offset = 0;
      }
          }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_ACCOUNT_BALANCE_LEN);
        offset = NP_BALANCE_LEN - 3;
      }


      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }
    /*END AHM*/

    /*
	- STATEMENT ------------------------------------------------------
    */
    /*BEGIN AHM 04/09/2004*/
       else if (memcmp(code, NP_STATEMENT, 2) == 0) {
                        if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES ) {
                                memcpy(record_data, tmpField , lgField );
                                GetIsoField( ISO_TRANS_CRNCY , P7Buffer, tmpField, &lgField);
                        FormatToStatement(nIndexCtx,record_data,
                                          statement,
                                  tmpField,
                                  lang);

                        offset = strlen(statement) - 3;
                        if ( offset < 0 ) {
                                        offset = 0;
                                memcpy(statement, "   ", 3);
                                }
                        if (deplacer(c_prints, searched_pos + 1, offset) == OK)
                                memcpy(searched_pos, statement, strlen(statement));
                        else {
                                        trace_event("End   insert_text(ERROR)",PROCESSING);
                                        return (ERROR);
                                }
                }
      else{
        memcpy(searched_pos, "***", 3);
                }
    }

    /*else if (memcmp(code, NP_STATEMENT, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
      { *memcpy(record_data, tmpField + 3,
                            lgField  - 3);*
		memcpy(record_data, tmpField , lgField);
		GetIsoField( ISO_TRANS_CRNCY , P7Buffer, tmpField, &lgField);
        FormatToStatement(nIndexCtx,
			  record_data,
                          statement,
                          tmpField);

        offset = strlen(statement) - 3;
        if ( offset < 0 )
        { offset = 0;
          memcpy(statement, "   ", 3);
        }

        if (deplacer(c_prints, searched_pos + 1, offset) == OK)
          memcpy(searched_pos, 
	         statement,
	         strlen(statement));
        else
        { trace_event("End   insert_text(ERROR)",PROCESSING);
	  return (ERROR);
        }
      }
      else
        memcpy(searched_pos, "***", 3);
    }*/
    /*END AHM*/
    
    /*
	- RESPONSE_CODE --------------------------------------------------
    */
    else if (memcmp(code, NP_RESPONSE_CODE, 2) == 0)
    { if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField, lgField);
        offset = strlen(field) - 3;
      }
      else
      { memset(field,	' ', NP_RESPONSE_CODE_LEN);
        offset = NP_RESPONSE_CODE_LEN - 3;
      }

      /* if faut inserer le message et non pas le code  */
      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - KNET_RESPONSE CODE --------------------------------------------- 
    else if (memcmp(code, NP_KNET_RESP_CODE, 2) == 0)
    { if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_KNET_RESP_CODE_LEN);
        download_knet_response_code(field, atoi(tmpField));
        memcpy(field + 2, " ", 1);
        offset = NP_KNET_RESP_CODE_LEN - 3;
      }
      else
      { memset(field,	' ', NP_KNET_RESP_CODE_LEN);
        offset = NP_KNET_RESP_CODE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }
	Non Utiliser pour le standard ---------------------------------------*/

    /* - CUTOFF DATA ---------------------------------------------------- */
    else if (memcmp(code, NP_CUTOFF_DATA, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField,
                      lgField);
        FormatToCutoffData(nIndexCtx,field, ENGLISH);
      }
      else
      { memset(field, 0, sizeof(field));
        memcpy(field, "THERE IS NO CUT OFF", 19);
      }

      offset = strlen(field) - 3;

      sprintf(sLine, "strlen(field)=[%d]", strlen(field));
	trace_event(sLine, PROCESSING);
      sprintf(sLine, "field=[%.70s]", field);
	trace_event(sLine, PROCESSING);

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, 
               field,
               strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- DEPOSIT_NUMBER --------------------------------------------------
    */
    else if (memcmp(code, NP_DEPOSIT_NUMBER, 2) == 0)
    {if (GetIsoField( ISO_RESERV_PRIV_USAGE_1 , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        sprintf(field,
                "%3d",
                atoi(tmpField + 3));
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_DEPOSIT_NUMBER_LEN);
        offset = NP_DEPOSIT_NUMBER_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
        - NOTE_1_NBR -----------------------------------------------------
    */
    else if (memcmp(code, NP_NOTE_1_NBR, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField+3,
               3);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_NOTE_1_NBR_LEN);
        offset = NP_NOTE_1_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
        - NOTE_2_NBR -----------------------------------------------------
    */
    else if (memcmp(code, NP_NOTE_2_NBR, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField + 6,
               3);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_NOTE_2_NBR_LEN);
        offset = NP_NOTE_2_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
        - NOTE_3_NBR -----------------------------------------------------
    */
    else if (memcmp(code, NP_NOTE_3_NBR, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField + 9,
               3);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_NOTE_3_NBR_LEN);
        offset = NP_NOTE_3_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
        - NOTE_4_NBR -----------------------------------------------------
    */
    else if (memcmp(code, NP_NOTE_4_NBR, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField + 12,
               3);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_NOTE_4_NBR_LEN);
        offset = NP_NOTE_4_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- TRANSACTION_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_TRANS_AMOUNT , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,	0, sizeof(field));
        memcpy(field,tmpField,lgField);
   	    GetIsoField(ISO_TRANS_CRNCY,P7Buffer, tmpField, &lgField);
        offset = FormatToAmount(nIndexCtx,field,
                                tmpField,
                                OK,
                                'L', 
				lang) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_AMOUNT_LEN);
        offset = NP_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- BILLING_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_BILLING_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_BILLING_AMOUNT , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,	0, sizeof(field));
        memcpy(field,tmpField,lgField);
   	    GetIsoField(ISO_BILLING_CRNCY,P7Buffer, tmpField, &lgField);
        offset = FormatToAmount(nIndexCtx,field,
                                tmpField,
                                OK,
                                'L') - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_AMOUNT_LEN);
        offset = NP_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- CONVERSION RATE --------------------------------------------
    */
    else if (memcmp(code, NP_CONVERSION_RATE, 2) == 0)
    {if (GetIsoField( ISO_BILLING_CNVRSN_RATE , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field,	0, sizeof(field));
        memcpy(field,tmpField,lgField);
        offset = FormatToRate(field, 'L') - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_RATE_LEN);
        offset = NP_RATE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }



    /*
	- FACILITY_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_FACILITY_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=23 ) 
      { memset(field,	0, sizeof(field));

        memcpy(field, tmpField + 43 - 3, 20);

        offset = FormatToBalance(nIndexCtx,field, 'L') - 3;
        if (offset < 0) offset = 0;
      }
	  }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_FACILITY_AMOUNT_LEN);
        offset = NP_FACILITY_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- PAST_DUE_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_PAST_DUE_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=43 ) 
      { memset(field,	0, sizeof(field));

        memcpy(field, tmpField + 23 - 3, 20);
        offset = FormatToBalance(nIndexCtx,field, 'L') - 3;
        if (offset < 0) offset = 0;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_PAST_DUE_AMOUNT_LEN);
        offset = NP_PAST_DUE_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- PENDING_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_PENDING_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=63 ) 
      { memset(field,	0, sizeof(field));

        memcpy(field, tmpField + 3 - 3, 20);
        offset = FormatToBalance(nIndexCtx,field, 'L') - 3;
        if (offset < 0) offset = 0;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_PENDING_AMOUNT_LEN);
        offset = NP_PENDING_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /*
	- UNUTILISED_AMOUNT ---------------------------------------------
    */
    else if (memcmp(code, NP_UNUTILISED_AMOUNT, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >=83 ) 
      { memset(field,	0, sizeof(field));
        memcpy(field, tmpField + 63 - 3, 20);
        offset = FormatToBalance(nIndexCtx,field, 'L') - 3;
        if (offset < 0) offset = 0;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', NP_UNUTILISED_AMOUNT_LEN);
        offset = NP_UNUTILISED_AMOUNT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN NUMBER ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_NBR_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_LOAN_NBR_OFF,
               LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN AMOUNT ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_AMT_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_LOAN_AMT_OFF,
               LN_LOAN_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_AMT_LEN);
        offset = LN_LOAN_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN START DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_START_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_LOAN_START_OFF,
               LN_LOAN_START_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_START_LEN);
        offset = LN_LOAN_START_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN EXPIRY DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_EXPIRY_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_LOAN_EXPIRY_OFF,
               LN_LOAN_EXPIRY_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_EXPIRY_LEN);
        offset = LN_LOAN_EXPIRY_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_AMT_NEXT_INSTL_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_AMT_NEXT_INSTL_OFF,
               LN_AMT_NEXT_INSTL_LEN);
         offset = FormatToAmount(nIndexCtx,field,
                                 currency,
                                 OK,
                                 'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
        offset = LN_AMT_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_DATE_NEXT_INSTL_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_DATE_NEXT_INSTL_OFF,
               LN_DATE_NEXT_INSTL_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
        offset = LN_DATE_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_AMT_LAST_PAID_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_AMT_LAST_PAID_OFF,
               LN_AMT_LAST_PAID_LEN);
         offset = FormatToAmount(nIndexCtx,field,
                                 currency,
                                 OK,
                                 'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_LAST_PAID_LEN);
        offset = LN_AMT_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_DATE_LAST_PAID_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_DATE_LAST_PAID_OFF,
               LN_DATE_LAST_PAID_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_LAST_PAID_LEN);
        offset = LN_DATE_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD INSTL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_INSTL_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_OUTSTD_INSTL_OFF,
               LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD BAL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_BAL_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_OUTSTD_BAL_OFF,
               LN_OUTSTD_BAL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_BAL_LEN);
        offset = LN_OUTSTD_BAL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE INSTL --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_INSTL_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 5 + NP_PAST_DUE_INSTL_OFF,
               LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE AMT --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_AMT_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_PAST_DUE_AMT_OFF,
               LN_PAST_DUE_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_AMT_LEN);
        offset = LN_PAST_DUE_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST ACCR INT -------------------------------------------- */
    else if (memcmp(code, NP_PAST_ACCR_INT_1, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 5 + NP_PAST_ACCR_INT_OFF,
               LN_PAST_ACCR_INT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_ACCR_INT_LEN);
        offset = LN_PAST_ACCR_INT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN NUMBER 2 ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_NBR_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
               LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN AMOUNT ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_AMT_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
                tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
               LN_LOAN_AMT_LEN);
         offset = FormatToAmount(nIndexCtx,field,
                                 currency,
                                 OK,
                                 'L') - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_AMT_LEN);
        offset = LN_LOAN_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN START DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_START_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
               LN_LOAN_START_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_START_LEN);
        offset = LN_LOAN_START_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN EXPIRY DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_EXPIRY_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
               LN_LOAN_EXPIRY_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_EXPIRY_LEN);
        offset = LN_LOAN_EXPIRY_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_AMT_NEXT_INSTL_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
               LN_AMT_NEXT_INSTL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
        offset = LN_AMT_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_DATE_NEXT_INSTL_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
               LN_DATE_NEXT_INSTL_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
        offset = LN_DATE_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_AMT_LAST_PAID_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
               LN_AMT_LAST_PAID_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_LAST_PAID_LEN);
        offset = LN_AMT_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_DATE_LAST_PAID_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
               LN_DATE_LAST_PAID_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_LAST_PAID_LEN);
        offset = LN_DATE_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD INSTL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_INSTL_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
               LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD BAL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_BAL_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency,  tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
               LN_OUTSTD_BAL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_BAL_LEN);
        offset = LN_OUTSTD_BAL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE INSTL --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_INSTL_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
               LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE AMT --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_AMT_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
                tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
               LN_PAST_DUE_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_AMT_LEN);
        offset = LN_PAST_DUE_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST ACCR INT -------------------------------------------- */
    else if (memcmp(code, NP_PAST_ACCR_INT_2, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
               LN_PAST_ACCR_INT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_ACCR_INT_LEN);
        offset = LN_PAST_ACCR_INT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN NUMBER 3 ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_NBR_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
               LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN AMOUNT ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_AMT_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
               LN_LOAN_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_AMT_LEN);
        offset = LN_LOAN_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN START DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_START_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
               LN_LOAN_START_LEN);
        offset = FormatToDate(field) - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_START_LEN);
        offset = LN_LOAN_START_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN EXPIRY DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_EXPIRY_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
               LN_LOAN_EXPIRY_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_EXPIRY_LEN);
        offset = LN_LOAN_EXPIRY_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_AMT_NEXT_INSTL_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
               LN_AMT_NEXT_INSTL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
        offset = LN_AMT_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_DATE_NEXT_INSTL_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
               LN_DATE_NEXT_INSTL_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
        offset = LN_DATE_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_AMT_LAST_PAID_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
               LN_AMT_LAST_PAID_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_LAST_PAID_LEN);
        offset = LN_AMT_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_DATE_LAST_PAID_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
               LN_DATE_LAST_PAID_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_LAST_PAID_LEN);
        offset = LN_DATE_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD INSTL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_INSTL_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField  + 2*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
               LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD BAL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_BAL_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
               LN_OUTSTD_BAL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_BAL_LEN);
        offset = LN_OUTSTD_BAL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE INSTL --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_INSTL_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
               LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE AMT --------------------------------------------- */
    /*else if (memcmp(code, NP_PAST_DUE_AMT_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
               LN_PAST_DUE_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_AMT_LEN);
        offset = LN_PAST_DUE_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }
    */

    /* - PAST ACCR INT -------------------------------------------- */
    else if (memcmp(code, NP_PAST_ACCR_INT_3, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
               LN_PAST_ACCR_INT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_ACCR_INT_LEN);
        offset = LN_PAST_ACCR_INT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN NUMBER 4 ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_NBR_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
               LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_NBR_LEN);
        offset = LN_LOAN_NBR_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN AMOUNT ------------------------------------------------ */
    else if (memcmp(code, NP_LOAN_AMT_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
               LN_LOAN_AMT_LEN);
         offset = FormatToAmount(nIndexCtx,field,
                                 currency,
                                 OK,
                                 'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_AMT_LEN);
        offset = LN_LOAN_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN START DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_START_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
               LN_LOAN_START_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_START_LEN);
        offset = LN_LOAN_START_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - LOAN EXPIRY DATE -------------------------------------------- */
    else if (memcmp(code, NP_LOAN_EXPIRY_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
               LN_LOAN_EXPIRY_LEN);
        offset = FormatToDate(field) - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_LOAN_EXPIRY_LEN);
        offset = LN_LOAN_EXPIRY_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_AMT_NEXT_INSTL_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
               LN_AMT_NEXT_INSTL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
        offset = LN_AMT_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE NEXT INSTL ---------------------------------------------- */
    else if (memcmp(code, NP_DATE_NEXT_INSTL_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
               LN_DATE_NEXT_INSTL_LEN);
        offset = FormatToDate(field) - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
        offset = LN_DATE_NEXT_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - AMT LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_AMT_LAST_PAID_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
               LN_AMT_LAST_PAID_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_AMT_LAST_PAID_LEN);
        offset = LN_AMT_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - DATE LAST PAID ----------------------------------------------- */
    else if (memcmp(code, NP_DATE_LAST_PAID_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
               LN_DATE_LAST_PAID_LEN);
        offset = FormatToDate(field) - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_DATE_LAST_PAID_LEN);
        offset = LN_DATE_LAST_PAID_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD INSTL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_INSTL_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
               LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_INSTL_LEN);
        offset = LN_OUTSTD_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - OUTSTD BAL ------------------------------------------------- */
    else if (memcmp(code, NP_OUTSTD_BAL_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
               LN_OUTSTD_BAL_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_OUTSTD_BAL_LEN);
        offset = LN_OUTSTD_BAL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE INSTL --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_INSTL_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
               LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
        offset = LN_PAST_DUE_INSTL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST DUE AMT --------------------------------------------- */
    else if (memcmp(code, NP_PAST_DUE_AMT_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
               LN_PAST_DUE_AMT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_DUE_AMT_LEN);
        offset = LN_PAST_DUE_AMT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - PAST ACCR INT -------------------------------------------- */
    else if (memcmp(code, NP_PAST_ACCR_INT_4, 2) == 0)
    {if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
      { memset(field,	0, sizeof(field));
        memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
        memcpy(field,
               tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
               LN_PAST_ACCR_INT_LEN);
        offset = FormatToAmount(nIndexCtx,field,
                                currency,
                                OK,
                                'L') - 3;
      }
     }
      else
      { memset(field, 0, sizeof(field));
        memset(field,	' ', LN_PAST_ACCR_INT_LEN);
        offset = LN_PAST_ACCR_INT_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - MESSAGE  ------------------------------------------- */
    else if (memcmp(code, NP_MESSAGE, 2) == 0)
    {if (GetIsoField( ISO_RESERVED_ISO_2 , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3)
      { memset(field, 0, sizeof(field));
        /*memcpy(field,tmpField+3,lgField-3);*/
	memcpy(field,tmpField,lgField);
        offset = FormatToMessage(field, lang, PRN_WIDTH) - 3;
      }
     }
      else
      { memset(field,	' ', NP_MESSAGE_LEN);
        offset = NP_MESSAGE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - TRANS_SEQ ------------------------------------------ */
    else if (memcmp(code, NP_TRANS_SEQ, 2) == 0)
    {if (GetIsoField( ISO_NETWORK_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField >= 3)
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField+3,
               lgField-3);
        offset = lgField - 6;
      }
     }
      else
      { memset(field,	' ', NP_TRANS_SEQ_LEN);
        offset = NP_TRANS_SEQ_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
	return (ERROR);
      }
    }

    /* - TRANS_NAME ----------------------------------------- */
    else if (memcmp(code, NP_TRANS_NAME, 2) == 0)
    {if (GetIsoField( ISO_RESERV_PRIV_USAGE_1 , P7Buffer, tmpField, &lgField) == SUCCES )
    { if ( lgField > 3)
      { memset(field, 0, sizeof(field));
        memcpy(field,
               tmpField + 3,
               lgField -3);
        offset = AlignField(field, 'C', NP_TRANS_NAME_LEN) - 3;
      }
     }
      else
      { memset(field,   ' ', NP_TRANS_NAME_LEN);
        offset = NP_TRANS_NAME_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }

    }

   /*
        - NP_SIMSIM_REF ----------------------------------------------
   */
    else if (memcmp(code, NP_SIMSIM_REF, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES && lgField >= 32)
      { memset(field, 0, sizeof(field));
        memcpy(field, tmpField + 8, NP_SIMSIM_REF_LEN);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_SIMSIM_REF_LEN);
        offset = NP_SIMSIM_REF_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }
   /*
        - NP_SIMSIM_CREDIT_BALANCE  ----------------------------------------------
   */
    else if (memcmp(code, NP_SIMSIM_CREDIT_BALANCE, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES )
      { memset(field, 0, sizeof(field));
        memset(field, '0', 4);
        memcpy(field + 4, tmpField, 8);

        memset(tmpField,        0, sizeof(tmpField));
        GetIsoField(ISO_TRANS_CRNCY,P7Buffer, tmpField, &lgField);
        memcpy(currency, tmpField, lgField);
        if(lang == ARABIC)
            offset = FormatToAmount(nIndexCtx,field,
                                    currency,
                                    OK,
                                    'R',
                                    lang)
                   - 3;
        else
            offset = FormatToAmount(nIndexCtx,field,
                                    currency,
                                    OK,
                                    'L',
                                    lang)
                   - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_SIMSIM_CREDIT_BALANCE_LEN);
        offset = NP_SIMSIM_CREDIT_BALANCE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

   /*
        - NP_SIMSIM_VALID_UNTIL ----------------------------------------------
   */
    else if (memcmp(code, NP_SIMSIM_VALID_UNTIL, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES && lgField >= 32)
      { memset(field, 0, sizeof(field));

        memcpy(field, tmpField + 8 + NP_SIMSIM_REF_LEN, 2);
        memcpy(field + 2, "/", 1);
        memcpy(field + 3, tmpField + 8 + NP_SIMSIM_REF_LEN + 2, 2);
        memcpy(field + 5, "/", 1);
        memcpy(field + 6, tmpField + 8 + NP_SIMSIM_REF_LEN + 2 + 2, 2);

        if (memcmp(field, "00/00/00", 8) == 0) {
               memcpy(field, "--/--/--", 8);
        }

        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_SIMSIM_VALID_UNTIL_LEN);
        offset = NP_SIMSIM_VALID_UNTIL_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }


   /*
        - NP_SIMSIM_EXPIRY_DATE ----------------------------------------------
   */
    else if (memcmp(code, NP_SIMSIM_EXPIRY_DATE, 2) == 0)
    {if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES  && lgField >= 32)
      {

        memset(field, 0, sizeof(field));

        memcpy(field, tmpField + 8 + NP_SIMSIM_REF_LEN + NP_SIMSIM_VALID_UNTIL_LEN, 2);
        memcpy(field + 2, "/", 1);
        memcpy(field + 3, tmpField + 8 + NP_SIMSIM_REF_LEN + NP_SIMSIM_VALID_UNTIL_LEN + 2, 2);
        memcpy(field + 5, "/", 1);
        memcpy(field + 6, tmpField + 8 + NP_SIMSIM_REF_LEN +  NP_SIMSIM_VALID_UNTIL_LEN + 2 + 2, 2);
        offset = strlen(field) - 3;
      }
      else
      { memset(field, 0, sizeof(field));
        memset(field,   ' ', NP_SIMSIM_EXPIRY_DATE_LEN);
        offset = NP_SIMSIM_EXPIRY_DATE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
        - Admin Card Balancing -------------------------------------------
    */
    else if (memcmp(code, NP_ADMIN_CARD_BALANCING, 2) == 0) {
               if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES ) {
			memset(record_data, 0, sizeof(record_data));
                        memcpy(record_data, tmpField + 3, lgField );
			sprintf(sLine, "record_data=[%s]", record_data);
			trace_event(sLine, TRACE);
                        /*FormatToBalancing(nIndexCtx,
                                          record_data,
                                          statement,
                                          lang);
			*/

                        offset = strlen(record_data) - 3;
                        if ( offset < 0 ) {
                                offset = 0;
                                memcpy(record_data, "   ", 3);
                        }
                        if (deplacer(c_prints, searched_pos + 1, offset) == OK)
                                memcpy(searched_pos, record_data, strlen(record_data));
                        else {
                                        trace_event("End   insert_text(ERROR)",PROCESSING);
                                        return (ERROR);
                        }
                } else{
                       memcpy(searched_pos, "***", 3);
                }
    }

    /*
        - ISS_RESP_CODE --------------------------------------------------
    */
    else if (memcmp(code, NP_ISS_RESP_CODE, 2) == 0)
    { if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField,    &lgField) == SUCCES
        && GetIsoField( ISO_SECURITY_DATA , P7Buffer, szScrtyData, &lgField) == SUCCES
         )
      { memset(field, 0, sizeof(field));
        memset(szIssRespCode, 0, sizeof(szIssRespCode));
        GetSecurityData(DESTINATION_RESOURCE, szScrtyData, szDestRsrc, &lgField);
        ret = TranslateRespCode (nIndexCtx, szDestRsrc, tmpField, "PR", szIssRespCode);
        if (ret == OK)
           memcpy(field, szIssRespCode, strlen(szIssRespCode));
        else
           memcpy(field, tmpField, strlen(tmpField));
        /*if (field[2] == '\0') { field[2] == ' '; field[3] = '\0';}*/
        if (field[2] == '\0') { field[2] = ' '; field[3] = '\0';}
        offset = 0;
      }
      else
      { memset(field,   ' ', NP_ISS_RESP_CODE_LEN);
        offset = NP_ISS_RESP_CODE_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, 3);
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /* - EMV_APPL_REF ------------------------------------------ */
    else if (memcmp(code, NP_EMV_APPL_REF, 2) == 0)
       {
       memset(tmpField, 0, sizeof(tmpField));
       memset(field, 0, sizeof(field));
       if (GetIsoField( ISO_RESERVED_ISO_4, P7Buffer, tmpField, &lgField) == SUCCES )
       {
          int nlLen;
          nlLen = 0;
          memset(field, 0, sizeof(field));
          memcpy(field, tmpField, lgField);
          offset = lgField - 3;
          sprintf(sLine,"EMV_APPL_REF FIELD IS %d|%s", offset, field);
          trace_event(sLine,TRACE);
        }
      else
      { memset(field,   ' ', NP_EMV_APPL_REF_LEN);
        offset = NP_EMV_APPL_REF_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /* - EMV_ARQC ------------------------------------------ */
    else if (memcmp(code, NP_EMV_ARQC, 2) == 0) {
       memset(tmpField, 0, sizeof(tmpField));
       memset(field, 0, sizeof(field));

       if (GetIsoField( ISO_ICC_DATA, P7Buffer, tmpField, &lgField) == SUCCES ) {
          TSTagIso       lTagIso;
          char           sWork[256];
          int nLen;
          nLen = 0;
          memset(field, 0, sizeof(field));
          memset(sWork, 0, sizeof(sWork));

          InitIsoIcTag (&lTagIso);
          AnalyseIsoIc (tmpField, lgField, &lTagIso);

          if (GetIsoIcTag("9F26", &lTagIso, sWork, &nLen) == SUCCESS) {
                memcpy(field, sWork, nLen);
                offset =  nLen - 3;
                sprintf(sLine," >> %s: [%03d] : [%s]", "9F26", nLen, sWork);
                trace_event(sLine,TRACE);
          } else {
            memset(field,   ' ', NP_EMV_ARQC_LEN);
            offset = NP_EMV_ARQC_LEN - 3;
          }

       } else {
          memset(field,   ' ', NP_EMV_ARQC_LEN);
          offset = NP_EMV_ARQC_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }


    /* - EMV_AID ------------------------------------------ */
    else if (memcmp(code, NP_PAST_DUE_AMT_3, 2) == 0)
       {

       memset(tmpField, 0, sizeof(tmpField));
       if (GetIsoField( ISO_RESERVED_ISO_3, P7Buffer, tmpField, &lgField) == SUCCES )
       {
          int nlLen;
          nlLen = 0;
          memset(field, 0, sizeof(field));
          memcpy(field, tmpField, lgField);
          offset = lgField - 3;
          sprintf(sLine,"MESSAGE FIELD IS %d|%s", offset, field);
          trace_event(sLine,TRACE);
      }
      else
      { memset(field,   ' ', NP_EMV_ARQC_LEN);
        offset = NP_EMV_ARQC_LEN - 3;
      }

      if (deplacer(c_prints, searched_pos + 1, offset) == OK)
        memcpy(searched_pos, field, strlen(field));
      else
      { trace_event("End   insert_text(ERROR)",PROCESSING);
        return (ERROR);
      }
    }

    /*
	- ABSENCE DU CODE ------------------------------------------------
    */
    else
    { sprintf(sLine,"End   insert_text(ERROR) -> code = %c%c",code[0],code[1]);
      trace_event(sLine, PROCESSING);
      return (ERROR);
    }

    /*
	- calcul des nouvelles valeurs des parametres de recherches -------
    */

    /*BEGIN AHM 22/02/2006: Cette partie est mis en commantaire le 2006/11/10*/
    /*if (current_pos == searched_pos ) {
        sprintf(sLine, "End   insert_text(OK 2)");
        trace_event(sLine, PROCESSING);
	return(OK);
    }*/
    /*END AHM*/

    current_pos = searched_pos;
    current_len = c_prints->len - (c_prints->data - current_pos);

  }
  trace_event("End   insert_text(OK)",PROCESSING);
  return(OK);
}

