#include <cstdio>
#include <iostream>
#include <stack>
#include <signal.h> 

//TEST STOPWATCH HERE
#include <chrono>

#include "SocketConnection.h"
#include "ConcurrentQueue.h"
#include "MessageFrame.h"
#include "BasicSafetyMessage.h" 
#include "RSU_Server_lever.h"

#define PORT 8888
Queue<MessageFrame_t*> MsgQueue;

extern int fileDesGlobal[2];
extern char* transmittingIP;

//Lever Config
extern bool lever_A_STOP_trigged;
extern bool lever_B_STOP_trigged;
extern double elapsedTime_LeverA;
extern double elapsedTime_LeverB;

void ProcessQueueMessages() {

	char msg[100];

	while (true) {

		auto item = MsgQueue.pop();
		memset(msg, 0x32, 100);

		// Process message from queue -> prints
		switch (item->messageId) {
			case 20: 
			{
				BasicSafetyMessage_t* bsm = &item->value.choice.BasicSafetyMessage;
				printf("Message received from: %s ...\n",
					transmittingIP);
				sprintf(msg, "Transmitted speed reading of: %ld\n", bsm->coreData.speed);
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
	printf("RSU Server \n");
	
	//01. Open socket for client to connect
	SocketConnection sc;

	// This thread is responsible for the actual processing of J2735
	// Message frames placed onto the queue within ReadPipeToProcessMessage
	// theoretically, this could be created within the aformentioned function above.
	std::thread processQueueMessage{ ProcessQueueMessages };

	// We now create in essence the server side of the process (eventually with fork)
	// We open a wifi socket and if ok we then need to simply wait for a connection
	// and push data to the pipe for the pseudo 'client' to process.
	// Connection error handling is handled within SocketConnection. 
	std::thread openMultipleConnection = std::thread{ SocketConnection::StartServer };	
		
	//Initialise lever sensor and setting pins to INPUT
	if (Lever_Switch::InitWiringPi() == -1)
	{
		std::cout << "WiringPi Lever init failed. \n";
		exit(-1);
	}
	else 
	{
		Lever_Switch::SetLeverPin();

		//LEVER A
		wiringPiISR(LeverAStart, INT_EDGE_FALLING, &Lever_Switch::StartStopWatch);
		wiringPiISR(LeverAStop, INT_EDGE_FALLING, &Lever_Switch::StopStopWatch);

		//LEVER B
		wiringPiISR(LeverBStart, INT_EDGE_FALLING, &Lever_Switch::StartStopWatch_LeverB);
		wiringPiISR(LeverBStop, INT_EDGE_FALLING, &Lever_Switch::StopStopWatch_LeverB);
	}

	// Handles CTRL^C for process termination
	signal(SIGINT, my_function);

	VerifySpeed vs;

	while (!flag) {
		//Speed Readings
		sc.ReadPipeToProcessMessage(fileDesGlobal[0]);

		//Start a thread to WAIT TO LISTEN lever click
		if (lever_A_STOP_trigged)
		{
			//NOTE: there are two avg readings given from this.
			double avgSpeedReadingLeverA = vs.CalculateSpeed(elapsedTime_LeverA);
			std::cout << "Average speed A: " << avgSpeedReadingLeverA << " cm/s\n";
			
			lever_A_STOP_trigged = false;
		}

		else if (lever_B_STOP_trigged)
		{
			double avgSpeedReadingLeverB = vs.CalculateSpeed(elapsedTime_LeverB);
			std::cout << "Average speed B: " << avgSpeedReadingLeverB << " cm/s\n";

			lever_B_STOP_trigged = false;
		}

		//std::cin.get();
	}

	return 0;
}
