/******************************************************************************/
/* (c) Hightech Payment Systems 2000                                          */
/*     Package de gestion de date V 2.00                                      */
/*                                                                            */
/*     current_date_gmt()                                                     */
/*     current_date_ddmmyy()                                                  */
/*     current_date()                                                         */
/*     current_date_mmdd()                                                    */
/*     current_date_yyyymmdd()                                                */
/*     current_date_yymmdd()                                                  */
/*     current_system_date()                                                  */
/*     current_hour()                                                         */
/*     char_to_date()                                                         */
/*     annee_bisextile()                                                      */
/*     ajouter_jour1()                                                        */
/*     ajouter_jour()                                                         */
/*     ajouter_mois1()                                                        */
/*     ajouter_mois()                                                         */
/*     ajouter_annee1()                                                       */
/*     ajouter_annee()                                                        */
/*     current_julian_date()                                                  */
/*     convert_to_julian_date()                                               */
/*     long_difference_date()                                                 */
/*     convert_julian_date_qjjj()                                             */
/*     current_day_of_week()                                                  */
/******************************************************************************/
#include <stdio.h>
#include <define.h>
#include <sys/time.h>
#include <time.h>
#include <purge_time.h>

static int l_month[] = {
   31,  /* Jan */
   28,  /* Fev */
   31,  /* Mar */
   30,  /* Avr */
   31,  /* Mai */
   30,  /* Jun */
   31,  /* Jui */
   31,  /* Aou */
   30,  /* Sep */
   31,  /* Oct */
   30,  /* Nov */
   31   /* Dec */
};

static char *t_month[] = {
   "",        /* Rien ! */
   "/01/",   /*  1 */
   "/02/",   /*  2 */
   "/03/",   /*  3 */
   "/04/",   /*  4 */
   "/05/",   /*  5 */
   "/06/",   /*  6 */
   "/07/",   /*  7 */
   "/08/",   /*  8 */
   "/09/",   /*  9 */
   "/10/",   /* 10 */
   "/11/",   /* 11 */
   "/12/"    /* 12 */
};

typedef struct date_t 
{
   char   jj[2];
   char    mm[4];
   char      aa[4];
} t_date;   /*  11 caracteres   */


