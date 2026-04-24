  /***********************************************************************
  **  nom	: dblconfig.h						**
  **  desc	: gestion de la configuration des GAB DIE BOLD
  ** auteur	: Younes OUADI (05 Jan 1999)				**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_DBLCONFIG_H
#define HPS_DBLCONFIG_H

typedef struct dbl_config_t
{
    char    fTerminalNumber [16];
    char    fAcqBank [6+1];/*ICH270715*/
    char    fEventCode;
    int     fProcessId;
    char    fConfigId   [ 5];
    char    fLoadStateTableFlag;
    char    fLoadScreenFlag;
    char    fLoadConfigParamFlag;
    char    fLoadFitFlag;
    char    fLoadDateFlag;
    char    fLoadFormatTamplateFlag;
    char    fLoadMasterKeyFlag;
    char    fLoadCommsKeyFlag;
    char    fLoadConfigIdFlag;
    char    fCmdGoInServiceFlag;
    char    fCmdGoOutServiceFlag;
    char    fCmdCloseVandalAllowFlag;
    char    fCmdOpenVandalDisallowFlag;
    char    fCmdRunSelfTestFlag;
    char    fCmdCondGoOutServiceFlag;
    char    fSendConfigInfoFlag;
    char    fSendSupplyCountersFlag;
    char    fSendClearSupplyCtrFlag;
    char    fSendEnhSupplyCtrFlag;
    char    fSendClearEnhSupplyCtrFlag;
} dbl_config;

#define	EVN_POWER_UP			'1'
#define	EVN_SUPER_EXIT			'2'
#define	EVN_ATM_MNG_CMD			'3'


#endif  /**** HPS_DBLCONFIG_H ****/
