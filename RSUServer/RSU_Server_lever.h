//CLASS FOR LEVER TRIGGER AND STOPWATCH

#ifdef RSU_Server_Lever_H
#define RSU_Server_Lever_H
#endif

#include <cstdio>
#include <chrono>
#include <thread>

#include <iostream>
#include <string>

#include <math.h>
#include <wiringPi.h>

//Lever A Pin config
static int LeverAStart = 2;
static int LeverAStop = 0;

//Lever B Pin config
static int LeverBStart = 4;
static int LeverBStop = 5;

namespace Lever_Switch
{
	int InitWiringPi();

	void SetLeverPin();
	void StartLever_A_Reading();

	//Stopwatch
	void StartStopWatch();
	void StopStopWatch();
	void StartStopWatch_LeverB();
	void StopStopWatch_LeverB();

};

class VerifySpeed 
{
	public:
		//Time taken from lever's sensor readings. 2 decimal point number
		double elapsedTime;
		//Calculate speed to cm/seconds
		double CalculateSpeed(double time);
		double Calculate_LeverReading_DevMin(double AvgSpeedLeverReading);
		double Calculate_LeverReading_DevMax(double AvgSpeedLeverReading);
		
	private:
		//Road length is tested at 100cm
		int roadDistance = 100;
};