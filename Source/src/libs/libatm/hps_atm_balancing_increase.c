
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
           char  filnam[30];
};
static struct sqlcxp sqlfpn =
{
    29,
    "hps_atm_balancing_increase.pc"
};


static unsigned int sqlctx = 1668336045;


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
5,0,0,1,143,0,6,85,0,0,6,6,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,1,1,0,0,1,3,0,0,2,9,0,
0,
};



/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      hps_atm_cutoff.pc                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      ATM CUTOFF Tools Management                                        */
/*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include <sqlca.h>
#include <sqlcpr.h>

#include <define.h>
#include <ndc_param.h>
#include <param_ctx.h>


  /***********************************************************************
  **  nom	: SetAtmBalancingIncr()					**
  **  desc	: recuperation des donnees de reconcilliation GAB	**
  **  entree	: pTermNbr	-> numero du terminal			**
  **  sortie	: pReconcData	-> donnees de reconcilliation au format	**
  **		PowerSEVEN						**
  **  retour	: OK	-> donnees recuperees				**
  **		  NOK	-> The atm was balanced				**
  **		  ERROR	-> erreur fatal					**
  **  auteur	: Younes OUADI (09 Sep 1999)				**
  **		(C) 1997 - 2001 : Hightech Payment Systems		**
  ***********************************************************************/
int	SetAtmBalancingIncr(
int	nIndexCtx,
char	*pTermNbr,
char	*pCardNumber,
char	pCassetteType,
int	pAmount,
char	*pReconcData
)
{
    struct sqlca sqlca;
    char	sLine [MAX_LINE_TRC];
/* EXEC SQL BEGIN DECLARE SECTION; */ 

    int		hOpCode;
    /* varchar	hTermNbr	[  15+1]; */ 
struct { unsigned short len; unsigned char arr[16]; } hTermNbr;

    /* varchar	hCardNbr	[  22+1]; */ 
struct { unsigned short len; unsigned char arr[23]; } hCardNbr;

    char	hCassetteType;
    int		hAmount;
    /* varchar	hReconcData	[1024+1]; */ 
struct { unsigned short len; unsigned char arr[1025]; } hReconcData;

    int		hRet;
/* EXEC SQL End  DECLARE SECTION; */ 


sprintf(sLine, "Start SetAtmBalancingIncr(%.15s,%.16s,%c,%d)", pTermNbr, pCardNumber, pCassetteType, pAmount);
trace_event(sLine, PROCESSING);

    /* - Raz des variables ------------------------------------------------- */
    memset(hTermNbr.arr,	0, sizeof(hTermNbr.arr));
    hTermNbr.len = 0;
    memset(hCardNbr.arr,	0, sizeof(hCardNbr.arr));
    hCardNbr.len = 0;
    memset(hReconcData.arr,	0, sizeof(hReconcData.arr));
    hReconcData.len = 0;

    hAmount = 0;
    hCassetteType = 0;


    /* - Init des variables ------------------------------------------------- */
    memcpy(hTermNbr.arr,	pTermNbr, strlen(pTermNbr));
    hTermNbr.len = strlen(pTermNbr);

    memcpy(hCardNbr.arr,	pCardNumber, strlen(pCardNumber));
    hCardNbr.len = strlen(pCardNumber);

    hCassetteType = pCassetteType;
    hAmount = pAmount;

    /* - Capture des exceptions -------------------------------------------- */
    /* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndexCtx].ctx; */ 

    /* EXEC SQL WHENEVER SQLERROR GOTO GetAtmBalancingDataErr; */ 



    /* - Traitement PL/SQL ------------------------------------------------- */
    /* EXEC SQL EXECUTE
    BEGIN
        :hRet := PCRD_SPECEFIC_ADMIN_CARD.MIDPOINT_ADJUSTMENT_INCR(:hTermNbr, :hCardNbr, :hCassetteType, :hAmount, :hReconcData);
    END;
    END-EXEC; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 6;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "begin :hRet := PCRD_SPECEFIC_ADMIN_CARD . MIDPOINT_ADJUST\
MENT_INCR ( :hTermNbr , :hCardNbr , :hCassetteType , :hAmount , :hReconcData )\
 ; END ;";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)&hRet;
    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqhstv[1] = (unsigned char  *)&hTermNbr;
    sqlstm.sqhstl[1] = (unsigned long )18;
    sqlstm.sqhsts[1] = (         int  )0;
    sqlstm.sqindv[1] = (         short *)0;
    sqlstm.sqinds[1] = (         int  )0;
    sqlstm.sqharm[1] = (unsigned long )0;
    sqlstm.sqadto[1] = (unsigned short )0;
    sqlstm.sqtdso[1] = (unsigned short )0;
    sqlstm.sqhstv[2] = (unsigned char  *)&hCardNbr;
    sqlstm.sqhstl[2] = (unsigned long )25;
    sqlstm.sqhsts[2] = (         int  )0;
    sqlstm.sqindv[2] = (         short *)0;
    sqlstm.sqinds[2] = (         int  )0;
    sqlstm.sqharm[2] = (unsigned long )0;
    sqlstm.sqadto[2] = (unsigned short )0;
    sqlstm.sqtdso[2] = (unsigned short )0;
    sqlstm.sqhstv[3] = (unsigned char  *)&hCassetteType;
    sqlstm.sqhstl[3] = (unsigned long )1;
    sqlstm.sqhsts[3] = (         int  )0;
    sqlstm.sqindv[3] = (         short *)0;
    sqlstm.sqinds[3] = (         int  )0;
    sqlstm.sqharm[3] = (unsigned long )0;
    sqlstm.sqadto[3] = (unsigned short )0;
    sqlstm.sqtdso[3] = (unsigned short )0;
    sqlstm.sqhstv[4] = (unsigned char  *)&hAmount;
    sqlstm.sqhstl[4] = (unsigned long )sizeof(int);
    sqlstm.sqhsts[4] = (         int  )0;
    sqlstm.sqindv[4] = (         short *)0;
    sqlstm.sqinds[4] = (         int  )0;
    sqlstm.sqharm[4] = (unsigned long )0;
    sqlstm.sqadto[4] = (unsigned short )0;
    sqlstm.sqtdso[4] = (unsigned short )0;
    sqlstm.sqhstv[5] = (unsigned char  *)&hReconcData;
    sqlstm.sqhstl[5] = (unsigned long )1027;
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
    if (sqlca.sqlcode < 0) goto GetAtmBalancingDataErr;
}



    /* - Examen du retour PL/SQL ------------------------------------------- */
    if (hRet != OK) {
        trace_event("End   SetAtmBalancingIncr(NOK)", PROCESSING);
        return hRet;
    }

    /* - Recuperation du resultat PL/SQL ------------------------------------ */
    memcpy(pReconcData, hReconcData.arr, hReconcData.len);

sprintf(sLine, "End   SetAtmBalancingIncr(%.45s)", pReconcData);
trace_event(sLine, PROCESSING);
return (OK);
GetAtmBalancingDataErr:
    sprintf(sLine, "%.79s", sqlca.sqlerrm.sqlerrmc);
    trace_event(sLine, ERROR);
    trace_event("End   SetAtmBalancingIncr(ERROR)", ERROR);
    return (ERROR);
}

