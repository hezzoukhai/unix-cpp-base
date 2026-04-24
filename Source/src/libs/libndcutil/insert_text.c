/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      insert_text.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      Insert Text Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include <define.h>
#include <ascii.h>
#include <paramdef.h>
#include <langue.h>
#include <iso_hps.h>
#include <security_data.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <iso_ictag.h>

#define	PRN_WIDTH	40

/* extern trace_event(char*, int); EBE140515: wrong signature*/

/*******************************************************************************
**  nom    : deplacer(ptr, pos, offset) -> deplace la sous chaine localisee   **
**		par 'pos' de la chaine 'ptr->data' de 'offset' carac          **
**  Entree : ptr -> pointeur sur la structure ndc_param.	              **
**		  pos -> position a partir de la quelle if faut	              **
**		deplacer.					              **
**		  offset -> distance de deplacement.		              **
**  Sortie : Aucune.							      **
**  Return : OK -> fonctionnement normal.			              **
**		  ERROR -> depacement de la langueur de la chaine             **
*******************************************************************************/
int deplacer(ndc_param	*ptr,char		*pos,int		offset)
{
	char detail[256];
	int counter;
	int  tmpLen;

	memset(detail, 0, sizeof(detail));

	sprintf(detail, "Start deplacer(%d, %d)", ptr->len, offset);
	trace_event(detail, PROCESSING);



	/* il faut deplacer au moins par trois caracteres */
	if (offset < 0)
		offset = 0;

	if (ptr->len + offset <= MAXLEN)
	{
		tmpLen = 0;
		for(counter = ptr->len; counter >= pos - ptr->data; counter--)
			ptr->data[counter + offset] = ptr->data[counter];


		ptr->len = ptr->len + offset;

		sprintf(detail, "End   deplacer(%d)", ptr->len);
		trace_event(detail, PROCESSING);
		return (OK);
	}

	sprintf(detail, "End   deplacer(ERROR)");
	trace_event(detail, PROCESSING);
	return (ERROR);

}


/* YOua 15Oct2003 */
int my_memchr(char * ptr, char c, int len)
{
	int n = 0;

	while (n < len && *(ptr + n) != c)
		n ++;

	if (*(ptr + n) != c)
		return (-1);

	return (n);
}
/* YOua 15Oct2003 */

