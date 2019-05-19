#include "RSU_Server_lever.h"
//Utility class for speed verification calculation
VerifySpeed vs;

double stdDev = 0.05;
double devSpeedReadingMax;
double devSpeedReadingMin;

auto startLever_A_Watch = std::chrono::high_resolution_clock::now();
auto startLever_B_Watch = std::chrono::high_resolution_clock::now();

bool lever_A_STOP_trigged = false;
bool lever_B_STOP_trigged = false;
double elapsedTime_LeverA;
double elapsedTime_LeverB;

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
	pinMode(LeverBStart, INPUT);
	pinMode(LeverBStop, INPUT);

}

void Lever_Switch::StartLever_A_Reading()
{
	//Flag to mark start has been clicked
	bool startCLickTriggered = false;

	//START LEVER
	if (digitalRead(LeverAStart) == 0)
	{
		printf("LEVER A - START stopwatch \n");
		auto startWatch = std::chrono::high_resolution_clock::now();

		//Flag for loop to enter and wait for STOP lever pressed
		startCLickTriggered = true;
		while (startCLickTriggered) {
			//STOP LEVER
			if (digitalRead(LeverAStop) == 0)
			{
				printf("LEVER A - STOP stopwatch \n");
				auto stopWatch = std::chrono::high_resolution_clock::now();
				
				//Calculate elapsed in seconds
				std::chrono::duration<double> elapsedTime = stopWatch - startWatch;

				//Time is stored in seconds and rounded to two-decimal places
				std::cout.precision(2);
				vs.elapsedTime = elapsedTime.count();
				double avgSpeedReading = vs.CalculateSpeed(vs.elapsedTime);

				std::cout << "Elapsed time: " << elapsedTime.count() << " s\n";
				std::cout << "Average speed: " << avgSpeedReading << " cm/s\n";
				break;
			}
		}
	}
};

//LEVER A
void Lever_Switch::StartStopWatch() 
{
	//printf("LEVER A - START stopwatch \n");
	printf("Lever A start triggered by: 192.168.43.56 \n");
	auto startWatch = std::chrono::high_resolution_clock::now();
	startLever_A_Watch = startWatch;

	//Straight system call to set pin's edge to none -> disable
	std::string strCommand = "gpio edge 2 none";
	const char *command_LeverA_Start = strCommand.c_str();
	std::system(command_LeverA_Start);
	return;
}

void Lever_Switch::StopStopWatch()
{
	//printf("LEVER A - STOP stopwatch \n");
	printf("Lever A stop triggered by: 192.168.43.56 \n");
	auto stopWatch = std::chrono::high_resolution_clock::now();

	//Calculate elapsed in seconds
	std::chrono::duration<double> elapsedTime = stopWatch - startLever_A_Watch;
	//Prints fixed-notation to 2 decimal place
	std::cout.precision(2);
	std::cout << "Lever A stop: " << elapsedTime.count() << " s\n" << std::fixed;

	//Straight system call to set pin's edge to none -> disable
	std::string strCommand = "gpio edge 0 none";
	const char *command_LeverA_Stop = strCommand.c_str();
	std::system(command_LeverA_Stop);

	//To store elapsed time count for speed calculation
	lever_A_STOP_trigged = true;
	elapsedTime_LeverA = elapsedTime.count();
}

//LEVERB
void Lever_Switch::StartStopWatch_LeverB()
{
	//printf("LEVER B - START stopwatch \n");
	printf("Lever B start triggered by: 192.168.43.212 \n");
	auto startWatch = std::chrono::high_resolution_clock::now();
	startLever_B_Watch = startWatch;

	//Straight system call to set pin's edge to none -> disable
	std::string strCommand = "gpio edge 4 none";
	const char *command_LeverB_Start = strCommand.c_str();
	std::system(command_LeverB_Start);
	return;
}

void Lever_Switch::StopStopWatch_LeverB()
{
	//printf("LEVER B - STOP stopwatch \n");
	printf("Lever B stop triggered by: 192.168.43.212 \n");
	auto stopWatch = std::chrono::high_resolution_clock::now();

	//Calculate elapsed in seconds
	std::chrono::duration<double> elapsedTime = stopWatch - startLever_B_Watch;

	std::cout.precision(2);
	std::cout << "Lever B elapsed time: " << elapsedTime.count() << " s\n" << std::fixed;

	//Straight system call to set pin's edge to none -> disable
	std::string strCommand = "gpio edge 5 none";
	const char *command_LeverB_Stop = strCommand.c_str();
	std::system(command_LeverB_Stop);

	lever_B_STOP_trigged = true;
	elapsedTime_LeverB = elapsedTime.count();
}

//Speed formulae where Speed = Distance / Time
double VerifySpeed::CalculateSpeed(double time)
{
	double calculatedSpeed = vs.roadDistance / time;
	return calculatedSpeed;
}

//To calculate a standard deviation +-5% from lever sensor reading
double VerifySpeed::Calculate_LeverReading_DevMin(double AvgSpeedLeverReading)
{
	double calculateDevFromReading = AvgSpeedLeverReading * (stdDev);
	return AvgSpeedLeverReading - calculateDevFromReading;
}

double VerifySpeed::Calculate_LeverReading_DevMax(double AvgSpeedLeverReading)
{
	double calculateDevFromReading = AvgSpeedLeverReading * (stdDev);
	return AvgSpeedLeverReading + calculateDevFromReading;
}