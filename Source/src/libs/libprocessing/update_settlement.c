
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
           char  filnam[21];
};
static struct sqlcxp sqlfpn =
{
    20,
    "update_settlement.pc"
};


static unsigned long sqlctx = 84018043;


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
            void  *sqhstv[6];
   unsigned int   sqhstl[6];
            int   sqhsts[6];
            void  *sqindv[6];
            int   sqinds[6];
   unsigned int   sqharm[6];
   unsigned int   *sqharc[6];
   unsigned short  sqadto[6];
   unsigned short  sqtdso[6];
} sqlstm = {10,6};

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
5,0,0,1,107,0,6,52,0,0,3,3,0,1,0,2,3,0,0,3,9,0,0,3,9,0,0,
32,0,0,2,141,0,6,124,0,0,5,5,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,3,9,0,0,3,9,0,0,
67,0,0,3,161,0,6,202,0,0,6,6,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,3,9,0,0,
3,9,0,0,
};


#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <param_ctx.h>
#include <iso_hps.h>
#include <event_auth.h>
#include <tlv_autho.h>

#include <sqlca.h>
#include <sqlcpr.h>

int UpdateSettlement  ( int nIndiceCtx , char *sTlvBuffer)
{
	struct sqlca sqlca;
	char sLine 		 [ MAX_LINE_TRC];
	int				nRetCode;


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

 	/* VARCHAR   	T_tlv_auth	  [ LG_MAX + 1]; */ 
struct { unsigned short len; unsigned char arr[2049]; } T_tlv_auth;

 	/* VARCHAR   	T_tlv_warning [ 6 + 1]; */ 
struct { unsigned short len; unsigned char arr[7]; } T_tlv_warning;

	int		    response_code;
	/* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

    trace_event ( "Start UpdateSettlement () ", PROCESSING );

    PrintTlvBuffer (  sTlvBuffer );


/****************************************************************************/
/*      INITIALISATION                                                      */
/****************************************************************************/

	memset ( T_tlv_auth.arr            ,  0  ,   sizeof(T_tlv_auth.arr )  );
	T_tlv_auth.len = 0;

	memset ( T_tlv_warning.arr         ,  0  ,   sizeof(T_tlv_warning.arr )  );
	T_tlv_warning.len = 0;

	memcpy ( T_tlv_auth.arr     ,  sTlvBuffer   ,   strlen(sTlvBuffer )  );
	T_tlv_auth.len = strlen(sTlvBuffer );



/****************************************************************************/
/*      Appel PL/SQL                                                       */
/****************************************************************************/
	response_code = SYSTEM_MALFUNCTION;
	/* EXEC SQL WHENEVER SQLERROR GOTO err_UpdateSettlement; */ 

	/* EXEC SQL EXECUTE
	BEGIN
        :response_code := PCRD_P7_SETTLEMENT_MNG.UPDATE_SETTLEMENT( 
																:T_tlv_auth,
																:T_tlv_warning); 
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 3;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response_code := PCRD_P7_SETTLEMENT_MNG . UPDATE_SETT\
LEMENT ( :T_tlv_auth , :T_tlv_warning ) ; END ;";
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
 sqlstm.sqhstv[1] = (         void  *)&T_tlv_auth;
 sqlstm.sqhstl[1] = (unsigned int  )2051;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&T_tlv_warning;
 sqlstm.sqhstl[2] = (unsigned int  )9;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) goto err_UpdateSettlement;
}


	
	sprintf(sLine,"End UpdateSettlement (%d,%.6s)", response_code,
														  T_tlv_warning.arr);
	trace_event(sLine,PROCESSING);
	return(response_code);

err_UpdateSettlement :
	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	sprintf(sLine, "%.40s",sqlca.sqlerrm.sqlerrmc);
	trace_event (  sLine , ERROR );
	trace_event("End UpdateSettlement (SYSTEM_MALFUNCTION)", PROCESSING);
	return( SYSTEM_MALFUNCTION );
}

int GetSettlementData  ( int nIndiceCtx , char *sTlvBuffer, char *sSettlementType, char *sResource)
{
	struct sqlca sqlca;
	char sLine 		 [ MAX_LINE_TRC];
	int				nRetCode;


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

 	/* VARCHAR   	T_tlv_auth	  	[ LG_MAX + 1]; */ 
struct { unsigned short len; unsigned char arr[2049]; } T_tlv_auth;

 	char       	T_resource_id   [ 6 + 1];
 	char       	T_sett_type     [ 3 + 1];
 	/* VARCHAR   	T_tlv_warning 	[ 6 + 1]; */ 
struct { unsigned short len; unsigned char arr[7]; } T_tlv_warning;

	int		    response_code;
	/* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

    trace_event ( "Start GetSettlementData () ", PROCESSING );

    PrintTlvBuffer (  sTlvBuffer );


/****************************************************************************/
/*      INITIALISATION                                                      */
/****************************************************************************/

	memset ( T_tlv_auth.arr            ,  0  ,   sizeof(T_tlv_auth.arr )  );
	T_tlv_auth.len = 0;

	memset ( T_tlv_warning.arr         ,  0  ,   sizeof(T_tlv_warning.arr )  );
	T_tlv_warning.len = 0;

	memset(T_resource_id, 0, sizeof(T_resource_id));
	memset(T_sett_type  , 0, sizeof(T_sett_type  ));

	memcpy ( T_tlv_auth.arr     ,  sTlvBuffer   ,   strlen(sTlvBuffer )  );
	T_tlv_auth.len = strlen(sTlvBuffer );

	memcpy(T_resource_id, sResource, 6);
	memcpy(T_sett_type  , sSettlementType, 3);

	sprintf(sLine," Resource Id      : %s|",T_resource_id);
	trace_event(sLine,TRACE);

	sprintf(sLine," Sett Type        : %s|",T_sett_type);
	trace_event(sLine,TRACE);

/****************************************************************************/
/*      Appel PL/SQL                                                       */
/****************************************************************************/
	response_code = SYSTEM_MALFUNCTION;
	/* EXEC SQL WHENEVER SQLERROR GOTO err_GetSettlementData; */ 

	/* EXEC SQL EXECUTE
	BEGIN
        :response_code := PCRD_P7_SETTLEMENT_MNG.GET_SETTLEMENT_DATA(   :T_resource_id,
																		:T_sett_type,
																		:T_tlv_auth,
																		:T_tlv_warning); 
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response_code := PCRD_P7_SETTLEMENT_MNG . GET_SETTLEM\
ENT_DATA ( :T_resource_id , :T_sett_type , :T_tlv_auth , :T_tlv_warning ) ; EN\
D ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )32;
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
 sqlstm.sqhstv[1] = (         void  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned int  )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)T_sett_type;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&T_tlv_auth;
 sqlstm.sqhstl[3] = (unsigned int  )2051;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&T_tlv_warning;
 sqlstm.sqhstl[4] = (unsigned int  )9;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) goto err_GetSettlementData;
}


	
	sprintf(sLine,"End GetSettlementData (%d,%.6s)", response_code,
														  T_tlv_warning.arr);
	trace_event(sLine,PROCESSING);
	return(response_code);

err_GetSettlementData :
	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	sprintf(sLine, "%.40s",sqlca.sqlerrm.sqlerrmc);
	trace_event (  sLine , ERROR );
	trace_event("End GetSettlementData (SYSTEM_MALFUNCTION)", PROCESSING);
	return( SYSTEM_MALFUNCTION );
}
int UpdateSettlementData  ( int nIndiceCtx , char *sTlvBuffer, char *sStatus, char *sSettlementType, char *sResource)
{
	struct sqlca sqlca;
	char sLine 		 [ MAX_LINE_TRC];
	int				nRetCode;


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

 	/* VARCHAR   	T_tlv_auth	  	[ LG_MAX + 1]; */ 
struct { unsigned short len; unsigned char arr[2049]; } T_tlv_auth;

 	char       	T_resource_id   [ 6 + 1];
 	char       	T_sett_type     [ 3 + 1];
 	char       	T_sett_status   [ 1 + 1];
 	/* VARCHAR   	T_tlv_warning 	[ 6 + 1]; */ 
struct { unsigned short len; unsigned char arr[7]; } T_tlv_warning;

	int		    response_code;
	/* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

    trace_event ( "Start UpdateSettlementData () ", PROCESSING );

    PrintTlvBuffer (  sTlvBuffer );


/****************************************************************************/
/*      INITIALISATION                                                      */
/****************************************************************************/

	memset ( T_tlv_auth.arr            ,  0  ,   sizeof(T_tlv_auth.arr )  );
	T_tlv_auth.len = 0;

	memset ( T_tlv_warning.arr         ,  0  ,   sizeof(T_tlv_warning.arr )  );
	T_tlv_warning.len = 0;

	memset(T_resource_id, 0, sizeof(T_resource_id));
	memset(T_sett_type  , 0, sizeof(T_sett_type  ));
	memset(T_sett_status  , 0, sizeof(T_sett_status  ));

	memcpy ( T_tlv_auth.arr     ,  sTlvBuffer   ,   strlen(sTlvBuffer )  );
	T_tlv_auth.len = strlen(sTlvBuffer );

	memcpy(T_resource_id, sResource, 6);
	memcpy(T_sett_type  , sSettlementType, 3);
	memcpy(T_sett_status  , sStatus, 1);

	sprintf(sLine," Resource Id      : %s|",T_resource_id);
	trace_event(sLine,TRACE);

	sprintf(sLine," Sett Type        : %s|",T_sett_type);
	trace_event(sLine,TRACE);

	sprintf(sLine," Status           : %s|",T_sett_status);
	trace_event(sLine,TRACE);

/****************************************************************************/
/*      Appel PL/SQL                                                       */
/****************************************************************************/
	response_code = SYSTEM_MALFUNCTION;
	/* EXEC SQL WHENEVER SQLERROR GOTO err_UpdateSettlementData; */ 

	/* EXEC SQL EXECUTE
	BEGIN
        :response_code := PCRD_P7_SETTLEMENT_MNG.UPDATE_SETTLEMENT_DATA(    :T_resource_id,
																			:T_sett_type,
																			:T_sett_status,
																			:T_tlv_auth,
																			:T_tlv_warning); 
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 6;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response_code := PCRD_P7_SETTLEMENT_MNG . UPDATE_SETT\
LEMENT_DATA ( :T_resource_id , :T_sett_type , :T_sett_status , :T_tlv_auth , :\
T_tlv_warning ) ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )67;
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
 sqlstm.sqhstv[1] = (         void  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned int  )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)T_sett_type;
 sqlstm.sqhstl[2] = (unsigned int  )4;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)T_sett_status;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&T_tlv_auth;
 sqlstm.sqhstl[4] = (unsigned int  )2051;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&T_tlv_warning;
 sqlstm.sqhstl[5] = (unsigned int  )9;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
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
 sqlcxt(&(tab_ParamCtx[nIndiceCtx].ctx), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) goto err_UpdateSettlementData;
}


	
	sprintf(sLine,"End UpdateSettlementData (%d,%.6s)", response_code,
														  T_tlv_warning.arr);
	trace_event(sLine,PROCESSING);
	return(response_code);

err_UpdateSettlementData :
	/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

	sprintf(sLine, "%.40s",sqlca.sqlerrm.sqlerrmc);
	trace_event (  sLine , ERROR );
	trace_event("End UpdateSettlementData (SYSTEM_MALFUNCTION)", PROCESSING);
	return( SYSTEM_MALFUNCTION );
}
