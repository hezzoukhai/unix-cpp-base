/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      dump.c                                                                 */
/*                                                                             */
/* FUNCTION                                                                    */
/*          Trace And Dump Tools Management                                     */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   29/09/2001 -         V 2.0.1                              */
/*        dump Of Ebcdic Messages                                              */
/*        H.YOUSFI   29/03/2003 -         V 2.0.2                              */
/*        Use the function stat instead of fstat to test on the physical file  */
/*        and not on the file id (HY 270302 )                                  */
/*        H.YOUSFI   14/05/2003 -         V 2.0.3                              */
/*        Ajout des millisecondes dans la trace                                */
/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdarg.h>
#include <fmtmsg.h>
#include <pthread.h>

#include <define.h>
#include <iso_hps.h>
#include <dump.h>
#include <p7_macros.h>
#include <p7_trace.h>




P7_DEF_SYM int		debug_option;						/*EBE120925: definition of debug_option. Check reference in dump.h */
P7_DEF_SYM char		trace_file[TRACE_FILE_LEN + 1];		/* Nom externe du fichier de trace */
P7_DEF_SYM char		group_code[6 + 1];					/* AZA10012018: Nom externe du code de groupe de banques */
P7_DEF_SYM int		max_size;							/* Taille max du fichier de trace  */
P7_DEF_SYM int		trace_level;						/* Niveau de trace du module       */
/*P7_DEF_SYM int		fd_trace;*/							/* Identifiant du fichier trace    */

static int				gs_trace_level; /*= P7_TL_TRACE|P7_TL_FATAL|P7_TL_ERROR|P7_TL_WARNING|P7_TL_PROCESSING|P7_TL_STREAM;*/
static int check_trace_size_mco(int max_size);

#ifdef P7_IMPORT_SYM
extern  pthread_mutex_t  trace_mutex;
extern	int fd_trace; /*IBO20231213 PLUTONL-7432*/
#else
pthread_mutex_t  trace_mutex = PTHREAD_MUTEX_INITIALIZER;
int				 fd_trace = -1; /*IBO20231213 PLUTONL-7432*/
#endif

void InitDumpParam()
{
	memset(trace_file, 0, sizeof(trace_file));
	/* AZA10012018 */
	memset(group_code, 0, sizeof(group_code));
	trace_level = STREAM;
}
/*ATH04062018 START */
/******************************************************************************/
/* Fonction isMultiCutoffEnabled.                                             */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de verifier si MultiCutoff activé. */
/* Entree      :  													          */
/* Sortie      : 1 Enabled                                               	  */
/*               0 Desabled                                                 */
/* Date/Auteur : 04/06/18 A. TAHSI                                            */
/* Revision    :                                                              */
/******************************************************************************/
int isMultiCutoffEnabled()
{
	char  mode[1 +1];
	
	memset(mode,0,sizeof(mode));
	sprintf(mode, "%s", getenv("MULTI_CUTOFF"));
	if (memcmp(mode, "1", 1) == 0)
	{
		/* MULTI_CUTOFF[ON] */
		return 1;
	}
	else
	{
		return 0;
	}
}

