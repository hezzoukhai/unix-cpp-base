#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include <thread_var.h>
#include <thr_hsm.h>
#include <queue.h>
#include <param_arg.h>


int GetNextCoNumber(int nIndex)
{
   tab_HsmCtx[nIndex].co_number = (tab_HsmCtx[nIndex].co_number + 1) % 100;
   return(tab_HsmCtx[nIndex].co_number);
}
