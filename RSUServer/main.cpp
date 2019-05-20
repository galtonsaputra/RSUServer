#include <cstdio>
#include <iostream>
#include <stack>
#include <signal.h> 

//TEST STOPWATCH HERE
#include <chrono>
//Map
#include <map>

#include "SocketConnection.h"
#include "ConcurrentQueue.h"
#include "MessageFrame.h"
#include "BasicSafetyMessage.h" 
#include "RSU_Server_lever.h"

#define PORT 8888
Queue<MessageFrame_t*> MsgQueue;

//<TemporaryID, messageFrame>
std::map<std::string, long> storeReceivedMessage;

//External variable for connection + pipe
extern int fileDesGlobal[2];
extern char* transmittingIP;

//Lever Config
extern bool lever_A_STOP_trigged;
extern bool lever_B_STOP_trigged;
extern double elapsedTime_LeverA;
extern double elapsedTime_LeverB;

//Global verification of speed
int speedCar_A;
int speedCar_B;

// set by sig handler
volatile sig_atomic_t flag = 0;

bool isSpeedinRange(unsigned rangeLow, unsigned rangeHigh, unsigned variable)
{
	return  ((variable - rangeLow) <= (rangeHigh - rangeLow));
}

void ActivateSpeedSensor()
{
	VerifySpeed vs;
	while (!flag)
	{
		if (lever_A_STOP_trigged)
		{
			double avgSpeedReadingLeverA = vs.CalculateSpeed(elapsedTime_LeverA);
			std::cout << "Average speed A: " << avgSpeedReadingLeverA << " cm/s\n";

			//Lever speed 20 (16-24). car speed 12
			double min = vs.Calculate_LeverReading_DevMin(avgSpeedReadingLeverA);
			double max = vs.Calculate_LeverReading_DevMax(avgSpeedReadingLeverA);
			
			std::cout.precision(2);
			//std::cout << "Range (-10%) Lev_A minimum: " << min << "\n";
			//std::cout << "Range (+10%) Lev_A maximum: " << max << "\n";
			
			//Verification Check that it falls within this range
			if (isSpeedinRange(min, max, speedCar_A))
			{				
				printf("Detected speed of CarOne: %d cm/sec is within normal range. \n", speedCar_A);
				printf("CarOne speed VERIFIFED! \n");
			}
			else if (speedCar_A == 0) 
			{
				printf("No speed reading received from CarOne... \n");
			}
			else
			{
				printf("CarOne speed is MISMATCH with speed sensor. \n");
				printf("Where broadcasted BSM-Speed of vehicle: %dcm/s \n", speedCar_A);
				printf("Tagging and initiating bad actor protocol... \n");
			}
			
			lever_A_STOP_trigged = false;
		}
		//B is good
		else if (lever_B_STOP_trigged)
		{
			double avgSpeedReadingLeverB = vs.CalculateSpeed(elapsedTime_LeverB);
			std::cout << "Average speed B: " << avgSpeedReadingLeverB << " cm/s \n";

			double min = vs.Calculate_LeverReading_DevMin(avgSpeedReadingLeverB);
			double max = vs.Calculate_LeverReading_DevMax(avgSpeedReadingLeverB);

			std::cout.precision(2);
			std::cout << "Range (-10%) Lev_B minimum: " << min << "\n";
			std::cout << "Range (+10%) Lev_B maximum: " << max << "\n";

			//Verification Check that it falls within this range
			if (isSpeedinRange(min, max, speedCar_B))
			{
				printf("Detected speed of CarTwo: %d cm/sec is within normal range. \n", speedCar_B);
				printf("CarTwo speed VERIFIFED! \n");
			}
			else if (speedCar_B == 0)
			{
				printf("No speed reading received from CarTwo... \n");
			}
			else
			{
				printf("CarTwo speed is MISMATCH with speed sensor. \n");
				printf("Broadcasted BSM-Speed of vehicle: %dcm/s \n", speedCar_B);
				printf("Tagging and initiating bad actor protocol... \n");
			}

			lever_B_STOP_trigged = false;
		}
	}
}

