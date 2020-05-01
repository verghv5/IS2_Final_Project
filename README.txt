IS2 Final Project README.

Author: Vivek Verghese
Date 04/30/2020


This README covers the format of the input files.

Map files are not handled in this code, but the "bike's" current readings will be stored as global variables in this case, and accessing them will be merely looking at the global variables.

All gradient data files are denoted as [map]_Gradient_Data.txt, and in the example, [map] will always be Example. There will also be recommended speed files denotes as [map]_Recommended_Speed.txt which is meant to know what speeds are recommended to go through curves or on straights. The format will be like rally race instructions (more on that below).

All files will reference data in metric. While the data can be converted based on preference on the display, the backend will only rely on metric.

[map]_Gradient_Data.txt:

	These files will give the data at each change on the course.

	Most of the data comes in the form of racers who have ridden along the route with tracking equipment on their bikes. One example 		for the Tour De France is from Mikel Nieve's data here:
	http://home.trainingpeaks.com/public/workout/MIM3PEJE55W3J6PJ26DBD35XXQ

	For the example file, since I have not written any sort of scrapers to average several racer's data, the numbers will be all 			made up.

	The the values are space separated as the program reads each value by space (new lines count as a space, but it makes it more 			readable to separate dependent information by line).

	Length_of_Course
	Distance_From_Start Grade
	.
	.
	.
	Length_of_Course 0.0
	(nothing on last line, but there needs to be a blank new line here)


[map]_Recommended_Speed.txt:

	These will be based off of physics calculations of max speeds that can be achieved on the course with respect to going around 			curves.

	The equation used to calculate this will be fairly simple, as this is more of a theoretical maximum rather than one that 			considers actual conditions (like rain, ice, wind, etc.). The equation is:

	speed = sqrt( (r * g * (sin(theta) + u * cos(theta)) ) / (cos(theta) - u * sin(theta)) )

	r is the radius of the curve
	g is the gravitational acceleration (9.81)
	theta is the angle of the bank
	u is the frictional constant of the road (~0.7 for dry roads, ~0.4 for wet roads)

	The the values are space separated as the program reads each value by space (new lines count as a space, but it makes it more 			readable to separate dependent information by line).

	Length_of_Course
	Distance_From_Start Grade
	.
	.
	.
	Length_of_Course 0.0
	(nothing on last line, but there needs to be a blank new line here)


Running the program:

	The program takes 4 arguments:
	 1.	The name of the map - in this case, the name is "Example"
	 2.	Debugging output - 0 means that only the end stats are printed, 1 means that simulation data is printed every 5 seconds.
	 3.	Gradient data output file - saves the current gradient reading every tick to this file
	 4.	Distance data output file - saves the current distance reading every tick to this file

	 When running the .exe from the command line, it should look like:

	 ./t.exe Example 0 g_out.txt d_out.txt