/*ATH04062018 END */
/******************************************************************************/
/* Fonction init_trace.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de prendre en charge les taille des*/
/*               fichiers trace.                                              */
/* Entree      : Nom physique du fichier trace                                */
/*               Taille maximum du fichier                                    */
/*               AZA10012018: Code du groupe de banques (group_code)          */
/* Sortie      : NOK si erreur                                                */
/*               OK Sinon                                                     */
/* Date/Auteur : 03/10/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int  init_trace()
{
	char    fichier_trace_etendu[128];
	struct  stat buf_stat;
	int   	retour;
	/*
	char    cpt_char[4];
	int     cpt = 0;*/
	static int	nTraceFileLen = 0;
	static int	nExtTraceFileLen = 0;
	static char ExtTraceFile[128];
	static int	nFileSeq = 0;
	
	if( nTraceFileLen == 0 )
	{
		nTraceFileLen = strlen(trace_file) - 3;
		if( nTraceFileLen <= 0 )
		{
			fprintf(stderr,"Invalid Trace file name : [%s]\n", trace_file);
			return NOK;
		}

		sprintf(ExtTraceFile,"%s/%s", getenv("TRACE"), trace_file);
		
		nExtTraceFileLen = strlen(ExtTraceFile) - 3;
	}
	/*
	memset(cpt_char, 0, sizeof(cpt_char));
	memcpy(cpt_char, trace_file + strlen(trace_file) - 3, 3);
	cpt = atoi(cpt_char);

	while(1)
	{
		sprintf(fichier_trace_etendu,"%s/%s",getenv("TRACE"), trace_file);
		sprintf(fichier_trace_etendu+strlen(fichier_trace_etendu)-3,"%03d",cpt);

		retour = stat(fichier_trace_etendu, &buf_stat);
		if ( retour < 0) 
		{
			break;
		}
		if (buf_stat.st_size < max_size * 1024) 
		{
			break;
		}

		cpt++;
	}
	*/
	while(1)
	{
		sprintf(fichier_trace_etendu,"%s",ExtTraceFile);
		sprintf(fichier_trace_etendu + nExtTraceFileLen,"%03d",nFileSeq);
		
		retour = stat(fichier_trace_etendu, &buf_stat);
		if ( retour < 0) 
		{
			break;
		}
		if (buf_stat.st_size < max_size * 1024) 
		{
			break;
		}

		nFileSeq++;
	}
	

	fd_trace = open(fichier_trace_etendu, O_RDWR|O_CREAT|O_APPEND, 0640);

	if (fd_trace < 0)
	{
		fprintf(stdout,"FICHIER : %s\n", fichier_trace_etendu);
		fprintf(stdout,"ERRNO :%d\n", errno);
		return(NOK);
	}
	
	sprintf(trace_file + nTraceFileLen, "%03d", nFileSeq);
	
	return(OK);
}

/******************************************************************************/
static int check_trace_size(/*int fd_trace,*/ int max_size)
{
	struct  stat buf_stat;
	int     retour;
	char    fichier_trace_etendu[64];

	sprintf(fichier_trace_etendu,"%s/%s", getenv("TRACE"), trace_file);

	retour = stat(fichier_trace_etendu, &buf_stat);

   /* retour = fstat(fd_trace, &buf_stat); */
/* End HY 270302 : Le cas ou le fichier est supprimer */

	if (retour < 0)
	{
		fprintf(stderr,"Errno stat file, errno : %d,file:[%s]\n", errno,fichier_trace_etendu);
		return(NOK);
	}

	if (buf_stat.st_size >= max_size * 1024) return(NOK);

	return(OK);
}

/******************************************************************************/
/* Fonction trace_event.                                                      */
/* --------------------                                                       */
/* Description : Cette fonction a pour but d'afficher sur l'ecran de la       */
/*               console ainsi que dans un fichier trace les messages donnes  */
/*               en parametre en prenant en compte le niveau de severite du   */
/*               message.                                                     */
/* Entree      : Message                                                      */
/*               Severite                                                     */
/* Sortie      : Aucune                                                       */
/* Date/Auteur : 20/11/95 A. ALAOUI                                           */
/* Revision    : 28/04/98 A. ALAOUI pour prendre en compte le PID du processus*/
/*               qui demande la trace.                                        */
/******************************************************************************/
void trace_event(char *mess, int level)
{
	int     retour;
	char    ligne[MAX_LINE_TRC + 1];
	char	sLine_Buffer[93 + 1];
	char    date [14 + 1];
	char    Mls  [ 4];
	char    Mls_tmp  [ 7];
	struct timeval tp;

	int   i,k;
	int	nLength, nMsgLen, nHeaderLen;

	memset(date, 0, sizeof(date));
	memset(ligne, 0, sizeof(ligne));
	memset(Mls, 0, sizeof(Mls));
	memset(Mls_tmp, 0, sizeof(Mls_tmp));


	if (level > trace_level)
	{
		return;
	}

	/* no need now as we are using mutex */
	if( fd_trace < 0 ) /*IBO20231213 PLUTONL-7432 , 0 is a valid file descriptor*/
		return;
	
	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);

	sprintf(Mls_tmp,"%06ld",tp.tv_usec);
	memcpy(Mls,Mls_tmp,3);
	
	
	/*Start ZKO PLUTONL-4563*/
	
	pthread_mutex_lock(&trace_mutex);/*EBE151127: put back mutex locks*/
		
	k=0;
	nMsgLen = strlen(mess);
	
	sprintf(ligne,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|%d| ",
			date,date+2,date+8,date+10,date+12,Mls,getpid(),p7_thread_id(),level);
	nHeaderLen = strlen(ligne);
			
	while(k < nMsgLen)
	{

		memset(sLine_Buffer,0,sizeof(sLine_Buffer));
		if((nMsgLen - k) > 93)
			memcpy(sLine_Buffer,mess+k,93);
		else
			memcpy(sLine_Buffer,mess+k,nMsgLen - k);
		
		sprintf(ligne + nHeaderLen,"%.93s",sLine_Buffer);
		
		nLength = strlen(ligne);
		if( nLength < 130 )
		{
			memset(ligne + nLength,' ',130 - nLength );
		}
		
		memcpy(ligne+130,".\n",2);
	
		write(fd_trace,ligne, 132);
		
		k=k+93;
	}
	
	retour = check_trace_size_mco( max_size); /* AZA10012018 */
	if (retour != OK) /* AZA10012018 */
	{
		retour = check_trace_size( max_size);
		if (retour != OK)
		{
			close(fd_trace);
			fd_trace = -1;
			init_trace();
		}
	} /* AZA10012018 */
	
	pthread_mutex_unlock(&trace_mutex);
	/*End ZKO PLUTONL-4563*/

	return;
}


