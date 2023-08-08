#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define TRUE 1
#define MSGLEN 32
#define FTOK_PATH "/etc/qtmpv/token.txt"


typedef struct
{
	long mType;
	char mText[MSGLEN];
} Msg;   


int main()
{
	Msg msg;
	int msgid;
	int key = ftok(FTOK_PATH, 1);
	unsigned randNumber;

	if (key == -1)
	{
	  	perror("ftok");
	   	exit(1);
	}
		
	if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1)
	{
   		perror("msgget");
		exit(1);
	}

	srand(time(NULL));

	msg.mType = 1;
	
	while (TRUE)
	{
		randNumber = rand() % 100;
		
		sprintf(msg.mText, "%u", randNumber);
		
		msgsnd(msgid, &msg, MSGLEN, 0);

		printf("Message send: %s\n", msg.mText);
		
	    usleep(200000);
	}
    
	exit(0);
}

