//CLASS FOR LEVER TRIGGER AND STOPWATCH

#ifdef RSU_Server_Lever_H
#define RSU_Server_Lever_H
#endif

#include <cstdio>
#include <chrono>
#include <iostream>

#include <math.h>
#include <wiringPi.h>

namespace Lever_Switch
{
	int InitWiringPi();

	void SetLeverPin();
	void StartLever_A_Reading();
};

class VerifySpeed 
{
	public:
		//Time taken from lever's sensor readings. 2 decimal point number
		double elapsedTIme;
		//Calculate speed to cm/seconds
		int CalculateSpeed(double* time);
	private:
		//Road plane is 100 centimeters
		int roadDistance = 100;
};