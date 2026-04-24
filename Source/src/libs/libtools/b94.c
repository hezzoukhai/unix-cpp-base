/* some very basic public-domain base64 functions */

/**************************************************/
/**			Mehdi Elyajizi 													 **/
/**     Functions : Encode / Decode in BASE 64.  **/
/**     	- b64_encode :												 **/
/**						Args in : unsigned char * str.		 **/	
/**											int len 								 **/
/**						Args out: unsigned char *out.			 **/
/**       - b64_decode :												 **/
/**						Args in : unsigned char * str.		 **/	
/**************************************************/


#include <b64.h>
#include <stdio.h>
#include <define.h>


void base94_encode_asc(const unsigned char *sSrc, int nSrcLen, unsigned char *sDst)
{
  unsigned char src_data[LG_MAX]; 
  unsigned char tmp_data[4+1]; 
  unsigned char enc_data[5+1]; 
  unsigned char dst_data[LG_MAX]; 
  unsigned long long acc;
  int nCtr, i, nOffset=0;
  
  trace_event("Start base94_encode_asc", TRACE);

  memset(src_data, 0, sizeof(src_data));
  memset(enc_data, 0, sizeof(enc_data));
  memset(dst_data, 0, sizeof(dst_data));
  memset(tmp_data, 0, sizeof(tmp_data));

  AsciiToHex (sSrc, src_data, nSrcLen);

  for (nCtr = 0; nCtr < (nSrcLen/2); )
  {
     acc = 0;

     memset(enc_data, 0, sizeof(enc_data));
     memset(tmp_data, 0, sizeof(tmp_data));
     memcpy(tmp_data, src_data + nCtr, 4);

     for (i = 0; i < 4; i++)
     {
        acc = acc * 256 + tmp_data[3 - i];
     }
     
     for (i = 0; i < 5; i++)
     {
        enc_data[i] = acc % 94 + 32;
        acc = acc / 94;
     }

     memcpy(dst_data + nOffset, enc_data, 5);
     nOffset += 5;
 
     nCtr+=4;
  }
  memcpy(sDst, dst_data, nOffset);
  
  trace_event("End   base94_encode_asc", TRACE);

}

void base94_encode_bin(const unsigned char *sSrc, int nSrcLen, unsigned char *sDst)
{
  unsigned char src_data[LG_MAX]; 
  unsigned char tmp_data[4+1]; 
  unsigned char enc_data[5+1]; 
  unsigned char dst_data[LG_MAX]; 
  unsigned long long acc;
  int nCtr, i, nOffset=0;
  
  trace_event("Start base94_encode_bin", TRACE);

  memset(src_data, 0, sizeof(src_data));
  memset(enc_data, 0, sizeof(enc_data));
  memset(dst_data, 0, sizeof(dst_data));
  memset(tmp_data, 0, sizeof(tmp_data));

  /*AsciiToHex (sSrc, src_data, nSrcLen);*/    
  memcpy(src_data, sSrc, nSrcLen);

  for (nCtr = 0; nCtr < nSrcLen; )
  {
     acc = 0;

     memset(enc_data, 0, sizeof(enc_data));
     memset(tmp_data, 0, sizeof(tmp_data));
     memcpy(tmp_data, src_data + nCtr, 4);

     for (i = 0; i < 4; i++)
     {
        acc = acc * 256 + tmp_data[3 - i];
     }
     
     for (i = 0; i < 5; i++)
     {
        enc_data[i] = acc % 94 + 32;
        acc = acc / 94;
     }

     memcpy(dst_data + nOffset, enc_data, 5);
     nOffset += 5;
 
     nCtr+=4;
  }
  memcpy(sDst, dst_data, nOffset);
  
  trace_event("End   base94_encode_bin", TRACE);

}

void base94_decode(const unsigned char *sSrc, int nSrcLen, unsigned char *sDst)
{
  char          sLine[MAX_LINE_TRC];
  char          sAsciiHex[2+1];
  unsigned char src_data[LG_MAX]; 
  unsigned char tmp_data[5+1]; 
  unsigned char enc_data[4+1]; 
  unsigned char dst_data[LG_MAX]; 
  unsigned long long acc;
  int nCtr, i, nOffset=0;
  
  trace_event("Start base94_decode", TRACE);

  memset(src_data, 0, sizeof(src_data));
  memset(enc_data, 0, sizeof(enc_data));
  memset(dst_data, 0, sizeof(dst_data));
  memset(tmp_data, 0, sizeof(tmp_data));
  memset(sLine,    0, sizeof(sLine));
  memset(sAsciiHex,0, sizeof(sAsciiHex));

  /*AsciiToHex (sSrc, src_data, nSrcLen);*/
  memcpy(src_data, sSrc, nSrcLen);

  for (nCtr = 0; nCtr < nSrcLen; )
  {
     acc = 0;

     memset(enc_data, 0, sizeof(enc_data));
     memset(tmp_data, 0, sizeof(tmp_data));
     memcpy(tmp_data, src_data + nCtr, 5);

     for (i = 0; i < 5; i++)
     {
        acc = acc * 94 + tmp_data[4 - i] - 32;
     }
     
     for (i = 0; i < 4; i++)
     {
        enc_data[i] = acc % 256;
        acc = acc / 256;
     }

     memcpy(dst_data + nOffset, enc_data, 4);
     nOffset += 4;
 
     nCtr+=5;
  }

  nCtr = 0;
  for (i=0; i<nOffset; i++)
   {
     memset(sAsciiHex,0, sizeof(sAsciiHex));
     sprintf(sAsciiHex, "%02X", dst_data[i]);
     memcpy(sDst + nCtr, sAsciiHex, 2);
     nCtr += 2;
   }
  
  /*HexToAscii(dst_data, sDst, nOffset);*/
  /*memcpy(sDst, dst_data, nOffset);*/
  
  trace_event("End   base94_decode", TRACE);

}