#ifndef HPS_SERVICE_H
#define HPS_SERVICE_H


typedef struct SService
{

 char     szMti			[ 4 + 1];
 char     szProcCode	[ 2 + 1];
 char     szFctCode		[ 3 + 1];
 char     szSrcAcc 		[ 2 + 1];
 char     szDestAcc 	[ 2 + 1];
 char     sZPaymType    [ 2  + 1]  ; /*ICH20190525*/

}TSService;

#endif /** HPS_SERVICE_H **/
