#ifndef RM_STRUCT_EVENT_H
#define RM_STRUCT_EVENT_H
typedef struct evtEM_s
{
   char    ressource    [ 2];
   char    cd_action    [ 3];
}evtEM_t;

typedef struct evtIC_s
{
   char   cd_event      [ 4];
}evtIC_t;
#endif /* RM_STRUCT_EVENT_H */