void ProcessQueueMessages() {

	char msg[100];
	char msgReceivedFrom[100];

	while (true) 
	{
		// Blocking call to wait for message
		auto item = MsgQueue.pop();
		memset(msg, 0x32, 100);		

		// Process message from queue -> prints
		switch (item->messageId) {
			case 20: 
			{
				BasicSafetyMessage_t* bsm = &item->value.choice.BasicSafetyMessage;

				//TemporaryID containing IP conversion to string
				std::string keyIP(bsm->coreData.id.buf, bsm->coreData.id.buf + bsm->coreData.id.size);
				storeReceivedMessage[keyIP] = bsm->coreData.speed;
				
				//Message print: Speed received from IP
				//sprintf(msgReceivedFrom, "Message received from: %s ...\n", bsm->coreData.id);
				//printf(msgReceivedFrom);
				//sprintf(msg, "Transmitted speed reading of: %ld\n", bsm->coreData.speed);
				//printf(msg);

				//A - CarOne Bad
				if (keyIP == "192.168.43.52") 
				{
					speedCar_A = bsm->coreData.speed;
				}

				//B - CarTwo Good
				else if (keyIP == "192.168.43.212")
				{
					speedCar_B = bsm->coreData.speed;
				}

				break;
			}
		}

		// Free the MessageFrame once we've processed it.
		// to think how to free when you compare and plug in to equation.
		ASN_STRUCT_FREE(asn_DEF_MessageFrame, item);
		memset(msg, 0, 100);
	}
	return;
}

//Thread being spawned from Main
std::thread openMultipleConnection;
std::thread processQueueMessage;
std::thread ActivateSensor;

////////////////////
/*SERVER SHUTDOWN*/
///////////////////
/*
Waits for any input key from user to initiate server shutdown.
Server shutdown hardware first, the last with client connection
and with master.

SocketConnection: Thread shutdown must start with a detach(); as join()
blocks (this)current main thread and wait for select() to return.
However select() is blocked on receiving activity in master_socket.
CloseServer() handles this by closing Master_Socket first then its pipe
fileDesGlobal[0] to trigger activity for select(). Allowing it to break
and return control back to (this)main process.

Note:
If a file descriptor being monitored by select() is closed in another
thread, the result is unspecified. On Linux (and
some other systems), closing the file descriptor in another thread
has no effect on select().  In summary, any application that relies
on a particular behavior in this scenario must be considered buggy.
*/
static void onexit(void)
{
	printf("SERVER SHUTTING DOWN... \n");
	flag = 1;

	printf("Cleaning-up ActiveSensor thread\n");
	ActivateSensor.join();

	printf("Detaching ProcessQueue thread\n");
	processQueueMessage.detach();

	printf("Closing socket connections. \n");
	openMultipleConnection.detach();
	SocketConnection::CloseServer();

	printf("GOODBYE\n");
}

void my_function(int sig) { // can be called asynchronously
	printf("\n Shutdown command detected. \n");
	exit(0);
}

int main()
{
	SocketConnection sc;
	printf("RSU Server Control \n");
	
	/** START RSU SERVER HOSTING **
	Opens RSU server side to process and incoming connection.
	- Creates and open a wifi socket to wait for a connection at PORT:8888
	- Manages multiple socket connection [open + close] notification
	- Starts a pipe to push data to the pipe for thread processQueueMessage to process.
	- Connection error handling is handled within SocketConnection, exiting immediately before starting any pipe or message read. 
	. */
	try
	{
		/*(1) DECODE J2735 MSG FROM PIPE
		Starts a thread to host 30 connections coming in.
		Clients are not spawned into new child-thread, using fd_select.
		Closing of server thread is invoked here, after detach(). 
		*/
		openMultipleConnection = std::thread{ SocketConnection::StartServer };

		/*(2) DECODE J2735 MSG FROM PIPE
		Pipe out received incoming speed messages to J2735 queue
		*/
		//std::thread readingMsg{ sc.ReadPipeToProcessMessage, fileDesGlobal[0] };

		/*(3) PROCESS RECEIVED J2735 MSG
		This thread is responsible for the actual processing of J2735.
		Message frames placed onto the queue within ReadPipeToProcessMessage,
		are decoded through its message.id type and output to console.
		*/
		processQueueMessage = std::thread{ ProcessQueueMessages };

		/*(4) INIT LEVER SENSOR
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

			//LEVER A - CarOne_Bad
			wiringPiISR(LeverAStart, INT_EDGE_FALLING, &Lever_Switch::StartStopWatch);
			wiringPiISR(LeverAStop, INT_EDGE_FALLING, &Lever_Switch::StopStopWatch);


			//LEVER B - CarTwo_Good
			wiringPiISR(LeverBStart, INT_EDGE_FALLING, &Lever_Switch::StartStopWatch_LeverB);
			wiringPiISR(LeverBStop, INT_EDGE_FALLING, &Lever_Switch::StopStopWatch_LeverB);
		}

		/*(5) START LEVER THREAD
		Starts a thread to activate sensors to:
		- Listen & wait for sensors lever click
		- Start/Stop stopwatch
		- Calculate AverageSpeed in cm/s
		*/
		ActivateSensor = std::thread{ ActivateSpeedSensor };

		signal(SIGINT, my_function);
		atexit(onexit);
		while (!flag) 
		{
			sc.ReadPipeToProcessMessage(fileDesGlobal[0]);	
		}
	}

	catch (...)
	{
		perror("ERR:");
	}

	return 0;
}
