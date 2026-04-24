#ifndef HPS_TLV_COM_H
#define HPS_TLV_COM_H

#define TLV_INFO_TAG_MAX_LEN	6

typedef struct tlv_info_s
{
   char    sTlvName[TLV_INFO_TAG_MAX_LEN + 1];
   int     nType;
   int     nLength;
}tlv_info_t;


#define   NOT_USED      -1

#endif  /* HPS_TLV_COM_H */