void trace_line(char *mess, int level)
{
	int     retour;
	char    ligne[MAX_LINE_TRC + 1];
	char    date [14 + 1];
	char    Mls  [ 4];
	char    Mls_tmp  [ 7];
	struct timeval tp;

	int   i;
	int	nLength;

	memset(date, 0, sizeof(date));
	memset(ligne, 0, sizeof(ligne));
	memset(Mls, 0, sizeof(Mls));
	memset(Mls_tmp, 0, sizeof(Mls_tmp));


	if (level > trace_level)
	{
		return;
	}

	/* no need now as we are using mutex */
	if( fd_trace <= 0 )
		return;

	strncpy(ligne,mess,sizeof(ligne) -1);

	nLength = strlen(ligne);
	if( nLength < 130 )
	{
		memset(ligne + nLength,' ',130 - nLength );
	}
	memcpy(ligne+130,".\n",2);


	/*EBE151127: put back mutex locks*/
	pthread_mutex_lock(&trace_mutex);
	retour = check_trace_size( max_size);
	if (retour != OK)
	{
		close(fd_trace);
		fd_trace = -1;
		init_trace();
	}

	write(fd_trace,ligne, 132);
	pthread_mutex_unlock(&trace_mutex);

	return;
}
int WriteEntete(char *sEntete)
{

	char    date [14 + 1];
	char    Mls [4];
	char    Mls_tmp  [ 7];
	struct timeval tp;


	memset(Mls, 0, sizeof(Mls));
	memset(Mls_tmp, 0, sizeof(Mls_tmp));

	memset(date, 0, sizeof(date));
	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);
	sprintf(Mls_tmp,"%06ld",tp.tv_usec);
	memcpy(Mls,Mls_tmp,3);
	sprintf(sEntete,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|D| ",
		date,date+2,date+8,date+10,date+12,Mls,getpid(),p7_thread_id());
	return(strlen(sEntete));
}
/******************************************************************************/

