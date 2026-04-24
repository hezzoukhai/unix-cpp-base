  /***********************************************************************
  **  nom	: vhsm_gen_zmk.c					**
  **  desc	: crefhion des paires zmk				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  **				(c) Hightech Payment Systems		**
  ***********************************************************************/
#include <stdio.h>

#include <define.h>
#include <hsm_inc.h>

#undef _FOR_INT_USE_ 
#undef _FOR_EXT_USE_ 
                     
#define _FOR_EXT_USE_
#include <libvhsm.h> 
#undef _FOR_EXT_USE_ 

  /***********************************************************************
  **  nom	: check_cmd_kq						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande KQ						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_cmd_kq( cmd, cmd_len , sModeFlag, sSchema_ID, sIMK_AC, sPAN, sPAN_SEQ, sATC , sUN, sData, sRecARQC, sARC )
unsigned char	cmd[];
int	cmd_len;
unsigned char	sModeFlag[];
unsigned char	sSchema_ID[];
unsigned char	sIMK_AC[];
unsigned char	sPAN[];
unsigned char	sPAN_SEQ[];
unsigned char	sATC[];
unsigned char	sUN[];
unsigned char	sData[];
unsigned char	sARC[];
unsigned char	sRecARQC[];
{
char        detail[256];
int             offset;
unsigned char	sDataLen[3];

trace_event("Start check_cmd_kq", PROCESSING);
memset (sDataLen, 0 , sizeof(sDataLen));
memset (detail, 0 , sizeof(detail));
/*
if(cmd_len != 18){
    trace_event("LANGUEUR INCORRECTE", DATA);
    trace_event("End   check_cmd_kq(NOK)", PROCESSING);
    return (NOK);
}
*/
/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;

memcpy(sModeFlag, cmd + offset,   1);
offset += 1;

memcpy(sSchema_ID, cmd + offset,   1);
offset += 1;

memcpy(sIMK_AC,        cmd + offset,   SIZE_KEY);
offset += SIZE_KEY;

memcpy(sPAN,        cmd + offset,   16); /** A rendre Paraméble */
offset += 16;

memcpy(sPAN_SEQ,        cmd + offset,   2);
offset += 2;

memcpy(sATC,        cmd + offset,   2); 
offset += 2;

memcpy(sUN,        cmd + offset,   4); 
offset += 4;


memcpy(sDataLen,        cmd + offset,   2); 
offset += 2;

memcpy(sData,        cmd + offset, cmd_len - offset - SIZE_KEY - 2 ); 
offset += 2;


memcpy(sRecARQC,   cmd + offset,   SIZE_KEY);
offset += SIZE_KEY;

memcpy(sARC,          cmd + offset,   2);
offset += 2;

sprintf(detail, "sIMK_AC %.16s, sSchema_ID %.16s, sPAN  %.16s, sPAN_SEQ  %.16s, sATC %.16s, sARC %.16s,  sUN %.16s, sDATA %.60s, sRecARQC %.16s", sIMK_AC, sSchema_ID, sPAN, sPAN_SEQ, sATC, sARC, sUN, sData, sRecARQC );
trace_event(detail, PROCESSING);

