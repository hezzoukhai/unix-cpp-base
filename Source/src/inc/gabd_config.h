  /***********************************************************************
  **  nom	: gabdconfig.h						**
  **  desc	: gestion de la configuration des GAB GABD		**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_GABDCONFIG_H
#define HPS_GABDCONFIG_H

typedef struct gabd_config_t
{
    char	fTerminalNumber	[16];
    char    fAcqBank 		[6+1];/*ICH270715*/
    char	fLoadScreenFlag;
    char	fLoadConfigParamFlag;
    char	fLoadFitFlag;
    char	fLoadDateTimeFlag;
    char	fLoadCommsKeyFlag;
    char	fSendArretComptableFlag;
    char	fSendEvenementFlag;
    char	fSendJournalDataFlag;
    char	fCmdGoOutServiceFlag ;
} gabd_config;

#endif  /**** HPS_GABDCONFIG_H ****/