/*******************************************************************************
**  nom    : insert_text(c_prints, P7Buffer) -> insert les donnees	      **
**		appropries de 'P7Buffer' dans 'c_prints->data'.             **
**								              **
**  Entree : c_prints -> structure ndc_param.		                      **
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

int	insert_text(
	int			nIndexCtx,
	ndc_param	*c_prints,
	TSIsoInfo	*P7Buffer,
	char		lang)
{
	char		sLine		[MAX_LINE_TRC];
	char		*searched_pos;
	char		*current_pos;
	char		code		[   3];
	char		record_data	[1024];
	char		statement	[1024];
	char		field		[1024];
	char		term_nbr	[  16];
	char		currency	[   4];
	char		tmpField	[LG_MAX];
	int			current_len;
	int			offset;
	int			lgField;
	int                     nLen;
	/* YOua 15Oct2003 */
	int             found_position;
	/* YOua 15Oct2003 */

	/* YOua 10Nov 2003 : Issuer Response Code Printing */
	char             szScrtyData[256];
	char             szDestRsrc[20];
	char             szIssRespCode[10];
	char             szActionFlag [ 3];
	int              ret;
	double    		lRateAmount;
	char      		sRateDcc        [12+ 1];
	char	  		sMinorUnit[ 1 + 1];
	int				nNbDecimales;
	int				i;
	TSTlvPrivate tlvInfo;
	TSTagIso     icInfo;

	memset(szScrtyData, 0, sizeof(szScrtyData));
	memset(szDestRsrc, 0, sizeof(szDestRsrc));
	memset(szIssRespCode, 0, sizeof(szIssRespCode));
	memset(sRateDcc   ,   0, sizeof(sRateDcc   ));
	memset(sMinorUnit   ,   0, sizeof(sMinorUnit   ));

	/* YOua 10Nov 2003 : Issuer Response Code Printing */


	sprintf(sLine, "Start insert_text(%c)", lang);
	trace_event(sLine, PROCESSING);

	memset(record_data,   0, sizeof(record_data));
	memset(statement,     0, sizeof(statement));
	memset(currency,	0, sizeof(currency));
	memset(tmpField,	0, sizeof(tmpField));
	memset(field,	        0, sizeof(field));
	memset(term_nbr,      0, sizeof(term_nbr));
	memset(code,	        0, sizeof(code));

	InitTlvPrivate(&tlvInfo);

	if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
	{
		AnalyseTlvPrivate(tmpField, lgField, &tlvInfo);
	}

	current_pos = c_prints->data;
	current_len = c_prints->len;

	/* YOua 15Oct2003 while((searched_pos=(char *)memchr(current_pos, STX, current_len)) != NULL)*/
	while((found_position = my_memchr(current_pos, STX, current_len)) != -1)
	{
		searched_pos = current_pos + found_position;
		/* YOua 15Oct2003 */

		memcpy(code, searched_pos + 1, 2);

		sprintf(sLine, "Field to insert : %.2s", code);
		trace_event(sLine, PROCESSING);

		/*EBETODO: we better make a check on isdigit and then atoi + switch*/
		/*
		- OPERATION_DATE -------------------------------------------------
		*/
		if (memcmp(code, NP_OPERATION_DATE, 2) == 0)
		{

			if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,		0, sizeof(field));
				memcpy(field,tmpField,lgField - 6 );
				/** offset = FormatToDate(field,lang) - 3;  **/
				offset = FormatToDate(field,'E') - 3; 
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_OPERATION_DATE_LEN);
				offset = NP_OPERATION_DATE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- OPERATION_DATE_TIME -------------------------------------------------
		*/
		else if (memcmp(code, NP_OPERATION_DATE_TIME, 2) == 0)
		{ 
			if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,		0, sizeof(field));
				memcpy(field, tmpField, lgField );
				/** offset = FormatToDateTime(field,lang) - 3; **/
				offset = FormatToDateTime(field,'E') - 3;
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_OPERATION_DATE_TIME_LEN);
				offset = NP_OPERATION_DATE_TIME_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- BALANCE_DATE -------------------------------------------------
		*/
		/** ******************************************************************************************************** **/
		/** M.A.WAHBI 20021218 : Comming this piece of code since it was twice, this one and the one right after it  **
		else if (memcmp(code, NP_BALANCE_DATE, 2) == 0)
		{ if ( GetIsoField( ISO_ACTION_DATE , P7Buffer, tmpField, &lgField) == SUCCES )
		{ memset(field,		0, sizeof(field));
		memcpy(field, tmpField, lgField ); 
		offset = FormatToDate(field,lang) - 3;
		}
		else
		{ memset(field, 0, sizeof(field));
		memset(field,	' ', NP_BALANCE_DATE_LEN);
		offset = NP_BALANCE_DATE_LEN - 3;
		}
		}
		** ******************************************************************************************************** **/

		/*
		- BALANCE_DATE -------------------------------------------------
		*/
		else if (memcmp(code, NP_BALANCE_DATE, 2) == 0)
		{ 
			if ( GetIsoField( ISO_ACTION_DATE , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,		0, sizeof(field));
				memcpy(field, tmpField,lgField); 
				offset = FormatToDate(field,lang) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_BALANCE_DATE_LEN);
				offset = NP_BALANCE_DATE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- OPERATION_HOUR -------------------------------------------------
		*/
		else if (memcmp(code, NP_OPERATION_HOUR, 2) == 0)
		{ 
			if ( GetIsoField( ISO_TRANS_DATE_TIME , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField+6,lgField-6); 
				/** offset = FormatToHour(field,lang) - 3; **/
				offset = FormatToHour(field,'E') - 3; 
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_OPERATION_HOUR_LEN);
				offset = NP_OPERATION_HOUR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*
		* 	-- TRUNC SOURCE_ACCOUNT -------------------------------------------------
		* 		*/
		else if (memcmp(code, NP_TRUNC_SOURCE_ACCOUNT, 2) == 0)
		{ 
			if ( GetIsoField( ISO_ACCOUNT_ID1 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if (lgField >= 3)
				{
					memset(field, 0, sizeof(field)); 
					memset(field, '-',lgField );
					memcpy(field, tmpField,4);
					memcpy(field+lgField-3, tmpField+lgField-3,3);

					sprintf(sLine, " trunc acc(%s,Length(%lu) lgField: (%d))",field,strlen(field), lgField);
					trace_event(sLine, PROCESSING); 

					offset = lgField - 3;
				}
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,   '-', NP_TRUNC_SOURCE_ACCOUNT_LEN);
					offset = NP_TRUNC_SOURCE_ACCOUNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	'-', NP_TRUNC_SOURCE_ACCOUNT_LEN);
				offset = NP_TRUNC_SOURCE_ACCOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}


		/*
		- CARD_NBR -------------------------------------------------
		*/
		else if (memcmp(code, NP_CARD_NBR, 2) == 0)
		{ 
			if ( GetIsoField( ISO_CARD_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField  );
				offset = lgField - 3;
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_CARD_NBR_LEN);
				offset = NP_CARD_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- TRUNC_CARD_NBR -------------------------------------------------
		*/
		else if (memcmp(code, NP_TRUNC_CARD_NBR, 2) == 0)
		{ 
			if ( GetIsoField( ISO_CARD_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));

				/** 
				memcpy(field, tmpField,lgField - 4 ); 
				if ( lang == ARABIC )
				memcpy(field + lgField - 4, "----", 4);
				else 
				memcpy(field + lgField - 4, "----", 4);
				**/
				memset(field, '-',lgField );
				memcpy(field, tmpField,6);
				memcpy(field+lgField-4, tmpField+lgField-4,4);

				offset = lgField - 3;

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_CARD_NBR_LEN);
				offset = NP_CARD_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- SOURCE_ACCOUNT -------------------------------------------------
		*/

		else if (memcmp(code, NP_SOURCE_ACCOUNT, 2) == 0)
		{ 
			if ( GetIsoField( ISO_ACCOUNT_ID1 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if (lgField >= 3)
				{ 
					memset(field, 0, sizeof(field));
					memcpy(field,tmpField,lgField); 
					offset = lgField - 3;
					/** offset = lgField ; **/
				}
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,   '-', NP_SOURCE_ACCOUNT_LEN);
					offset = NP_SOURCE_ACCOUNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	'-', NP_SOURCE_ACCOUNT_LEN);
				offset = NP_SOURCE_ACCOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*
		- TARGET_ACCOUNT -------------------------------------------------
		*/
		else if (memcmp(code, NP_TARGET_ACCOUNT, 2) == 0)
		{ 
			if ( GetIsoField( ISO_ACCOUNT_ID2 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				if (lgField >= 3)
				{
					memcpy(field, tmpField,lgField);
					offset = strlen(field) - 3; 
				}
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,' ', NP_TARGET_ACCOUNT_LEN);
					offset = NP_TARGET_ACCOUNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_TARGET_ACCOUNT_LEN);
				offset = NP_TARGET_ACCOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- ATM_NUMBER -----------------------------------------------------
		*/
		else if (memcmp(code, NP_ATM_NUMBER, 2) == 0)
		{
			if (GetIsoField( ISO_TERMINAL_NBR, P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField, lgField );
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_ATM_NUMBER_LEN);
				offset = NP_ATM_NUMBER_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*
		- CASHSEND REFERENCE -----------------------------------------------------
		*/ 
		else if (memcmp(code, NP_CASHSEND_REF, 3) == 0)
		{
			if ( GetIsoField( ISO_RESERVED_ISO_5 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));

				memcpy(field, tmpField,NP_CASHSEND_REF_LEN);
				offset = NP_CASHSEND_REF_LEN - 3; 

			}
			else
			{ 
			    trace_event("End   insert_text(ERROR NP_CASHSEND_REF)",PROCESSING);
				return (ERROR);
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*- CASHSEND AMOUNT -----------------------------------------------------
		*/ 
		else if (memcmp(code, NP_AMOUNT_CASHSEND, 3) == 0)
		{
			if ( GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField + 8 ,12);
				offset = strlen(field) - 3; 
                
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_AMOUNT_CS_LEN);
				offset = NP_AMOUNT_CS_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}		 
		/*
		- ATM_NAME -----------------------------------------------------
		*/
		else if (memcmp(code, NP_ATM_NAME, 2) == 0)
		{
			if (GetIsoField( ISO_TERMINAL_NBR, P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memset(term_nbr, 0, sizeof(term_nbr));
				memcpy(term_nbr,tmpField,lgField);
				/* PROD00051898: acquirer id would be schemes acquirer id instead of bank code
				GetIsoField( ISO_ACQR_ID, P7Buffer, tmpField, &lgField);*/
				if(GetTlvPrivate (ACQUIRER_BANK_TAG, &tlvInfo, tmpField, &lgField) != SUCCESS)
						memcpy(tmpField, "000001", 6);
				
				offset = GetPtServName(nIndexCtx,term_nbr,tmpField, field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_ATM_NAME_LEN);
				offset = NP_ATM_NAME_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/** FOR SIPEM **/
		/*
		- NP_RFU_95 => BRANCH NAME -----------------------------------------------------
		*/
		else if (memcmp(code, NP_RFU_95, 2) == 0)
		{
			if (GetIsoField( ISO_TERMINAL_NBR, P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memset(term_nbr, 0, sizeof(term_nbr));
				memcpy(term_nbr,tmpField,lgField);

				if(GetTlvPrivate (ACQUIRER_BANK_TAG, &tlvInfo, tmpField, &lgField) != SUCCESS)
						memcpy(tmpField, "000001", 6);
				
				offset = GetBranchName(nIndexCtx,term_nbr,tmpField, field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', 30);
				offset = 30 - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/** FOR SIPEM **/

		/*
		- OPERATION_NUMBER -----------------------------------------------
		*/
		else if (memcmp(code, NP_OPERATION_NUMBER, 2) == 0)
		{
			if (GetIsoField( ISO_AUDIT_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field,tmpField,lgField);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_OPERATION_NUMBER_LEN);
				offset = NP_OPERATION_NUMBER_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- BALANCE --------------------------------------------------------
		*/
		else if ( ( memcmp(code, NP_BALANCE, 2)		== 0)
			||  ( memcmp(code, NP_AVAILABLE_BALANCE, 2)	== 0)
			||  ( memcmp(code, NP_LOAN_BALANCE, 2)	== 0)
			)
		{

			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=20 ) 
				{ 
					memset(field,		0, sizeof(field));

					/* YOua 17Oct2003 : il faut baser la recherche sur le AMOUNT_TYPE */
					/*
					*         memcpy(field, tmpField, 20);
					*                 */
					if (memcmp(code, NP_LOAN_BALANCE, 2) == 0)
						memcpy(field, tmpField, 20);
					else
					{
						offset = 0;
						GetIsoField(ISO_TRANS_CRNCY,P7Buffer, currency, &nLen);
						while(offset < lgField)
						{
							if (memcmp(tmpField + offset + 2, "02", 2) == 0 && memcmp(tmpField + offset + 4, currency, 3) == 0 )
							{
								memcpy(field, tmpField + offset, 20);
								break;
							}

							offset += 20;
						}

						if (strlen(field) == 0)
						{
							offset = 0;
							memcpy(field, tmpField + offset, 20);
							while(offset < lgField)
							{
								if (memcmp(tmpField + offset + 2, "01", 2) == 0  && memcmp(tmpField + offset + 4, currency, 3) == 0)
								{
									memcpy(field, tmpField + offset, 20);
									break;
								}

								offset += 20;
							}
						}
					}

					/* YOua 17Oct2003 */
					offset = 0;
					if (strlen(field) > 0)
					{
						if(lang == ARABIC)
							offset = FormatToBalance(nIndexCtx,field, 'R',lang) - 3;
						else
							offset = FormatToBalance(nIndexCtx,field, 'L',lang) - 3;
					}

					if (strlen(field) == 0)
					{
						memset(field, ' ', NP_BALANCE_LEN);
						offset = NP_BALANCE_LEN - 3;
					}
				}
				else
				{
					memset(field, ' ', NP_BALANCE_LEN);
					offset = NP_BALANCE_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_BALANCE_LEN);
				offset = NP_BALANCE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*
		- ACCOUNT_BALANCE ------------------------------------------------
		*/
		else if ( memcmp(code, NP_ACCOUNT_BALANCE, 2)	== 0)
		{
			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=20 ) 
				{ 
					memset(field,		0, sizeof(field));
					/* YOua 17Oct2003 : il faut baser la recherche sur le AMOUNT_TYPE */
					/*
					memcpy(field, tmpField + 20, 20);
					*/
					offset = 0;
					while(offset < lgField)
					{
						if (memcmp(tmpField + offset + 2, "01", 2) == 0)
						{
							memcpy(field, tmpField + offset, 20);
							break;
						}

						offset += 20;
					}
					if (strlen(field) == 0)
					{
						memset(field, ' ', NP_BALANCE_LEN);
						offset = NP_BALANCE_LEN - 3;
					}
					/* YOua 17Oct2003 */

					if(lang == ARABIC)
						offset = FormatToBalance(nIndexCtx,field, 'R',lang) - 3;
					else
						offset = FormatToBalance(nIndexCtx,field, 'L',lang) - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}
				}
				else
				{ 
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_ACCOUNT_BALANCE_LEN);
					offset = NP_BALANCE_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_ACCOUNT_BALANCE_LEN);
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
		- STATEMENT ------------------------------------------------------
		*/
		else if (memcmp(code, NP_STATEMENT, 2) == 0) 
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES ) 
			{ 
				memcpy(record_data, tmpField , lgField );
				GetIsoField( ISO_TRANS_CRNCY , P7Buffer, tmpField, &lgField);
				FormatToStatement(nIndexCtx,record_data,
					statement,
					tmpField,
					lang);
				/**'E'); **/

				offset = strlen(statement) - 3;
				if ( offset < 0 ) 
				{ 
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

		/*
		* 	- NP_ENHACED_STAMENT ----takes into consideration lang--------------------------------------------------
		*/
		else if (memcmp(code, NP_ENHACED_STAMENT, 2) == 0) 
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES ) 
			{ 
				memcpy(record_data, tmpField , lgField );
				GetIsoField( ISO_TRANS_CRNCY , P7Buffer, tmpField, &lgField);
				FormatToStatement(nIndexCtx,record_data,
					statement,
					tmpField,
					lang);


				offset = strlen(statement) - 3;
				if ( offset < 0 ) 
				{ 
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

		/*
		- COMPANY ------------------------------------------------------
		*/
		else if (memcmp(code, NP_COMPANY, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memcpy(record_data, tmpField ,lgField );
				offset = FormatToCompany(record_data,
					statement,
					lang);

				offset = strlen(statement)-3 ;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(statement, "   ", 3);
				}

				if (deplacer(c_prints, searched_pos + 1, offset) == OK)
					memcpy(searched_pos, statement,  strlen(statement));
				else
				{ 
					trace_event("End   insert_text(ERROR)",PROCESSING);
					return (ERROR);
				}
			}
			else
				memcpy(searched_pos, "***", 3);
		}
		/*
		- BILLING --------------------------------------------------
		*/
		else if (memcmp(code, NP_BILL_PAYMENT, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memcpy(record_data, tmpField, lgField);
				offset = FormatToBillPayment(nIndexCtx,
					record_data,
					statement,
					lang);

				offset = strlen(statement) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(statement, "   ", 3);
				}
				if (deplacer(c_prints, searched_pos + 1, offset) == OK)
					memcpy(searched_pos, statement, strlen(statement));
				else
				{ 
					trace_event("End   insert_text(ERROR)",PROCESSING);
					return (ERROR);
				}
			}
			else
				memcpy(searched_pos, "***", 3);
		}
		/*
		- RESPONSE_CODE --------------------------------------------------
		*/
		else if (memcmp(code, NP_RESPONSE_CODE, 2) == 0)
		{ 
			if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField, &lgField) == SUCCES && strlen(tmpField) == 3)
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField, lgField);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field,	' ', NP_RESPONSE_CODE_LEN);
				offset = NP_RESPONSE_CODE_LEN - 3;
			}

			/* if faut inserer le message et non pas le code  */
			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* - NP_RESPONSE_LABEL ------------------------------------------ */
		else if (memcmp(code, NP_RESPONSE_LABEL, 2) == 0)
		{
			memset (tmpField,       0, sizeof(tmpField));
			memset (field,          0, sizeof(field));
			memset (szActionFlag,   0, sizeof(szActionFlag));
			offset = -1;
			if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField,    &lgField) == SUCCES )
			{ 


				if ( GetActionInfo( nIndexCtx,tmpField,szActionFlag,field) == OK )
					offset = strlen (field) - 3;

				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_RESPONSE_LABEL_LEN);
				offset = NP_RESPONSE_LABEL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		} 
		/* YOua 10Nov 2003 : Issuer Response Code Printing */
		/*
		- ISS_RESP_CODE --------------------------------------------------
		*/
		else if (memcmp(code, NP_ISS_RESP_CODE, 2) == 0)
		{ 
			if ( GetIsoField( ISO_RESPONSE_CODE , P7Buffer, tmpField,    &lgField) == SUCCES
				&& GetIsoField( ISO_SECURITY_DATA , P7Buffer, szScrtyData, &lgField) == SUCCES
				)
			{ 
				memset(field, 0, sizeof(field));
				memset(szIssRespCode, 0, sizeof(szIssRespCode));
				GetSecurityData(DESTINATION_RESOURCE, szScrtyData, szDestRsrc, &lgField);
				ret = TranslateRespCode (nIndexCtx, szDestRsrc, tmpField, "PR", szIssRespCode);
				if (ret == OK)
					memcpy(field, szIssRespCode, strlen(szIssRespCode));
				else
					memcpy(field, tmpField, strlen(tmpField));
				if (field[2] == '\0') { field[2] == ' '; field[3] = '\0';}
				offset = 0;
			}
			else
			{ 
				memset(field,	' ', NP_ISS_RESP_CODE_LEN);
				offset = NP_ISS_RESP_CODE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, 3);
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* YOua 10Nov 2003 : Issuer Response Code Printing */

		/* - CUTOFF DATA ---------------------------------------------------- */
		else if (memcmp(code, NP_CUTOFF_DATA, 2) == 0)
		{
			/*tmpField*/
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField);
				offset = FormatToCutoffData(nIndexCtx,field, ENGLISH);
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, "THERE IS NO CUT OFF", 19);
			}



			offset = strlen(field) - 3;
			if ( offset < 0 )
			{ 
				offset = 0;
				memcpy(field, "   ", 3);
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field,strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- DEPOSIT_NUMBER --------------------------------------------------
		*/
		else if (memcmp(code, NP_DEPOSIT_NUMBER, 2) == 0)
		{
			if (GetIsoField( ISO_RESERVED_ISO_2 , P7Buffer, tmpField, &lgField) == SUCCES && lgField > 0)
			{ 
				memset(field, 0, sizeof(field));
				sprintf(field, "%3d", atoi(tmpField));
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_DEPOSIT_NUMBER_LEN);
				offset = NP_DEPOSIT_NUMBER_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - NP_EMV_APP_LABEL ------------------------------------------ */
		else if (memcmp(code, NP_EMV_APP_LABEL, 2) == 0)
		{
			memset (tmpField, 0, sizeof(tmpField));
			memset (field,    0, sizeof(field));
			InitIsoIcTag (&icInfo);	
			if (GetIsoField( ISO_ICC_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 

				AnalyseIsoIc(tmpField, lgField, &icInfo);

				offset = -1;

				if (GetIsoIcTag (ISO_TAG_APP_NAME ,&icInfo, field, &lgField) == SUCCES)
					offset = lgField - 3;
				else if (GetIsoIcTag (ISO_TAG_APP_LABEL ,&icInfo, field, &lgField) == SUCCES)
					offset = lgField - 3;

				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_TRANS_SEQ_LEN);
				offset = NP_TRANS_SEQ_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}  
		/* - NP_EMV_AID ------------------------------------------ */
		else if (memcmp(code, NP_EMV_AID, 2) == 0)
		{
			memset (tmpField, 0, sizeof(tmpField));
			memset (field,    0, sizeof(field));
			InitIsoIcTag (&icInfo);	
			if (GetIsoField( ISO_ICC_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 

				AnalyseIsoIc(tmpField, lgField, &icInfo);

				offset = -1;
				if (GetIsoIcTag (ISO_TAG_APP_SELECTED ,&icInfo, field, &lgField) == SUCCES)
					offset = lgField - 3;

				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_TRANS_SEQ_LEN);
				offset = NP_TRANS_SEQ_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ trace_event("End   insert_text(ERROR)",PROCESSING);
			return (ERROR);
			}
		}

		/*
		- NP_DEPOSITED_NOTES_DETAIL added by TBO21122010------------------------------------------------------
		*/	
		else if (memcmp(code, NP_DEPOSITED_NOTES_DETAIL, 2) == 0) 
		{

			if (GetIsoField (ISO_ADTNL_DATA_PRIV, P7Buffer, tmpField, &lgField) == SUCCESS) 
			{ 

				memset (record_data, 0, sizeof(record_data));
				memset (term_nbr,    0, sizeof(term_nbr));
				memset (statement,  0, sizeof(statement));
				memset (field,  0, sizeof(field)); /*DDA12012017*/

				GetIsoField( ISO_TERMINAL_NBR, P7Buffer, term_nbr, &offset);	


				if(GetTlvPrivate (ATM_DEPOSIT_NOTES, &tlvInfo, record_data, &lgField) == SUCCESS)
				{
					if(GetTlvPrivate (ACQUIRER_BANK_TAG, &tlvInfo, field, &lgField) != SUCCESS)
						memcpy(field, "000001", 6); /*default it in worst case scenario*/
					offset = FormatToDepNotes(nIndexCtx, term_nbr, field , record_data, statement, lang);
				} 


				offset = strlen(statement) - 3;
				if ( offset < 0 ) 
				{ 
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
			else
			{
				memcpy(searched_pos, "***", 3);
			}
		}

		/*
		- NOTE_1_NBR -----------------------------------------------------
		*/
		else if (memcmp(code, NP_NOTE_1_NBR, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField, 3);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,   ' ', NP_NOTE_1_NBR_LEN);
				offset = NP_NOTE_1_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- NOTE_2_NBR -----------------------------------------------------
		*/
		else if (memcmp(code, NP_NOTE_2_NBR, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField + 3, 3);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,   ' ', NP_NOTE_2_NBR_LEN);
				offset = NP_NOTE_2_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- NOTE_3_NBR -----------------------------------------------------
		*/
		else if (memcmp(code, NP_NOTE_3_NBR, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField + 6, 3);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,   ' ', NP_NOTE_3_NBR_LEN);
				offset = NP_NOTE_3_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- NOTE_4_NBR -----------------------------------------------------
		*/
		else if (memcmp(code, NP_NOTE_4_NBR, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_DATA_NATL , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField + 9, 3);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,   ' ', NP_NOTE_4_NBR_LEN);
				offset = NP_NOTE_4_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- TRANSACTION_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_TRANS_AMOUNT , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,	0, sizeof(field));
				memcpy(field,tmpField,lgField);
				memset(tmpField,        0, sizeof(tmpField));
				GetIsoField(ISO_TRANS_CRNCY,P7Buffer, tmpField, &lgField);
				memcpy(currency,tmpField,lgField);
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

				if (offset < 0)
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_AMOUNT_LEN);
					offset = NP_AMOUNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_AMOUNT_LEN);
				offset = NP_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- BILLING_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_BILLING_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_BILLING_AMOUNT , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,	0, sizeof(field));
				memcpy(field,tmpField,lgField);
				memset(tmpField, 0 , sizeof(tmpField));
				GetIsoField(ISO_BILLING_CRNCY,P7Buffer, tmpField, &lgField);
				if(lang == ARABIC)
					offset = FormatToAmount(nIndexCtx,field,
					tmpField,
					OK,
					'R',
					lang)
					- 3;
				else
					offset = FormatToAmount(nIndexCtx,field,
					tmpField,
					OK,
					'L',
					lang)
					- 3;

				if (offset < 0)
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_AMOUNT_LEN);
					offset = NP_AMOUNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_AMOUNT_LEN);
				offset = NP_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- CONVERSION RATE --------------------------------------------
		*/
		/*  else if (memcmp(code, NP_CONVERSION_RATE, 2) == 0) MUW 15052012*/
		else if (memcmp(code, NP_UTILITY_DATA, 2) == 0)
		{
			if (GetIsoField( ISO_KEY_MNG_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,	0, sizeof(field));
				memcpy(field,tmpField,lgField);
				offset = strlen(field) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_RATE_LEN);
				offset = NP_RATE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- FACILITY_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_FACILITY_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=60 ) 
				{ 
					memset(field,	0, sizeof(field));

					memcpy(field, tmpField + 40, 20);

					if(lang == ARABIC)
						offset = FormatToBalance(nIndexCtx,field, 'R', lang) - 3;
					else
						offset = FormatToBalance(nIndexCtx,field, 'L', lang) - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}
				}
				/* YOua 30Dec2003 : Bug */
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_FACILITY_AMOUNT_LEN);
					offset = NP_FACILITY_AMOUNT_LEN - 3;
				}
				/* YOua 30Dec2003 : Bug */
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_FACILITY_AMOUNT_LEN);
				offset = NP_FACILITY_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- PAST_DUE_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_PAST_DUE_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=40 ) 
				{ 
					memset(field,	0, sizeof(field));

					memcpy(field, tmpField + 20, 20);

					if(lang == ARABIC)
						offset = FormatToBalance(nIndexCtx,field, 'R', lang) - 3;
					else
						offset = FormatToBalance(nIndexCtx,field, 'L', lang) - 3;
					if (offset < 0)      
					{
						memset(field, 0, sizeof(field));
						memset(field,	' ', NP_PAST_DUE_AMOUNT_LEN);
						offset = NP_PAST_DUE_AMOUNT_LEN - 3;
					}
				}
				/* YOua 30Dec2003 : Bug */
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_PAST_DUE_AMOUNT_LEN);
					offset = NP_PAST_DUE_AMOUNT_LEN - 3;
				}
				/* YOua 30Dec2003 : Bug */
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_PAST_DUE_AMOUNT_LEN);
				offset = NP_PAST_DUE_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- PENDING_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_PENDING_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=20 ) 
				{ 
					memset(field,	0, sizeof(field));

					memcpy(field, tmpField , 20);

					if(lang == ARABIC)
						offset = FormatToBalance(nIndexCtx,field, 'R', lang) - 3;
					else
						offset = FormatToBalance(nIndexCtx,field, 'L', lang) - 3;
					if (offset < 0) 
					{
						memset(field, 0, sizeof(field));
						memset(field,	' ', NP_PENDING_AMOUNT_LEN);
						offset = NP_PENDING_AMOUNT_LEN - 3;
					}
				}
				/* YOua 30Dec2003 : Bug */
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_PENDING_AMOUNT_LEN);
					offset = NP_PENDING_AMOUNT_LEN - 3;
				}
				/* YOua 30Dec2003 : Bug */
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_PENDING_AMOUNT_LEN);
				offset = NP_PENDING_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/*
		- UNUTILISED_AMOUNT ---------------------------------------------
		*/
		else if (memcmp(code, NP_UNUTILISED_AMOUNT, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_AMOUNTS , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >=80 ) 
				{ 
					memset(field,	0, sizeof(field));

					memcpy(field, tmpField + 60, 20);

					if(lang == ARABIC)
						offset = FormatToBalance(nIndexCtx,field, 'R', lang) - 3;
					else
						offset = FormatToBalance(nIndexCtx,field, 'L', lang) - 3;

					if (offset < 0) 
					{
						memset(field, 0, sizeof(field));
						memset(field,	' ', NP_UNUTILISED_AMOUNT_LEN);
						offset = NP_UNUTILISED_AMOUNT_LEN - 3;
					}
				}
				/* YOua 30Dec2003 : Bug */
				else
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_UNUTILISED_AMOUNT_LEN);
					offset = NP_UNUTILISED_AMOUNT_LEN - 3;
				}
				/* YOua 30Dec2003 : Bug */
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_UNUTILISED_AMOUNT_LEN);
				offset = NP_UNUTILISED_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN NUMBER ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_NBR_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_LOAN_NBR_OFF,
						LN_LOAN_NBR_LEN);
					offset = LN_LOAN_NBR_LEN - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}
				}
				else
				{ 
					memset(field, 0, sizeof(field));
					memset(field,	' ', LN_LOAN_NBR_LEN);
					offset = LN_LOAN_NBR_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_NBR_LEN);
				offset = LN_LOAN_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN AMOUNT ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_AMT_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_LOAN_AMT_OFF,
						LN_LOAN_AMT_LEN);
					if ( lang == ARABIC )
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
					if (offset < 0)
					{ 
						memset(field, 0, sizeof(field));
						memset(field,	' ', LN_LOAN_AMT_LEN);
						offset = LN_LOAN_AMT_LEN - 3;
					}
				}
				else
				{ 
					memset(field, 0, sizeof(field));
					memset(field,	' ', LN_LOAN_AMT_LEN);
					offset = LN_LOAN_AMT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_AMT_LEN);
				offset = LN_LOAN_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN START DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_START_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_LOAN_START_OFF,
						LN_LOAN_START_LEN);
					offset = FormatToDate(field,lang) - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_START_LEN);
				offset = LN_LOAN_START_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN EXPIRY DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_EXPIRY_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_LOAN_EXPIRY_OFF,
						LN_LOAN_EXPIRY_LEN);
					offset = FormatToDate(field,lang) - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_EXPIRY_LEN);
				offset = LN_LOAN_EXPIRY_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_AMT_NEXT_INSTL_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{
				offset = -1;
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_AMT_NEXT_INSTL_OFF,
						LN_AMT_NEXT_INSTL_LEN);
					if ( lang == ARABIC )
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
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
				offset = LN_AMT_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_DATE_NEXT_INSTL_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				offset = -1;
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_DATE_NEXT_INSTL_OFF,
						LN_DATE_NEXT_INSTL_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
				offset = LN_DATE_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_AMT_LAST_PAID_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				offset = -1;
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_AMT_LAST_PAID_OFF,
						LN_AMT_LAST_PAID_LEN);
					if ( lang == ARABIC )
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
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_LAST_PAID_LEN);
				offset = LN_AMT_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_DATE_LAST_PAID_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_DATE_LAST_PAID_OFF,
						LN_DATE_LAST_PAID_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
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
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_OUTSTD_INSTL_OFF,
						LN_OUTSTD_INSTL_LEN);
					offset = LN_OUTSTD_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_INSTL_LEN);
				offset = LN_OUTSTD_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD BAL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_BAL_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_OUTSTD_BAL_OFF,
						LN_OUTSTD_BAL_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_BAL_LEN);
				offset = LN_OUTSTD_BAL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE INSTL --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_INSTL_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 5 + NP_PAST_DUE_INSTL_OFF,
						LN_PAST_DUE_INSTL_LEN);
					offset = LN_PAST_DUE_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
				offset = LN_PAST_DUE_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE AMT --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_AMT_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_PAST_DUE_AMT_OFF,
						LN_PAST_DUE_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_AMT_LEN);
				offset = LN_PAST_DUE_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST ACCR INT -------------------------------------------- */
		else if (memcmp(code, NP_PAST_ACCR_INT_1, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 5 + NP_PAST_ACCR_INT_OFF,
						LN_PAST_ACCR_INT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_ACCR_INT_LEN);
				offset = LN_PAST_ACCR_INT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN NUMBER 2 ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_NBR_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
						LN_LOAN_NBR_LEN);
					offset = LN_LOAN_NBR_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_NBR_LEN);
				offset = LN_LOAN_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN AMOUNT ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_AMT_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
						LN_LOAN_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_AMT_LEN);
				offset = LN_LOAN_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN START DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_START_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
						LN_LOAN_START_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_START_LEN);
				offset = LN_LOAN_START_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN EXPIRY DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_EXPIRY_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
						LN_LOAN_EXPIRY_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_EXPIRY_LEN);
				offset = LN_LOAN_EXPIRY_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_AMT_NEXT_INSTL_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
						LN_AMT_NEXT_INSTL_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
				offset = LN_AMT_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_DATE_NEXT_INSTL_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
						LN_DATE_NEXT_INSTL_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
				offset = LN_DATE_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_AMT_LAST_PAID_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
						LN_AMT_LAST_PAID_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_LAST_PAID_LEN);
				offset = LN_AMT_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_DATE_LAST_PAID_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
						LN_DATE_LAST_PAID_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_LAST_PAID_LEN);
				offset = LN_DATE_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD INSTL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_INSTL_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
						LN_OUTSTD_INSTL_LEN);
					offset = LN_OUTSTD_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_INSTL_LEN);
				offset = LN_OUTSTD_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD BAL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_BAL_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency,  tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
						LN_OUTSTD_BAL_LEN);
					if ( lang == ARABIC )
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
					if (offset <0)
					{ 
						memset(field, 0, sizeof(field));
						memset(field,	' ', LN_OUTSTD_BAL_LEN);
						offset = LN_OUTSTD_BAL_LEN - 3;
					}
				}
				else
				{ 
					memset(field, 0, sizeof(field));
					memset(field,	' ', LN_OUTSTD_BAL_LEN);
					offset = LN_OUTSTD_BAL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_BAL_LEN);
				offset = LN_OUTSTD_BAL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE INSTL --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_INSTL_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
						LN_PAST_DUE_INSTL_LEN);
					offset = LN_PAST_DUE_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
				offset = LN_PAST_DUE_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE AMT --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_AMT_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
						LN_PAST_DUE_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_AMT_LEN);
				offset = LN_PAST_DUE_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST ACCR INT -------------------------------------------- */
		else if (memcmp(code, NP_PAST_ACCR_INT_2, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 2*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
						LN_PAST_ACCR_INT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_ACCR_INT_LEN);
				offset = LN_PAST_ACCR_INT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN NUMBER 3 ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_NBR_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
						LN_LOAN_NBR_LEN);
					offset = LN_LOAN_NBR_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_NBR_LEN);
				offset = LN_LOAN_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN AMOUNT ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_AMT_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 

					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
						LN_LOAN_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_AMT_LEN);
				offset = LN_LOAN_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN START DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_START_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
						LN_LOAN_START_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_START_LEN);
				offset = LN_LOAN_START_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN EXPIRY DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_EXPIRY_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
						LN_LOAN_EXPIRY_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_EXPIRY_LEN);
				offset = LN_LOAN_EXPIRY_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_AMT_NEXT_INSTL_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
						LN_AMT_NEXT_INSTL_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
				offset = LN_AMT_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_DATE_NEXT_INSTL_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
						LN_DATE_NEXT_INSTL_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
				offset = LN_DATE_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_AMT_LAST_PAID_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
						LN_AMT_LAST_PAID_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_LAST_PAID_LEN);
				offset = LN_AMT_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_DATE_LAST_PAID_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
						LN_DATE_LAST_PAID_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_LAST_PAID_LEN);
				offset = LN_DATE_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD INSTL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_INSTL_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField  + 2*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
						LN_OUTSTD_INSTL_LEN);
					offset = LN_OUTSTD_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_INSTL_LEN);
				offset = LN_OUTSTD_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD BAL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_BAL_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				offset = -1;
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
						LN_OUTSTD_BAL_LEN);
					if ( lang == ARABIC )
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
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_BAL_LEN);
				offset = LN_OUTSTD_BAL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE INSTL --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_INSTL_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
						LN_PAST_DUE_INSTL_LEN);
					offset = LN_PAST_DUE_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
				offset = LN_PAST_DUE_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE AMT --------------------------------------------- */
		else if (memcmp(code, NP_PAST_DUE_AMT_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
						LN_PAST_DUE_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_AMT_LEN);
				offset = LN_PAST_DUE_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST ACCR INT -------------------------------------------- */
		else if (memcmp(code, NP_PAST_ACCR_INT_3, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3*NP_LOAN_DATA_LEN + 5)
				{
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 2*NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
						LN_PAST_ACCR_INT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_ACCR_INT_LEN);
				offset = LN_PAST_ACCR_INT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN NUMBER 4 ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_NBR_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_NBR_OFF,
						LN_LOAN_NBR_LEN);
					offset = LN_LOAN_NBR_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_NBR_LEN);
				offset = LN_LOAN_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN AMOUNT ------------------------------------------------ */
		else if (memcmp(code, NP_LOAN_AMT_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_AMT_OFF,
						LN_LOAN_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_AMT_LEN);
				offset = LN_LOAN_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN START DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_START_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_START_OFF,
						LN_LOAN_START_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_START_LEN);
				offset = LN_LOAN_START_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - LOAN EXPIRY DATE -------------------------------------------- */
		else if (memcmp(code, NP_LOAN_EXPIRY_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_LOAN_EXPIRY_OFF,
						LN_LOAN_EXPIRY_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_LOAN_EXPIRY_LEN);
				offset = LN_LOAN_EXPIRY_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_AMT_NEXT_INSTL_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_AMT_NEXT_INSTL_OFF,
						LN_AMT_NEXT_INSTL_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_NEXT_INSTL_LEN);
				offset = LN_AMT_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE NEXT INSTL ---------------------------------------------- */
		else if (memcmp(code, NP_DATE_NEXT_INSTL_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_DATE_NEXT_INSTL_OFF,
						LN_DATE_NEXT_INSTL_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_NEXT_INSTL_LEN);
				offset = LN_DATE_NEXT_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - AMT LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_AMT_LAST_PAID_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_AMT_LAST_PAID_OFF,
						LN_AMT_LAST_PAID_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_AMT_LAST_PAID_LEN);
				offset = LN_AMT_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - DATE LAST PAID ----------------------------------------------- */
		else if (memcmp(code, NP_DATE_LAST_PAID_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_DATE_LAST_PAID_OFF,
						LN_DATE_LAST_PAID_LEN);
					offset = FormatToDate(field,lang) - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_DATE_LAST_PAID_LEN);
				offset = LN_DATE_LAST_PAID_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD INSTL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_INSTL_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_INSTL_OFF,
						LN_OUTSTD_INSTL_LEN);
					offset = LN_OUTSTD_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_INSTL_LEN);
				offset = LN_OUTSTD_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - OUTSTD BAL ------------------------------------------------- */
		else if (memcmp(code, NP_OUTSTD_BAL_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_OUTSTD_BAL_OFF,
						LN_OUTSTD_BAL_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_OUTSTD_BAL_LEN);
				offset = LN_OUTSTD_BAL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - PAST DUE INSTL --------------------------------------------- */
		/*else if (memcmp(code, NP_PAST_DUE_INSTL_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_INSTL_OFF,
						LN_PAST_DUE_INSTL_LEN);
					offset = LN_PAST_DUE_INSTL_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_INSTL_LEN);
				offset = LN_PAST_DUE_INSTL_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		*/

		/* - PAST DUE AMT --------------------------------------------- */
		/*else if (memcmp(code, NP_PAST_DUE_AMT_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_DUE_AMT_OFF,
						LN_PAST_DUE_AMT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_DUE_AMT_LEN);
				offset = LN_PAST_DUE_AMT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		*/

		/* - PAST ACCR INT -------------------------------------------- */
		/*else if (memcmp(code, NP_PAST_ACCR_INT_4, 2) == 0)
		{
			if (GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 4*NP_LOAN_DATA_LEN + 5)
				{ 
					memset(field,	0, sizeof(field));
					memcpy(currency, tmpField + 7, LN_ACC_CUR_LEN);
					memcpy(field,
						tmpField + 3*NP_LOAN_DATA_LEN + 5 + NP_PAST_ACCR_INT_OFF,
						LN_PAST_ACCR_INT_LEN);
					if ( lang == ARABIC )
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
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', LN_PAST_ACCR_INT_LEN);
				offset = LN_PAST_ACCR_INT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		*/
		/*  DCC RATE */ /* CFE */
		else if (memcmp(code, NP_DCC_RATE, 2) == 0)
		{
			if(GetTlvPrivate (DCC_DATA, &tlvInfo, tmpField, &lgField) == SUCCESS)
			{ 
				memset(field, 0, sizeof(field));
				memcpy(sRateDcc, tmpField + P7_F048_P56_RATE_OFST + 1    , P7_F048_P56_RATE_LEN - 1);
				memcpy(sMinorUnit, tmpField + P7_F048_P56_RATE_OFST  , 1);
				nNbDecimales = atoi(sMinorUnit);
				lRateAmount = atoi(sRateDcc);
				for(i = 0; i < nNbDecimales; i++ )
				{
					lRateAmount/=10.0;
				}
				sprintf(field, "%5.2lf", lRateAmount);
				offset = strlen(field) - 3;
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_DCC_RATE_LEN);
				offset = NP_DCC_RATE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* DCC AMOUNT */
		else if (memcmp(code, NP_DCC_AMOUNT, 2) == 0)
		{
			if(GetTlvPrivate (DCC_DATA, &tlvInfo, tmpField, &lgField) == SUCCESS)
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField + P7_F048_P56_DCC_AMOUNT_OFST, P7_F048_P56_DCC_AMOUNT_LEN);
				memcpy(currency, tmpField + P7_F048_P56_DCC_CCY_OFST, P7_F048_P56_DCC_CCY_LEN);

				if(lang == ARABIC)
					offset = FormatToAmount(nIndexCtx, 
							 field,
							 currency,
							 OK,
							 'R',
							 lang) - 3;
				else
					offset = FormatToAmount(nIndexCtx,
							 field,
							 currency,
							 OK,
							 'L',
							 lang) - 3;

				if (offset < 0)
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_DCC_AMNT_LEN);
					offset = NP_DCC_AMNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_DCC_AMNT_LEN);
				offset = NP_DCC_AMNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* ACCESS FEES */
		else if (memcmp(code, NP_FEE_AMOUNT, 2) == 0)
		{			
			if ( GetIsoField(ISO_FEES_AMOUNT, P7Buffer, tmpField, &lgField) == SUCCES)
			{
				memset(field, 0, sizeof(field));
				memcpy(field ,tmpField + 4, 8);
				GetIsoField( ISO_TRANS_CRNCY, P7Buffer, tmpField, &lgField);
				memcpy(currency, tmpField, lgField);

				if(lang == ARABIC)
					offset = FormatToAmount(nIndexCtx, 
							 field,
							 currency,
							 OK,
							 'R',
							 lang) - 3;
				else
					offset = FormatToAmount(nIndexCtx,
							 field,
							 currency,
							 OK,
							 'L',
							 lang) - 3;

				if (offset < 0)
				{
					memset(field, 0, sizeof(field));
					memset(field,	' ', NP_FEE_AMNT_LEN);
					offset = NP_FEE_AMNT_LEN - 3;
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_FEE_AMNT_LEN);
				offset = NP_FEE_AMNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* - MESSAGE  ------------------------------------------- */
		else if (memcmp(code, NP_MESSAGE, 2) == 0)
		{
			memset(tmpField, 0, sizeof(tmpField));
			if (GetIsoField( ISO_RESERVED_ISO_2 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				if ( lgField >= 3)
				{
					memset(field, 0, sizeof(field));
					memcpy(field,tmpField,lgField);
					offset = FormatToMessage(field, lang, PRN_WIDTH) - 3;
					if ( offset < 0 )
					{ 
						offset = 0;
						memcpy(field, "   ", 3);
					}

				}
			}
			else
			{ 
				memset(field,	' ', NP_MESSAGE_LEN);
				offset = NP_MESSAGE_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}

		/* - TRANS_SEQ ------------------------------------------ */
		else if (memcmp(code, NP_TRANS_SEQ, 2) == 0)
		{
			if (GetIsoField( ISO_ADTNL_DATA_PRIV , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field, 0, sizeof(field));



				memcpy (field, "0001", 4);
				offset = 1;

				/*if (GetTlvPrivate (TERMINAL_SEQ_NB, &tlvInfo, field, &lgField)== SUCCESS)*/
				if (GetTlvPrivate (EXTERNAL_STAN_TAG, &tlvInfo, field, &lgField)== SUCCESS)

					offset = strlen(field) - 3;

				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}

			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_TRANS_SEQ_LEN);
				offset = NP_TRANS_SEQ_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/* - TRANS_NAME ----------------------------------------- */
		else if (memcmp(code, NP_TRANS_NAME, 2) == 0)
		{
			if (GetIsoField( ISO_RESERV_PRIV_USAGE_1 , P7Buffer, tmpField, &lgField) == SUCCES && ( lgField > 0))
			{ 
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField);
				offset = AlignField(field, 'C', NP_TRANS_NAME_LEN) - 3;
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}

			}
			else
			{ 
				memset(field,	' ', NP_TRANS_NAME_LEN);
				offset = NP_TRANS_NAME_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}

		}
		/* MUW 10082011 */
		/*	- Consumer Id -------------------------------------------------
		*/
		/*else if (memcmp(code, NP_CONSUMER_ID, 2) == 0)
		{ 
			if ( GetIsoField( ISO_RECORD_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{      	
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField);
				offset = strlen(field) - 3; 

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', 20);
				offset = 20 - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}*/
		/* MUW 15052011 */
		/*	- Gen. UBP Print Data -------------------------------------------------
		*/
		else if (memcmp(code, NP_UTILITY_DATA, 2) == 0)
		{ 
			if ( GetIsoField( ISO_KEY_MNG_DATA , P7Buffer, tmpField, &lgField) == SUCCES )
			{      	
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField);
				offset = strlen(field) - 3; 
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_TRANS_NAME_LEN);
				offset = NP_TRANS_NAME_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ trace_event("End   insert_text(ERROR)",PROCESSING);
			return (ERROR);
			}
		}

		/*	- Surplus Amount -------------------------------------------------
		*/
	/*	else if (memcmp(code, NP_SURPLUS_AMT, 2) == 0)
		{ 
			/* if ( GetIsoField( ISO_RESERVED_ISO_3 , P7Buffer, tmpField, &lgField) == SUCCES ) */

			/*if (GetIsoField( ISO_RESERVED_ISO_3 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,	0, sizeof(field));
				memcpy(field,tmpField,lgField);
				memset(tmpField,        0, sizeof(tmpField));
				GetIsoField(ISO_TRANS_CRNCY,P7Buffer, tmpField, &lgField);
				memcpy(currency,tmpField,lgField);
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
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_AMOUNT_LEN);
				offset = NP_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}	*/
		
		/*	- Utility Amount -------------------------------------------------
		*/
	/*	else if (memcmp(code, NP_UTILITY_AMT, 2) == 0)
		{ 

			if (GetIsoField( ISO_RESERVED_ISO_4 , P7Buffer, tmpField, &lgField) == SUCCES )
			{ 
				memset(field,	0, sizeof(field));
				memcpy(field,tmpField,lgField);
				memset(tmpField,        0, sizeof(tmpField));
				GetIsoField(ISO_TRANS_CRNCY,P7Buffer, tmpField, &lgField);
				memcpy(currency,tmpField,lgField);
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
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_AMOUNT_LEN);
				offset = NP_AMOUNT_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ 
				trace_event("End   insert_text(ERROR)",PROCESSING);
				return (ERROR);
			}
		}
		/*
		- ABSENCE DU CODE ------------------------------------------------
		*/
		
		/* MBB 16052012 ICPS-CONV SIPEM */
		/* RRN Print Data 
		/* NP_RFU_95
		*/
		else if (memcmp(code, NP_AMOUNT_DEPOSIT, 2) == 0)
		{ 
			if ( GetIsoField( ISO_REFERENCE_NBR , P7Buffer, tmpField, &lgField) == SUCCES )
			{   
				printf(sLine, "ICPS-CONV SIPEM ISO_REFERENCE_NBR <%s>, lgField <%d>", tmpField, lgField);
				trace_event(sLine, PROCESSING);
				
				memset(field, 0, sizeof(field));
				memcpy(field, tmpField,lgField);
				offset = strlen(field) - 3; 
				if ( offset < 0 )
				{ 
					offset = 0;
					memcpy(field, "   ", 3);
				}
				sprintf(sLine, "ICPS-CONV SIPEM field <%s>, offset <%d>", field, offset);
				trace_event(sLine, PROCESSING);

			}
			else
			{ 
				memset(field, 0, sizeof(field));
				memset(field,	' ', NP_REF_NBR_LEN);
				offset = NP_REF_NBR_LEN - 3;
			}

			if (deplacer(c_prints, searched_pos + 1, offset) == OK)
				memcpy(searched_pos, field, strlen(field));
			else
			{ trace_event("End   insert_text(ERROR)",PROCESSING);
			return (ERROR);
			}
		}
		else
		{ 
			sprintf(sLine,"End   insert_text(ERROR) -> code = %c%c",code[0],code[1]);
			trace_event(sLine, PROCESSING);
			return (ERROR);
		}
		/** End MBB 16052024 */

		/*
		- calcul des nouvelles valeurs des parametres de recherches -------
		*/
		current_pos = searched_pos;
		current_len = c_prints->len - (c_prints->data - current_pos); 
	}

	trace_event("End   insert_text(OK)",PROCESSING);
	return(OK);
}

