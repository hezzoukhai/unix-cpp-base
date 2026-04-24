/******************************************************************************/
/* Nom : x_or.c                                                               */
/******************************************************************************/
#include <stdio.h>
#include <string.h>
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
/*
main()
{
char sortie0[17];
char sortie1[17];
char sortie2[17];
char sortie3[17];
char sortie4[17];
char sortie5[17];
char sortie6[17];
char sortie7[17];

char block1[17];
char block2[17];
char block3[17];

char pbk_clair[17];
char pbk_encrypte[17];

memcpy(block1,"014F8FD0B031209D",16);
memcpy(block2,"014F8FD0B031209D",16);
memcpy(block3,"014F8FD0B031209D",16);

x_or(block1,block2,sortie0);
x_or(sortie0,block3,sortie1);
printf("ZMK : %.16s\n",sortie1);

DES(1, 'H', "0000000000000000", sortie1, sortie2);
printf("KCV : %.16s\n",sortie2);
}
*/