void dump_buffer(unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{

	char          ligne[LG_MAX*3];
	char          entete_ligne[60];
	char          sLine[256];
	char          sWork[LG_MAX];

	int           nLen;
	int				j;

	int				i, nb_ligne, position_hex, position_car;
	char			date[14 + 1];
	int				dump_with;

	
	trace_event("Start dump_buffer()", FATAL);

	if (debug_option != 1 ) 
	{
		trace_event("End   dump_buffer(VOID)", FATAL);
		return;
	}

	/* for every 18 bytes in buffer, we dump 132 bytes. Our buffer is 3*4k, so we only dump the first 1600 bytes*/
	if( SizeBuffer > 1600 )
	{
		SizeBuffer = 1600;
		trace_event(">>> Partial dump", FATAL);
	}


	memset(sWork, 0, sizeof(sWork));
	memset(date, 0, sizeof(date));
	memset(entete_ligne, 0, sizeof(entete_ligne));
	memset(ligne, 0, sizeof(ligne));
	memset(sLine, 0, sizeof(sLine));


	current_date(date);
	current_hour(date + 8);



	memset(entete_ligne, ' ', sizeof(entete_ligne)-1);
	nLen = WriteEntete(entete_ligne);
	memset(ligne, ' ', sizeof(ligne)-1);
	memset(sLine, ' ', sizeof(sLine)-1);
	nb_ligne = 0;
	position_hex = 38 + nLen;
	position_car = 0 + nLen;


	if (Set == 'E')
		EbcdicToAscii(Buffer, sWork, SizeBuffer);
	else if (Set == 'Y') 
		ExtendedEbcdicToAscii(Buffer, sWork, SizeBuffer);/*NAB26032020  SIA*/
	else
		memcpy(sWork, Buffer, SizeBuffer);


	dump_with = nLen + (18 * 2) + 2 + (18 * 3);
	for (i = 0, j = 0; i < SizeBuffer; i++, j++)
	{
		/*EBE140806: just to avoid using %
		if ((i != 0) && ((i % 18) == 0))*/
		if (j == 18)
		{
			j = 0;
			memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
			memcpy(ligne + (nb_ligne * 132) + nLen + 36, " |", 2);
			memset(ligne + (nb_ligne * 132) + dump_with, ' ', 132 - dump_with);
			memcpy(ligne + (nb_ligne * 132) + 130, ".\n", 2);

			nb_ligne++;
			position_hex = (nb_ligne * 132) + nLen + 36 + 2;
			position_car = (nb_ligne * 132) + nLen;
		}
		sprintf(ligne + position_hex, "%02X ", *(Buffer + i));
		position_hex += 3;


		if (isprint(*(sWork + i)))
			ligne[position_car] = *(sWork + i);
		else
			ligne[position_car] = '.';
		ligne[position_car + 1] = ' ';

		position_car += 2;
	}



	if (j < 18)
	{
		for (i = position_car; i < (nb_ligne * 132) + nLen + 36; i++) ligne[i]= ' ';
		for (i = position_hex; i < (nb_ligne * 132) + 131; i++) ligne[i] = ' ';
	}

	memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
	memcpy(ligne + (nb_ligne * 132) + nLen + 36, " |", 2);
	memcpy(ligne + (nb_ligne * 132) + 130, ".\n", 2);
	i = (nb_ligne + 1) * 132;

	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);

    pthread_mutex_lock(&trace_mutex);	/*NAK BOC ACT : PROD00038949*/

	memcpy(sLine + nLen + 38, "START               ", 20);
	write(fd_trace, sLine, 132);

	if (origine == 'B')
		memcpy(sLine + nLen + 38, "QUEUE               ", 20);
	else if (origine == 'L')
		memcpy(sLine + nLen + 38, "NETWORK             ", 20);
	write(fd_trace, sLine, 132);

	if (sens == 'I')
		memcpy(sLine + nLen + 38, "INCOMING            ", 20);
	else if (sens == 'O')
		memcpy(sLine + nLen + 38, "OUTGOING            ", 20);
	write(fd_trace, sLine, 132);

	memcpy(sLine + nLen + 38, "TIMESTAMP           ", 20);
	memcpy(sLine + nLen + 38 + 20, date, 4);
	memcpy(sLine + nLen + 38 + 20 + 4, date + 8, 6);
	write(fd_trace, sLine, 132);


	write(fd_trace, ligne, i);

	memcpy(sLine + nLen + 38, "END                 ", 20);
	write(fd_trace, sLine, 132);

pthread_mutex_unlock(&trace_mutex); /*NAK BOC ACT : PROD00038949*/
	memset(sWork, 0, sizeof(sWork));
	trace_event("End   dump_buffer(VOID)", FATAL);
	return;
}

