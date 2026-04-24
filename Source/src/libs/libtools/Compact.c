#include<stdio.h>

main()
{
unsigned char   pin[17];
unsigned char   pinCompacted[17];
   char      sLine[132];
   int      cpt;


memset(pin, 0, sizeof(pin));
memset(pinCompacted, 0, sizeof(pinCompacted));

memcpy(pin, "7F5CA0AF",8);

fprintf(stderr, "Start CompactPIN(%.16s) \n", pin);

   for(cpt = 0; cpt < 8; cpt ++)
   {
      pinCompacted[cpt] = ((pin[cpt*2   ] & 0x0F) << 4)
          | (pin[cpt*2 + 1] & 0x0F);
   }
fprintf(stderr, "End   CompactPIN(%.16s) \n", pinCompacted);
   return ;
}
