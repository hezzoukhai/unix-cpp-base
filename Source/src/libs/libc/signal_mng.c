#include <stdio.h>
#include <signal.h>
#include <define.h>


/******************************************************************************/
/* Fonction signal_blocking..                                                  */
/* ------------------------                                                   */
/* Description : Cette fonction a pour but de bloquer un signal pour le       */
/*               processus courant.                                           */
/* Entree      : Signal a bloquer.                                            */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK  sinon                                                    */
/* Date/Auteur : 12/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int signal_blocking(signal)
int signal;
{
   int      test_blocage;
#ifdef SOLARIS          
   sigset_t  mask;
#elif LINUX          
   sigset_t  *mask;/* Mehdi Elyajizi Linux */
#else
   struct sigset_t  mask;
#endif

   trace_event("Start signal_blocking()", PROCESSING);
#ifdef LINUX            
   sigemptyset(mask);/* Mehdi Elyajizi Linux */
   sigaddset(mask, signal);/* Mehdi Elyajizi Linux */
#else
   sigemptyset(&mask);
   sigaddset(&mask, signal);
#endif

#ifdef SOLARIS
   test_blocage = thr_sigsetmask(SIG_BLOCK, &mask, NULL);
#elif LINUX
   test_blocage = pthread_sigmask(SIG_BLOCK, mask, NULL);
#else
   test_blocage = sigthreadmask(SIG_BLOCK, &mask, NULL);
#endif
   if (test_blocage < 0)
   {
      trace_event("End   signal_blocking(NOK)", PROCESSING);
      return(NOK);
   }
   trace_event("End   signal_blocking(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction signal_unblocking.                                                */
/* ------------------------                                                   */
/* Description : Cette fonction a pour but de debloquer un signal pour le     */
/*               processus courant.                                           */
/* Entree      : Signal a bloquer.                                            */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK  sinon                                                    */
/* Date/Auteur : 12/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int signal_unblocking(signal)
int signal;
{
   int      test_deblocage;
   sigset_t mask;

   trace_event("Start signal_unblocking()", PROCESSING);
   sigemptyset(&mask);
   sigaddset(&mask, signal);
/**Linux*/
#ifdef SOLARIS
   test_deblocage = thr_sigsetmask(SIG_UNBLOCK, &mask, NULL);
#elif LINUX
   test_deblocage = pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
#else
   test_deblocage = sigthreadmask(SIG_UNBLOCK, &mask, NULL);
#endif
   if (test_deblocage < 0)
   {
      trace_event("End   signal_unblocking(NOK)", PROCESSING);
      return(NOK);
   }
   trace_event("End   signal_unblocking(OK)", PROCESSING);
   return(OK);
}


/******************************************************************************/
/* Fonction install_handler.                                                  */
/* ------------------------                                                   */
/* Description : Cette fonction a pour but de mettre en place un traitement   */
/*               particulier pour un signal particulier.                      */
/* Entree      : Signal a intercepter.                                        */
/*               Traitement particulier a mettre en place.                    */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK  sinon                                                    */
/* Date/Auteur : 12/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int      install_handler(int signal, void (*processing)(int))
{
   int       test_install_handler;
   struct    sigaction mask;


   trace_event("Start install_handler()", PROCESSING);

   mask.sa_handler = processing;
   sigemptyset(&mask.sa_mask);
   mask.sa_flags = SA_SIGINFO;

   test_install_handler = sigaction(signal, &mask, NULL);
   if (test_install_handler < 0)
   {
      trace_event("End   install_handler(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   install_handler(OK)", PROCESSING);

   return(OK);
}


/******************************************************************************/
/* Fonction desinstall_handler.                                               */
/* ----------------------------                                               */
/* Description : Cette fonction a pour but de desinstalle un traitement       */
/*               particulier pour un signal particulier.                      */
/* Entree      : Signal a traiter en mode defaut.                             */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK  sinon                                                    */
/* Date/Auteur : 12/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int desinstall_handler(signal)
int signal;
{
   int       test_desinstall_handler;
   struct    sigaction mask;

   trace_event("Start desinstall_handler()", PROCESSING);
   mask.sa_handler = SIG_DFL;
   sigemptyset(&mask.sa_mask);
   mask.sa_flags = 0;

   test_desinstall_handler = sigaction(signal, &mask, NULL);
   if (test_desinstall_handler < 0)
   {
      trace_event("End   desinstall_hanlder(NOK)", PROCESSING);
      return(NOK);
   }
   trace_event("End   desinstall_handler(OK)", PROCESSING);
   return(OK);
}



/******************************************************************************/
/* Fonction ingnore_signal.                                                   */
/* -----------------------                                                    */
/* Description : Cette fonction a pour but de faire ignore par le processus   */
/*               l'arrivee un signal.                                         */
/* Entree      : Signal a ignorer.                                            */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK  sinon                                                    */
/* Date/Auteur : 12/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int ignore_signal(signal)
int signal;
{
   int       test_ignore;
   struct    sigaction mask;

   trace_event("Start ignore_signal()", PROCESSING);
   mask.sa_handler = SIG_IGN;
   sigemptyset(&mask.sa_mask);
   mask.sa_flags = 0;

   test_ignore = sigaction(signal, &mask, NULL);
   if (test_ignore < 0)
   {
      trace_event("End   ignore_signal(NOK)", PROCESSING);
      return(NOK);
   }
   trace_event("End   ignore_signal(OK)", PROCESSING);
   return(OK);
}
/******************************************************************************/
unsigned long int p7_thread_id()
{
#ifdef LINUX
#include <sys/syscall.h>
	return syscall(SYS_gettid);
#else
	return pthread_self();
#endif
}




