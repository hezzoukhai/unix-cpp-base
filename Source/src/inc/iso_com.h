#ifndef HPS_ISO_COM_H
#define HPS_ISO_COM_H

typedef struct fld_info_s
{
   int     type;
   int     length;
}fld_info_t;

typedef struct tag_info_s
{
   char    sTagName[8];
   int     nType;
   int     nLength;
}tag_info_t;

typedef struct pds93_info_s
{
   char    sPdsName[3];
   int     nType;
   int     nLength;
}pds93_info_t;

typedef struct pds_info_s
{
   char    sPdsName[2];
   int     nType;
   int     nLength;
}pds_info_t;

int   IsFieldPresent(int    field_no,   char * BitMap);
#define   NOT_USED      -1

#endif  /* HPS_ISO_COM_H */