void dump_buffer_on_file(int nFileFd, unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{

	char          ligne[LG_MAX*3];
	char          entete_ligne[60];
	char          sLine[256];
	char          sWork[LG_MAX];

	int           nLen;
	int				j;

	int				i, nb_ligne, position_hex, position_car;
	char			date[14 + 1];
	int				dump_with;


	/* for every 18 bytes in buffer, we dump 132 bytes. Our buffer is 3*4k, so we only dump the first 1600 bytes*/
	if( SizeBuffer > 1600 )
	{
		SizeBuffer = 1600;
	}


	memset(sWork, 0, sizeof(sWork));
	memset(date, 0, sizeof(date));
	memset(entete_ligne, 0, sizeof(entete_ligne));
	memset(ligne, 0, sizeof(ligne));
	memset(sLine, 0, sizeof(sLine));


	current_date(date);
	current_hour(date + 8);



	memset(entete_ligne, ' ', sizeof(entete_ligne)-1);
	nLen = WriteEntete(entete_ligne);
	memset(ligne, ' ', sizeof(ligne)-1);
	memset(sLine, ' ', sizeof(sLine)-1);
	nb_ligne = 0;
	position_hex = 38 + nLen;
	position_car = 0 + nLen;


	if (Set == 'E')
		EbcdicToAscii(Buffer, sWork, SizeBuffer);
	else
		memcpy(sWork, Buffer, SizeBuffer);


	dump_with = nLen + (18 * 2) + 2 + (18 * 3);
	for (i = 0, j = 0; i < SizeBuffer; i++, j++)
	{
		
		if (j == 18)
		{
			j = 0;
			memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
			memcpy(ligne + (nb_ligne * 132) + nLen + 36, " |", 2);
			memset(ligne + (nb_ligne * 132) + dump_with, ' ', 132 - dump_with);
			memcpy(ligne + (nb_ligne * 132) + 130, ".\n", 2);

			nb_ligne++;
			position_hex = (nb_ligne * 132) + nLen + 36 + 2;
			position_car = (nb_ligne * 132) + nLen;
		}
		sprintf(ligne + position_hex, "%02X ", *(Buffer + i));
		position_hex += 3;


		if (isprint(*(sWork + i)))
			ligne[position_car] = *(sWork + i);
		else
			ligne[position_car] = '.';
		ligne[position_car + 1] = ' ';

		position_car += 2;
	}



	if (j < 18)
	{
		for (i = position_car; i < (nb_ligne * 132) + nLen + 36; i++) ligne[i]= ' ';
		for (i = position_hex; i < (nb_ligne * 132) + 131; i++) ligne[i] = ' ';
	}

	memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
	memcpy(ligne + (nb_ligne * 132) + nLen + 36, " |", 2);
	memcpy(ligne + (nb_ligne * 132) + 130, ".\n", 2);
	i = (nb_ligne + 1) * 132;

	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);


	memcpy(sLine + nLen + 38, "START               ", 20);
	write(nFileFd, sLine, 132);

	if (origine == 'B')
		memcpy(sLine + nLen + 38, "QUEUE               ", 20);
	else if (origine == 'L')
		memcpy(sLine + nLen + 38, "NETWORK             ", 20);
	write(nFileFd, sLine, 132);

	if (sens == 'I')
		memcpy(sLine + nLen + 38, "INCOMING            ", 20);
	else if (sens == 'O')
		memcpy(sLine + nLen + 38, "OUTGOING            ", 20);
	write(nFileFd, sLine, 132);

	memcpy(sLine + nLen + 38, "TIMESTAMP           ", 20);
	memcpy(sLine + nLen + 38 + 20, date, 4);
	memcpy(sLine + nLen + 38 + 20 + 4, date + 8, 6);
	write(nFileFd, sLine, 132);


	write(nFileFd, ligne, i);

	memcpy(sLine + nLen + 38, "END                 ", 20);
	write(nFileFd, sLine, 132);

	return;
}

