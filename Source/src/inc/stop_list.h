#ifndef HPS_STOP_LIST_H
#define HPS_STOP_LIST_H

typedef struct SStopList
{

  char card_number     		[22];
  char stop_list_event 		[ 3];
  char stop_list_add_response   [ 3];
  char stop_list_action		[ 3];
  char stop_list_date  		[ 8];

} TSStopList;

#endif /**  HPS_STOP_LIST_H **/
