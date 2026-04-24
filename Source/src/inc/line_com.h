#ifndef HPS_LINE_COM_H
#define HPS_LINE_COM_H

/*int ReadLineMsg  (char *sBuffer, int *nLength);
int WriteLineMsg (char *sBuffer, int nLength);
*/
int ReadLineMsg  (int nFd ,char *sBuffer, int *nLength); 
int WriteLineMsg (int nFdC,char *sBuffer, int nLength);

#endif /* HPS_LINE_COM_H */
