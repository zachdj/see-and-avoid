# See and Avoid #

Project created by Andy Morgan and Zach Jones
Auburn REU on SMART UAVs 2016

Research Coordinator: Dr. Richard Chapman

REU Director: Dr. Saad Biaz

### What is this repository for? ###

This project seeks to develop a robust "See and Avoid" system for autonomous
UAVs. An OpenGL environment is used to render a sample cockpit video. 
The OpenGL rendering is fed into a vision processing algorithm that uses
the OpenCV library. Aircraft and other obstacles are identified and 
tracked using the vision processing algorithm.  When an obstacle is 
determined to be a collision threat, an avoidance maneuver will be attempted.

This work was done in accordance to "see and avoid" regulations set forth
by Federal Aviation Administration (FAA). The system is intended for use 
as a secondary avoidance system. Other systems including use of TCAS or ADS-B
provide additional information needed for efficient avoidance.  

The project has been supported by the NSF REU site at Auburn University 2016.

### How do I get set up? ###

To get started, download and install the latest version of MS Visual Studio. 
This project was created in MSVC 2015 Update 2 and has been successfully
run on Update 1 and Update 3. The solution contains required OpenCV and OpenGL 
library files. To run, open the .sln file. Build the project in x86 Release mode.
OpenCV does not allow run in Debug mode due to a missing .dll file (that CMake
cannot build). 

The project successfully links required files to the project in the project
folders. There is one Environment value you will need to change in Visual Studio.
Source control does not track this variable since it is unique to each user. 

To change the required variable, do:

- Make sure you have "see-and'avoid" selected in the Solution Explorer (ensures proper change of properties)
- Project -> Properties
- Debugging -> Environment -> Edit
- In the top text field insert:
	 PATH=%PATH%;$(ProjectDir)dlls;

NOTE: The Evaluated value field will update. Click OK and save. Compile using x86 Release.

### Program Specifics ###

- All units are based on the measurement of 1 foot
- All speeds are in ft/s
- Two threads run in parallel: one for OpenGL Rendering, one for OpenCV Processing



### Contribution guidelines ###

Want to contribute?  Contact Dr. Richard Chapman at Auburn University.
Department of Computer Science and Software Engineering

### Contributors ###

* Zach Jones
* Andy Morgan