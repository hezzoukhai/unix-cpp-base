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

int b64_decode(unsigned char* str)
{
    unsigned char *cur, *start;
    int d, dlast, phase;
    unsigned char c;
    static int table[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
    };

    d = dlast = phase = 0;
    start = str;
    for (cur = str; *cur != '\0'; ++cur )
    {
        /* handle newlines as seperate chunks */
        if(*cur == '\n' || *cur == '\r')
        {
            phase = dlast = 0;
            continue;
        }

        d = table[(int)*cur];
        if(d != -1)
        {
            switch(phase)
            {
            case 0:
                ++phase;
                break;
            case 1:
                c = ((dlast << 2) | ((d & 0x30) >> 4));
                *str++ = c;
                ++phase;
                break;
            case 2:
                c = (((dlast & 0xf) << 4) | ((d & 0x3c) >> 2));
                *str++ = c;
                ++phase;
                break;
            case 3:
                c = (((dlast & 0x03 ) << 6) | d);
                *str++ = c;
                phase = 0;
                break;
            }
            dlast = d;
        }
    }
    *str = '\0';
    return str - start;
}



void b64_encode (unsigned char *str, int len, unsigned char *out)
{
    int i,j;
    unsigned char cur[1024];/* = out;*/
    static char table[64] = {
        'A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X',
        'Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n',
        'o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3',
        '4','5','6','7','8','9','+','/'
    };
		fprintf(stderr, "start b64_encode\n");
		
    for (i = 0,j=0; i < len; i += 3,j+=4)
    {
    	  /* fprintf(stderr, "... 001 [0x%02X->0x%02X]\n",str[0],str[0] >> 2); */
        cur[j+0] = table[str[0] >> 2];
        /*fprintf(stderr, "... 001.0\n");
        fprintf(stderr, "i[%d].j[%d].cur0[%c].str[0]=[0x%02X][%d].str[0]>>2=[0x%02X][%d].table[str[0]>>2]=[%c]\n",
         i, j, cur[j+0],str[0],str[0],str[0] >> 2,str[0] >> 2,table[str[0]>>2]); */
        cur[j+1] = table[((str[0] & 3) << 4) + (str[1] >> 4)];
        cur[j+2] = table[((str[1] & 0xf) << 2) + (str[2] >> 6)];
        cur[j+3] = table[str[2] & 0x3f];
        str += 3;
        			/*	fprintf(stderr, "i[%d].j[%d].cur[0x%02X 0x%02X 0x%02X 0x%02X].[%.4s]\n", i, j, cur[j],cur[j+1],cur[j+2],cur[j+3],cur+j); */
    }
		
   /* fprintf(stderr, "... 00 j=[%d]\n",j);*/
    if (i == len + 1)
        /** *(cur - 1) = '='; **/
        cur[j] = '=';
    else if (i == len + 2)
        /** *(cur - 1) = *(cur - 2) = '='; **/
        cur[j] = cur[j-1] = '=';
        
 /*   fprintf(stderr, "... 003\n"); */
    cur[++j] = '\0';
    
   
  /*  fprintf(stderr, "... 004\n"); */
    /*out = cur;*/
    sprintf(out,"%s",cur);
 /*   fprintf(stderr, "... 005\n");*/
}
