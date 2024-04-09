#include "common.h"
#include <time.h>
#include <pthread.h>

#define MAX_CLIENTS 64

int socket1;
int run = 1;
void handler(int sig)
{
	run = 0;
}

typedef struct 
{
    int id;
	struct sockaddr socket;
	int refreshed;
}
client;

char * gettime()
{
	time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

	char* res = asctime (timeinfo);
	res[strlen(res)-1] = 0;
    return res;
}

int find_free_client_id(client* clients)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
		if (clients[i].id == -1)
			return i;
	return -1;
}

void refresh_thread_func(client* clients)
{
	message msg;
	msg.mtype = REFRESH;
	while (1)
	{
		for (int i = 0; i < MAX_CLIENTS; i++)
			if (clients[i].id != -1)
			{
				clients[i].refreshed = 0;
				sendto(socket1, (const char *)&msg, sizeof(int), 0, &clients[i].socket, sizeof clients[i].socket);
			}
		sleep(10);
		for (int i = 0; i < MAX_CLIENTS; i++)
			if (clients[i].id != -1 && !clients[i].refreshed)
			{
				printf("Disconnecting client %d, timed out\n", clients[i].id);
				clients[i].id = -1;
			}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2){
		printf("Wrong number of arguments!\n");
		return 0;
	}

	int PORT = atoi(argv[1]);

	setbuf(stdout,NULL);
	setbuf(stdin,NULL);
	
	int status;
    struct sockaddr_in ser, cli;

    if ((socket1 = socket(AF_INET,SOCK_DGRAM,0)) == -1)
    {
        printf("Socket error!\n");
        return 0;
    }

    memset(&ser, 0, sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(PORT);
    ser.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((status = bind(socket1, (struct sockaddr *) &ser, sizeof ser)) == -1)
    {
        printf("Bind error!\n");
        return 0;
    }

	fcntl(socket1, F_SETFL, O_NONBLOCK);

    FILE* fd = fopen("./log.txt", "a");

 	printf("Server started. Listening...\n");
	fprintf(fd, "%s: Server started.\n", gettime());

	struct sigaction a;
	a.sa_handler = handler;
	a.sa_flags = 0;
	sigemptyset( &a.sa_mask );
	sigaction( SIGINT, &a, NULL );
	
	client clients [MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++)
		clients[i].id = -1;
	size_t buf_size;
	message msg;
	int client_id;
	int message_length;

	pthread_t refresh_thread;
	if (pthread_create(&refresh_thread, NULL, (void *)&refresh_thread_func, (void *)clients) != 0)
    {
        printf("Error creating a thread!\n");
        return 0;
    }

	while (run)
	{
		while (run)
		{
			message_length = sizeof cli;
			buf_size = recvfrom(socket1, (char *)&msg, MESSAGE_MAX, MSG_DONTWAIT, (struct sockaddr*) &cli, (socklen_t *) &message_length);

			if (buf_size != -1)
				break;
		}

		//printf("Message received, type: %d, text: %s, bytes: %d\n", msg.mtype, msg.mtext, (int)buf_size);

		if (buf_size == -1 || !run)
			continue;
		/*if (buf_size < 8)
		{
			printf("Invalid message received.\n");
			fprintf(fd, "%s: Invalid message received.\n", gettime());
			continue;
		}*/
		if (msg.mtype != REFRESH)
		{
			if (msg.mtype == INIT)
				printf("Message received, %d bytes: ", (int)buf_size);
			else
			{
				memcpy(&client_id, &msg.mtext, sizeof(int));
				printf("Message received from client %d, %d bytes: ", client_id, (int)buf_size);
			}
		}
        switch(msg.mtype){
            case STOP:
                printf("STOP\n");
				printf("Disconnecting client %d\n", client_id);
				fprintf(fd, "%s: Client %d: Received STOP\n", gettime(), client_id);
                clients[client_id].id = -1;
                break;
            case LIST:
                printf("LIST\n");
				fprintf(fd, "%s: Client %d: Received LIST\n", gettime(), client_id);
				printf("Listing clients:\n");
                for (int i = 0; i < MAX_CLIENTS; i++)
					if (clients[i].id != -1)
						printf("Client id: %d\n", clients[i].id);
                break;
            case INIT:
                printf("INIT\n");
				message response;
				client_id = find_free_client_id(clients);
				if (client_id == -1)
				{
					response.mtype = STOP;
					sendto(socket1, (const char *)&response, sizeof(int), 0, (struct sockaddr*) &cli, sizeof cli);
					printf("Disconnecting client, max amount of clients reached!\n");
					break;
				}
				memcpy(&clients[client_id].socket, &cli, sizeof (struct sockaddr));
				clients[client_id].refreshed = 1;
				response.mtype = INIT;
				memcpy(&response.mtext, &client_id, sizeof(int));
				sendto(socket1, (const char *)&response, sizeof(int) * 2, 0, &clients[client_id].socket, sizeof clients[client_id].socket);
				clients[client_id].id = client_id;
        		printf("New client, ID: %d\n", client_id);
				fprintf(fd, "%s: Client %d: Received INIT\n", gettime(), client_id);
                break;
            case TO_ALL:
                printf("TO_ALL\n");
				int client_id;
				memcpy(&client_id, &msg.mtext, sizeof(int));
                for (int i = 0; i < MAX_CLIENTS; i++)
					if (clients[i].id != -1 && i != client_id)
						sendto(socket1, (const char *)&msg, buf_size, 0, &clients[i].socket, sizeof clients[client_id].socket);
				msg.mtext[buf_size] = '\0';
				printf("Sending message from client %d to all: %s\n",client_id, &msg.mtext[sizeof(int)]);
				fprintf(fd, "%s: Client %d: Received TO_ALL, message: %s\n", gettime(), client_id,&msg.mtext[sizeof(int)]);
                break;
            case TO_ONE:
                printf("TO_ONE\n");
				int sender_client_id;
				memcpy(&sender_client_id, &msg.mtext, sizeof(int));
				int receiver_client_id;
				memcpy(&receiver_client_id, &msg.mtext[sizeof(int)], sizeof(int));
				if (clients[receiver_client_id].id == -1)
					break;
				sendto(socket1, (const char *)&msg, buf_size, 0, &clients[receiver_client_id].socket, sizeof clients[client_id].socket);
				msg.mtext[buf_size] = '\0';
				printf("Sending message from client %d to client %d: %s\n",sender_client_id, receiver_client_id, &msg.mtext[sizeof(int)*2]);
				fprintf(fd, "%s: Client %d: Received TO_ONE %d, message: %s\n", gettime(), client_id, receiver_client_id, &msg.mtext[sizeof(int)*2]);
                break;
			case REFRESH: ;
				int client_id2 = 0;
				memcpy(&client_id2, &msg.mtext, sizeof(int));
				clients[client_id2].refreshed = 1;
				break;
			default:
				break;
        }
	}

	printf("\n");

	pthread_cancel(refresh_thread);

	for (int i = 0; i < MAX_CLIENTS; i++)
		if (clients[i].id != -1)
		{
			msg.mtype = STOP;
			printf("Disconnecting client %d\n", clients[i].id);
			sendto(socket1, (const char *)&msg, sizeof(int), 0, &clients[i].socket, sizeof clients[i].socket);
		}

	close(socket1);

	printf("Server stopped.\n");
	fprintf(fd, "%s: Server stopped\n", gettime());
	return 0;
}