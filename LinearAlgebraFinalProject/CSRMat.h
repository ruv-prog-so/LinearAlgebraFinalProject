
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>

#ifndef _INCL_CSRMAT_H_
#define _INCL_CSRMAT_H_

class CSRMat {
  // Variables
	std::vector<double> vals;   // Matrix non zero values
	std::vector<int> colIndx;   // Column values
	std::vector<int> rowIndx;    // Row begin pointer values
	int m_row_count = 0;
	int m_column_count = 0;
	int m_value_count = 0;

  // Function declaration
  private:
	bool compareTuple(const std::tuple<double, int, int> lhs, const std::tuple<double, int, int> rhs);

  public:
	CSRMat(void);
	~CSRMat(void);
	CSRMat(std::string strFile);  // Pass in a file to load a matrix into an CSR matrix
	CSRMat(int r, int c);  // Create a new matrix with r rows and c columns
	CSRMat(CSRMat* given);
	CSRMat(double vals[], int colIndx[], int rowPtr[]);
	void MulScalar(double s);
	void MulMatrix(CSRMat mul);
	void Identity(int r, int c);
	void Identity(int r);
	static CSRMat getTransposed(CSRMat givenMatrix);
	void Transpose();
	bool isSymmetric();
	int readCSRFile(std::string strFile);
	void print();
	int getRowCount() { return m_row_count; }
	int getColumnCount() { return m_column_count; }
};

#endif