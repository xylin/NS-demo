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
//M*/

//NDSubtractor.cpp : Implementation file 
//
#include <limits>
#include <math.h>

#include "NDSubtractor.h"


NDSubtractor::NDSubtractor()
:m_iScaleType(0), m_bFailed(true), m_iMinOrig(USHRT_MAX), m_iMaxOrig(0), m_iFileSize(0)
{
    m_bCheckedandInited = false;    
}

NDSubtractor::NDSubtractor(const string strFileIn1, const string strFileIn2, const string strFileOut, const int iScaleType)
:m_iScaleType(0), m_bFailed(true), m_iMinOrig(USHRT_MAX), m_iMaxOrig(0), m_iFileSize(0)
{
    m_bCheckedandInited = CheckParamsAndInit(strFileIn1, strFileIn2, strFileOut, iScaleType);
}

NDSubtractor::~NDSubtractor()
{
    m_inFile1.close();
    m_inFile2.close();
    m_outFile.close();    
}

void NDSubtractor::Execute()
{
    if(!m_bCheckedandInited)
    {
        m_bFailed=true;
        cerr << "Please call initialising or/and make sure the inputs are valid."<<endl;
        return;
    }
    
	m_inFile1.seekg(0, ifstream::beg);
	m_inFile2.seekg(0, ifstream::beg);    
	m_outFile.seekp(0, ofstream::beg);
    
	m_iMinOrig = LONG_MAX;
    m_iMaxOrig = LONG_MIN;
	
    long int* piResult = new long int[m_iFileSize];
	memset(piResult, 0, sizeof(long int)*m_iFileSize);
    
	if(!piResult)
	{
        m_bFailed=true;
		cerr << "Cant allocate memory" << endl;        
		return;
	}
    
	long int iIndex(0);

	while(true)
	{
		USHORT iNumIn1(0), iNumIn2(0); 
		
		m_inFile1.read(reinterpret_cast<char*>(&iNumIn1), sizeof(USHORT));
		m_inFile2.read(reinterpret_cast<char*>(&iNumIn2), sizeof(USHORT));
        
		if(m_inFile1.eof() || m_inFile2.eof())	
			break;

		long int iResult = (long int)iNumIn1 - (long int)iNumIn2;
        
		if(iResult>m_iMaxOrig)
			m_iMaxOrig = iResult;
        
		if(iResult<m_iMinOrig)
			m_iMinOrig = iResult;
		
        piResult[iIndex++]=iResult;
	}
    
    Scaling(piResult, m_iFileSize);     
	
    for(int i=0; i<m_iFileSize; ++i)
    {
        USHORT iDump = (USHORT)(piResult[i]);
        
        m_outFile.write(reinterpret_cast<char*>(&iDump), sizeof(USHORT));
    }

	delete [] piResult;
    piResult=NULL;
    
    m_inFile1.close();
    m_inFile2.close();
    m_outFile.close();
    
    m_bFailed=false;      
}


bool NDSubtractor::CheckInputImageDimention()
{
	iFILE_SIZE iSz1 = GetFileSize(m_inFile1);
	iFILE_SIZE iSz2 = GetFileSize(m_inFile2);

	//cout << iSz1 << endl;
	bool bSameSize=false;
    
	if(iSz1==iSz2 && (iSz1%2==0))//assume 16 bit per pixel
	{
		bSameSize = true;
              
        m_iFileSize = iSz1/2;//always even
    }

	return bSameSize;
}

void NDSubtractor::Scaling(long int* piResults, const int iLength)
{
    switch (m_iScaleType)
    {
        case 0:
            Scaling_AbsDiff(piResults, iLength);
            break;
        case 1:
            Scaling_Wrapping(piResults, iLength);
            break;
        case 2:
            Scaling_Linear(piResults, iLength);
            break;
        case 3:
            Scaling_Saturation(piResults, iLength);
			break;
        default:
			cerr<<"wrong input"<<endl;
            break;
    }
}

void NDSubtractor::Scaling_Linear(long int*piResults, const int iLength)
{
   const double dFactor = (double)(USHRT_MAX - 0) / (double)(m_iMaxOrig - m_iMinOrig);
    
    for(int i=0; i<iLength; ++i)
    {
        piResults[i] = (USHORT)floor((piResults[i] - m_iMinOrig) * dFactor+0.5) + 0;
    }

}

void NDSubtractor::Scaling_Wrapping(long int*piResults, const int iLength)
{
    for(int i=0; i<iLength; ++i)
    {
        if(piResults[i]<0)
            piResults[i] += USHRT_MAX;
    }    
}

void NDSubtractor::Scaling_Saturation(long int*piResults, const int iLength)
{
    for(int i=0; i<iLength; ++i)
    {
        if(piResults[i]<0)
            piResults[i] = 0;
    }
}

void NDSubtractor::Scaling_AbsDiff(long int*piResults, const int iLength)
{
    for(int i=0; i<iLength; ++i)
    {
        if(piResults[i]<0)
            piResults[i] = abs(piResults[i]);
    }    
}

iFILE_SIZE NDSubtractor::GetFileSize(ifstream& in)
{
	iFILE_SIZE iSz=0;

	if(in.is_open())
	{
		in.seekg(0, ifstream::end);
		iSz = in.tellg(); 
		in.seekg(0, ifstream::beg);//get back
	}
	return iSz;
}

bool NDSubtractor::CheckParamsAndInit(const string strFileIn1, const string strFileIn2, const string strFileOut, const int iScaleType)
{    
    m_strFileIn1 = strFileIn1;
    m_strFileIn2 = strFileIn2;
    
    m_strOut = strFileOut;
    
    m_iScaleType = iScaleType;    
    
    m_inFile1.open(m_strFileIn1.c_str(), std::ios::in | std::ios::binary);
    m_inFile2.open(m_strFileIn2.c_str(), std::ios::in | std::ios::binary);
   
    m_outFile.open(m_strOut.c_str(), std::ios::out | std::ios::binary);
    
    if(!m_inFile1 || !m_inFile2 || !m_outFile || m_iScaleType<0 || m_iScaleType>3)
    {
        cerr << "Error in openning/creating files"<<endl;
        return false;
    }
    
    //check the input images have same width and height
    //without header, can only compare the disk size
    bool bSameSize = CheckInputImageDimention();
    
    if(!bSameSize)
    {
        cerr << "Input files not same dimension"<<endl;
        return false;
    };
    
    return true;
}