trace_event("End   check_cmd_kq(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_resp_kr						**
  **  desc	: reponse du VHSM a la commande KQ			**
  **  entree	: error_code	-> code erreur				**
  ***********************************************************************/
build_resp_kr(error_code, arpc_genetared, resp , resp_len)
unsigned char		error_code[];
unsigned char		resp[];
unsigned char		arpc_genetared[];
int		*resp_len;
{
trace_event("Start build_resp_kr()", PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "KR", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, arpc_genetared , 8);
*resp_len += SIZE_KEY;

trace_event("End   build_resp_kr(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: cmd_kq_processing					**
  **  desc	: traitement de la commande AS par VHSM			**
  ***********************************************************************/
cmd_kq_processing( error_code, sIMK_AC, sSchema_ID, sPAN, sPAN_SEQ, sATC , sARC, sUN, sDATA , sRecARQC, sARPC  )
unsigned char		error_code[];
unsigned char		sIMK_AC[];
unsigned char		sSchema_ID[];
unsigned char		sPAN[];
unsigned char		sPAN_SEQ[];
unsigned char		sATC[];
unsigned char		sARC[];
unsigned char		sUN[];
unsigned char		sDATA[];
unsigned char		sRecARQC[];
unsigned char		sARPC[];
{
char        detail[256];
unsigned char   sTMP1                              [4096];
unsigned char   sTMP2                              [4096];
unsigned char   sY                                   [17];
unsigned char   sZR                                  [17];
unsigned char   sZL                                  [17];
unsigned char   sCMK_AC                             [256]; /** Card Master AC Key derived from the IMK-AC **/
unsigned char   sSZL                                 [17];
unsigned char   sSZR                                 [17];
unsigned char   sSK_AC                              [256]; /** Session Key Derived from the CMK-AC **/
unsigned char   sDATA_PADDED                       [4096];
int j;
int i;
unsigned char   sARQC                               [256];




trace_event("Start cmd_kq_processing()", PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(detail, 	0, 	256);
memset(sTMP1, 	0, 	sizeof(sTMP1));
memset(sTMP2, 	0, 	sizeof(sTMP2));
memset(sY, 	0, 	sizeof(sY));
memset(sZR, 	0, 	sizeof(sZR));
memset(sZL, 	0, 	sizeof(sZL));
memset(sCMK_AC,	0, 	sizeof(sCMK_AC));
memset(sSZL, 	0, 	sizeof(sSZL));
memset(sSZR, 	0, 	sizeof(sSZR));
memset(sSK_AC, 	0, 	sizeof(sSK_AC));
memset(sDATA_PADDED, 	0, 	sizeof(sDATA_PADDED));
memset(sARQC, 	0, 	sizeof(sARQC));
memset(sARPC, 	0, 	sizeof(sARPC));

sprintf( detail, "sIMK_AC %.16s, sSchema_ID %.16s, sPAN  %.16s, sPAN_SEQ  %.16s,  sATC %.16s,  sARC %.16s,  sUN %.16s, sDATA %.60s, sRecARQC %.16s", sIMK_AC, sSchema_ID, sPAN, sPAN_SEQ, sATC, sARC, sUN, sDATA, sRecARQC );
trace_event(detail, PROCESSING);


/* ERROR CODE  */
	memcpy(error_code, NO_ERROR, 2);
	sprintf(sTMP1, "%022d", 0);
	memcpy(sTMP1+22-2-strlen(sPAN), sPAN, strlen(sPAN));
	memcpy(sTMP1+22-2, sPAN_SEQ, 2);
	memcpy(sY, sTMP1+6, 16);
	fct_3DES(1, sIMK_AC, sY, sZL);

	memset(sTMP2, 0, sizeof(sTMP2));
	fct_XOR(sY, "FFFFFFFFFFFFFFFF", sTMP2);
	fct_3DES(1, sIMK_AC, sTMP2, sZR);

	fct_Adjust_Parity ( sZL, sZL );
	fct_Adjust_Parity ( sZR, sZR );

        sprintf(sCMK_AC, "%.16s%.16s", sZL, sZR);
        if ( '1' == sSchema_ID[0] )
        {
           sprintf(sTMP1, "%016d", 0);
           memcpy(sTMP1+ 0, sATC, 4);
           memcpy(sTMP1+ 8, sUN, 8);

           memcpy(sTMP1+4, "F0", 2);
           fct_3DES(1, sCMK_AC, sTMP1, sSZL);

           memcpy(sTMP1+4, "0F", 2);
           fct_3DES(1, sCMK_AC, sTMP1, sSZR);

           fct_Adjust_Parity ( sSZL, sSZL );
           fct_Adjust_Parity ( sSZR, sSZR );
        }
        else
        {
           memcpy(sSZL, sZL, 16);
           memcpy(sSZR, sZR, 16);
        }
        sprintf(sSK_AC, "%.16s%.16s", sSZL, sSZR);


        memcpy(sDATA_PADDED, sDATA, strlen(sDATA));

/** Padding to Get a full number of block with 8 Bytes **/
        sprintf(sTMP1, "%018d", 0);
        if('1' == sSchema_ID[0])
           sprintf(sTMP1, "8%017d", 0);

        j=(strlen(sDATA_PADDED)+2) % 16;
        memcpy(sDATA_PADDED+strlen(sDATA_PADDED), sTMP1, 2+( 16-(j==0?16:j)) );

/** Computing the ARQC **/
        for (i=0; i< strlen(sDATA_PADDED); i+=16)
        {

           memcpy(sTMP1, sDATA_PADDED+i, strlen(sDATA_PADDED)-i<16?strlen(sDATA_PADDED)-i:16);

           if ( i )
              fct_XOR(sTMP1, sTMP2, sTMP1);

           fct_3DES(1, sSZL, sTMP1, sTMP2);

        }

        fct_3DES(-1, sSZR, sTMP2, sTMP1);

        fct_3DES(1, sSZL, sTMP1, sARQC);

        sprintf(sTMP1, "%.4s%012d", sARC, 0);

        fct_XOR(sARQC, sTMP1, sTMP2);

        fct_3DES(1, sCMK_AC, sTMP2, sARPC);

	if ( memcmp (sARQC,sRecARQC, 16 ) != 0 )
		memcpy(error_code, HSM_VERIFICATION_FAILED, 2);

	printf(detail, "End   process_kq_cmd(%.2s)", error_code);
	trace_event(detail, PROCESSING);

	trace_event("End   cmd_kq_processing(OK)", PROCESSING);
	return (OK);
}

  /***********************************************************************
  **  nom	: v_command_kq						**
  ***********************************************************************/
v_command_kq(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char		error_code  [       2];
unsigned char		sModeFlag     [2];
unsigned char		sSchema_ID     [2];
unsigned char		sIMK_AC   [33];
unsigned char		sPAN     [17];
unsigned char		sPAN_SEQ [3];
unsigned char		sATC [5];
unsigned char		sUN [9];
unsigned char		sDATA [1024];
unsigned char		sRecARQC [17];
unsigned char		sARPC [17];
unsigned char		sARC [5];

int		retour;

trace_event("Start v_command_kq()", PROCESSING);
memset ( sModeFlag 	, 0 , sizeof(sModeFlag));
memset ( sSchema_ID	, 0 , sizeof(sSchema_ID));
memset ( sIMK_AC  	, 0 , sizeof(sIMK_AC));
memset ( sPAN     	, 0 , sizeof(sIMK_AC));
memset ( sPAN_SEQ 	, 0 , sizeof(sPAN_SEQ));
memset ( sATC 		, 0 , sizeof(sATC));
memset ( sUN 		, 0 , sizeof(sUN));
memset ( sDATA 		, 0 , sizeof(sDATA));
memset ( sRecARQC 	, 0 , sizeof(sRecARQC));
memset ( sARC 		, 0 , sizeof(sARC));
memset ( sARPC 	, 0 , sizeof(sARPC));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_cmd_kq( cmd, cmd_len , sModeFlag, sSchema_ID, sIMK_AC, sPAN, sPAN_SEQ, sATC , sUN, sDATA, sRecARQC, sARC );
if(retour != OK){
    trace_event("ERROR DE SYNTAXE DE LA COMMANDE KQ", PROCESSING);
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  PROCESS COMMAND KQ */
    retour = cmd_kq_processing( error_code, sIMK_AC, sSchema_ID, sPAN, sPAN_SEQ, sATC , sARC, sUN, sDATA , sRecARQC, sARPC);
    if(retour != OK){
	trace_event("End   v_command_kq(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE KR  */
retour = build_resp_kr( error_code, sARPC, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_kq(NOK)", PROCESSING);
    return (NOK);
}

trace_event("End   v_command_kq(OK)", PROCESSING);
return (OK);
}

