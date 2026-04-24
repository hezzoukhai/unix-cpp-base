#ifndef HPS_ISO3_HPS_H
#define HPS_ISO3_HPS_H

#include <iso8583.h>

void IniIsoMoneyTransDataTlv(TlvInfo* pkTlvInfo);
void IniCBDataTlv(TlvInfo* pkTlvInfo);
void IniTknDataTlv(TlvInfo* pkTlvInfo);
void IniSaleInfoDataTlv(TlvInfo* pkTlvInfo);
void IniSaleItemsDataTlv(TlvInfo* pkTlvInfo);

#endif /** HPS_ISO_HPS_H **/
