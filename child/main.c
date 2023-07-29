#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define TRUE 1
#define MSGLEN 32
#define KEY 1208


typedef struct
{
	long mType;
	char mText[MSGLEN];
} Msg;   


int main()
{
	Msg msg;
	int msgid;
	unsigned randNumber;

	if ((msgid = msgget(KEY, 0666 | IPC_CREAT)) == -1)
	{
		perror("Error with message ID (msgget)!");
		exit(1);
	}

	msg.mType = 1;

	srand(time(NULL));
	
	while (TRUE)
	{
		char message[MSGLEN];
		randNumber = rand() % 100;

		sprintf(message, "%u", randNumber);

		msgsnd(msgid, &msg, sizeof(msg), 0);
		
	    usleep(1000);
	}
    
	exit(0);
}

