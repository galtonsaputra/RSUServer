#include "RSU_Server_lever.h"

//Lever A Pin config
static int LeverAStart = 7;
static int LeverAStop = 0;

//StopWatch config -> using high_res_clock as stopwatch
auto startWatch = std::chrono::high_resolution_clock::now();
auto stopWatch = std::chrono::high_resolution_clock::now();

VerifySpeed vs;

int Lever_Switch::InitWiringPi()
{
	if (wiringPiSetup() == -1)
	{
		return -1;
	}
	return 0;
}

//Set lever pinMode to INPUT
void Lever_Switch::SetLeverPin()
{
	pinMode(LeverAStart, INPUT);
	pinMode(LeverAStop, INPUT);
}

void Lever_Switch::StartLever_A_Reading()
{
	//Flag to mark start has been clicked
	bool startCLickTriggered = false;

	//START
	if (digitalRead(LeverAStart) == 0)
	{
		printf("LEVER A - START stopwatch \n");
		auto startWatch = std::chrono::high_resolution_clock::now();

		//Flag for loop to enter and wait for STOP lever pressed
		startCLickTriggered = true;
		while (startCLickTriggered) {
			//STOP
			if (digitalRead(LeverAStop) == 0)
			{
				printf("LEVER A - STOP stopwatch \n");
				auto stopWatch = std::chrono::high_resolution_clock::now();
				
				//Calculate elapsed in seconds
				std::chrono::duration<double> elapsedTime = stopWatch - startWatch;

				//Time is stored in seconds and rounded to two-decimal places
				std::cout.precision(2);
				vs.elapsedTIme = elapsedTime.count();
				std::cout << "Elapsed time: " << elapsedTime.count() << " s\n";
				break;
			}
		}
	}
};

//Speed formulae where Speed = Distance / Time
int VerifySpeed::CalculateSpeed(double* time)
{
	//YOU NEED TO TRUNCATE to two decimal place.
	//double roundNumber = time.setPrecision(2);
	
	double calculatedSpeed = vs.roadDistance / *time;
	return calculatedSpeed;
}