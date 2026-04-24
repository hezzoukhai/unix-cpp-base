#ifndef HPS_PISO_COM_H
#define HPS_PISO_COM_H

int ReadLineMsg  (int nFdConn, char *sBuffer, int *nLength);
int WriteLineMsg (int nFdConn, char *sBuffer, int nLength);

#endif /* HPS_PISO_COM_H */