/******************************************************************************/
/* Fonction qui charge dans le buffer transmis la date courante au format     */
/* MMDDHHMMSS sur 10 positions,                                               */
/******************************************************************************/
int current_gmt_date(date_heure_jour)
char date_heure_jour[12];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   char hh_c[3];
   char mi_c[3];
   char ss_c[3];

   if (date_heure_jour == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(date_heure_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = gmtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   memset(date_heure_jour, 0, 12);

   sprintf(aa_c,"%04d",date_r.tm_year+1900);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(jj_c,"%02d",date_r.tm_mday);

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mi_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   memcpy(date_heure_jour +  0, aa_c+2, 2);
   memcpy(date_heure_jour +  2, mm_c, 2);
   memcpy(date_heure_jour +  4, jj_c, 2);
   memcpy(date_heure_jour +  6, hh_c, 2);
   memcpy(date_heure_jour +  8, mi_c, 2);
   memcpy(date_heure_jour + 10, ss_c, 2);

   return(0);
}

int current_date_yyyymmddhhmmss(date_heure_jour)
char date_heure_jour[14];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   char hh_c[3];
   char mi_c[3];
   char ss_c[3];

   if (date_heure_jour == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(date_heure_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = gmtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   memset(date_heure_jour, 0, 12);

   sprintf(aa_c,"%04d",date_r.tm_year+1900);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(jj_c,"%02d",date_r.tm_mday);

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mi_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   memcpy(date_heure_jour +  0, aa_c, 4);
   memcpy(date_heure_jour +  4, mm_c, 2);
   memcpy(date_heure_jour +  6, jj_c, 2);
   memcpy(date_heure_jour +  8, hh_c, 2);
   memcpy(date_heure_jour +  10, mi_c, 2);
   memcpy(date_heure_jour + 12, ss_c, 2);

   return(0);
}
/******************************************************************************/
/* Fonction qui charge dans le buffer transmis la date courante au format     */
/* DDMMYY sur 6 positions                                                     */
/******************************************************************************/
int current_date_ddmmyy(date_du_jour)
char date_du_jour[8];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char   jj_c[3];
   char   mm_c[3];
   char   aa_c[5];

   if (date_du_jour == (char *)NULL)
   {
      trace_event("DAT001IM", ERROR);
      memset(date_du_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);

   sprintf(jj_c,"%02d",date_r.tm_mday);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(aa_c,"%04d",date_r.tm_year + 1900);

   memcpy(date_du_jour  ,jj_c,2);
   memcpy(date_du_jour+2,mm_c,2);
   memcpy(date_du_jour+4,aa_c + 2,2);

   return(0);
}

/******************************************************************************/
/* Fonction qui charge dans le buffer transmis la date courante au format     */
/* MMDDYYYY sur 6 positions                                                    */
/******************************************************************************/
int current_date(char date_du_jour[])
{

   time_t d1;
   struct tm *tm_date;
   struct tm tm_date_r;
  /*SKO290509*/ /* char   rien[100]; */
   char   jj_c[3];
   char   mm_c[3];
   char   aa_c[5];
   char   date_tampon[27];

   if (date_du_jour == (char *)NULL)
   {
      memset(date_du_jour,'0',6);
      return(-1);
   }
   time(&d1);
   tm_date = localtime_r(&d1, &tm_date_r);

   sprintf(jj_c,"%02d",tm_date_r.tm_mday);
   sprintf(mm_c,"%02d",tm_date_r.tm_mon+1);

   memcpy(date_tampon, asctime(&tm_date_r), 26); /*PLUTONL-4202- asctime from tm_date to &tm_date_r*/
   memcpy(aa_c, date_tampon+20, 4);
   memcpy(date_du_jour  , jj_c, 2);
   memcpy(date_du_jour+2, mm_c, 2);
   memcpy(date_du_jour+4, aa_c, 4);

   return(0);
}

/******************************************************************************/
/******************************************************************************/
/* Fonction current_date_mmdd.                                                */
/* --------------------------                                                 */
/* Description : Fonction qui charge dans le buffer transmis la date courante */
/*               au format MMDD sur 4 position                    */
/* Entree      : La position actuelle du pointeur sur le buffer d'envoi       */
/* Sortie      :                                                              */
/* Date/Auteur : 11/09/96 Y. KAOUTAR                                          */
/* Revision    :                                                              */
/******************************************************************************/

int current_date_mmdd(date_du_jour)
char date_du_jour[5];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char   jj_c[3];
   char   mm_c[3];

   if (date_du_jour == (char *)NULL)
   {
      trace_event("DAT000IM", ERROR);
      memset(date_du_jour,'0',4);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);

   sprintf(jj_c,"%02d",date_r.tm_mday);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);

   memcpy(date_du_jour  ,mm_c,2);
   memcpy(date_du_jour+2,jj_c,2);
   date_du_jour[4] = '\0';

   return(0);
}


/******************************************************************************/
/* Fonction qui charge dans le buffer transmis la date courante au format     */
/* YYYYMMDD sur 6 positions                                                    */
/******************************************************************************/
int current_date_yyyymmdd(date_du_jour)
char date_du_jour[10];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char   jj_c[3];
   char   mm_c[3];
   char   aa_c[5];
   char   date_tampon[27];

   if (date_du_jour == (char *)NULL)
   {
      trace_event("DAT001IM", ERROR);
      memset(date_du_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);
   sprintf(jj_c,"%02d",date_r.tm_mday);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);

   memcpy(date_tampon, asctime(date), 26);
   memcpy(aa_c,date_tampon+20,4);

   memcpy(date_du_jour  ,aa_c,4);
   memcpy(date_du_jour+4,mm_c,2);
   memcpy(date_du_jour+6,jj_c,2);

   return(0);
}

/******************************************************************************/
/* Fonction qui charge dans le buffer transmis la date courante au format     */
/* YYMMDD sur 6 positions                                                     */
/******************************************************************************/
int current_date_yymmdd(date_du_jour)
char date_du_jour[8];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char   jj_c[3];
   char   mm_c[3];
   char   aa_c[5];

   if (date_du_jour == (char *)NULL)
   {
      trace_event("DAT001IM", ERROR);
      memset(date_du_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);

   sprintf(jj_c,"%02d",date_r.tm_mday);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(aa_c,"%04d",date_r.tm_year + 1900);

   memcpy(date_du_jour  ,aa_c + 2 ,2);
   memcpy(date_du_jour+2,mm_c,2);
   memcpy(date_du_jour+4,jj_c,2);

   return(0);
}

