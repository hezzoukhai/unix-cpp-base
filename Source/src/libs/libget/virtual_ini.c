#include <stdio.h>

#include <virtual_record.h>


void InitVirtualRecord(TSVirtualRecord *Virtual)
{
   memset ( (char *)Virtual, 0, sizeof(TSVirtualRecord));
}
