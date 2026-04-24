#include <stdio.h>


main (int argc, char **argv)
{
int i;
unsigned char crypt_pin[17];
unsigned char pin_block[9];
unsigned char a;
unsigned char b;
char detail[132];

memset(pin_block,0,sizeof(pin_block));
memset(crypt_pin,0,sizeof(crypt_pin));
/*
pin_block[0] = 0X;
pin_block[1] = 0X;
pin_block[2] = 0X;
pin_block[3] = 0X;
pin_block[4] = 0X;
pin_block[5] = 0X;
pin_block[6] = 0X;
pin_block[7] = 0X;
*/

memcpy(pin_block,"7F5CA0AF",8);



fprintf(stderr, "Start expand_pin(%.8s) \n", pin_block);

for(i = 0 ; i < 8 ; i++)
{
a = (*(pin_block + i) >> 4);
b = (*(pin_block + i) & 0x0F);
*(crypt_pin + (2 * i)) = a;
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
fprintf(stderr, "End expand_pin(%.16s)\n", crypt_pin);
return;
}

