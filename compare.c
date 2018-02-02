#include <stdio.h>
#include <string.h>
/* Add prototype support.  */
#ifndef PROTO
#if defined (USE_PROTOTYPES) ? USE_PROTOTYPES : defined (__STDC__)
#define PROTO(ARGS) ARGS
#else
#define PROTO(ARGS) ()
#endif
#endif

#include "md5.h"

void concatSalt(char* input)
{
	char extra[] = "/cardreader/listen";
	strcat(input, extra);
}

void filterID(char* input, char* output)
{
    int i;

	for (i=2; i<10; i++)
	{
		output[i-2] = input[i];
	}
	output[i-2] = '\0';
	
}
