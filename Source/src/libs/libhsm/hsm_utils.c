#include <stdio.h>
#include <signal.h>
#include <hsm_inc.h>
#include <dump.h>
#include <queue.h>
#include <commands.h>
#include <event_auth.h>
#define _EXT_VAR_
#include <thr_hsm.h>
#undef _EXT_VAR_



/** Ajout de la fonction HsmQueryBin le 10/10/2003*/

/******************************************************************************/
char decomp_f(w)
char w;
{
   if ((w>=0) && (w<=9)) return(w+0x30);
   return(w+0x37);
}


/******************************************************************************/
void decomp(x,r)
char x;
char *r;
{
   *(r+1) = decomp_f((char)(x & 0x0F));
   *r     = decomp_f((char)((x & 0xF0) >> 4));
}


/******************************************************************************/
void decomp_all(A,B)
char A[8];
char B[16];
{
   int i;

   for(i=0;i<8;i++) decomp(A[i],B+(2*i));
}

/******************************************************************************/

void decimalisation(table_decimalisation,cle)
char table_decimalisation[17];
char cle[17];
{
   int i;
   int indice_decimal;

   for(i=0;i<16;i++)
   {
      if (cle[i] > 0x39) indice_decimal = cle[i] - 0x41 + 10;
      else indice_decimal = cle[i] - 0x30;
      cle[i] = table_decimalisation[indice_decimal];
   }
}

/******************************************************************************/

unsigned char comp_g(w)
unsigned char w;
{
   if ((w>='0') && (w<='9')) return(w-0x30);
   return(w-0x37);
}


/******************************************************************************/
void comp(x,r)
unsigned char *x;
unsigned char *r;
{
   *r = comp_g(x[1]) + (16*comp_g(x[0]));
}


/******************************************************************************/
void comp_all(A,B)
unsigned char A[16];
unsigned char B[8];
{
   int i;

   for(i=0;i<8;i++) comp(A+(2*i),(B+i));
}


/******************************************************************************/
void x_or(entre1,entre2,sortie)
unsigned char entre1[16];
unsigned char entre2[16];
unsigned char sortie[16];
{
   int i;
   unsigned char Entree1[8];
   unsigned char Entree2[8];
   unsigned char Sortie[8];

   comp_all(entre1,Entree1);
   comp_all(entre2,Entree2);
   for(i=0;i<8;i++) *(Sortie+i)=*(Entree1+i)^*(Entree2+i);
   decomp_all(Sortie,sortie);
}


