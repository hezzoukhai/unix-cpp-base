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


int isMultiCutoffEnabled()
{
	return 0;
}

/*ATH04062018 END */

/******************************************************************************/
#define BUFFER_SIZE				(1024*1024)				/* 1Mb */
#define FILE_NAME_SIZE			256
#define TRACE_LINE_LEN			100
#define MAX_ALLOWED_SIZE		(4096*1024)

/******************************************************************************/

#ifndef P7_IMPORT_SYM
pthread_mutex_t	gs_buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

char			gs_trace_file   [  FILE_NAME_SIZE + 1];
size_t			gs_max_size;
int				gs_trace_level = 0;
int				gs_fd_trace = -1;

char			gs_trace_buffer[BUFFER_SIZE];
size_t			gs_buffer_offset = 0;
int				gs_sleep_timer=1;
pid_t			gs_proc_pid;

char		group_code[6 + 1];

#else

extern pthread_mutex_t	gs_buffer_mutex;

extern char				gs_trace_file   [ FILE_NAME_SIZE + 1];
extern size_t			gs_max_size;
extern int				gs_trace_level;
extern int				gs_fd_trace;

extern char				gs_trace_buffer[BUFFER_SIZE + 132];
extern size_t			gs_buffer_offset;
extern int				gs_sleep_timer;
extern pid_t			gs_proc_pid;
extern char				group_code[ ];
#endif

/*Old lib variables*/
P7_DEF_SYM int		debug_option;						/*EBE120925: definition of debug_option. Check reference in dump.h */
P7_DEF_SYM char		trace_file[FILE_NAME_SIZE + 1];		/* Nom externe du fichier de trace */
P7_DEF_SYM int		max_size;							/* Taille max du fichier de trace  */
P7_DEF_SYM int		trace_level;						/* Niveau de trace du module       */

P7_DEF_SYM int		fd_trace;							/* Identifiant du fichier trace    */

/******************************************************************************/
/******************************************************************************/

int is_trace_level_on(E_TRACE_LEVEL level)
{
	if( gs_trace_level&level )
		return OK;
	return NOK;
}

void InitDumpParam()
{
	memset(trace_file, 0, sizeof(trace_file));
	trace_level = STREAM;
}
/******************************************************************************/
static int init_next_trace_file()
{
	char			extendend_trace_file[FILE_NAME_SIZE + 1];
	struct stat		buf_stat;
	int				result;
	char			buf[512];
	char*			spTracePath;
	static int	nTraceFileLen = 0;
	static int	nExtTraceFileLen = 0;
	static char ExtTraceFile[FILE_NAME_SIZE + 1];
	static int	nFileSeq = 0;


	if( gs_fd_trace >= 0 )
	{
		close(gs_fd_trace);
	}

	spTracePath = getenv("TRACE");

	if( nTraceFileLen == 0 )
	{
		nTraceFileLen = strlen(gs_trace_file) - 3;
		if( nTraceFileLen <= 0 )
		{
			fprintf(stderr,"Invalid Trace file name : [%s]\n", gs_trace_file);
			return NOK;
		}
		sprintf(ExtTraceFile,"%s/%s",spTracePath, gs_trace_file);
		nExtTraceFileLen = strlen(ExtTraceFile) - 3;
	}

	/* On entry, lock should be held by calling function */

	while(1)
	{
		sprintf(extendend_trace_file,"%s",ExtTraceFile);
		sprintf(extendend_trace_file + nExtTraceFileLen,"%03d",nFileSeq);

		result = stat(extendend_trace_file, &buf_stat);
		if ( result < 0) 
		{
			break;
		}
		
		/*PLT_PERF_2*/
		if(gs_max_size > MAX_ALLOWED_SIZE)
		{ 
			gs_max_size = MAX_ALLOWED_SIZE;
			fprintf(stderr,"over size");
		}
		
		if (buf_stat.st_size < gs_max_size) 
		{
			break;
		}

		nFileSeq++;
	}


	gs_fd_trace = open(extendend_trace_file, O_RDWR|O_CREAT|O_APPEND, 0640);
	if (gs_fd_trace < 0)
	{
		fprintf(stderr,"Unable to open file : %s\n", extendend_trace_file);
		fprintf(stderr,"errno :%d\n", errno);
		return(NOK);
	}

	fd_trace = gs_fd_trace; /* needed by PrintTlvBuffer ... */

	sprintf(buf,"File FD: %d\n",gs_fd_trace);
	write(gs_fd_trace,buf,strlen(buf));
	sprintf(gs_trace_file + nTraceFileLen, "%03d", nFileSeq);


	return OK;

}
/******************************************************************************/
int flush_trace_file()
{
	struct stat	buf_stat;
	int			left_size;
	int			nResult;
	int			nRemainingFreeSpace;
	int			nSizeBlk1;
	int			nSizeBlk2;
	char		extendend_trace_file[FILE_NAME_SIZE + 1];


	pthread_mutex_lock(&gs_buffer_mutex);


	/*nResult = fstat(gs_fd_trace,&buf_stat);*/

	sprintf(extendend_trace_file,"%s/%s",getenv("TRACE"), gs_trace_file);
	nResult = stat(extendend_trace_file, &buf_stat);
	if( nResult < 0 )
	{
		fprintf(stderr,"Errr: fstat(ret:%d)(errono:%d,fd:%d)\n",nResult,errno,gs_fd_trace);
		init_next_trace_file();
		write(gs_fd_trace,gs_trace_buffer,gs_buffer_offset);
		gs_buffer_offset = 0;
		pthread_mutex_unlock(&gs_buffer_mutex);
		return OK;
	}


		/*PLT_PERF_2*/
	if(gs_max_size > MAX_ALLOWED_SIZE)
	{ 
		gs_max_size = MAX_ALLOWED_SIZE;
		fprintf(stderr,"over size");
	}
		
	nRemainingFreeSpace = gs_max_size - buf_stat.st_size;
	if(  nRemainingFreeSpace <= 0 )
	{
		init_next_trace_file();

		write(gs_fd_trace,gs_trace_buffer,gs_buffer_offset);
		gs_buffer_offset = 0;
		
		pthread_mutex_unlock(&gs_buffer_mutex);
		return OK;
	}

	if( nRemainingFreeSpace < gs_buffer_offset)
	{
		nSizeBlk1 = nRemainingFreeSpace;
		nSizeBlk2 = gs_buffer_offset - nRemainingFreeSpace;

		write(gs_fd_trace,gs_trace_buffer,nSizeBlk1);

		init_next_trace_file();

		write(gs_fd_trace,gs_trace_buffer + nSizeBlk1,nSizeBlk2);
		gs_buffer_offset = 0;

	}
	else
	{
		write(gs_fd_trace,gs_trace_buffer,gs_buffer_offset);
		gs_buffer_offset = 0;
	}



	pthread_mutex_unlock(&gs_buffer_mutex);


	return OK;
}
/******************************************************************************/
static void* thread_trace(void* arg)
{

	while(1)
	{
		sleep(gs_sleep_timer);
		flush_trace_file();
	}
}

