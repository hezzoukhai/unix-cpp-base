
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[24];
};
static struct sqlcxp sqlfpn =
{
    23,
    "get_last_transaction.pc"
};


static unsigned int sqlctx = 619892635;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[6];
   unsigned long  sqhstl[6];
            int   sqhsts[6];
            short *sqindv[6];
            int   sqinds[6];
   unsigned long  sqharm[6];
   unsigned long  *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {12,6};

/* SQLLIB Prototypes */
extern sqlcxt (/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlcx2t(/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlbuft(/*_ void **, char * _*/);
extern sqlgs2t(/*_ void **, char * _*/);
extern sqlorat(/*_ void **, unsigned int *, void * _*/);

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem(/*_ char *, int * _*/);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,12330,31,0,0,
5,0,0,1,180,0,6,85,0,0,6,6,0,1,0,2,3,0,0,1,9,0,0,2,9,0,0,2,9,0,0,2,9,0,0,2,9,0,
0,
};


/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      get_last_transaction.pc                                                  */
/*                                                                             */
/* Description                                                                 */
/*      Get Last Transaction                                                    */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <define.h>
#include <paramdef.h>
#include <langue.h>
#include <param_ctx.h>
#include <sqlca.h>
#include <sqlcpr.h>

/******************************************************************************/
/* Fonction get_last_transaction                                              */
/* -----------------------------	                                      */
/* Description : Fonction qui recupere le numero de compte pour une carte     */
/*		 donne.         					      */
/* Entree      : le numero de compte					      */
/*               le code da la banque					      */
/*               le numero de la carte					      */
/* Sortie      : OK						              */
/*               NOK                   			                      */
/*               MALFUNCTION                                                  */
/* Date/Auteur : 04/05/2007 A. HMAMI                                          */
/* Revision    :                                                              */
/******************************************************************************/
int    get_last_trx(nIndexCtx,
                    slCardNumber,
                    sLastTrxAmount,
                    sLastTrxAccount,
                    sLastTrxDate,
                    sLastTrxTerminal)
int	        nIndexCtx;
char            *slCardNumber;
char            *sLastTrxAmount;
char            *sLastTrxAccount;
char            *sLastTrxDate;
char            *sLastTrxTerminal;
{

char		sLine[80];

/* EXEC SQL BEGIN DECLARE SECTION; */ 

  /* VARCHAR 	T_card_number  [22]; */ 
struct { unsigned short len; unsigned char arr[22]; } T_card_number;

  /* VARCHAR 	T_transaction_amount [22]; */ 
struct { unsigned short len; unsigned char arr[22]; } T_transaction_amount;

  /* VARCHAR 	T_source_account [22]; */ 
struct { unsigned short len; unsigned char arr[22]; } T_source_account;

  /* VARCHAR 	T_transmission_date [22]; */ 
struct { unsigned short len; unsigned char arr[22]; } T_transmission_date;

  /* VARCHAR 	T_terminal_id     [22]; */ 
struct { unsigned short len; unsigned char arr[22]; } T_terminal_id;

  int	 	response_code;
/* EXEC SQL END DECLARE SECTION; */ 


sprintf(sLine, "Start get_last_trx(%.22s)", slCardNumber);
trace_event(sLine, PROCESSING);

memset(T_card_number.arr, 0, sizeof(T_card_number.arr));
T_card_number.len = 0; 

memset(T_transaction_amount.arr, 0, sizeof(T_transaction_amount.arr));
T_transaction_amount.len = 0;

memset(T_source_account.arr, 0, sizeof(T_source_account.arr));
T_source_account.len = 0;

memset(T_transmission_date.arr, 0, sizeof(T_transmission_date.arr));
T_transmission_date.len = 0;

memset(T_terminal_id.arr, 0, sizeof(T_terminal_id.arr));
T_terminal_id.len = 0;

memcpy(T_card_number.arr, slCardNumber, strlen(slCardNumber));
T_card_number.len = strlen(slCardNumber);

response_code = 909;
/* EXEC SQL WHENEVER SQLERROR GOTO account_card; */ 

/* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndexCtx].ctx; */ 

/* EXEC SQL EXECUTE
DECLARE 
BEGIN
	:response_code   :=  PCRD_ATM_TOOLS.GET_LAST_TRANSACTION(:T_card_number,
    	                                                                    :T_transaction_amount,
        	                                                            :T_source_account,
            	                                                            :T_transmission_date,
                	                                                    :T_terminal_id);
	

END;
END-EXEC; */ 

{
struct sqlexd sqlstm;
sqlstm.sqlvsn = 12;
sqlstm.arrsiz = 6;
sqlstm.sqladtp = &sqladt;
sqlstm.sqltdsp = &sqltds;
sqlstm.stmt = "declare BEGIN :response_code := PCRD_ATM_TOOLS . GET_LAST_TRA\
NSACTION ( :T_card_number , :T_transaction_amount , :T_source_account , :T_tra\
nsmission_date , :T_terminal_id ) ; END ;";
sqlstm.iters = (unsigned int  )1;
sqlstm.offset = (unsigned int  )5;
sqlstm.cud = sqlcud0;
sqlstm.sqlest = (unsigned char  *)&sqlca;
sqlstm.sqlety = (unsigned short)256;
sqlstm.occurs = (unsigned int  )0;
sqlstm.sqhstv[0] = (unsigned char  *)&response_code;
sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
sqlstm.sqhsts[0] = (         int  )0;
sqlstm.sqindv[0] = (         short *)0;
sqlstm.sqinds[0] = (         int  )0;
sqlstm.sqharm[0] = (unsigned long )0;
sqlstm.sqadto[0] = (unsigned short )0;
sqlstm.sqtdso[0] = (unsigned short )0;
sqlstm.sqhstv[1] = (unsigned char  *)&T_card_number;
sqlstm.sqhstl[1] = (unsigned long )24;
sqlstm.sqhsts[1] = (         int  )0;
sqlstm.sqindv[1] = (         short *)0;
sqlstm.sqinds[1] = (         int  )0;
sqlstm.sqharm[1] = (unsigned long )0;
sqlstm.sqadto[1] = (unsigned short )0;
sqlstm.sqtdso[1] = (unsigned short )0;
sqlstm.sqhstv[2] = (unsigned char  *)&T_transaction_amount;
sqlstm.sqhstl[2] = (unsigned long )24;
sqlstm.sqhsts[2] = (         int  )0;
sqlstm.sqindv[2] = (         short *)0;
sqlstm.sqinds[2] = (         int  )0;
sqlstm.sqharm[2] = (unsigned long )0;
sqlstm.sqadto[2] = (unsigned short )0;
sqlstm.sqtdso[2] = (unsigned short )0;
sqlstm.sqhstv[3] = (unsigned char  *)&T_source_account;
sqlstm.sqhstl[3] = (unsigned long )24;
sqlstm.sqhsts[3] = (         int  )0;
sqlstm.sqindv[3] = (         short *)0;
sqlstm.sqinds[3] = (         int  )0;
sqlstm.sqharm[3] = (unsigned long )0;
sqlstm.sqadto[3] = (unsigned short )0;
sqlstm.sqtdso[3] = (unsigned short )0;
sqlstm.sqhstv[4] = (unsigned char  *)&T_transmission_date;
sqlstm.sqhstl[4] = (unsigned long )24;
sqlstm.sqhsts[4] = (         int  )0;
sqlstm.sqindv[4] = (         short *)0;
sqlstm.sqinds[4] = (         int  )0;
sqlstm.sqharm[4] = (unsigned long )0;
sqlstm.sqadto[4] = (unsigned short )0;
sqlstm.sqtdso[4] = (unsigned short )0;
sqlstm.sqhstv[5] = (unsigned char  *)&T_terminal_id;
sqlstm.sqhstl[5] = (unsigned long )24;
sqlstm.sqhsts[5] = (         int  )0;
sqlstm.sqindv[5] = (         short *)0;
sqlstm.sqinds[5] = (         int  )0;
sqlstm.sqharm[5] = (unsigned long )0;
sqlstm.sqadto[5] = (unsigned short )0;
sqlstm.sqtdso[5] = (unsigned short )0;
sqlstm.sqphsv = sqlstm.sqhstv;
sqlstm.sqphsl = sqlstm.sqhstl;
sqlstm.sqphss = sqlstm.sqhsts;
sqlstm.sqpind = sqlstm.sqindv;
sqlstm.sqpins = sqlstm.sqinds;
sqlstm.sqparm = sqlstm.sqharm;
sqlstm.sqparc = sqlstm.sqharc;
sqlstm.sqpadto = sqlstm.sqadto;
sqlstm.sqptdso = sqlstm.sqtdso;
sqlcxt(&(tab_ParamCtx[nIndexCtx].ctx), &sqlctx, &sqlstm, &sqlfpn);
if (sqlca.sqlcode < 0) goto account_card;
}



if (response_code == OK)
{
	memcpy(sLastTrxAmount, T_transaction_amount.arr, T_transaction_amount.len);
        sprintf(sLine, "transaction_amount(%.24s)", sLastTrxAmount);
	trace_event(sLine, PROCESSING);

	memcpy(sLastTrxAccount, T_source_account.arr, T_source_account.len);
        sprintf(sLine, "source_account(%.24s)", sLastTrxAccount);
	trace_event(sLine, PROCESSING);

	memcpy(sLastTrxDate, T_transmission_date.arr, T_transmission_date.len);
        sprintf(sLine, "transmission_date(%.24s)", sLastTrxDate);
	trace_event(sLine, PROCESSING);

	memcpy(sLastTrxTerminal, T_terminal_id.arr, T_terminal_id.len);
        sprintf(sLine, "terminal_id(%.24s)", sLastTrxTerminal);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "End   get_last_trx(OK)");
	trace_event(sLine, PROCESSING);
	return(OK); 
}
sprintf(sLine, "End   get_last_trx(NOK)");
trace_event(sLine, PROCESSING);
return(NOK);

account_card :
	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	sprintf(sLine, "%s",sqlca.sqlerrm.sqlerrmc);
	trace_event(sLine, TRACE);
	trace_event("End  get_last_trx(MALFUNCTION)", PROCESSING);
	return(MALFUNCTION);
}