void dump_buffer_debug(unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{
	char          ligne[3 * 10240];
	char          entete_ligne[60];
	char          sLine[256];
	char          sWork[LG_MAX];

	int           nLen;

	int  i, nb_ligne, position_hex, position_car;
	char    date[14 + 1];

	/* if ( trace_level < DATA) return; */
	trace_event("Start dump_buffer_debug()", FATAL);



	if (SizeBuffer > LG_MAX)
		return;

	if (debug_option != 1)
	{
		trace_event("End dump_buffer_debug(PCI)", FATAL);
		return;
	}

	memset(sWork, 0, sizeof(sWork));
	memset(date, 0, sizeof(date));

	current_date(date);
	current_hour(date + 8);

	memset(entete_ligne, 0, sizeof(entete_ligne));

	memset(entete_ligne, ' ', sizeof(entete_ligne)-1);
	nLen = WriteEntete(entete_ligne);
	memset(ligne, 0, sizeof(ligne));
	memset(sLine, 0, sizeof(sLine));
	memset(ligne, ' ', sizeof(ligne)-1);
	memset(sLine, ' ', sizeof(sLine)-1);
	nb_ligne = 0;
	position_hex = 38 + nLen;
	position_car = 0 + nLen;


	if (Set == 'E')
		EbcdicToAscii(Buffer, sWork, SizeBuffer);
	else
		memcpy(sWork, Buffer, SizeBuffer);


	for (i = 0; i<SizeBuffer; i++)
	{
		if ((i != 0) && ((i % 18) == 0))
		{
			memset(ligne + (nb_ligne * 132) + 36 + nLen + 58, ' ', 6);
			memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
			memcpy(ligne + nLen + 36 + (nb_ligne * 132), " |", 2);
			memcpy(ligne + 130 + (nb_ligne * 132), ".\n", 2);
			nb_ligne++;
			position_hex = (nb_ligne * 132) + nLen + 38;
			position_car = (nb_ligne * 132) + nLen;
		}
		sprintf(ligne + position_hex, "%02X ", (unsigned char)*(sWork + i));
		position_hex += 3;
		if (isprint(*(sWork + i)))
			memcpy(ligne + position_car, sWork + i, 1);
		else
			memcpy(ligne + position_car, ".", 1);

		memcpy(ligne + position_car + 1, " ", 1);

		position_car += 2;
	}
	for (i = position_car; i<(nb_ligne * 132) + 36; i++) memcpy(ligne + i, " ", 1);
	for (i = position_hex; i<(nb_ligne * 132) + 131; i++) memcpy(ligne + i, " ", 1);

	ligne[i + 1] = '\n';

	memcpy(ligne + (nb_ligne * 132), entete_ligne, nLen);
	memcpy(ligne + nLen + 36 + (nb_ligne * 132), " |", 2);
	memcpy(ligne + 130 + (nb_ligne * 132), ".\n", 2);

	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);

	/*pthread_mutex_lock(&trace_mutex);*//*YK 10g*/
	memcpy(sLine + nLen + 38, "START               ", 20);
	write(fd_trace, sLine, 132);

	if (origine == 'B')
		memcpy(sLine + nLen + 38, "QUEUE               ", 20);
	if (origine == 'L')
		memcpy(sLine + nLen + 38, "NETWORK             ", 20);
	write(fd_trace, sLine, 132);

	if (sens == 'I')
		memcpy(sLine + nLen + 38, "INCOMING            ", 20);
	if (sens == 'O')
		memcpy(sLine + nLen + 38, "OUTGOING            ", 20);
	write(fd_trace, sLine, 132);

	memcpy(sLine + nLen + 38, "TIMESTAMP           ", 20);
	memcpy(sLine + nLen + 38 + 20, date, 4);
	memcpy(sLine + nLen + 38 + 24, date + 8, 6);
	write(fd_trace, sLine, 132);

	write(fd_trace, ligne, i + 1);

	memcpy(sLine + nLen + 38, "END                 ", 20);
	write(fd_trace, sLine, 132);
	/*pthread_mutex_unlock(&trace_mutex);*//*YK 10g*/
	trace_event("End   dump_buffer(VOID)", FATAL);
	return;
}
/*END Mehdi Elyajizi  010807  */
/*End MKB060309 PCI Norme Integration */

/******************************************************************************/
/* Fonction dump_internal                                                     */
/* ----------------------                                                     */
/* Description : Cette fonction a pour but de dumper un buffer au format      */
/*               PowerSEVEN.                                                  */
/* Entree      : Buffer a dumper.                                             */
/* Sortie      : Rien.                                                        */
/******************************************************************************/
/*Not used + libcomora is a low level lib and hence should not reference a high level lib
void dump_internal(TSIsoInfo *msgInfo)
{
	int  i;
	char ligne[512];
	char tmpField[512];
	int  tmpLen;

	if (trace_level < TRACE)
	{
		trace_event("End   dump_internal()", PROCESSING);
		return;
	}

	trace_event("---------------------------------------------------", TRACE);
	memset(ligne, 0, sizeof(ligne));
	memcpy(ligne, " N      |       [ DATA ]\n", 25);

	write(fd_trace, ligne, strlen(ligne));

	for(i=0;i<128;i++)
	{
		switch(i)
		{
		case ISO_PIN_DATA :
			memset(ligne, 0, sizeof(ligne));
			if (GetIsoField(i, msgInfo, tmpField, &tmpLen) == SUCCES)
			{
				sprintf(ligne,"%03d %03d | ", i, tmpLen);
				memcpy(ligne+10, "PIN DATA", 8);
				memcpy(ligne+11+8,"\0", 1);
				write(fd_trace,ligne, strlen(ligne));
				write(fd_trace,"\n", 1);
			}

		default :
			memset(ligne, 0, sizeof(ligne));
			if (GetIsoField(i, msgInfo, tmpField, &tmpLen) == SUCCES)
			{
				sprintf(ligne,"%03d %03d | ", i, tmpLen);
				memcpy(ligne+10, tmpField, tmpLen);
				memcpy(ligne+11+tmpLen,"\0", 1);
				write(fd_trace,ligne, strlen(ligne));
				write(fd_trace,"\n", 1);
			}
		}
	}
	
	trace_event("---------------------------------------------------", TRACE);
}
*/


void error_to_console(source, severity, message, fix)
	char *source;
