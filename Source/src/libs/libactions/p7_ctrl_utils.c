/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      auth_utils.c                                                           */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Utilities for autorisation : Local autorisation/ Stand In Autoris...*/
/*         Check data integrity of TLV data                                    */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Add a call of PutEvent during the check of the integrity of the TLV  */
/*        data see (HYJCC120502)                                               */
/*******************************************************************************/

#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>
/*******************************************************************************/


