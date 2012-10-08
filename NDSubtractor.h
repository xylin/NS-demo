/*M///////////////////////////////////////////////////////////////////////////////////////
//=====================
//10/Sep/2012
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

#ifndef _NDSUBTRACTOR_H
#define _NDSUBTRACTOR_H

//ndsubtracotr.h : header file 
//

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

typedef std::ifstream::pos_type iFILE_SIZE;
typedef unsigned short USHORT;

///////////////////////////////////////////////////////
// Class   : NDSubtractor
// Purpose : Subtract 16 bit int image
///////////////////////////////////////////////////////

class NDSubtractor
{
public:
    //Constructor
	NDSubtractor();
    NDSubtractor(const string strFileIn, const string strFileIn2, const string strFileOut, const int iScaleType);
	
	virtual ~NDSubtractor();

	void Execute() ;
    
    bool GetStatus()const {return m_bFailed;}

private:
    //Input parameters
    string m_strFileIn1;
    string m_strFileIn2;      
	int m_iScaleType;

    //Output parameters
    string m_strOut;	

private:		
	ifstream m_inFile1;
	ifstream m_inFile2;
	ofstream m_outFile;

	int m_iScalingType;

	bool m_bFailed;
    bool m_bCheckedandInited;
    
    long int m_iMinOrig;
    long int m_iMaxOrig;
    
    long int m_iFileSize;
    
private:

	//Check disk size of file
	iFILE_SIZE GetFileSize(ifstream& in);
    
	//check two image have same size
	bool CheckInputImageDimention();
    
	//Subtract
	bool SubtractImg();
    
    //Check param and init
    bool CheckParamsAndInit(const string strFileIn, const string strFileIn2, const string strFileOut, const int iScaleType);
    
    //Scale
	void Scaling(long int*piResults, const int iLength);
    void Scaling_AbsDiff(long int*piResults, const int iLength);
    void Scaling_Saturation(long int*piResults, const int iLength);
    void Scaling_Wrapping(long int*piResults, const int iLength);
    void Scaling_Linear(long int*piResults, const int iLength);
};


#endif
