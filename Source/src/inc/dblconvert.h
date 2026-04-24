  /***********************************************************************
  **  nom	: ndcconvert.h						**
  **  desc	:
  **  auteur	: Younes OUADI (14 Aut 1999)				**
  **			(c) 1999 Hightech Payment Systems		**
  ***********************************************************************/
#ifndef PWC_DBL_CONV__H
#define PWC_DBL_CONV__H

typedef struct TDblConvert
{
    int		fOpReq;
    char	fProcessingCode	[ 6];
    char	fFunctionCode	[ 3];
} SDblConvert;

typedef struct STransNameT
{
    int		fOpReq;
    char	*fAraName;
    char	*fEngName;
    char	*fFreName;
} STransName;

#define DBLCONVERT_SIZE         17

#endif