int  init_trace()
{

	gs_max_size = max_size * 1024;
	if( gs_max_size < BUFFER_SIZE )
	{
		fprintf(stderr,"Trace file size should be at least %d Kb\n",BUFFER_SIZE/1024);
		return NOK;
	}

	gs_proc_pid = getpid();
	gs_trace_level = 0;
	gs_fd_trace = -1;
	switch(trace_level)
	{
	case STREAM:
		gs_trace_level |= P7_TL_STREAM;
	case PROCESSING:
		gs_trace_level |= P7_TL_PROCESSING;
	case TRACE:
		gs_trace_level |= P7_TL_TRACE;
	case NOACTION:
		gs_trace_level |= P7_TL_DATA;
	case ERROR:
		gs_trace_level |= P7_TL_ERROR;
	case FATAL:
		gs_trace_level |= P7_TL_FATAL;
	}

	return init_trace_file(trace_file);
}

/*PLT_PERF*/
void  init_trace_level()
{
	gs_trace_level = 0;
	switch(trace_level)
	{
	case FATAL:
		gs_trace_level |= P7_TL_FATAL;
		break;
	case ERROR:
		gs_trace_level |= P7_TL_ERROR;
		break;
	case DATA:
		gs_trace_level |= P7_TL_DATA;
		break;
	case TRACE:
		gs_trace_level |= P7_TL_TRACE;
		break;
	case PROCESSING:
		gs_trace_level |= P7_TL_PROCESSING;
		break;
	case STREAM:
		gs_trace_level |= P7_TL_STREAM;
		break;
	}
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
void trace_event(char* mess, int level)
{
	int				result;
	char			line[MAX_LINE_TRC];
	char			Mls  [ 8];
	struct timeval	tp;
	char			date[24];
	int				i;
	int				offset;
	int				nLength;

	/*TODO or &? */
	if( level > gs_trace_level )
		return;

	memset(date, '\0', sizeof(date));
	memset(Mls, '\0', sizeof(Mls));

	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);

	sprintf(Mls,"%06d",tp.tv_usec);


	sprintf(line,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|%d| %.93s",
		date,date+2,date+8,date+10,date+12,Mls,gs_proc_pid,p7_thread_id(),level,mess);



	nLength = strlen(line);
	if( nLength < 130 )
	{
		memset(line + nLength,' ',130 - nLength );
	}
	nLength = 130;

	memcpy(line+nLength,".\n",2);nLength+=2;


	offset = BUFFER_SIZE - (int)gs_buffer_offset;
	if( offset < nLength || gs_buffer_offset > BUFFER_SIZE )
		flush_trace_file();


	pthread_mutex_lock(&gs_buffer_mutex);
	memcpy(gs_trace_buffer + gs_buffer_offset,line,nLength);
	gs_buffer_offset += nLength;
	pthread_mutex_unlock(&gs_buffer_mutex);

}


