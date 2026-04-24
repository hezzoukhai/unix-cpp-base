#ifndef HPS_PRE_CONTROL_H
#define HPS_PRE_CONTROL_H
#include <define.h>
#include <virtual_record.h>
#include <iso_hps.h>

int GetRessourceService (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetCardRange        (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetRoutingEmva      (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetNetworkSwitch    (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetStopList         (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetCard             (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetRangeSwitch      (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetCardActivity     (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetCardProduct      (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetProductSwitch    (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetProductSwitch    (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetBalance          (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetShadowAccountActivity (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetClientActivity   (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetAcquirerCenter   (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetIssuerCenter     (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetAcquirerBank     (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetIssuerBank       (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetNetwork          (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetBankNetwork      (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetAcquirerOutlet   (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetExceptionFile    (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetPeriodicity      (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetScenario         (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int ReplaceRoutingCode  (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetLimitIndexException  ( int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetPeriodicityException ( int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetScenarioException    ( int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetActionWhenEvent  (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int GetRessourceActionTranslation  (int nIndiceCtx, TSIsoInfo *msgInfo, 
                                         TSVirtualRecord *vitual_record);
int CheckPreControlLevel           ( TSVirtualRecord *vitual_record);
int CheckPinRetry                  ( TSVirtualRecord *vitual_record);
int CheckExpiry                    ( TSVirtualRecord *vitual_record);
int CheckIfBankingAccountLevel     ( TSVirtualRecord *vitual_record);
int CheckIfShadowAccountLevel      ( TSVirtualRecord *vitual_record);
int CheckIfClientLevel             ( TSVirtualRecord *vitual_record);
int CheckServiceCode               ( TSVirtualRecord *vitual_record);
int CheckExceptionRouting          ( TSVirtualRecord *vitual_record);

#endif  /** HPS_PRE_CONTROL_H **/