/******************************************************************************/
/* Fonction qui charge dans le buffer transmis l'heure courante au format     */
/* HHMMSS sur 6 positions.                                                    */
/******************************************************************************/
int current_system_date(date_heure_jour)
char date_heure_jour[17];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   char hh_c[3];
   char mi_c[3];
   char ss_c[3];

   if (date_heure_jour == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(date_heure_jour,'0',6);
      return(-1);
   }

   time(&lg);

   date = localtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   memset(date_heure_jour, 0, 17);

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mi_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   sprintf(jj_c,"%02d",date_r.tm_mday);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(aa_c,"%04d",date_r.tm_year + 1900);

   memcpy(date_heure_jour +  0, jj_c, 2);
   memcpy(date_heure_jour +  2, "/",  1);
   memcpy(date_heure_jour +  3, mm_c, 2);
   memcpy(date_heure_jour +  5, "/",  1);
   memcpy(date_heure_jour +  6, aa_c + 2, 2);
   memcpy(date_heure_jour +  8, " ",  1);
   memcpy(date_heure_jour +  9, hh_c, 2);
   memcpy(date_heure_jour + 11, ":",  1);
   memcpy(date_heure_jour + 12, mi_c, 2);
   memcpy(date_heure_jour + 14, ":",  1);
   memcpy(date_heure_jour + 15, ss_c, 2);

   return(0);
}

