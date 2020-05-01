/*
	Sample code for controlling the various sensors for
	the VR Bike Trainer.

	Author: Vivek Verghese
	Date: 04/30/2020
	Class: Inventor's Studio 2
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include "mtrand.h"
#include <stdlib.h>
#include <cassert>

double currentSpeed = 0; // The current speed of the user
double currentGradient = 0; // The gradient "sensor" in this example

void adjustGradient(double adjustment)
{
	/*
		This function sends the data to the motor. In this case, it only changes
		a global variable.
	*/
	currentGradient += adjustment;
}

void readSpeed()
{
	/*
		This function reads the data from the sensor and changes it in the program.
		In this program, it just uses a random function to increase/decrease the speed
		within a small boundary.
	*/
	static MTRand mtrand;
	bool positive = mtrand.randInt(1);
	double newSpeed = mtrand.rand(0.9);

	if ((positive || newSpeed >= currentSpeed / 6) && currentSpeed < 18.0)
		currentSpeed += newSpeed;
	else
		currentSpeed -= newSpeed;
}

double readGradient()
{
	/*
		This reads the gradient sensor on the bike. In this case it just reads the variable
		and returns it.
	*/
	return currentGradient;
}

int main(int argc, char** argv)
{
	if (argc != 5) // Ensure that the program is given the proper parameters on start.
	{
		std::cerr << "The proper usage is ./" << argv[0] << " map debugging_output gradient_output distance_output" << std::endl;
		return 1;
	}

	std::string map = argv[1];
	bool debuggingOutput = atoi(argv[2]);

	std::ofstream gradientOutput(argv[3]);
	std::ofstream distanceOutput(argv[4]);

	std::string gradientDataFile = map + "_Gradient_Data.txt"; // Used to load the gradient data
	std::string recommendedSpeedFile = map + "_Recommended_Speed.txt"; // Used to load the max speed through curves

	std::ifstream inGradient(gradientDataFile);
	std::ifstream inSpeed(recommendedSpeedFile);

	double currentDistance = 0;

	double totalDistance = 0;
	double tmpDistance = 0;

	double targetGradient = 0;
	double recommendedSpeed = 0;

	double nextGradientChange = 0;
	double nextSpeedChange = 0;

	double gradientAdjustment = 0;

	inGradient >> totalDistance >> nextGradientChange >> targetGradient;
	assert(nextGradientChange == 0 && targetGradient == 0.0);

	inSpeed >> tmpDistance >> nextSpeedChange >> recommendedSpeed;
	assert(nextSpeedChange == 0 && recommendedSpeed == 0.0);

	assert(totalDistance == tmpDistance);

	clock_t currentTime = clock();
	clock_t outputTimer = clock(); // The program will output when the output time reads 

	clock_t startTime = clock();

	while (currentDistance < totalDistance)
	{
		if ( debuggingOutput && ((double)clock() - (double)outputTimer) / CLOCKS_PER_SEC >= 5.0)
		{
			outputTimer = clock();
			double timeElapsed = ((double)outputTimer - (double)startTime) / CLOCKS_PER_SEC;
			std::cout << "Time Elapsed: " << (int)timeElapsed / 3600 << "h:" << ((int)timeElapsed / 60) % 60
						<< "m:" << (int)timeElapsed % 60 << "s" << std::endl;
			std::cout << "Progress: " << currentDistance << "m / " << totalDistance << "m" << std::endl;
			std::cout << "Average Speed: " << currentDistance / timeElapsed << "m/s or " << currentDistance / timeElapsed * 2.237 << "mi/hr" << std::endl;
			std::cout << "Current Speed: " << currentSpeed << "m/s or " << currentSpeed * 2.237 << "mi/hr" << std::endl;
			std::cout << "Current Gradient: " << readGradient() << std::endl << std::endl;
		}
		double n = 0.01;
		while( ((double)clock() - (double)currentTime)/CLOCKS_PER_SEC < n );
		currentTime = clock();

		if (currentDistance >= nextGradientChange)
		{
			inGradient >> nextGradientChange >> targetGradient;
			gradientAdjustment = ((targetGradient - readGradient()) * n) / ((nextGradientChange - currentDistance) / currentSpeed);
		}
		if (currentDistance >= nextSpeedChange)
			inSpeed >> nextSpeedChange >> recommendedSpeed;

		readSpeed();
		currentDistance += (currentSpeed * n);

		adjustGradient(gradientAdjustment);
		
		gradientOutput << readGradient() << std::endl;
		distanceOutput << currentDistance << std::endl;
	}

	clock_t endTime = clock();

	double totalTime = ((double)endTime - (double)startTime) / CLOCKS_PER_SEC;
	std::cout << "\tEnd of Workout!" << std::endl;
	std::cout << "Time Elapsed: " << (int)totalTime / 3600 << "h:" << ((int)totalTime / 60) % 60
				<< "m:" << (int)totalTime % 60 << "s" << std::endl;
	std::cout << "Progress: " << currentDistance << "m / " << totalDistance << "m" << std::endl;
	std::cout << "Average Speed: " << currentDistance / totalTime << "m/s or " << currentDistance / totalTime * 2.237 << "mi/hr" << std::endl;
	std::cout << "Current Speed: " << currentSpeed << "m/s or " << currentSpeed * 2.237 << "mi/hr" << std::endl;
	std::cout << "Current Gradient: " << readGradient() << std::endl << std::endl;
	return 0;
}