void trace_line(char* data, int level)
{
	int				result;
	char			line[MAX_LINE_TRC];
	int				i;
	int				offset;
	int				nLength;

	/*TODO or &? */
	if( level > gs_trace_level )
		return;

	nLength = strlen(data);
	if( nLength > 130 )
	{
		nLength = 130;
	}
	memcpy(line,data,nLength);

	if( nLength < 130 )
	{
		memset(line + nLength,' ',130 - nLength );
	}
	nLength = 130;

	memcpy(line+nLength,".\n",2);nLength+=2;

	offset = BUFFER_SIZE - (int)gs_buffer_offset;
	if( offset < nLength || gs_buffer_offset >= BUFFER_SIZE )
		flush_trace_file();


	pthread_mutex_lock(&gs_buffer_mutex);
	memcpy(gs_trace_buffer + gs_buffer_offset,line,nLength);
	gs_buffer_offset += nLength;
	pthread_mutex_unlock(&gs_buffer_mutex);

}


void trace_message(E_TRACE_LEVEL level, const char* szMessage,...)
{
	char            sLine[MAX_LINE_TRC];
	va_list			arg;
	va_start(arg, szMessage);
	vsnprintf(sLine,MAX_LINE_TRC, szMessage, arg);
	va_end(arg);
	trace_event(sLine,level);
}


void dump_buffer(unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{


	char          	sLine[256];
	char          	sLineTemplate[256];
	char          	entete_ligne[128];
	int				entete_len;

	char          	sWork[LG_MAX*2];
	int           	nLen;
	int				j;

	int				i, nb_ligne, position_hex, position_car;
	int				dump_with;

	char			Mls  [ 8];
	struct timeval tp;
	char			date[24];



	trace_event("Start dump_buffer()",P7_TL_FATAL);


	memset(entete_ligne, 0, sizeof(entete_ligne));
	memset(sLineTemplate, 0, sizeof(sLineTemplate));

	memset(date, '\0', sizeof(date));
	memset(Mls, '\0', sizeof(Mls));
	memset(sLine, ' ', sizeof(sLine)-1);
	memset(sLineTemplate, ' ', sizeof(sLineTemplate)-1);

	if( SizeBuffer > sizeof(sWork) )
	{
		SizeBuffer = sizeof(sWork);
	}


	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);

	sprintf(Mls,"%06d",tp.tv_usec);

	sprintf(entete_ligne,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|D| ",
		date,date+2,date+8,date+10,date+12,Mls,gs_proc_pid,p7_thread_id());
	

	entete_len = strlen(entete_ligne);
	nLen = entete_len;

	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);

	if (Set == 'E')
		EbcdicToAscii(Buffer, sWork, SizeBuffer);
	else
		memcpy(sWork, Buffer, SizeBuffer);


	flush_trace_file();

	pthread_mutex_lock(&gs_buffer_mutex);

	memcpy(sLine + nLen + 38, "START               ", 20);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132); gs_buffer_offset+=132;


	if (origine == 'B')
		memcpy(sLine + nLen + 38, "QUEUE               ", 20);
	else if (origine == 'L')
		memcpy(sLine + nLen + 38, "NETWORK             ", 20);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132); gs_buffer_offset+=132;

	if (sens == 'I')
		memcpy(sLine + nLen + 38, "INCOMING            ", 20);
	else if (sens == 'O')
		memcpy(sLine + nLen + 38, "OUTGOING            ", 20);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132); gs_buffer_offset+=132;

	memcpy(sLine + nLen + 38, "TIMESTAMP           ", 20);
	memcpy(sLine + nLen + 38 + 20, date, 4);
	memcpy(sLine + nLen + 38 + 20 + 4, date + 8, 6);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132); gs_buffer_offset+=132;


	nb_ligne = 0;
	position_hex = 38 + nLen;
	position_car = 0 + nLen;

	dump_with = nLen + (18 * 2) + 2 + (18 * 3);
	for (i = 0, j = 0; i < SizeBuffer; i++, j++)
	{
		if (j == 18)
		{
			j = 0;
			position_hex = 38 + nLen;
			position_car = 0 + nLen;

			memcpy(sLine , entete_ligne, nLen);
			memcpy(sLine + nLen + 36, " |", 2);
			memset(sLine + dump_with, ' ', 132 - dump_with);
			memcpy(sLine + 130, ".\n", 2);

			memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132);
			gs_buffer_offset += 132;

		}

		sprintf(sLine + position_hex, "%02X ", *(Buffer + i));
		position_hex += 3;

		if (isprint((unsigned char)(*(sWork + i))))
			sLine[position_car] = *(sWork + i);
		else
			sLine[position_car] = '.';
		sLine[position_car + 1] = ' ';

		position_car += 2;
	}

	if (j < 18)
	{
		for (i = position_car; i < nLen + 36; i++) sLine[i]= ' ';
		for (i = position_hex; i < 131; i++) sLine[i] = ' ';
	}


	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132);
	gs_buffer_offset += 132;


	memset(sLine, ' ', sizeof(sLine)-1);
	memcpy(sLine, entete_ligne, nLen);
	memcpy(sLine + nLen + 36, " |", 2);
	memcpy(sLine + 130, ".\n", 2);
	memcpy(sLine + nLen + 38, "END                 ", 20);
	memcpy(gs_trace_buffer + gs_buffer_offset,sLine,132);
	gs_buffer_offset += 132;

	pthread_mutex_unlock(&gs_buffer_mutex);


	trace_event("End   dump_buffer(VOID)",P7_TL_FATAL);
	return;
}

