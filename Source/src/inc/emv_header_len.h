/*******************************************************************************/
/*  (c) Hightech Payment Systems 2006                                          */
/*  NAME                                                                       */
/*      emv_header_len.h                                                       */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv Header Length (file card production) 		       */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   17/07/2006 -         V 2.2.0                             */
/*******************************************************************************/

#ifndef EMV_HEADER_LEN_H
#define EMV_HEADER_LEN_H


#define nLenH1    5 /*Longeur en decimal du seperateur entre Mag data et Chip Data*/
#define nLenH2	  7 /*c'est la longeur en decimal de la longeur du reste(en general c'est fixe)*/
#define nLenH3    4 /*Longeur de l'AID (en general c'est fixe)*/
#define nLenH4    2 /*Longeur en Hex de la longeur du reste (en general c'est fixe)*/
#define nLenH5   12 /*Longeur du (Bin+Padding+KeK Version*/
#define nLenH6    2 /*Longeur en Hex de la longeur du reste (en general c'est fixe)*/


#endif
