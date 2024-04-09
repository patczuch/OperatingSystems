#include "common.h"
#include <sys/select.h>

#define MAX_COMMAND_LENGTH 256
#define MAX_QUEUE_NAME_LENGTH 256

int input_available()  
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds));
}

int run = 1;
void handler(int sig)
{
	run = 0;
}

int main(int argc, char **argv)
{
	if (argc != 3){
		printf("Wrong number of arguments!\n");
		return 0;
	}

	char* IP = argv[1];
	int PORT = atoi(argv[2]);

	setbuf(stdout,NULL);
	setbuf(stdin,NULL);

	int status, socket1;
    struct sockaddr_in ser;

    if ((socket1 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket error!\n");
        return 0;
    }

    memset(&ser, 0, sizeof ser);
    ser.sin_family = AF_INET;
    ser.sin_port = htons(PORT);
    ser.sin_addr.s_addr = inet_addr(IP);

    if ((status = connect(socket1, (struct sockaddr*)&ser, sizeof ser)) < 0)
    {
        printf("Connect error!\n");
        return 0;
    }

	printf("Client started\n");

	message msg;
	msg.mtype = INIT;
	write(socket1, (const char *)&msg, sizeof(int));
	
	while (read(socket1, &msg, MESSAGE_MAX) == -1);

	if (msg.mtype != INIT)
	{
		printf("Identification error!\n");
        exit(0);
	}

	int client_id;
	memcpy(&client_id, &msg.mtext, sizeof(int));
    printf("ID received: %d\n", client_id);
	
	struct sigaction a;
	a.sa_handler = handler;
	a.sa_flags = 0;
	sigemptyset( &a.sa_mask );
	sigaction( SIGINT, &a, NULL );

	size_t buf_size;
	char command [MAX_COMMAND_LENGTH];

	int stopped_by_server = 0;
	while (run)
	{
		buf_size = recv (socket1, (char *)&msg, MESSAGE_MAX, MSG_DONTWAIT );
		if (buf_size != -1)
		{
			printf("Received message from server, %d bytes: ", (int)buf_size);
			msg.mtext[buf_size] = '\0';
			int client_id2;
			switch(msg.mtype)
			{
				case STOP:
					printf("STOP\n");
					run = 0;
					stopped_by_server = 1;
					break;
				case LIST:
					printf("LIST\n");
					break;
				case INIT:
					printf("INIT\n");
					break;
				case TO_ALL:
					printf("TO_ALL\n");
					memcpy(&client_id2, &msg.mtext, sizeof(int));
					printf("Client %d: %s\n",client_id2, &msg.mtext[sizeof(int)]);
					break;
				case TO_ONE:
					printf("TO_ONE\n");
					memcpy(&client_id2, &msg.mtext, sizeof(int));
					printf("Client %d (whisper): %s\n",client_id2, &msg.mtext[sizeof(int)*2]);
					break;
				case REFRESH:
					printf("REFRESH\n");
					memcpy(&msg.mtext, &client_id,  sizeof(int));
					write(socket1, (const char *)&msg, sizeof(int)*2);
					break;
				default:
					break;
			}
		}
		if (input_available())
		{
			//printf("test\n");
			size_t command_length = MAX_COMMAND_LENGTH;
			memset(command, '\0', MAX_COMMAND_LENGTH);
			char * c = command;
			getline(&c, &command_length, stdin);
			memset(msg.mtext, '\0', sizeof(msg.mtext));
			memcpy(&msg.mtext, &client_id, sizeof(int));
			c = strtok(c, " ");
			if (strcmp(c, "LIST\n") == 0)
			{
				msg.mtype = LIST;
				write(socket1, (const char *)&msg, sizeof(int) * 2);
			}
			else if (strcmp(c, "STOP\n") == 0)
			{
				run = 0;
			}
			else if (strcmp(c, "2ALL") == 0)
			{
				c = strtok(NULL, " ");
				if (c == NULL || strlen(c) < 2)
					continue;
				//printf("%s\n",c);
				c[strlen(c)] = ' ';
				//printf("%s\n",c);
				c[strlen(c)-1] = '\0';
				//printf("%s\n",c);
				msg.mtype = TO_ALL;
				memcpy(&msg.mtext[sizeof(int)], c, strlen(c));
				printf("Sending message to all: %s\n", &msg.mtext[sizeof(int)]);
				write(socket1, (const char *)&msg, sizeof(int) * 2 + strlen(c) + 1);
			}
			else if (strcmp(c, "2ONE") == 0)
			{
				c = strtok(NULL, " ");
				int receiver_id = atoi(c);
				msg.mtype = TO_ONE;
				memcpy(&msg.mtext[sizeof(int)], &receiver_id, sizeof(int));
				c = strtok(NULL, " ");
				if (c == NULL || strlen(c) < 2)
					continue;
				c[strlen(c)] = ' ';
				c[strlen(c)-1] = '\0';
				memcpy(&msg.mtext[sizeof(int)*2], c, strlen(c));
				printf("Sending message to client %d: %s\n",receiver_id, &msg.mtext[sizeof(int)*2]);
				write(socket1, (const char *)&msg, sizeof(int) * 3 + strlen(c) + 1);
			}
		}
	}

	if (stopped_by_server == 0)
	{
		msg.mtype = STOP;
		memcpy(&msg.mtext, &client_id, sizeof(int));
		write(socket1, (const char *)&msg, sizeof(int) * 2);
		printf("Sent STOP message to the server.\n");
	}
	else
		printf("Received STOP message from the server.\n");

	close(socket1);
	printf("Client stopped.\n");
	return 0;
}