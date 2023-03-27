For complete beginners:
Use an IDE to compile this code into a dll. I recommend Visual Studio 2019 Community. Its free and widely used.
Open the .sln (solution) file with the IDE program. This file is just to build the dll.
Next open AH6.cpp and the .h files to edit them to your needs. The .cpp file contains all the available functions from DCS and the .h files are to help separate and organize systems.
To create the .dll file, click Build>Build Solution or right click the project in the solution explorer.



How to alter the EFM for CH53: this doesnt apply to all EFMs as they all have different file structures.....
Once you have Visual Studio setup and downloaded, the process only takes a few minutes each time.

1) Download Visual Studio (VS)    https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&rel=16

2) Once VS installed double click the CH53.sln file (Found in the CH53 folder in "CH53 EFM/CH53/CH53.sln").  This is the "solution" that keeps all the project files together and eventually creates the .dll. VS may need to download some files (C++) and various bits and pieces...... all good, say yes and let VS do its thing.

3) Once VS is working properly ... it may take a bit to get all the right files downloaded after rebooting PC etc. 

4) Open/ double click CH53.sln. .... Go to "Build" tab and click "Rebuild Solution"....... It should create a new CH53.dll file in the /bin folder (CH53 /bin).  That is all there is to it.

5) BUT that will just be the same as the last .dll. SO to edit the EFM you will need to dig into the files........ and make some changes......

6) Without actual Wind tunnel/flight model data, the process of making the CH53 fly like real, is going to be hit and miss, but it is possible. 
Make bakup copies of the files first, and make commented out copies of the bits you change in the files, to remind you what changes you have done.

In View tab open Solution Explorer and you can open and edit the header files (.h files) once you have finished editing the .h files (!!!) .... Go to "Build" tab and click "Rebuild Solution"

For the CH53, the main files that effect the Flight model are as follows..... :

a) This file is seperate from the EFM but in the main folder instead: "CH53/FM/CH53-config.lua"

alter these lines at the top, these make a huge difference to the FM....... self explanatory::

	center_of_mass		= {-1.793, -1.874, 0},	--x,y,z
	moment_of_inertia		 = {18000,	 76162, 80778},

b) "CH53 EFM/CH53/CH53/CH53Constants.h"

change these values:: 1)wingSpan_FT, 2)wingArea_FT2 , 3)diskArea	

c) "CH53 EFM/CH53/CH53/CH53AeroData.h"

You can alter any, and all of this data .........alter the lines a little at a time  (there are "12 points that correspond to airspeed1, which are:  -40, -30, -20, 0, 20, 30, 40, 60, 80, 100, 120, 130 knots").
For easy chages alter the lines that have this comment after them.....
(most important value for overall lift)
(most important value for pitch control)
(most important value for roll control)
(most important value for yaw control)

d) The main file "CH53 EFM/CH53/CH53/CH53.cpp"

alter the number at the end of the line...... make a bakup copy first of the line by copying the bit you are changing place it at the ned of the line and comment it out at the end like I did for the pedal input e.g. //Helicopter::PedalInput * 5.5;

	//-----CONTROL DYNAMICS------------------------
	Helicopter::RollControl = limit((Helicopter::RollInput + Helicopter::rollTrim), -1, 1) * 10.125;
	Helicopter::PitchControl = limit((Helicopter::PitchInput + Helicopter::pitchTrim), -1, 1) * 18;
	Helicopter::PedalControl = Helicopter::PedalInput * 7.5;	//Helicopter::PedalInput * 5.5;	
	Helicopter::CollectiveControl = Helicopter::CollectiveInput * 15.64;   


7) Once you have made a change (recommend to make simple changes at first and to only one file at a time, so you can tell what the reaction is in sim): save all the files.

8) Open the CH53.sln again and "Rebuild" the file to create a new CH53.dll that now has your altered Flight model. Copy the CH53.dll to the main bin folder (check the dates and time of creation to ensure you are using the new one) and run the sim.

9) watch the helicopter spin wildly into the ground, you crash and die...... reload the files and start again......


