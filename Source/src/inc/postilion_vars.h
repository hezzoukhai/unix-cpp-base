/*******************************************************************************
**
**
**
**
*******************************************************************************/

#define         PROCESSED     1 /** TRX Already on AUTHO_ACTIVITY            **/
#define         NOT_PROCESSED 2 /** TRX Not on AUTHO_ACTIVITY                **/
#define         WAITING_UPD   3 /** TRX Should make Update of AUTHO_ACTIVITY **/

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

