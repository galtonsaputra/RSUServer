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

void ActivateSpeedSensor()
{
	VerifySpeed vs;
	while (true)
	{
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
	}
}

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
	SocketConnection sc;
	printf("RSU Server Controls \n");
	
	/*(1) START RSU SERVER HOSTING
	Opens RSU server side to process and incoming connection.
	- Creates and open a wifi socket to wait for a connection at PORT:8888
	- Manages multiple socket connection [open + close] notification
	- Starts a pipe to push data to the pipe for thread processQueueMessage to process.
	- Connection error handling is handled within SocketConnection, exiting immediately before starting any pipe or message read. 
	. */

	std::thread openMultipleConnection = std::thread{ SocketConnection::StartServer };

	/*(2) PROCESS RECEIVED J2735 MSG
	This thread is responsible for the actual processing of J2735.
	Message frames placed onto the queue within ReadPipeToProcessMessage,
	are decoded through its message.id type and output to console.
	*/
	std::thread processQueueMessage{ ProcessQueueMessages };
		
	/*(3) INIT LEVER SENSOR
	Initialise lever sensor and setting pins to INPUT
	*/
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

	/*(4) START LEVER THREAD
	Starts a thread to activate sensors to:
	- Listen & wait for sensors lever click
	- Start/Stop stopwatch
	- Calculate AverageSpeed in cm/s
	*/
	std::thread ActivateSensor{ ActivateSpeedSensor };

	//Initialise Lever readings class
	VerifySpeed vs;

	// Handles CTRL^C for process termination
	signal(SIGINT, my_function);

	//Main loop
	while (!flag) 
	{		
		/*(5) DECODE J2735 MSG FROM PIPE
		Pipe out received incoming speed messages to J2735 queue
		*/
		sc.ReadPipeToProcessMessage(fileDesGlobal[0]);

		//std::cin.get();
	}

	return 0;
}