int   severity;
char *message;
char *fix;
{
	int local_severity = MM_INFO;

	if (severity == 0) local_severity = MM_NOSEV;
	else
		if (severity == 1) local_severity = MM_INFO;
		else
			if (severity == 2) local_severity = MM_WARNING;
			else
				if (severity == 3) local_severity = MM_ERROR;
				else
					if (severity == 4) local_severity = MM_HALT;


	fmtmsg(MM_CONSOLE, source,  local_severity, message, fix, "");
}

/******************************************************************************/
/* Fonction dump_event.                                                       */
/* --------------------                                                       */
/******************************************************************************/
void dump_event(mess, level)
	char *mess;
int   level;
{
	int     retour;
	/*SKO290509*/ /* char    ligne[MAX_LINE_TRC + 1]; */
	/*SKO290509*/ /* char    date [14]; */
	/*SKO290509*/ /*char    Mls  [ 2]; */

	/*SKO290509* int   i; */

	if (level <= trace_level)
	{
		/*pthread_mutex_lock(&trace_mutex);*//*YK 10g*/
		if (fd_trace > 0)
		{
			retour = check_trace_size_mco( max_size); /* AZA10012018 */
			if (retour != OK) /* AZA10012018 */
			{
			retour = check_trace_size(max_size);
			if (retour != OK)
			{
				close(fd_trace);
				init_trace();
			}
			} /* AZA10012018 */
		}
		/*pthread_mutex_unlock(&trace_mutex);*/


		/*pthread_mutex_lock(&trace_mutex);*/
		write(fd_trace,mess, strlen(mess));
		/*pthread_mutex_unlock(&trace_mutex);*/
	}
	return;
}


/*EBE130812*/
void dump_backtrace()
{
#ifdef LINUX
	void *buffer[LG_MAX];
	int nptrs=backtrace(buffer, LG_MAX);	
	backtrace_symbols_fd(buffer,nptrs,fd_trace);
	/* For AIX, use procstack pid */
#elif  defined(_AIX)
	char	szCmd[LG_MAX];
	char    fichier_trace_etendu[LG_MAX];

	sprintf(fichier_trace_etendu,"%s/%s", getenv("TRACE"), trace_file);
	sprintf(szCmd,"procstack %d >> %s",getpid(),fichier_trace_etendu);
	trace_event("================[Start BACKTRACE]===================",ERROR);
	trace_event(szCmd,ERROR);
	system(szCmd);
	trace_event("================[End BACKTRACE]===================",ERROR);
#endif
}

void trace_message(int level, const char* szMessage,...)
{
	char            sLine[BUF_LG_MAX];
	va_list			arg;
	
	int				offset;
	int				nLength;

	va_start(arg, szMessage);
	vsnprintf(sLine,BUF_LG_MAX, szMessage, arg);
	va_end(arg);
	
	nLength = strlen(sLine);
	if( nLength <= 80 )
	{
	trace_event(sLine,level);
	}
	else
	{
		offset = 0;
		while(offset < nLength )
		{
			trace_event(sLine + offset,level);
			offset += 93;
		}
	}
}
/******************************************************************************/
P7_INLINE int	
is_trace_level_on(E_TRACE_LEVEL level)
{
	return (gs_trace_level >= level );
}

