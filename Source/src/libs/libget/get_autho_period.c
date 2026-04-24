
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned long magic;
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
           char  filnam[20];
};
static struct sqlcxp sqlfpn =
{
    19,
    "get_autho_period.pc"
};


static unsigned long sqlctx = 38695627;


static struct sqlexd {
   unsigned int   sqlvsn;
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
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {10,4};

/* SQLLIB Prototypes */
extern sqlcxt (/*_ void **, unsigned long *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlcx2t(/*_ void **, unsigned long *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlbuft(/*_ void **, char * _*/);
extern sqlgs2t(/*_ void **, char * _*/);
extern sqlorat(/*_ void **, unsigned long *, void * _*/);

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem(/*_ char *, int * _*/);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{10,12330,0,0,0,
5,0,0,1,308,0,6,39,0,0,4,4,0,1,0,3,3,0,0,1,97,0,0,2,97,0,0,2,97,0,0,
};


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <param_ctx.h>
#include <iso_hps.h>
#include <virtual_record.h>

#include <sqlca.h>
#include <sqlcpr.h>



int GetAuthoPeriod  (int nIndiceCtx, TSIsoInfo *msgInfo, 
					TSVirtualRecord *virtual_record)
{
	struct sqlca sqlca;
	char detail [ MAX_LINE_TRC];

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

 	char   PERIOD_CODE  [ 3 ];
 	char   PERIOD_TYPE  [ 1 ];
 	char   PERIOD_VALUE [ 1 ];
	int	response_code;
	/* EXEC SQL END DECLARE SECTION; */ 


        /* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 


	trace_event ( "Start GetAuthoPeriod() ", PROCESSING );
 	memset (   PERIOD_CODE  , 0 ,  3  );
 	memset (   PERIOD_TYPE  , 0 ,  1  );
 	memset (   PERIOD_VALUE , 0 ,  1  );

	memcpy ( PERIOD_CODE,virtual_record->autho_period.period_code,3);

	response_code = 909;
	/* EXEC SQL WHENEVER SQLERROR GOTO err_get_autho_period; */ 

	/* EXEC SQL EXECUTE
	    DECLARE T_P7_AUTHO_PERIOD   P7_AUTHO_PERIOD%ROWTYPE;
	BEGIN

       :response_code := PCRD_GET_PARAM_AUTHO_ROWS.GET_P7_AUTHO_PERIOD
						(   :PERIOD_CODE, 
	                                            T_P7_AUTHO_PERIOD 
	                                        );


		IF :response_code = 0 THEN
		:PERIOD_TYPE        := T_P7_AUTHO_PERIOD.PERIOD_TYPE;
		:PERIOD_VALUE       := T_P7_AUTHO_PERIOD.PERIOD_VALUE;
		END IF;
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "declare T_P7_AUTHO_PERIOD P7_AUTHO_PERIOD % ROWTYPE ; BEGIN \
:response_code := PCRD_GET_PARAM_AUTHO_ROWS . GET_P7_AUTHO_PERIOD ( :PERIOD_CO\
DE , T_P7_AUTHO_PERIOD ) ; IF :response_code = 0 THEN :PERIOD_TYPE := T_P7_AUT\
HO_PERIOD . PERIOD_TYPE ; :PERIOD_VALUE := T_P7_AUTHO_PERIOD . PERIOD_VALUE ; \
END IF ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&response_code;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)PERIOD_CODE;
 sqlstm.sqhstl[1] = (unsigned int  )3;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)PERIOD_TYPE;
 sqlstm.sqhstl[2] = (unsigned int  )1;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)PERIOD_VALUE;
 sqlstm.sqhstl[3] = (unsigned int  )1;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt(&(tab_ParamCtx[nIndiceCtx].ctx), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) goto err_get_autho_period;
}



	if ( response_code == 0 )
	{
	        memcpy ( virtual_record->autho_period.period_type,
							PERIOD_TYPE,1);
	        memcpy ( virtual_record->autho_period.period_value,
							PERIOD_VALUE,1);
	        trace_event ( "End GetAuthoPeriod( OK ) ", PROCESSING );
		return(OK);
	}
	trace_event ( "End GetAuthoPeriod( NOK ) ", PROCESSING );
	return(NOK);

err_get_autho_period :
	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	sprintf(detail, "%s",sqlca.sqlerrm.sqlerrmc);


	if (memcmp(detail,"ORA-01405",9) == 0)
	{
	        memcpy ( virtual_record->autho_period.period_type,
							PERIOD_TYPE,1);
	        memcpy ( virtual_record->autho_period.period_value,
							PERIOD_VALUE,1);
	        trace_event ( "End GetAuthoPeriod( OK ) ", PROCESSING );
		return(OK);
	}

	trace_event (  detail , ERROR );
	trace_event ( "End GetAuthoPeriod( NOK ) ", PROCESSING );
	return( NOK );
}
