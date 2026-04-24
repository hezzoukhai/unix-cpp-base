#include <stdio.h>
#include <fcntl.h>

main()
{
int i;
int c;

i=0;

while ((c=getchar()) != EOF)
{
	fprintf(stdout,"%02X ", c);
    i++;
    if (i==16) 
	{
		fprintf(stdout,"\n");
		i=0;
	}
}
}
