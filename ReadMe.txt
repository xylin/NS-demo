========================================================================
    CONSOLE APPLICATION : imgSub Project Overview
========================================================================

///////////////////////////////////////////////////////////////////////////////////////
10-Sep-2012
Implemented by Xinyu Lin

Demo for image subtraction

1. This is demo code for subtracting two 16 bit unsigned images  

2. win32 console-based, tested in windows 7 with visual studio 2008.

3. Options are provided for handling the negative values.

4. single thread, exception handling not implemented (to do)..

5. Depend on std; if required, pure C implementation can be provided

/////////////////////////////////////////////////////////////////////////////
Command line Usage: 

	NDImageSub srcFileName1 srcFileName2 outputFileName iType



Parameters:	
	srcFileName1 	– string indicating the full path and name for first input file
	srcFileName2 	– string indicating the full path and name for second input file
	outputFileName 	– string indicating the full path and name for output file 
	iType		– options for handling negative value
			  abasolute value: 0
			  'wraping': 1
			  linear transform: 2
		  	  'saturation':3	


/////////////////////////////////////////////////////////////////////////////
imgSub.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

imgSub.cpp
    This is the main application source file.


NDSubtractor.cpp
    This is source file for class to perform the subtraction

/////////////////////////////////////////////////////////////////////////////