void dump_buffer_debug(unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{
	dump_buffer(Buffer,SizeBuffer,Set,origine,sens);
}

void dump_buffer_on_file(int nFileFd, unsigned char *Buffer, int SizeBuffer, char Set, char origine, char sens)
{

	char          ligne[LG_MAX*3];
	char          entete_ligne[60];
	char          sLine[256];
	char          sWork[LG_MAX];
	char			Mls  [ 8];
	struct timeval tp;
	int           nLen;
	int				j;

	int				i, nb_ligne, position_hex, position_car;
	char			date[24 + 1];
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

	gettimeofday(&tp,NULL);

	sprintf(Mls,"%06d",tp.tv_usec);

	memset(entete_ligne, ' ', sizeof(entete_ligne)-1);



	sprintf(entete_ligne,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|D| ",
		date,date+2,date+8,date+10,date+12,Mls,gs_proc_pid,p7_thread_id());

	nLen = strlen(entete_ligne);


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

/******************************************************************************/
/* Fonction init_trace.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de prendre en charge les taille des*/
/*               fichiers trace.                                              */
/* Entree      : Nom physique du fichier trace                                */
/*               Taille maximum du fichier                                    */
/* Sortie      : NOK si erreur                                                */
/*               OK Sinon                                                     */
/* Date/Auteur : 03/10/96 A. ALAOUI                                           */
/* Revision    :                                                              */
/******************************************************************************/
int  init_trace_file(char* trace_file)
{


	pthread_t	trace_tid;

	memset(gs_trace_file, '\0', sizeof(gs_trace_file));

	if( gs_max_size == 0 )
	{
		gs_max_size = BUFFER_SIZE*1024;
	}

	strncpy(gs_trace_file,trace_file,sizeof(gs_trace_file) - 1);
	init_next_trace_file();

	if (pthread_create(&trace_tid, NULL, thread_trace, NULL) < 0) {
		fprintf(stderr, "pthread_create Error %d\n", errno);
		return NOK;
	}

	trace_event("***************************************",P7_TL_TRACE);
	trace_event("**         New Session Started       **",P7_TL_TRACE);
	trace_message(P7_TL_TRACE,	"**    pid[%08d] ppid[%08d]   **",gs_proc_pid,getppid());
	trace_event("***************************************",P7_TL_TRACE);
	flush_trace_file();

	return(OK);
}

void close_trace()
{
	flush_trace_file();
	if( gs_fd_trace >= 0 )
		close(gs_fd_trace);
}


/*EBE130812*/
void dump_backtrace()
{
	char	sLine[MAX_LINE_TRC];

	flush_trace_file();
	sprintf(sLine,"================[Start BACKTRACE]===================");
	write(gs_fd_trace,sLine,strlen(sLine));

#ifdef LINUX
#include <execinfo.h>

	void *buffer[LG_MAX];
	int nptrs=backtrace(buffer, LG_MAX);	
	backtrace_symbols_fd(buffer,nptrs,gs_fd_trace);

#elif  defined(_AIX)
	/* For AIX, use procstack pid */
	char	szCmd[LG_MAX];
	char    fichier_trace_etendu[LG_MAX];
	sprintf(fichier_trace_etendu,"%s/%s",getenv("TRACE"), gs_trace_file);
	sprintf(szCmd,"procstack %d >> %s",getpid(),fichier_trace_etendu);
	system(szCmd);
#endif

	sprintf(sLine,"================[End   BACKTRACE]===================");
	write(gs_fd_trace,sLine,strlen(sLine));

}


int  init_trace_mco()
{
	init_trace();
}

void trace_event_mco(char *mess, int level)
{
	trace_event(mess,level);
}
