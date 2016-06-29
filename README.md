# README #

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
provide additional information needed for properely efficient avoidance.  

The project has been supported by the NSF REU site at Auburn University 2016.

### How do I get set up? ###

To get started, download and install the latest version of MS Visual Studio. 
This project was created in MSVC 2015 Update 2. The solution contains 
required OpenCV and OpenGL library files. To run, open the .sln file.
Build the project in x86 Release mode. OpenCV does not allow run in Debug
mode due to a missing .dll file. 

The project successfully links required files to the project in the project
folders. There is no need to change envirnoment variables to link libraries to
the project. 

### Program Specifics ###

- All units are based on the measurement of 1 foot
- All speeds are in ft/s
- Two threads are built: one for OpenGL Rendering, one for OpenCV Processing
- The RANDOM variable at the top of main determines if planes are randomly generated


### Contribution guidelines ###

Want to contribute?  Contact Dr. Richard Chapman at Auburn University.
Department of Computer Science and Software Engineering

### Contributors ###

* Zach Jones
* Andy Morgan