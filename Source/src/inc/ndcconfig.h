  /***********************************************************************
  **  nom	: ndcconfig.h						**
  **  desc	: gestion de la configuration des GAB NDC		**
  ** auteur	: Younes OUADI (05 Jan 1999)				**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_NDCCONFIG_H
#define HPS_NDCCONFIG_H

typedef struct ndc_config_t
{
    char	fTerminalNumber	[16];
    char	fAcqBank	    [6+1];
    char	fEventCode;
    int		fProcessId;
    char	fConfigId	[ 5];
    char	fLoadStateTableFlag;
    char	fLoadScreenFlag;
    char	fLoadConfigParamFlag;
    char	fLoadFitFlag;
    char	fLoadDateFlag;
    char	fLoadMasterKeyFlag;
    char	fLoadCommsKeyFlag;
    char	fLoadConfigIdFlag;
    char	fSendConfigIdFlag;
    char	fSendSupplyCountersFlag;
    char	fSendDateTimeFlag;
    char	fSendSuppliesDataFlag;
    char	fSendFitnessDataFlag;
    char	fCmdGoInServiceFlag;
    char	fCmdGoOutServiceFlag;
	char	fLoadCashDepositCounters; /* TBO26122010 'DP01', 'Deposit Device' for Wincor */
} ndc_config;

#define	EVN_POWER_UP			'1'
#define	EVN_SUPER_EXIT			'2'
#define	EVN_ATM_MNG_CMD			'3'


#endif  /**** HPS_NDCCONFIG_H ****/
