
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#ifndef _INCL_CSRMAT_H_
#define _INCL_CSRMAT_H_

class CSRMat {
  // Variables
	double * vals;   // Matrix non zero values
	int * colIndx;   // Column values
	int * rowPtr;    // Row begin pointer values

  // Function declaration
  public:
	CSRMat(void);
	CSRMat(std::string strFile);
	CSRMat(double vals[], int colIndx[], int rowPtr[]);
	CSRMat Multiply(CSRMat matB);
	CSRMat Transpose();
	bool isSymmetric();
	int readCSRFile(std::string strFile);
};

#endif