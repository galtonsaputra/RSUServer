#include "SocketConnection.h"
#include <signal.h>

#define TRUE 1 
#define FALSE 0 
#define PORT 8888

int fileDesGlobal[2];
char* transmittingIP;

extern stack<MessageFrame_t*> mystack;
// set by sig handler
extern volatile sig_atomic_t flag;

/*
	Read the pipe so we can process in-coming messages from the server
	the data is a collection of encoded MessageFrames as bytes.
*/
stack<MessageFrame_t*> mystack;
void SocketConnection::ReadPipeToProcessMessage(int pipe)
{
	uint8_t* rcvPipeBuffer = (uint8_t*)calloc(1, 1024);

	//Blocking call to wait and read pipe's message
	int bytesRead = read(pipe, rcvPipeBuffer, 256);
	if (bytesRead == -1) {
		return;
	}
	MessageFrame_t* msgFrame = 0;
	asn_dec_rval_t rslt = asn_decode(0, ATS_DER, &asn_DEF_MessageFrame, (void**)&msgFrame, rcvPipeBuffer, bytesRead);

	if (rslt.code == RC_OK) {
		MsgQueue.push(msgFrame);
		mystack.push(msgFrame);
	}

	memset(rcvPipeBuffer, 0, 256);
}

//Create a pipe and used for full-duplex
int SocketConnection::CreatePipe()
{
	//int pipeStatus = pipe(fileDes);
	int pipeStatus = pipe(fileDesGlobal);
	if (pipeStatus == -1) return -1;
	return 0;
}

int max_sd;
int master_socket;

void SocketConnection::CloseServer()
{
	close(master_socket);
	close(fileDesGlobal[0]);
}

void SocketConnection::StartServer()
{
	int opt = TRUE;
	int addrlen, new_socket, client_socket[30],
		max_clients = 30, activity, i, valread, sd;

	struct sockaddr_in address;
	char buffer[1025];

	//set of socket descriptors 
	fd_set readfds;

	//Create Pipe and start thread to listen and read pipe messages
	SocketConnection socketConnection;
	int pipeCreation = socketConnection.CreatePipe();
	if (pipeCreation == -1) 
	{
		perror("Pipe creation error: \n");
	}

	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	//create a master socket 
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	//type of socket created 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("RSUServer is listening on port %d \n", PORT);

	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//accept the incoming connection 
	addrlen = sizeof(address);
	puts("Waiting for client connections ...");

	while (!flag)
	{
		//clear the socket set 
		FD_ZERO(&readfds);

		//add master socket to set 
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set 
		for (i = 0; i < max_clients; i++)
		{
			//socket descriptor 
			sd = client_socket[i];

			//if valid socket descriptor then add to read list 
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function 
			if (sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if (activity == -1) 
		{
			break;
		}

		if ((activity < 0) && (errno != EINTR))
		{
			printf("select error");
		}

		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("Client (accept) connection failed: ");
				exit(EXIT_FAILURE);
			}

			printf("New client connected... IP: %s, SOCKET: %d \n", inet_ntoa(address.sin_addr), new_socket);

			//aAdd new client sockets to arrayOfSockets
			for (i = 0; i < max_clients; i++)
			{
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding '%s' to list of connected clients. \n", inet_ntoa(address.sin_addr));

					break;
				}
			}
		}

		//Continuously poll for recv command from connected client list
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds))
			{
				//Check for closing socket
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					//Somebody has disconnected. Obtain IP details to notify disconnecting client.
					getpeername(sd, (struct sockaddr*)&address, \
						(socklen_t*)&addrlen);
					printf("Client %s disconnected...\n",
						inet_ntoa(address.sin_addr));

					//Close the socket and mark as 0 in list for reuse 
					close(sd);
					client_socket[i] = 0;
				}

				//Relay message recv from each unique socket and write to pipe.
				else
				{
					if (valread != -1 && valread != 0) 
					{
						transmittingIP = inet_ntoa(address.sin_addr);
						write(fileDesGlobal[1], buffer, valread);
					}
				}
			}
		}
	}

	return;
}