#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>


#define TRUE 1
#define KEY 1208
#define BUFFER 64;


typedef struct    
{
	long long data;
} Data;

	
int main()
{
	int msqid;
	key_t key = KEY;
	size_t bufLen = BUFFER;
    Data data;
	
	while (TRUE)
	{
		data.data = rand(); 
		if ((msqid = msgget(key, KEY)) < 0)
		{
			perror("Error with msgget");
			exit(1);
		}

		if (msgsnd(msqid, &data, bufLen, IPC_NOWAIT) < 0)
		{
			perror("Error with msgsnd");
			exit(0);
		}
	}
	printf("Hello World!\n");
	
	exit(0);
}