/* AZA10012018 */
int  init_trace_mco()
{
	char    fichier_trace_etendu[128];
	struct  stat buf_stat;
	int   	retour;
	/*
	char    cpt_char[4];
	int     cpt = 0;*/
	static int	nTraceFileLen = 0;
	static int	nExtTraceFileLen = 0;
	static char ExtTraceFile[128];
	static int	nFileSeq = 0;
	
	if( nTraceFileLen == 0 )
	{
		nTraceFileLen = strlen(trace_file) - 3;
		if( nTraceFileLen <= 0 )
		{
			fprintf(stderr,"Invalid Trace file name : [%s]\n", trace_file);
			return NOK;
		}
		/* AZA10012018 */
		if ( isMultiCutoffEnabled() ) /* ATH04062018*/ /* ISA17082018 */
		{	
			if ( strcmp(group_code,"") != 0 ) /* ISA17082018 */
				sprintf(ExtTraceFile,"%s/%s/%s", getenv("TRACE"), group_code, trace_file);
			else
				sprintf(ExtTraceFile,"%s/%s", getenv("TRACE"), trace_file);
		/* ATH04062018 START */
		}
		else
			sprintf(ExtTraceFile,"%s/%s", getenv("TRACE"), trace_file);
		/* ATH04062018 END */
		
		nExtTraceFileLen = strlen(ExtTraceFile) - 3;
	}
	/*
	memset(cpt_char, 0, sizeof(cpt_char));
	memcpy(cpt_char, trace_file + strlen(trace_file) - 3, 3);
	cpt = atoi(cpt_char);

	while(1)
	{
		sprintf(fichier_trace_etendu,"%s/%s",getenv("TRACE"), trace_file);
		sprintf(fichier_trace_etendu+strlen(fichier_trace_etendu)-3,"%03d",cpt);

		retour = stat(fichier_trace_etendu, &buf_stat);
		if ( retour < 0) 
		{
			break;
		}
		if (buf_stat.st_size < max_size * 1024) 
		{
			break;
		}

		cpt++;
	}
	*/
	while(1)
	{
		sprintf(fichier_trace_etendu,"%s",ExtTraceFile);
		sprintf(fichier_trace_etendu + nExtTraceFileLen,"%03d",nFileSeq);
		
		retour = stat(fichier_trace_etendu, &buf_stat);
		if ( retour < 0) 
		{
			break;
		}
		if (buf_stat.st_size < max_size * 1024) 
		{
			break;
		}

		nFileSeq++;
	}
	

	fd_trace = open(fichier_trace_etendu, O_RDWR|O_CREAT|O_APPEND, 0640);

	if (fd_trace < 0)
	{
		fprintf(stdout,"FICHIER : %s\n", fichier_trace_etendu);
		fprintf(stdout,"ERRNO :%d\n", errno);
		return(NOK);
	}
	
	sprintf(trace_file + nTraceFileLen, "%03d", nFileSeq);
	return(OK);
}

/* AZA10012018 */
static int check_trace_size_mco(/*int fd_trace,*/ int max_size)
{
	struct  stat buf_stat;
	int     retour;
	char    fichier_trace_etendu[64];

	/* AZA10012018 */
	

	if ( memcmp( group_code , "DFLGRP" , 6 ) != 0 || isMultiCutoffEnabled() ) /* ATH04062018*/
		sprintf(fichier_trace_etendu,"%s/%s/%s", getenv("TRACE"), group_code, trace_file);
	/* ATH04062018 START */
	else
		sprintf(fichier_trace_etendu,"%s/%s", getenv("TRACE"), trace_file);
	/* ATH04062018 END */
	
	
	retour = stat(fichier_trace_etendu, &buf_stat);

   /* retour = fstat(fd_trace, &buf_stat); */
/* End HY 270302 : Le cas ou le fichier est supprimer */

	if (retour < 0)
	{
		fprintf(stderr,"Errno stat file, errno : %d,file:[%s]\n", errno,fichier_trace_etendu);
		return(NOK);
	}

	if (buf_stat.st_size >= max_size * 1024) return(NOK);

	return(OK);
}

/* AZA10012018 */
void trace_event_mco(char *mess, int level)
{
	int     retour;
	char    ligne[MAX_LINE_TRC + 1];
	char    date [14 + 1];
	char    Mls  [ 4];
	char    Mls_tmp  [ 7];
	struct timeval tp;

	int   i;
	int	nLength;

	memset(date, 0, sizeof(date));
	memset(ligne, 0, sizeof(ligne));
	memset(Mls, 0, sizeof(Mls));
	memset(Mls_tmp, 0, sizeof(Mls_tmp));


	if (level > trace_level)
	{
		return;
	}

	/* no need now as we are using mutex */
	if( fd_trace <= 0 )
		return;


	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);

	sprintf(Mls_tmp,"%06ld",tp.tv_usec);
	memcpy(Mls,Mls_tmp,3);

	sprintf(ligne,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|%d| %.93s",
		date,date+2,date+8,date+10,date+12,Mls,getpid(),p7_thread_id(),level,mess);

	nLength = strlen(ligne);
	if( nLength < 130 )
	{
		memset(ligne + nLength,' ',130 - nLength );
	}
	memcpy(ligne+130,".\n",2);


	/*EBE151127: put back mutex locks*/
	pthread_mutex_lock(&trace_mutex);
	
	retour = check_trace_size_mco( max_size);
	if (retour != OK)
	{
		close(fd_trace);
		fd_trace = -1;
		init_trace_mco();
	}

	write(fd_trace,ligne, 132);
	pthread_mutex_unlock(&trace_mutex);

	return;
}

/*PLT_PERF*/
void  init_trace_level()
{
	gs_trace_level = trace_level;
	
}
