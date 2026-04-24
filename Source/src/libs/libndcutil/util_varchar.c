/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      util_varchar.c                                                         */
/*                                                                             */
/* Description                                                                 */
/*      Tools Management                                                       */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <define.h>


/******************************************************************************/
/* Fonction extract_varchar                                                   */
/* --------------------------                                                 */
/* Description : Cette fonction a pour but d'extraire la taille et valeur     */
/*               d'une variable de type VARCHAR                               */
/* Entree      : la chaine source contenant la longueur et valeur du VARCHAR  */
/* Sortie      : Taille                                                       */
/*               Valeur du VARCHAR                                            */
/* Date/Auteur : 31/10/96 M. MORTADY                                          */
/* Revision    :                                                              */
/******************************************************************************/

int extract_varchar(V_source, V_target)
char *V_source;
char *V_target;
{
   int  lg =0;
   char ch_lg[3];

   memset(ch_lg, 0, sizeof(ch_lg));
   memcpy(ch_lg, V_source, 2);
   lg =atoi(ch_lg);
   if (lg > 0) memcpy(V_target, V_source + 2, lg);
   return(lg);

}

/************************************************************************/
/************************************************************************/

void charge_label(char *V_target,char *V_label,int  V_lg)
{
   memset(V_target,       0, 50);
   memcpy(V_target, V_label, V_lg);
   return;
}


/************************************************************************/
/************************************************************************/
void format_to_varchar(source, destination)
char *source;
char *destination;
{
   int     size_string;
   char    *buffer;

   size_string = strlen(source);
   buffer      = (char *) malloc(size_string + 2);

   if (size_string == 0) memcpy(buffer, "00", 2);
   else
   {
      sprintf(buffer,    "%02d",  size_string);
      memcpy(buffer + 2,  source, size_string);
   }
   memcpy(destination, buffer, size_string + 2);

   free(buffer);

   return;
}


int size_without_blank(source, lg)
char *source;
int   lg;
{
   int i = 0;

   while( (*(source + i) != ' ') && (i < lg)) i++;
   return(i);

}
