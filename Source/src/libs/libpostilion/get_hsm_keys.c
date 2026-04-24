
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
           char  filnam[16];
};
static struct sqlcxp sqlfpn =
{
    15,
    "get_hsm_keys.pc"
};


static unsigned int sqlctx = 2424739;


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
   unsigned char  *sqhstv[8];
   unsigned long  sqhstl[8];
            int   sqhsts[8];
            short *sqindv[8];
            int   sqinds[8];
   unsigned long  sqharm[8];
   unsigned long  *sqharc[8];
   unsigned short  sqadto[8];
   unsigned short  sqtdso[8];
} sqlstm = {12,8};

/* SQLLIB Prototypes */
extern sqlcxt ( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlcx2t( void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * );
extern sqlbuft( void **, char * );
extern sqlgs2t( void **, char * );
extern sqlorat( void **, unsigned int *, void * );

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem( unsigned char *, signed int * );

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{12,12330,31,0,0,
5,0,0,1,88,0,6,57,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,2,9,0,0,2,9,0,0,
36,0,0,2,104,0,6,140,0,0,5,5,0,1,0,2,3,0,0,1,9,0,0,1,3,0,0,2,9,0,0,2,9,0,0,
71,0,0,3,85,0,6,215,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
98,0,0,4,102,0,6,296,0,0,4,4,0,1,0,2,3,0,0,1,97,0,0,1,9,0,0,1,9,0,0,
129,0,0,5,102,0,6,368,0,0,4,4,0,1,0,2,3,0,0,1,97,0,0,1,9,0,0,1,9,0,0,
160,0,0,6,85,0,6,428,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
187,0,0,7,85,0,6,495,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
214,0,0,8,97,0,6,566,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,2,9,0,0,2,9,0,0,
245,0,0,9,97,0,6,639,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,2,9,0,0,3,9,0,0,
276,0,0,10,97,0,6,711,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,3,9,0,0,3,9,0,0,
307,0,0,11,110,0,6,791,0,0,5,5,0,1,0,2,3,0,0,1,9,0,0,1,97,0,0,2,9,0,0,2,9,0,0,
342,0,0,12,80,0,6,859,0,0,3,3,0,1,0,2,3,0,0,1,9,0,0,2,9,0,0,
369,0,0,13,95,0,6,919,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
396,0,0,14,88,0,6,987,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,2,97,0,0,2,97,0,0,
427,0,0,15,81,0,6,1057,0,0,3,3,0,1,0,2,3,0,0,1,9,0,0,3,9,0,0,
454,0,0,16,174,0,6,1182,0,0,8,8,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,1,9,0,0,1,9,0,0,
1,9,0,0,1,9,0,0,3,9,0,0,
501,0,0,17,89,0,6,1255,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
528,0,0,18,126,0,6,1334,0,0,5,5,0,1,0,2,3,0,0,1,97,0,0,1,97,0,0,1,97,0,0,3,9,0,
0,
563,0,0,19,95,0,6,1412,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,3,9,0,0,
594,0,0,20,212,0,6,1509,0,0,7,7,0,1,0,2,3,0,0,1,9,0,0,3,9,0,0,3,3,0,0,1,9,0,0,
1,9,0,0,2,9,0,0,
637,0,0,21,124,0,6,1600,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,2,9,0,0,
668,0,0,22,93,0,6,1651,0,0,3,3,0,1,0,2,3,0,0,1,9,0,0,3,9,0,0,
695,0,0,23,89,0,6,1731,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,3,9,0,0,
726,0,0,24,103,0,6,1807,0,0,4,4,0,1,0,2,3,0,0,1,97,0,0,1,9,0,0,1,9,0,0,
757,0,0,25,103,0,6,1878,0,0,4,4,0,1,0,2,3,0,0,1,97,0,0,1,9,0,0,1,9,0,0,
788,0,0,26,86,0,6,1940,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
815,0,0,27,86,0,6,2012,0,0,3,3,0,1,0,2,3,0,0,1,97,0,0,2,9,0,0,
};


#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <param_ctx.h>
#include <hsm_inc.h>
/*
#include <sqlca.h>
#include <sqlcpr.h>
*/


int SetMAC1( int nIndiceCtx, char *resource_id, char *MAC, char *Check_value )
{

    struct sqlca sqlca;
    char sLine [ MAX_LINE_TRC];

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int		 		response;
        char             T_resource_id		[ 6 +  1];
        /* varchar          T_MAC		[256 +  1]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_MAC;
  /* TDES */
        /* VARCHAR          T_check_value		[256 +  1]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_check_value;

	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

        #ifdef DEBUGIN
		sprintf(sLine, "Start SetMAC1(%.6s, %.48s, %.16s)", resource_id, MAC, Check_value);
		trace_event(sLine, PROCESSING);
	#else
		sprintf(sLine, "Start set_MAC1(%.6s)", resource_id);
		trace_event(sLine, PROCESSING);
        #endif
	memset(T_MAC.arr,             0, sizeof(T_MAC.arr));
	T_MAC.len = 0;
	memset(T_resource_id,    0, sizeof(T_resource_id));
	memset(T_check_value.arr, 0, sizeof(T_check_value.arr));
	T_check_value.len = 0;

	memcpy(T_resource_id, resource_id, 6); 
	memcpy(T_MAC.arr,      MAC,   strlen(MAC)); 
	T_MAC.len = strlen(MAC);
	memcpy ( T_check_value.arr,  Check_value,  strlen (Check_value) ); 
	T_check_value.len = strlen(Check_value);

	response = 909;
	/* EXEC SQL WHENEVER SQLERROR GOTO set_awk_exception; */ 

	/* EXEC SQL EXECUTE
	BEGIN
		:response := PCRD_POS_MNG_KEY.SET_MAC1_KEY(	:T_resource_id,
						 							:T_MAC,
						 							:T_check_value
												);
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response := PCRD_POS_MNG_KEY . SET_MAC1_KEY ( :T_reso\
urce_id , :T_MAC , :T_check_value ) ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )726;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&response;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned long )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&T_MAC;
 sqlstm.sqhstl[2] = (unsigned long )259;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&T_check_value;
 sqlstm.sqhstl[3] = (unsigned long )259;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
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
 if (sqlca.sqlcode < 0) goto set_awk_exception;
}



	if (response == OK)
	{
		trace_event("End   SetMAC1(OK)", PROCESSING);
        return(OK);
	}

	trace_event("End   SetMAC1(NOK)", PROCESSING);
	return(NOK);

	set_awk_exception :
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

    sprintf(sLine, "%s",sqlca.sqlerrm.sqlerrmc);
    trace_event(sLine, ERROR);                                                                                                     
    trace_event("End  SetMAC1 (MALFUNCTION)", PROCESSING);                                                                           
    return(HSM_MALFUNCTION);

}

/*****************************************************************************/
/* SetMAC2                                                        	         */
/*----------------                                                           */
/*  Fonction : fonction qui fait la mise a jour d'une valeur MAC2.            */
/*  Entree   : L'identificateur du resource.                                */
/*  Sortie   : La cle MAC2.                                      	         */
/*****************************************************************************/

int SetMAC2( int nIndiceCtx, char *resource_id, char *MAC, char *Check_value )
{

    struct sqlca sqlca;
    char sLine [ MAX_LINE_TRC];

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int		 		response;
        char             T_resource_id		[ 6 +  1];
        /* varchar          T_MAC		[256 +  1]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_MAC;
  /* TDES */
        /* VARCHAR          T_check_value		[256 +  1]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_check_value;

	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

        #ifdef DEBUGIN
		sprintf(sLine, "Start SetMAC2(%.6s, %.48s, %.16s)", resource_id, MAC, Check_value);
		trace_event(sLine, PROCESSING);
	#else
		sprintf(sLine, "Start set_MAC2(%.6s)", resource_id);
		trace_event(sLine, PROCESSING);
        #endif
	memset(T_MAC.arr,             0, sizeof(T_MAC.arr));
	T_MAC.len = 0;
	memset(T_resource_id,    0, sizeof(T_resource_id));

	memset(T_check_value.arr, 0, sizeof(T_check_value.arr));
	T_check_value.len = 0;

	memcpy(T_resource_id, resource_id, 6); 
	memcpy(T_MAC.arr, MAC,   strlen(MAC)); 
	T_MAC.len = strlen(MAC);
	memcpy ( T_check_value.arr,  Check_value,  strlen (Check_value) ); 
	T_check_value.len = strlen(Check_value);

	response = 909;
	/* EXEC SQL WHENEVER SQLERROR GOTO set_awk_exception; */ 

	/* EXEC SQL EXECUTE
	BEGIN
		:response := PCRD_POS_MNG_KEY.SET_MAC2_KEY(	:T_resource_id,
						 							:T_MAC,
						 							:T_check_value
												);
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response := PCRD_POS_MNG_KEY . SET_MAC2_KEY ( :T_reso\
urce_id , :T_MAC , :T_check_value ) ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )757;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&response;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned long )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&T_MAC;
 sqlstm.sqhstl[2] = (unsigned long )259;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)&T_check_value;
 sqlstm.sqhstl[3] = (unsigned long )259;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
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
 if (sqlca.sqlcode < 0) goto set_awk_exception;
}



	if (response == OK)
	{
		trace_event("End   SetMAC2(OK)", PROCESSING);
        return(OK);
	}

	trace_event("End   SetMAC2(NOK)", PROCESSING);
	return(NOK);

	set_awk_exception :
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

    sprintf(sLine, "%s",sqlca.sqlerrm.sqlerrmc);
    trace_event(sLine, ERROR);                                                                                                     
    trace_event("End  SetMAC2 (MALFUNCTION)", PROCESSING);                                                                           
    return(HSM_MALFUNCTION);

}


/*****************************************************************************/
/* GetMAC1                                                        	         */
/*----------------                                                           */
/*  Fonction : fonction qui recupere la cle MAC1.                             */
/*  Entree   : L'identificateur du resource.                                */
/*  Sortie   : La cle MAc.                                      	         */
/*****************************************************************************/

int     GetMAC1 ( int nIndiceCtx , char *resource_id, char *MAC)
{
    struct sqlca sqlca;
    char sLine [ MAX_LINE_TRC];

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int		 response;
        char     T_resource_id		[ 6 + 1 ];
        /* VARCHAR     T_MAC    			[256 + 1 ]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_MAC;
 
	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

	sprintf(sLine, "Start GetMAC1 (%.6s)", resource_id);
	trace_event(sLine, PROCESSING);

	memset(T_MAC.arr    ,       0, sizeof(T_MAC.arr));
  T_MAC.len = 0;
  
	memset(T_resource_id,  0, sizeof(T_resource_id));

	memcpy(T_resource_id, resource_id, 6); 

	sprintf(sLine, "GET_MAC1_KEY (%.6s)", T_resource_id);
	trace_event(sLine, PROCESSING);
	
	response = 909;
	/* EXEC SQL WHENEVER SQLERROR GOTO mac_key_exception; */ 

	/* EXEC SQL EXECUTE
	BEGIN
		:response := PCRD_POS_MNG_KEY.GET_MAC1_KEY(:T_resource_id,
						 	:T_MAC
							);
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response := PCRD_POS_MNG_KEY . GET_MAC1_KEY ( :T_reso\
urce_id , :T_MAC ) ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )788;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&response;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned long )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&T_MAC;
 sqlstm.sqhstl[2] = (unsigned long )259;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
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
 if (sqlca.sqlcode < 0) goto mac_key_exception;
}



	sprintf(sLine, "response :  (%d)", response);
	trace_event(sLine, PROCESSING);

	if (response == OK)
	{
        memcpy(MAC, T_MAC.arr, T_MAC.len );
        #ifdef DEBUGIN
        	sprintf(sLine, "End   GetMAC1 (%.256s)", MAC);
        	trace_event(sLine, PROCESSING);
	#else
        	sprintf(sLine, "End   GetMAC1 (OK)");
        	trace_event(sLine, PROCESSING);        	
        #endif
        return(OK);
	}

	trace_event("End   GetMAC1 (NOK)", PROCESSING);
	return(NOK);

	mac_key_exception :
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

    sprintf(sLine, "%s",sqlca.sqlerrm.sqlerrmc);
    trace_event(sLine, ERROR);                                                                                                     
    trace_event("End  GetMAC1(MALFUNCTION)", PROCESSING);                                                                           
    return(HSM_MALFUNCTION);

}


/*****************************************************************************/
/* GetMAC2                                                        	         */
/*----------------                                                           */
/*  Fonction : fonction qui recupere la cle MAC2.                             */
/*  Entree   : L'identificateur du resource.                                */
/*  Sortie   : La cle MAC.                                      	         */
/*****************************************************************************/


int     GetMAC2 ( int nIndiceCtx , char *resource_id, char *MAC)
{
    struct sqlca sqlca;
    char sLine [ MAX_LINE_TRC];

	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int		 response;
        char     T_resource_id		[ 6 + 1 ];
        /* VARCHAR     T_MAC    			[256 + 1 ]; */ 
struct { unsigned short len; unsigned char arr[257]; } T_MAC;
 
	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL CONTEXT USE :tab_ParamCtx[nIndiceCtx].ctx; */ 

	sprintf(sLine, "Start GetMAC2 (%.6s)", resource_id);
	trace_event(sLine, PROCESSING);

	memset(T_MAC.arr    ,       0, sizeof(T_MAC.arr));
  T_MAC.len = 0;
  
	memset(T_resource_id,  0, sizeof(T_resource_id));

	memcpy(T_resource_id, resource_id, 6); 

	sprintf(sLine, "GET_MAC2_KEY (%.6s)", T_resource_id);
	trace_event(sLine, PROCESSING);
	
	response = 909;
	/* EXEC SQL WHENEVER SQLERROR GOTO mac_key_exception; */ 

	/* EXEC SQL EXECUTE
	BEGIN
		:response := PCRD_POS_MNG_KEY.GET_MAC2_KEY(:T_resource_id,
						 	:T_MAC
							);
	END;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :response := PCRD_POS_MNG_KEY . GET_MAC2_KEY ( :T_reso\
urce_id , :T_MAC ) ; END ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )815;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&response;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)T_resource_id;
 sqlstm.sqhstl[1] = (unsigned long )7;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)&T_MAC;
 sqlstm.sqhstl[2] = (unsigned long )259;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
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
 if (sqlca.sqlcode < 0) goto mac_key_exception;
}



	sprintf(sLine, "response :  (%d)", response);
	trace_event(sLine, PROCESSING);

	if (response == OK)
	{
        memcpy(MAC, T_MAC.arr, T_MAC.len );
        #ifdef DEBUGIN
        	sprintf(sLine, "End   GetMAC2 (%.256s)", MAC);
        	trace_event(sLine, PROCESSING);
	#else
        	sprintf(sLine, "End   GetMAC2 (OK)");
        	trace_event(sLine, PROCESSING);        	
        #endif
        return(OK);
	}

	trace_event("End   GetMAC2 (NOK)", PROCESSING);
	return(NOK);

	mac_key_exception :
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

    sprintf(sLine, "%s",sqlca.sqlerrm.sqlerrmc);
    trace_event(sLine, ERROR);                                                                                                     
    trace_event("End  GetMAC2 (MALFUNCTION)", PROCESSING);                                                                           
    return(HSM_MALFUNCTION);

}
/* END ADD AHA 02032009 */


/* Mehdi Elyajizi veification DAC ignoré fonctionnalité MasterCard et DAC non envoyé systématiquement */
/* MYJ231007 Mehdi Elyajizi Emv Control verification flag **/
