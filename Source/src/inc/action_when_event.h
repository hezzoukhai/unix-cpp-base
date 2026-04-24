#ifndef HPS_ACTION_WHEN_EVENT_H
#define HPS_ACTION_WHEN_EVENT_H


typedef struct SActionWhenEvent
{

 char     event_code		[ 3];
 char     code_action		[ 3];
 char     data_capture_mode	[ 1];

}TSActionWhenEvent;

#endif /** HPS_ACTION_WHEN_EVENT_H **/