/******************************************************************************/
numeric_extract(entree,sortie)
char *entree,
*sortie;
{
   int i, j;

   i = 0;
   j = 0;

   memset(sortie,0,17);
   for(i=0;i<16;i++)
   {
      if (!isdigit(entree[i])) continue;
      sortie[j] = entree[i];
      j++;
   }
}
/******************************************************************************/
/* expand_pin.                                                                */
/* ----------                                                                 */
/******************************************************************************/
void expand_pin(crypt_pin, pin_block)
unsigned char           *crypt_pin;
unsigned char           *pin_block;
{
   int             i;
   unsigned char   a,b;
   char ligne[MAX_LINE_TRC];

   /*sprintf(ligne, "Start expand_pin(%.8s)", pin_block);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start expand_pin()", PROCESSING);


   for(i = 0 ; i < 8 ; i++)
   {

      a = (*(pin_block + i) >> 4);
      b = (*(pin_block + i) & 0x0F);

      *(crypt_pin + (2 * i))     = a;
      *(crypt_pin + (2 * i + 1)) = b;
   }

   for (i=0;i<16;i++)
   {
      if ( ( *(crypt_pin + i) >= 0x00 ) && ( *(crypt_pin + i) <= 0x09) )
         *(crypt_pin + i) += 0x30;
      else
         if ( *(crypt_pin + i) == 0x0A ) *(crypt_pin + i) = 0x41;
         else
            if ( *(crypt_pin + i) == 0x0B ) *(crypt_pin + i) = 0x42;
            else
               if ( *(crypt_pin + i) == 0x0C ) *(crypt_pin + i) = 0x43;
               else
                  if ( *(crypt_pin + i) == 0X0D ) *(crypt_pin + i) = 0x44;
                  else
                     if ( *(crypt_pin + i) == 0X0E ) *(crypt_pin + i) = 0x45;
                     else
                        if ( *(crypt_pin + i) == 0X0F ) *(crypt_pin + i) = 0x46;
                        else *(crypt_pin + i) = 0x00;
   }

   /*sprintf(ligne, "End   expand_pin(%.16s)", crypt_pin);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("End   expand_pin(OK)", PROCESSING);
   return;
}
/******************************************************************************/
/* Fonction compress_pin.                             */
/* ---------------------                                                 */
/******************************************************************************/
void    compress_pin(target, source)
char    *target;
char    *source;
{
   int    i;
   unsigned char   a,b,c;

   trace_event("Start compress_pin()", PROCESSING);

   for(i = 0; i < 8; i++)
   {
      a = *(source + 2 * i);
      b = *(source + 2 * i + 1);
      c = 0x00;

      if (a == 0x41) a = 0x0A;
      else
         if (a == 0x42) a = 0x0B;
         else
            if (a == 0x43) a = 0x0C;
            else
               if (a == 0x44) a = 0x0D;
               else
                  if (a == 0x45) a = 0x0E;
                  else
                     if (a == 0x46) a = 0x0F;

      if (b == 0x41) b = 0x0A;
      else
         if (b == 0x42) b = 0x0B;
         else
            if (b == 0x43) b = 0x0C;
            else
               if (b == 0x44) b = 0x0D;
               else
                  if (b == 0x45) b = 0x0E;
                  else
                     if (b == 0x46) b = 0x0F;

      c = (unsigned char) ((a << 4) & 0xF0) | (b & 0x0F);

      *(target +  i)  = c;
   }

   trace_event("End   compress_pin()", PROCESSING);
   return;
}
/******************************************************************************/
/* calc_pin.                                                                  */
/* ----------                                                                 */
/******************************************************************************/
int  calc_pin(int nIndiceCtx, char *card, char *pin_key, char *pin)
{
   int  i, retour;
   char    sHsmResultCode [5];

   char sortie[17], entree[17], Buffer[512], Buffer_hsm_recu[512];
   char table_de_decimalisation[]= {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5'   };

   trace_event("Start calc_pin()",PROCESSING);
   memset(sHsmResultCode , 0, sizeof(sHsmResultCode));
   memset(sortie, 0, sizeof(sortie));
   memset(entree, 0, sizeof(entree));
   memcpy(entree, "00000000", 8);
   memcpy(entree + 8, card + strlen(card) - 8, 8);

   command_HE(Buffer, pin_key, entree);
   retour = HsmQuery(nIndiceCtx, Buffer, Buffer_hsm_recu);
   if(retour == NOT_AVAILABLE)
   {
      trace_event("HSM NOT AVAILABLE", ERROR );
      return(NOK);
   }

   else if(retour == HSM_MALFUNCTION)
   {
      trace_event("MALFUNCTION ERROR",ERROR);
      return(NOK);
   }

   retour = command_HF(Buffer_hsm_recu, sortie, sHsmResultCode);
   if ( retour != OK ) return(NOK);


   for(i=0 ; i<PINLENGTH ;i++)
   {
      if (sortie[i] == 'A') sortie[i] = '0';
      else
         if (sortie[i] == 'B') sortie[i] = '1';
         else
            if (sortie[i] == 'C') sortie[i] = '2';
            else
               if (sortie[i] == 'D') sortie[i] = '3';
               else
                  if (sortie[i] == 'E') sortie[i] = '4';
                  else
                     if (sortie[i] == 'F') sortie[i] = '5';
   }

   memcpy(pin, sortie, PINLENGTH);

   trace_event("End   calc_pin()",PROCESSING);
   /*SKO290509*/ return(0);
}


/******************************************************************************/
/* calc_pin_block.                                                            */
/* ----------                                                                 */
/******************************************************************************/

calc_pin_block(int nIndiceCtx, char *card, char *pin_key, char *pin_block)
{
   char pin[5];
   char value[3];
   char entree1[17];
   char entree2[17];

   trace_event("Start calc_pin_block()",PROCESSING);

   memset(entree1, 0, sizeof(entree1));
   memset(entree2, 0, sizeof(entree2));
   memset(pin,     0, sizeof(pin));
   memset(value,   0,             3);

   calc_pin(nIndiceCtx, card, pin_key, pin);

   memset(entree1, 'F',   16);
   sprintf( value , "%02d" , PINLENGTH );
   memcpy(entree1,  value,            2);
   memcpy(entree1+2, pin,     PINLENGTH);

   memcpy(entree2,   "0000",                 4);
   memcpy(entree2+4, card+strlen((char*)card) - 13, 12);
   x_or((unsigned char *)entree1, (unsigned char *)entree2, (unsigned char *)pin_block);

   trace_event("End   calc_pin_block(VOID)",PROCESSING);
   return;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int HsmQuery( int nIndiceCtx, pid_t nHsmPid , char *BuffSnd , char *BuffRcv )
{
    int        BalStatus;
    TSHsmExchange   sBalMessage;
	char			sLine[MAX_LINE_TRC];

   
	memset((char *)&sBalMessage, 0, sizeof(TSHsmExchange));
    memset( tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal)); 
	sBalMessage.sHeader.nIndex     = nIndiceCtx;
	sBalMessage.sHeader.tid        = pthread_self();
	sBalMessage.sHeader.nCoNumber  = GetNextCoNumber(nIndiceCtx);

	sBalMessage.sHeader.nPartnerId = nHsmPid;
    sBalMessage.sHeader.nSourceId  = getpid();
    sBalMessage.sHeader.nCommand   = HSM_REQUEST;
    memcpy ( sBalMessage.sBody, BuffSnd , strlen ( BuffSnd ));
    sBalMessage.sHeader.nLen =  strlen ( BuffSnd );

	/*sprintf(sLine,"TO HSM:%d-->%s|",sBalMessage.sHeader.nLen, sBalMessage.sBody);
	trace_event(sLine,TRACE);*/ /** HM - 09022010 - PA_DSS **/

    if ( WriteBalHsm ( nBalHsm, &sBalMessage) != OK)
    {
       trace_event("Can Not Write Message To Hsm Queue", PROCESSING);
       trace_event ( "End  HsmQuery ( NOK )", PROCESSING );
       return( HSM_MALFUNCTION );
    }

   memset ( (char *)&sBalMessage , 0, sizeof ( TSHsmExchange ));

   BalStatus = WaitHsmResponse(nIndiceCtx, HSM_TIME_OUT_VALUE, BuffRcv);

   if ( BalStatus  != OK ) 
   {
      trace_event ("Time Out Situation With HSM", PROCESSING);
      trace_event ( "End HsmQuery ( NOK )", PROCESSING );
      return ( NOT_AVAILABLE );
   }
	/*sprintf(sLine,"FROM HSM: <--%s|",BuffRcv);
	trace_event(sLine,TRACE);*/ /** HM - 09022010 - PA_DSS **/

   trace_event ( "End HsmQuery ( OK )", PROCESSING );
   return ( OK );
}


int HsmQueryBin( int nIndiceCtx, pid_t nHsmPid , char *BuffSnd ,int nLength,  char *BuffRcv )
{
    int        BalStatus;
    TSHsmExchange   sBalMessage;
	char			sLine[MAX_LINE_TRC];

   
   trace_event ( "Start HsmQueryBin ()", PROCESSING );

	memset((char *)&sBalMessage, 0, sizeof(TSHsmExchange));
    memset( tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal)); 
	sBalMessage.sHeader.nIndex     = nIndiceCtx;
	sBalMessage.sHeader.tid        = pthread_self();
	sBalMessage.sHeader.nCoNumber  = GetNextCoNumber(nIndiceCtx);

	sBalMessage.sHeader.nPartnerId = nHsmPid;
    sBalMessage.sHeader.nSourceId  = getpid();
    sBalMessage.sHeader.nCommand   = HSM_REQUEST;
    memcpy ( sBalMessage.sBody, BuffSnd , nLength);
    sBalMessage.sHeader.nLen =  nLength;


	sprintf(sLine, "GLOBAL PartnerID = %d; SourceID = %d",nHsmPid , getpid());
	trace_event(sLine,TRACE);
	
	
	sprintf(sLine,"TO HSM:%d-->%s|",sBalMessage.sHeader.nLen, sBalMessage.sBody);
	trace_event(sLine,TRACE);


    if ( WriteBalHsm ( nBalHsm, &sBalMessage) != OK)
    {
       trace_event("Can Not Write Message To Hsm Queue", PROCESSING);
       trace_event ( "End  HsmQuery ( NOK )", PROCESSING );
       return( HSM_MALFUNCTION );
    }

   memset ( (char *)&sBalMessage , 0, sizeof ( TSHsmExchange ));

   BalStatus = WaitHsmResponse(nIndiceCtx, HSM_TIME_OUT_VALUE, BuffRcv);

   if ( BalStatus  != OK ) 
   {
      trace_event ("Time Out Situation With HSM", PROCESSING);
      trace_event ( "End HsmQueryBin ( NOK )", PROCESSING );
      return ( NOT_AVAILABLE );
   }
	/*sprintf(sLine,"FROM HSM: <--%s|",BuffRcv);
	trace_event(sLine,TRACE);*/ /** HM - 09022010 - PA_DSS **/

   trace_event ( "End   HsmQueryBin ( OK )", PROCESSING );
   return ( OK );
}
