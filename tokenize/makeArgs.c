#include "makeArgs.h"
void clean(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
	{
		free(argv[x]);
		argv[x] = NULL;
	}// end for

	free(argv);
	argv = NULL;
}// end clean

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}// end printargs

int makeargs(char *s, char *** argv)
{
	char copy[MAX];
	strcpy(copy, s);

	int count = 0;
	char * saveptr = NULL;

	char * token = strtok_r(copy, " ", &saveptr);

	while(token != NULL)
	{
		count ++;
		token = strtok_r(NULL, " ", &saveptr);
	}

	if(count == 0)
		return -1;

	(*argv) = (char **)calloc(count + 1, sizeof(char *));
	
	token = strtok_r(s, " ", &saveptr);
	(*argv)[0] = (char *)calloc(strlen(token) + 1, sizeof(char));
	strcpy((*argv)[0], token);

	int x;
	for(x = 1; x < count; x++)
	{
		token = strtok_r(NULL, " ", &saveptr);
		(*argv)[x] = (char *)calloc(strlen(token) + 1, sizeof(char));
		strcpy((*argv)[x], token);
	}// end for	

   	return count;

}// end makeArgs
