/*****************************************************************************/
/* (c) Hightech Payment Systems 2000                                         */
/*     Declaration des variables necessaires au fonctionnement du module de  */
/*     trace.                                                                */
/*****************************************************************************/

#ifndef HPS_DUMP_H
#define HPS_DUMP_H

#define TRACE_DIRECTORY "/pcard5/v22dev/usr/trace"

/*EBE140421: Adding extern to "explicitly not define" those variables by including this header file */
extern char		trace_file   [];	/* Nom externe du fichier de trace */
extern char		group_code   [];	/* AZA10012018: Nom externe du groupe de banques */ 
extern int		max_size;            /* Taille max du fichier de trace  */
extern int		trace_level;         /* Niveau de trace du module       */
extern int		fd_trace;            /* Identifiant du fichier trace    */
extern int		debug_option;

void InitDumpParam	();
void trace_event     (char * mess,   int level);
void trace_event_mco (char * mess,   int level); /* AZA10012018 */
void dump_buffer     (unsigned char * Buffer,int SizeBuffer, char Set, char origine, char sens);
void error_to_console(char *source, int severity, char *message, char *fix);

unsigned long int p7_thread_id();

#define TRACE_FILE_LEN			64

#endif /** HPS_DUMP_H **/
