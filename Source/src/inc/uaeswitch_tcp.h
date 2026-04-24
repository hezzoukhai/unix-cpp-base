/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef PWC_UAE_SWI_TCP__H
#define PWC_UAE_SWI_TCP__H


/* - PowerCARD Modules Public Functions Members ---------------------------- */
extern int  InitCltCom    (char *sServerName, char *sPort);
extern int  InitSrvComms  (int nPort);
extern int  AcceptCall    (char calling_address[]);
extern int  AcceptCallSrv (char calling_address[]);
extern void StopCltCom    (int nFd);
extern int  ReadLineMsg   (int nFd, char *sBuffer, int *nLength);
extern int  WriteLineMsg  (int nFd, char *sBuffer, int nLength);


/* - Constantes Definition Section ----------------------------------------- */
#define DISCONNECT          99


/* - Macros Definition Section --------------------------------------------- */


/* - Data Type definition Section ------------------------------------------ */


/* - Modules Public Functions Members --------------------------------------- */


/* - Modules Public Data Members -------------------------------------------- */
#endif



