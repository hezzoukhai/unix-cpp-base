  /***********************************************************************
  **  nom	: ndcconvert.h						**
  **  desc	:
  **  auteur	: Younes OUADI (14 Aut 1999)				**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef HPS_NDCCONVERT_H
#define HPS_NDCCONVERT_H

/*EBE170428: increase field's sizes +1*/
typedef struct TNdcConvert
{
    int		fOpReq;
    char	fProcessingCode	[ 6 + 1];
    char	fFunctionCode	[ 3 + 1];
} SNdcConvert;

typedef struct STransNameT{
    int		fOpReq;
    char	*fAraName;
    char	*fEngName;
    char	*fFreName;
} STransName;

#endif /**** HPS_NDCCONVERT_H *****/
