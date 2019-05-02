#include <cstdio>
#include <iostream>
#include <stack>
#include <signal.h> 

#include "SocketConnection.h"
#include "ConcurrentQueue.h"
#include "MessageFrame.h"
#include "BasicSafetyMessage.h" 

Queue<MessageFrame_t*> MsgQueue;

#define PORT 8888

void ProcessQueueMessages() {

	char msg[100];
	SocketConnection sc;

	while (true) {

		auto item = MsgQueue.pop();
		memset(msg, 0x32, 100);

		// Process message from queue -> prints
		switch (item->messageId) {
			case 20: 
			{
				BasicSafetyMessage_t* bsm = &item->value.choice.BasicSafetyMessage;
				sprintf(msg, "Speed %ld\n", bsm->coreData.speed);
				printf(msg);
				break;
			}

		}
		// Free the MessageFrame once we've processed it.
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, item);
		memset(msg, 0, 100);
	}
}

// set by sig handler
volatile sig_atomic_t flag = 0;
void my_function(int sig) { // can be called asynchronously
	flag = 1; // set flag
}

int main()
{
	printf("RSU Server");
	
	//01. Open socket for client to connect
	SocketConnection sc;

	// Create the pipe so we can later fork the porcess 
	// for intra process communication.
	int pipeCreation = sc.CreatePipe();

	// Create a thread that blocks whilst reading data from the pipe as this
	// is a non assigned thread we have no way to clear it up? This will
	// be the client portion of the fork i.e., pid = 0 (which is invalid).
	//std::thread readingMsg { sc.ReadPipeToProcessMessage, sc.fileDes[0] };


	// This thread is responsible for the actual processing of J2735
	// Message frames placed onto the queue within ReadPipeToProcessMessage
	// theoretically, this could be created within the aformentioned function above.
	std::thread processQueueMessage{ ProcessQueueMessages };

	// We now create in essence the server side of the process (eventually with fork)
	// We open a wifi socket and if ok we then need to simply wait for a connection
	// and push data to the pipe for the pseudo 'client' to process.
	int connection = sc.ConnectUsing("IPv4", "", 8888);

	if (connection == -1) { 
		perror("Client connection ERROR: "); 
		exit(-1);
	}

	std::cout << 
		"Client connected...";

	//Start a thread to READ raw bits

	std::thread processor = std::thread 
		{ SocketConnection::ReadClient, sc._wifiConnection.socket, sc.fileDes[1]};
	//sc.StartThread();
	//sc.ReadClient(sc._wifiConnection.socket, sc.fileDes[1]);
	
	// Handles CTRL^C for process termination
	signal(SIGINT, my_function);
	while (!flag) {
		//std::cout << 
			//"Client sending data now hopefully...";
		sc.ReadPipeToProcessMessage(sc.fileDes[0]);
		//std::thread readingMsg { sc.ReadPipeToProcessMessage, sc.fileDes[0] };
		std::cin;
	}

	return 0;
}
