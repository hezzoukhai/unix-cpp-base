#include <string.h>
#include <define.h>
#include <zlib.h>


/*
compress (Bytef *dest, uLongf *destLen,
                                 const Bytef *source, uLong sourceLen));

*/
/*compressBound OF((uLong sourceLen));*/

/*
 *
 * uncompress OF((Bytef *dest, uLongf *destLen,
                                   const Bytef *source, uLong sourceLen));
 */
/*
Z_OK            0
Z_STREAM_END    1
Z_NEED_DICT     2
Z_ERRNO        (-1)
Z_STREAM_ERROR (-2)
Z_DATA_ERROR   (-3)
Z_MEM_ERROR    (-4)
Z_BUF_ERROR    (-5)
Z_VERSION_ERROR (-6)
*/


int  CompressMsg(char  *sSrcBuffer,char	*sDstBuffer,int nLength,int *nLengthDest)
{

	char    sLine[MAX_LINE_TRC];
	int    	nResult;
	uLong 	ucompSize ;
	uLong 	compSize;

	sprintf(sLine,"Start CompressMsg(nLength:[%d])",nLength);
	trace_event(sLine,PROCESSING);

	 ucompSize = nLength + 1; /* "Msg" + NULL delimiter. */
	 compSize = compressBound(ucompSize);

	 nResult	= compress((Bytef *)sDstBuffer, &compSize, (Bytef *)sSrcBuffer, ucompSize);

	 *nLengthDest = compSize;

	if(nResult != Z_OK)
	{
		sprintf(sLine,"End CompressMsg(nResult:[%d])",nResult);
		trace_event(sLine,PROCESSING);
	}

	sprintf(sLine,"End CompressMsg(compSize:[%lu])",compSize);
	trace_event(sLine,PROCESSING);

	return(OK);

}

int  UnCompressMsg(char	*sSrcBuffer,char  *sDstBuffer,int compSize,int *nLength)
{

	char    sLine[MAX_LINE_TRC];
	int    	nResult;
	uLong 	ucompSize ;


	ucompSize= *nLength + 1;

	sprintf(sLine,"Start UnCompressMsg(compSize: %d,%lu)",compSize,ucompSize);
	trace_event(sLine,PROCESSING);

	nResult	=uncompress((Bytef *)sDstBuffer, &ucompSize, (Bytef *)sSrcBuffer, compSize);

	*nLength = ucompSize -1 ;


	if(nResult != Z_OK)
	{
		sprintf(sLine,"End UnCompressMsg(nResult:[%d])",nResult);
		trace_event(sLine,PROCESSING);
	}

	sprintf(sLine,"End UnCompressMsg(ucompSize:[%lu])",ucompSize);
	trace_event(sLine,PROCESSING);

	return(OK);

}
