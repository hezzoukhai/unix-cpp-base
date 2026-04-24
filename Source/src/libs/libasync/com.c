/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      com.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      Asynchrone Com Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include      <stdio.h>
#include      <fcntl.h>
#include      <termio.h>
#include      <signal.h>
#include      <define.h>
#include      <ascii.h>
#include      <errno.h>

/******************************************************************************/
/* Fonction open_device                                                       */
/* --------------------                                                       */
/* Description : Cette fonction a pour but d'ouvrir un port asynchrone sur la */
/*               machine UNIX.                                                */
/* Entree      : Identifiant du port a ouvrir.                                */
/* Sortie      : Identifiant interne du port.                                 */
/*               NOK en cas d'erreur.                                         */
/*               OK  sinon.                                                   */
/* Date/Auteur : 30/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int InitSrvComms_async(char *device_name, int *fdevice)
{
   trace_event("Start open_device()", PROCESSING);

   *fdevice = open(device_name, O_RDWR);

   if ( *fdevice < 0 )
   {
      trace_event("End   open_device(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   open_device(OK)", PROCESSING);
   return(OK);
}



/******************************************************************************/
/* Fonction close_device                                                      */
/* ---------------------                                                      */
/* Description : Cette fonction a pour but de fermer un port asynchrone sur   */
/*               la machine UNIX.                                             */
/* Entree      : Identifiant interne du port a fermer.                        */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK sinon.                                                    */
/* Date/Auteur : 30/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int close_device(int fdevice)
{
   int retour;

   trace_event("Start close_device()", PROCESSING);

   retour = close(fdevice);
   if (retour < 0)
   {
      trace_event("End   close_device(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   close_device(OK)", PROCESSING);
   return(OK);
}



/******************************************************************************/
/* Fonction init_parametre                                                    */
/* -----------------------                                                    */
/* Description : Cette fonction a pour but de forcer les parametres de        */
/*               l'application sur le port asynchrone.                        */
/* Entree      : Identifiant interne du port a fermer.                        */
/*               Liste des parametres a mettre en place.                      */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK sinon.                                                    */
/* Date/Auteur : 30/08/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int set_async_param(int fdevice, char baud_rate, char stop_bit, char data_bit, char parity, char flow_control, char char_set)
{
   int test;
   struct termio arg;

   trace_event("Start init_parametre()", PROCESSING);


   test = ioctl( fdevice, TCGETA, &arg );

   if (test < 0)
   {
      trace_event("End   init_parametre(NOK)", PROCESSING);
      return(NOK);
   }

   arg.c_cflag = B9600+CS8+CREAD+CLOCAL;
   arg.c_lflag     = 0;
   arg.c_cc[VTIME] = 0;
   arg.c_cc[VMIN]  = 1;

   test = ioctl( fdevice, TCSETA, &arg );
   if (test < 0)
   {
      trace_event("End   init_parametre(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   init_parametre(OK)", PROCESSING);
   return(test);
}
/******************************************************************************/
/* Fonction ReadLineHsm                                                       */
/* -----------------------                                                    */
/* Description : Cette fonction a pour but d'ecrire un buffer sur un port     */
/*               asynchrone.                                                  */
/* Entree      : Identifiant interne du port sur lequel il faut lire.         */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               Buffer lu ainsi que sa longueur.                             */
/* Revision    :                                                              */
/******************************************************************************/
int ReadLineHsm( int nFdConn, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nLenMsg = 0;
   int   retour;
   char  car=0x00;

   sprintf(sLine,"Start ReadLineHsm( nFdConn = %d )\n", nFdConn);
   trace_event(sLine, PROCESSING);

   memset(sBuffRcv, 0, sizeof(sBuffRcv));

   while(car != ETX)
   {
      retour = read(nFdConn, &car, 1);
      if (retour == 1)
      {
         *(sBuffRcv + nLenMsg ) = car;
         nLenMsg++;
      }
      else 
      {
         sprintf(sLine,"Error Read Msg To HSM : %d \n", errno);
         trace_event (sLine, ERROR);
         trace_event("End   ReadLineHsm(NOK)", PROCESSING);
         return(NOK);
      }
   }

   dump_buffer(sBuffRcv, nLenMsg, 'A', 'L', 'I');
   memcpy( sBuffer, sBuffRcv + 1 +4, nLenMsg - 2-4);
   *nLength = nLenMsg - 2-4;
   trace_event("End   ReadLineHsm(OK)", PROCESSING);
   return (OK);
}

/******************************************************************************/
/* Fonction WriteLineHsm                                                      */
/* --------------------                                                       */
/* Description : Cette fonction a pour but d'ecrire un buffer sur un port     */
/*               asynchrone.                                                  */
/* Entree      : Identifiant interne du port sur lequel il faut envoye.       */
/*               Le buffer a transmettre.                                     */
/*               La longueur du buffer a transmettre.                         */
/* Sortie      : NOK en cas d'erreur.                                         */
/*               OK sinon.                                                    */
/******************************************************************************/
int WriteLineHsm( int nFdConn, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char  sLine   [ MAX_LINE_TRC ];
   int   retour;

   sprintf(sLine,"Start WriteLineHsm( nFdConn = %d )\n", nFdConn);
   trace_event(sLine, PROCESSING);
   memset (sBuffSnd, 0, LG_MAX);

   sBuffSnd[0] = STX;
   memcpy (sBuffSnd +1 , "HPS7", 4);
   memcpy (sBuffSnd +1 +4, sBuffer, nLength);
   sBuffSnd[nLength+4+1] = ETX;
   dump_buffer(sBuffSnd, nLength+2+4, 'A', 'L', 'O');
   retour = write ( nFdConn, sBuffSnd, nLength+2+4+1);
   if (retour <= 0)
   {
      sprintf(sLine,"Error Sending Msg To HSM : %d \n", errno);
      trace_event (sLine, ERROR);
      return(NOK);
   }
   memset(sBuffSnd, 0, sizeof(sBuffSnd));
   trace_event("End   WriteLineHsm(OK)", PROCESSING);
   return(OK);
}
