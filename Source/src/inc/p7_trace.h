#ifndef P7_TRACE_H
#define P7_TRACE_H



/*
typedef enum {
	P7_TL_TRACE			= (1<<0)	,
	P7_TL_FATAL			= (1<<1)	,
	P7_TL_ERROR			= (1<<2)	,
	P7_TL_WARNING		= (1<<3)	,
	P7_TL_PROCESSING	= (1<<4)	,
	P7_TL_STREAM		= (1<<5)	,

} E_TRACE_LEVEL;
*/

/*
FORCE   	0			-> Traces that need to be always printed
FATAL   	1			-> Process will stop( because of fatal exception/error or through an administrative command)
ERROR		2			-> Processing error(unexpected exception: disconnection) but the process continues to run
DATA 		3			-> (WARNINGS)Data Error(e.g malformated msg, unexpected value, mandatory fields...)
INFO		4			-> unexpected behavior which is not an error: card found in saf, standin processing,...etc
TRACE      	5			-> DumpIso/DumpNw + traces which are not too frequent but informative
PROCESSING 	6			-> Start function/End function/function parameters IN&OUT/PrintTlvBuffer
STREAM    	7			-> intermediate processing data traces
*/

typedef enum {
	P7_TL_FORCE 		= 0 ,
	P7_TL_FATAL			    ,
	P7_TL_ERROR				,
	P7_TL_DATA				,
	P7_TL_INFO				,
	P7_TL_TRACE				,
	P7_TL_PROCESSING		,
	P7_TL_STREAM			,

} E_TRACE_LEVEL;

extern  int		is_trace_level_on(E_TRACE_LEVEL level);

/****
extern int					init_trace(char* trace_file);
extern void					trace_event(char* mess, E_TRACE_LEVEL level);
extern void					trace_message	(E_TRACE_LEVEL level, const char* fmt,...);
extern void					dump_buffer     (unsigned char * buffer,int nSize, char Set, char origine, char sens);
extern void					backtrace();
extern void					close_trace();
**/


#endif