/******************************************************************************/
/* Fonction qui charge dans le buffer transmis l'heure courante au format     */
/* HHMMSS sur 6 positions.                                                    */
/******************************************************************************/
int current_hour(heure_du_jour)
char heure_du_jour[6];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char hh_c[3];
   char mm_c[3];
   char ss_c[3];

   if (heure_du_jour == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(heure_du_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mm_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   memcpy(heure_du_jour,  hh_c,2);
   memcpy(heure_du_jour+2,mm_c,2);
   memcpy(heure_du_jour+4,ss_c,2);
   return(0);
}


/******************************************************************************/
/* Fonction qui charge dans le buffer transmis l'heure courante au format     */
/* HHMM sur 4 positions.                                                      */
/******************************************************************************/
int current_hour_hhmm(heure_du_jour)
char heure_du_jour[5];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;
   char hh_c[3];
   char mm_c[3];

   if (heure_du_jour == (char *)NULL)
   {
      trace_event("current_hour_hhmm()", ERROR);
      memset(heure_du_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mm_c,"%02d",date_r.tm_min + 1);

   memcpy(heure_du_jour,  hh_c,2);
   memcpy(heure_du_jour+2,mm_c,2);
   heure_du_jour[4] = '\0';

   return(0);
}

/******************************************************************************/
/* Fonction qui transforme la date numerique en date alphanumerique.          */
/* Entree DDMMAAAA sur 8 positions                                            */
/* Sortie DDMMMAAAA sur 8 positions                                           */
/******************************************************************************/
int  char_to_date(date,date_cible)
char date[8];
char date_cible[10];
{
   char    mois[3];
   int     int_mois;

   memset(mois,0,3);
   memcpy(((t_date *)date_cible)->jj,date,2);
   strncpy(mois,date+2,2);

   if ((!isdigit(mois[0])) || (!isdigit(mois[1])))
   {
      trace_event("DAT004IM", ERROR);
      /*EBE140523 memset(date_cible,'0',sizeof(date_cible));*/
	  memset(date_cible,'0',8);
      return(-1);
   }

   int_mois = atoi(mois);
   if ((int_mois <=0) || (int_mois > 12))
   {
      trace_event("DAT005IM", ERROR);
      memset(date_cible,'0',10);
      return(-1);
   }

   memcpy(((t_date *)date_cible)->mm,t_month[int_mois],4);
   memcpy(((t_date *)date_cible)->aa,date+4,4);

   return(0);
}

/*****************************************************************************/
/* Test si l'annee est bissextile ou pas                                     */
/*****************************************************************************/
int annee_bissextile(annee)
int annee;
{
   if ((annee % 400) == 0) return(1);
   if ((annee % 100) == 0) return(0);
   if ((annee %   4) == 0) return(1);
   else return(0);
}


/******************************************************************************/
/* Ajout d'un jour a une date au format DDMMYY                                */
/******************************************************************************/
int ajouter_jour1(date)
char date[6];
{
   char jj_c[3];
   char mm_c[3];
   char aa_c[3];
   int  jj_d, mm_d, aa_d;

   memset(jj_c,0,sizeof(jj_c));
   memset(mm_c,0,sizeof(mm_c));
   memset(aa_c,0,sizeof(aa_c));

   memcpy(jj_c,date,2);
   memcpy(mm_c,date+2,2);
   memcpy(aa_c,date+4,2);

   jj_d = atoi(jj_c);
   mm_d = atoi(mm_c);
   aa_d = atoi(aa_c);


   if (mm_d != 2)
   {
      if (jj_d < l_month[mm_d-1]) jj_d++;
      else
      {
         jj_d = 1;
         ajouter_mois1(date);
      }
   }
   else
   {
      if (jj_d < l_month[mm_d-1] + annee_bissextile(aa_d)) jj_d++;
      else
      {
         jj_d = 1;
         ajouter_mois1(date);
      }
   }

   sprintf(jj_c,"%02d",jj_d);
   memcpy(date,jj_c,2);

   return(0);
}


/******************************************************************************/
/* Ajout d'un jour a une date au format DDMMYYYY                              */
/******************************************************************************/
int ajouter_jour(date)
char date[8];
{
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   int  jj_d, mm_d, aa_d;

   memset(jj_c,0,sizeof(jj_c));
   memset(mm_c,0,sizeof(mm_c));
   memset(aa_c,0,sizeof(aa_c));

   memcpy(jj_c,date,2);
   memcpy(mm_c,date+2,2);
   memcpy(aa_c,date+4,4);

   jj_d = atoi(jj_c);
   mm_d = atoi(mm_c);
   aa_d = atoi(aa_c);


   if (mm_d != 2)
   {
      if (jj_d < l_month[mm_d-1]) jj_d++;
      else
      {
         jj_d = 1;
         ajouter_mois(date);
      }
   }
   else
   {
      if (jj_d < l_month[mm_d-1] + annee_bissextile(aa_d)) jj_d++;
      else
      {
         jj_d = 1;
         ajouter_mois(date);
      }
   }

   sprintf(jj_c,"%02d",jj_d);
   memcpy(date,jj_c,2);

   return(0);
}


/******************************************************************************/
/* Ajout d'un mois a une date au format DDMMYY                                */
/******************************************************************************/
int ajouter_mois1(date)
char date[6];
{
   char mm_c[3];
   int  mm_d;

   memset(mm_c,0,sizeof(mm_c));

   memcpy(mm_c,date+2,2);

   mm_d = atoi(mm_c);

   if (mm_d < 12) mm_d++;
   else
   {
      mm_d = 1;
      ajouter_annee1(date);
   }

   sprintf(mm_c,"%02d",mm_d);
   memcpy(date+2,mm_c,2);
   return(0);
}

/******************************************************************************/
/* Ajout d'un mois a une date au format DDMMYYYY                              */
/******************************************************************************/
int ajouter_mois(date)
char date[8];
{
   char mm_c[3];
   int  mm_d;

   memset(mm_c,0,sizeof(mm_c));

   memcpy(mm_c,date+2,2);

   mm_d = atoi(mm_c);

   if (mm_d < 12) mm_d++;
   else
   {
      mm_d = 1;
      ajouter_annee(date);
   }

   sprintf(mm_c,"%02d",mm_d);
   memcpy(date+2,mm_c,2);
   return(0);
}


/******************************************************************************/
/* Ajout d'un an a une date au format DDMMYY                                  */
/******************************************************************************/
int ajouter_annee1(date)
char date[6];
{
   char aa_c[5];
   int  aa_d;

   memset(aa_c,0,sizeof(aa_c));

   memcpy(aa_c,date+4,2);
   aa_d = atoi(aa_c);
   aa_d++;
   sprintf(aa_c,"%02d",aa_d);
   memcpy(date+4,aa_c,2);
   return(0);
}

/******************************************************************************/
/* Ajout d'un an a une date au format DDMMYYYY                                */
/******************************************************************************/
int ajouter_annee(date)
char date[9];
{
   char aa_c[5];
   int  aa_d;

   memset(aa_c,0,sizeof(aa_c));

   memcpy(aa_c,date+4,4);
   aa_d = atoi(aa_c);
   aa_d++;
   sprintf(aa_c,"%04d",aa_d);
   memcpy(date+4,aa_c,4);
   return(0);
}

/******************************************************************************/
/* Charge la date courante au format julian date.                             */
/******************************************************************************/
int current_julian_date(date_du_jour)
char date_du_jour[3];
{
   time_t lg;
   struct tm *date;
   struct tm date_r;

   time(&lg);
   date = localtime_r(&lg, &date_r);
   sprintf(date_du_jour,"%03d",date_r.tm_yday+1);
   return(0);
}


/******************************************************************************/
/* Convertie la date donnee en parametre au format DDMMYYYY en julian date    */
/******************************************************************************/
int convert_to_julian(date,julian_date)
char date[8];
char julian_date[3];
{

   char julian_date_tampon[4];
   int  aa_d;
   char aa_c[5];

   int  mm_d;
   char mm_c[3];

   int  jj_d;
   char jj_c[3];

   int  julian=0;
   int  i;

   memset(aa_c,0,sizeof(aa_c));
   memcpy(aa_c,date+4,4);
   aa_d = atoi(aa_c);

   memset(mm_c,0,sizeof(mm_c));
   memcpy(mm_c,date+2,2);
   mm_d = atoi(mm_c);

   memset(jj_c,0,sizeof(jj_c));
   memcpy(jj_c,date,2);
   jj_d = atoi(jj_c);

   julian = jj_d;

   for(i=0;i<mm_d-1;i++) julian += l_month[i];

   julian += annee_bissextile(aa_d);

   sprintf(julian_date_tampon,"%03d",julian);
   memcpy(julian_date,julian_date_tampon,3);
   return(julian);
}


/*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
/******************************************************************************/
/* Convert julian date YDDD to GMT date YYYYMMDD                              */
/******************************************************************************/
void julian_to_gmt(char *julian_date,char *gmt_date)
{
    
   time_t lg;
   struct tm *date;
   struct tm date_r;
   int i;
   int mm_d;
   int jj_d;
   int  aa_d;
   char aa_c[5];
   char julian_tmp[4];
   
   memset(aa_c , 0 , sizeof(aa_c));
   memset(julian_tmp ,0 , sizeof(julian_tmp));
   time(&lg);
   date = gmtime_r(&lg, &date_r);
   sprintf(aa_c,"%04d",date_r.tm_year+1900);
   aa_c[3] = julian_date[0];
   aa_d = atoi(aa_c);
   memcpy(julian_tmp,julian_date+1,3);
   
   jj_d = atoi(julian_tmp);
    if (annee_bissextile(aa_d)) 
        l_month[1] = 29;
    for (i = 0; i < 12; i++) 
    {
        if (jj_d <= l_month[i]) 
        {
            mm_d = i + 1;
            break;
        }
        jj_d -= l_month[i];
    }
    
   snprintf(gmt_date, 9, "%04d%02d%02d", aa_d, mm_d, jj_d);
}
/*End PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

long difference_date(date1, date2)
char *date1;
char *date2;
{
   return(atol(date1) - atol(date2));
}

int len(i)
int i;
{
   int j=1;
   while((i/=10) > 0) ++j;
   return (j);
}

current_julian_date_qjjj(date_du_jour)
char date_du_jour[4];
{
   time_t lg;
   char aa_c[5];
   char dd_c[4];
   struct tm *date;
   struct tm date_r;

   time(&lg);
   date = localtime_r(&lg, &date_r);

   sprintf(aa_c        ,  "%04d",date_r.tm_year);
   sprintf(dd_c          ,"%03d",date_r.tm_yday+1);
   memcpy(date_du_jour+0, aa_c + 3, 1);
   memcpy(date_du_jour+1, dd_c    , 3);
   return(0);
}

current_day_of_week(date_du_jour)
char date_du_jour[1];
{
   time_t lg;
   char dd_c[2];
   struct tm *date;
   struct tm date_r;

   time(&lg);
   date = localtime_r(&lg, &date_r);

   sprintf(dd_c          ,"%01d",date_r.tm_wday);
   memcpy(date_du_jour, dd_c    , 1);
   return(0);
}

/*NAB2508 PLUTONL-3482*/

void get_local_time(struct timeval* stime)
{
 	struct tm *date;
	struct tm  tm_date_r;
	struct tm  gtime;
	time_t t ;
	int timezonez_diff ,timezonez_diff_h ,timezonez_diff_m ;
	

	gettimeofday(stime,NULL);/*to get time in micro seconds*/

	date 	= localtime_r(&(stime->tv_sec), &tm_date_r);/*get local time */
	date 	= gmtime_r(&(stime->tv_sec), &gtime);
	
	timezonez_diff_h = tm_date_r.tm_hour - gtime.tm_hour;
	timezonez_diff_m = tm_date_r.tm_min -  gtime.tm_min;

	if (tm_date_r.tm_mday == gtime.tm_mday )
 
	{
		timezonez_diff = (timezonez_diff_h*3600)+(timezonez_diff_m*60);
	}
	else if (tm_date_r.tm_mday > gtime.tm_mday )
	{
		timezonez_diff = (timezonez_diff_h*3600)+(timezonez_diff_m*60)+(24*3600);
	}
	else if(tm_date_r.tm_mday < gtime.tm_mday )
	{
		timezonez_diff = (timezonez_diff_h*3600)+(timezonez_diff_m*60)-(24*3600);
	}
	
	t = stime->tv_sec + timezonez_diff ;

	/*t = stime->tv_sec + tm_date_r.tm_gmtoff ;*/
	
	stime->tv_sec = t;

}
/*NAB2508 PLUTONL-3482*/

void get_timer(app_time)
int  *app_time;
{
   struct timeval  V_time;

   gettimeofday(&V_time, NULL);
   *app_time = V_time.tv_sec;

   return;
}

current_date_yymmddhhmmss(date_heure_jour)
char date_heure_jour[12];
{
   long lg;
   struct tm *date;
   struct tm date_r;
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   char hh_c[3];
   char mi_c[3];
   char ss_c[3];

   if (date_heure_jour == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(date_heure_jour,'0',6);
      return(-1);
   }

   time(&lg);
   date = localtime_r(&lg, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   memset(date_heure_jour, 0, 12);

   sprintf(aa_c,"%04d",date_r.tm_year + 1900);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(jj_c,"%02d",date_r.tm_mday);

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mi_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   memcpy(date_heure_jour +  0, aa_c + 2, 2);
   memcpy(date_heure_jour +  2, mm_c, 2);
   memcpy(date_heure_jour +  4, jj_c, 2);
   memcpy(date_heure_jour +  6, hh_c, 2);
   memcpy(date_heure_jour +  8, mi_c, 2);
   memcpy(date_heure_jour + 10, ss_c, 2);

   return(0);
}

void GetAcquirerPurgeTimeMS(struct timeval * sPurgeTime)
{
   char sLine[MAX_LINE_TRC];
   int time_ms;
   int nUsec;
   
   trace_event("Start GetAcquirerPurgeTimeMS()",TRACE);

   gettimeofday(sPurgeTime,NULL);
   sPurgeTime->tv_sec += nTimerAcq/1000;
   
   time_ms = nTimerAcq%1000;
   
   if(time_ms > 0)
   {
	   nUsec = sPurgeTime->tv_usec + time_ms*1000;
	   
	   if (nUsec < 1000000)
			sPurgeTime->tv_usec = nUsec;
	   else
	   {
		   sPurgeTime->tv_sec += 1;
		   sPurgeTime->tv_usec = nUsec - 1000000;
	   }
   }
	   
   sPurgeTime->tv_sec -=  1000000000;
   sprintf(sLine,"End GetAcquirerPurgeTimeMS(%ld,%ld,%d)",sPurgeTime->tv_sec,sPurgeTime->tv_usec,time_ms);
   trace_event(sLine,PROCESSING);
}

void GetIssuerPurgeTimeMS(struct timeval * sPurgeTime)
{
   char sLine[MAX_LINE_TRC];
   int nDelta = 0;
   int nUsec;
   int time_ms;
   
   trace_event("Start GetIssuerPurgeTimeMS()",TRACE);

   gettimeofday(sPurgeTime, NULL);
   nDelta = nTimerAcq - nIssAcqDelta;
   
   sPurgeTime->tv_sec += nDelta/1000;
   
   time_ms = nDelta%1000;
   
   nUsec = sPurgeTime->tv_usec + time_ms*1000;
   
   if(time_ms > 0)
   {
	   if (nUsec < 1000000)
			sPurgeTime->tv_usec = nUsec;
	   else
	   {
		   sPurgeTime->tv_sec += 1;
		   sPurgeTime->tv_usec = nUsec - 1000000;
	   }
   }
	   
   sPurgeTime->tv_sec -=  1000000000;
   
   sprintf(sLine,"End GetIssuerPurgeTimeMS(%ld,%ld,%d)",sPurgeTime->tv_sec,sPurgeTime->tv_usec,time_ms);
   trace_event(sLine,PROCESSING);
 
}

void GetIssuerPurgeTime( time_t * nPurgeTime)
{
   char sLine[MAX_LINE_TRC];

   trace_event("Start GetIssuerPurgeTime()",PROCESSING);
   
   /*Start 07072020 PLUTONL-2086*/
   if (nTimerAcq > 300)
   {
      struct timeval  sPurgeTime; 
      GetIssuerPurgeTimeMS(&sPurgeTime);       
      *nPurgeTime = sPurgeTime.tv_sec;
   }
   /*End 07072020 PLUTONL-2086*/
   else
   {
      time(nPurgeTime);
      *nPurgeTime += nTimerAcq - nIssAcqDelta;
      *nPurgeTime -=  1000000000;  
   }
   
   sprintf(sLine,"End GetIssuerPurgeTime(%ld)",*nPurgeTime);
   trace_event(sLine,PROCESSING);
}

void GetAcquirerPurgeTime( time_t * nPurgeTime)
{
   char sLine[MAX_LINE_TRC];

   trace_event("Start GetAcquirerPurgeTime()",PROCESSING);
   /*Start 07072020 PLUTONL-2086*/
   if (nTimerAcq > 300)
   {
      struct timeval  sPurgeTime; 
      GetAcquirerPurgeTimeMS(&sPurgeTime);
       
      *nPurgeTime = sPurgeTime.tv_sec;
   }
   /*End 07072020 PLUTONL-2086*/
   else
   {
      time(nPurgeTime);
      *nPurgeTime +=nTimerAcq;
      *nPurgeTime -=  1000000000;   
   }
   
   sprintf(sLine,"End GetAcquirerPurgeTime(%ld)",*nPurgeTime);
   trace_event(sLine,PROCESSING);
}

/*Start HAL ENHQ2.20 completion advice Art 1.1 in SAF just with Acq*/
void GetSafPurgeTimeMS( struct timeval * sPurgeTime)
{
  char sLine[MAX_LINE_TRC];
   int time_ms;
   int nUsec;
   
   trace_event("Start GetSafPurgeTimeMS()",TRACE);

   gettimeofday(sPurgeTime,NULL);
   sPurgeTime->tv_sec += nTimerAcq/1000;
   
   time_ms = nTimerAcq%1000;
   
   if(time_ms > 0)
   {
	   nUsec = sPurgeTime->tv_usec + time_ms*1000;
	   
	   if (nUsec < 1000000)
			sPurgeTime->tv_usec = nUsec;
	   else
	   {
		   sPurgeTime->tv_sec += 1;
		   sPurgeTime->tv_usec = nUsec - 1000000;
	   }
   }
	   
   sPurgeTime->tv_sec -=  1000000000;
   sprintf(sLine,"End GetSafPurgeTimeMS(%ld,%ld,%d)",sPurgeTime->tv_sec,sPurgeTime->tv_usec,time_ms);
   trace_event(sLine,PROCESSING);
}
/*End HAL ENHQ2.20 completion advice Art 1.1*/

void GetSafPurgeTime( time_t * nPurgeTime)
{
   char sLine[MAX_LINE_TRC];
   trace_event("Start GetSafPurgeTime()",TRACE);

   time(nPurgeTime);
   *nPurgeTime += nTimerAcq;
   *nPurgeTime -=  1000000000;
   sprintf(sLine,"End GetSafPurgeTime(%ld)",*nPurgeTime);
   trace_event(sLine,PROCESSING);
}


int get_tz_date(int nGmtOfstMinutes, char* szTime)
{
   time_t	nTime;
   struct tm *date;
   struct tm date_r;
   char jj_c[3];
   char mm_c[3];
   char aa_c[5];
   char hh_c[3];
   char mi_c[3];
   char ss_c[3];

   if (szTime == (char *)NULL)
   {
      trace_event("DAT003IM", ERROR);
      memset(szTime,'0',6);
      return(-1);
   }

   time(&nTime);
   nTime += nGmtOfstMinutes*60;

   date = gmtime_r(&nTime, &date_r);
   date_r.tm_min--;
   if (date_r.tm_min == 60) date_r.tm_min = 0;

   memset(szTime, 0, 13);

   sprintf(aa_c,"%04d",date_r.tm_year+1900);
   sprintf(mm_c,"%02d",date_r.tm_mon+1);
   sprintf(jj_c,"%02d",date_r.tm_mday);

   sprintf(hh_c,"%02d",date_r.tm_hour);
   sprintf(mi_c,"%02d",date_r.tm_min + 1);
   sprintf(ss_c,"%02d",date_r.tm_sec);

   memcpy(szTime +  0, aa_c+2, 2);
   memcpy(szTime +  2, mm_c, 2);
   memcpy(szTime +  4, jj_c, 2);
   memcpy(szTime +  6, hh_c, 2);
   memcpy(szTime +  8, mi_c, 2);
   memcpy(szTime + 10, ss_c, 2);
   szTime[12]='\0';

   return(0);
}


/*Start ZKO10032022 PLUTONL-4411*/

void GetCaptureIntPurgeTime(struct timeval * pkAuthorCaptureTime, struct timeval * sPurgeTime)
{
   char sLine[MAX_LINE_TRC];
   int nDelta = 0;
   int time_ms;
   int nUsec;
   
   trace_event("Start GetCaptureIntPurgeTime()",PROCESSING);

   sPurgeTime->tv_sec = pkAuthorCaptureTime->tv_sec;
   sPurgeTime->tv_usec = pkAuthorCaptureTime->tv_usec;

   nDelta = nTimerAcq - g_nAuthReplyBackPeriod;

   sPurgeTime->tv_sec += nDelta/1000;
   
   time_ms = nDelta%1000;
   
   if(time_ms > 0)
   {
	   nUsec = sPurgeTime->tv_usec + time_ms*1000;
	   
	   if (nUsec < 1000000)
			sPurgeTime->tv_usec = nUsec;
	   else
	   {
		   sPurgeTime->tv_sec += 1;
		   sPurgeTime->tv_usec = nUsec - 1000000;
	   }
   }
	   
   sPurgeTime->tv_sec -=  1000000000;
   sprintf(sLine,"End GetCaptureIntPurgeTime(%ld,%ld)",sPurgeTime->tv_sec,sPurgeTime->tv_usec);
   trace_event(sLine,PROCESSING);
}

void GetRoutingIntPurgeTime (struct timeval * pkAuthorCaptureTime, struct timeval * sPurgeTime)
{
   char sLine[MAX_LINE_TRC];
   int nDelta = 0;
   int nUsec;
   int time_ms;
   
   trace_event("Start GetRoutingIntPurgeTime ()",PROCESSING);

   sPurgeTime->tv_sec = pkAuthorCaptureTime->tv_sec;
   sPurgeTime->tv_usec = pkAuthorCaptureTime->tv_usec;

   nDelta = nTimerAcq - nIssAcqDelta;
   
   sPurgeTime->tv_sec += nDelta/1000;
   
   time_ms = nDelta%1000;
   
   nUsec = sPurgeTime->tv_usec + time_ms*1000;
   
   if(time_ms > 0)
   {
	   if (nUsec < 1000000)
			sPurgeTime->tv_usec = nUsec;
	   else
	   {
		   sPurgeTime->tv_sec += 1;
		   sPurgeTime->tv_usec = nUsec - 1000000;
	   }
   }
	   
   sPurgeTime->tv_sec -=  1000000000;
   
   sprintf(sLine,"End GetRoutingIntPurgeTime (%ld,%ld)",sPurgeTime->tv_sec,sPurgeTime->tv_usec);
   trace_event(sLine,PROCESSING);
 
}

/*End ZKO10032022 PLUTONL-4411*/
