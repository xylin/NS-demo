// imgSub.cpp : Defines the entry point for the console application.
//
/*M///////////////////////////////////////////////////////////////////////////////////////
//=====================
//10-Sep-2012
//Implemented by Xinyu Lin
//xinyulin70@gmail.com
//=====================
//Demo for image subtraction
//
//1. This is demo code for subtracting two 16 bit unsigned images.
//
//2. win32 console-based, tested in windows 7 with visual studio 2008.
//
//3. Options are provided for handling the negative values.
//
//4. single thread, exception handling not implemented (to do).
//
//5. Depend on std; if required,pure C implementation can be provided. 
//
//=====================
//Command line Usage: 
//
//		NDImageSub srcFileName1 srcFileName2 outputFileName iType
//
//=====================
//Parameters:	
//
//		srcFileName1	– string indicating the full path and name for first input file
//		srcFileName2	– string indicating the full path and name for second input file
//		outputFileName	– string indicating the full path and name for output file 
//		iType			– options for handling negative value
//						  abasolute value: 0
//						  'wraping': 1
//						  linear transform: 2
//						  'saturation':3						
//
//
//M*/

#include "stdafx.h"

#include <math.h>

#include <limits>
#include <iostream>
#include <fstream>
#include <string>

#include "NDSubtractor.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc<5)
	{
		cerr << "Please provide correct input and output" <<endl;
		return 1;
	}
	cout << endl;

	string strIn1(argv[1]);
	string strIn2(argv[2]);
	string strOut(argv[3]);

	int iType = atoi(argv[4]);

	NDSubtractor sub(strIn1, strIn2, strOut, iType);

	sub.Execute();

	if(sub.GetStatus())
	{
		cerr << "Subtraction Failed"<<endl;
		return 1;		
	}
	else
	{
		cerr << "Subtraction Success"<<endl;
		return 0;			
	};	
}

