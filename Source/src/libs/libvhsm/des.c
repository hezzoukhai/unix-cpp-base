/* ---------------------- des.c --------------------------- */
/* Functions and tables for DES encryption and decryption
 */

#include <stdio.h>
#include <string.h>
#include "des.h"

/* -------- 48-bit key permutation ------- */
struct ks    {
    char ki[6];
};

/* ------- two halves of a 64-bit data block ------- */
struct LR    {
    int L;
    int R;
};

static struct ks keys[16];

static void rotate(unsigned char *c, int n);
static int fourbits(struct ks, int s);
static int sixbits(struct ks, int s);
static void inverse_permute(int *op,int *ip,int *tbl,int n);
static void permute(int *op, int *ip, int *tbl, int n);
static int f(int blk, struct ks ky);
static struct ks KS(int n, char *key);
static void swapbyte(int *l);

/* ----------- initialize the key -------------- */
void initKey(char *key)
{
    int i;
    for (i = 0; i < 16; i++)
        keys[i] = KS(i, key);
}

/* ----------- encrypt an 8-byte block ------------ */
void encrypt(char *blk)
{
   struct LR ip, op;
   int temp;
   int n;

   memcpy(&ip, blk, sizeof(struct LR));
   /* -------- initial permuation -------- */
   permute(&op.L, &ip.L, (int *)IPtbl, 64);
   swapbyte(&op.L);
   swapbyte(&op.R);
   /* ------ swap and key iterations ----- */
   for (n = 0; n < 16; n++)    {
       temp = op.R;
       op.R = op.L ^ f(op.R, keys[n]);
       op.L = temp;
   }
   ip.R = op.L;
   ip.L = op.R;
   swapbyte(&ip.L);
   swapbyte(&ip.R);
   /* ----- inverse initial permutation ---- */
   inverse_permute(&op.L, &ip.L,
       (int *)IPtbl, 64);
   memcpy(blk, &op, sizeof(struct LR));
}


/* ----------- decrypt an 8-byte block ------------ */
void decrypt(char *blk)
{
   struct LR ip, op;
   int temp;
   int n;

   memcpy(&ip, blk, sizeof(struct LR));
   /* -------- initial permuation -------- */
   permute(&op.L, &ip.L, (int *)IPtbl, 64);
   swapbyte(&op.L);
   swapbyte(&op.R);
   ip.R = op.L;
   ip.L = op.R;
   /* ------ swap and key iterations ----- */
   for (n = 15; n >= 0; --n)    {
       temp = ip.L;
       ip.L = ip.R ^ f(ip.L, keys[n]);
       ip.R = temp;
   }
   swapbyte(&ip.L);
   swapbyte(&ip.R);
   /* ----- inverse initial permuation ---- */
   
   inverse_permute(&op.L, &ip.L,
       (int *)IPtbl, 64);
   memcpy(blk, &op, sizeof(struct LR));
}


/* ------- inverse permute a 64-bit string ------- */
static void inverse_permute(int *op,int *ip,int *tbl,int n)
{
    int i;
    int *pt = (int *)Pmask;

    *op = *(op+1) = 0;
    for (i = 0; i < n; i++)    {
       if ((*ip & *pt) || (*(ip+1) & *(pt+1)))  {
           *op |= *tbl;
           *(op+1) |= *(tbl+1);
        }
        tbl += 2;
        pt += 2;
   }
}

/* ------- permute a 64-bit string ------- */
static void permute(int *op, int *ip, int *tbl, int n)
{
    int i;
    int *pt = (int *)Pmask;

    *op = *(op+1) = 0;
    for (i = 0; i < n; i++)    {
        if ((*ip & *tbl) || (*(ip+1) & *(tbl+1))) {
            *op |= *pt;
            *(op+1) |= *(pt+1);
        }
        tbl += 2;
        pt += 2;
    }
}

/* ----- Key dependent computation function f(R,K) ----- */
static int f(int blk, struct ks key)
{
    struct LR ir;
    struct LR or;
    int i;

    union    {
        struct LR f;
        struct ks kn;
    } tr = {0,0}, kr = {0,0};

    ir.L = blk;
    ir.R = 0;

    kr.kn = key;

    swapbyte(&ir.L);
    swapbyte(&ir.R);

    permute(&tr.f.L, &ir.L, (int *)Etbl, 48);

	
    tr.f.L ^= kr.f.L;
    tr.f.R ^= kr.f.R;

   /*   the DES S function: ir.L = S(tr.kn);  */
    ir.L = 0;
    for (i = 0; i < 8; i++)    {
        int four = fourbits(tr.kn, i);
        ir.L |= four << ((7-i) * 4);
    }
    swapbyte(&ir.L);

    ir.R = or.R = 0;
    permute(&or.L, &ir.L, (int *)Ptbl, 32);

    swapbyte(&or.L);
    swapbyte(&or.R);

    return or.L;
}

/* ------- extract a 4-bit stream from the block/key ------- */
static int fourbits(struct ks k, int s)
{
    int i = sixbits(k, s);
    int row, col;
    row = ((i >> 4) & 2) | (i & 1);
    col = (i >> 1) & 0xf;
    return stbl[s][row][col];
}

/* ---- extract 6-bit stream fr pos s of the  block/key ---- */
static int sixbits(struct ks k, int s)
{
    int op = 0;
    int n = (s);
    int i;
    for (i = 0; i < 2; i++)    {
        int off = ex6[n][i][0];
        unsigned char c = k.ki[off];
        c >>= ex6[n][i][1];
        c <<= ex6[n][i][2];
        c &=  ex6[n][i][3];
        op |= c;
    }
    return op;
}


/* ---------- DES Key Schedule (KS) function ----------- */
static struct ks KS(int n, char *key)
{
    static unsigned char cd[8];
    static int its[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    union    {
        struct ks kn;
        struct LR filler;
    } result;

    if (n == 0)
        permute((int *)cd, (int *) key, (int *)PC1tbl, 64);

    rotate(cd, its[n]);
    rotate(cd+4, its[n]);

    permute(&result.filler.L, (int *)cd, (int *)PC2tbl, 48);
    return result.kn;
}


/* rotate a 4-byte string n (1 or 2) positions to the left */
static void rotate(unsigned char *c, int n)
{
    int i;
    unsigned j, k;
    k = ((*c) & 255) >> (8 - n);
    for (i = 3; i >= 0; --i)    {
        j = ((*(c+i) << n) + k);
        k = (j >> 8) & 255;
        *(c+i) = j & 255;
    }
    if (n == 2)
       *(c+3) = (*(c+3) & 0xc0) | ((*(c+3) << 4) & 0x30);
    else
       *(c+3) = (*(c+3) & 0xe0) | ((*(c+3) << 4) & 0x10);
}

/* -------- swap bytes in a int integer ---------- */
static void swapbyte(int *l)
{
   char *cp = (char *) l;
   char t = *(cp+3);

   *(cp+3) = *cp;
   *cp = t;
   t = *(cp+2);
   *(cp+2) = *(cp+1);
   *(cp+1) = t;
}


