#ifndef RM_PROTO_H
#define RM_PROTO_H

#include <gabd_state.h>
#define NB_ETAT_MAX 100

struct proto_s
{
   void (*fonc)();
   void (*Minifonc)();
};

struct proto_s tab_proto[NB_ETAT_MAX];
#endif
