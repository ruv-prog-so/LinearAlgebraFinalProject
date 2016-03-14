
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <algorithm>
#include <time.h>

#ifndef _INCL_CSRMAT_H_
#define _INCL_CSRMAT_H_

using namespace std;

const int ROW_COL_MISMATCH = 5;
const int OUT_OF_RANGE = 6;
const int NULL_VALUE = 7;

// File type constants
const int FILE_TYPE_MAT = 1;
const int FILE_TYPE_ADJ = 2;
const bool FILE_ADJ_CSR_HAS_VALUE = true;

class CSRMat {
  // Variables
	std::vector<double> vals_list;   // Matrix non zero values
	std::vector<int> col_indx_list;   // Column values
	std::vector<int> row_indx_list;   // Row begin pointer values
	int m_row_count = 0;
	int m_column_count = 0;
	int m_value_count = 0;

  // Function declaration
  private:
	void removeAt(int col, int row); // Remove the given value on given row and given col
	void insertAt(double value, int row, int column, int indx); // Insert a value at the given row / column
	void processTuples(vector<std::tuple<int, int, double>> *tuples, int vertCnt); // Process the tuples into the CSR

  public:
	CSRMat(void);
	~CSRMat(void);
	CSRMat(std::string strFile, int type, bool hasVal);     // Pass in a file to load a matrix into an CSR matrix
	CSRMat(int rowCnt, int colCnt);            // Create a new matrix with r rows and c columns
	CSRMat(CSRMat* given);
	CSRMat(double valArr[], int colArr[], int rowArr[], int rows, int cols);
	
	void MulScalar(double s);
	vector<double> MulVector(vector<double> *vect);
	CSRMat MulByMat(CSRMat *mul);
	
	void Identity(int r, int c);
	void Identity(int r);

	static CSRMat TransposeMat(CSRMat givenMatrix);
	void Transpose();
	bool isSymmetric();
	void print();

	int readMatFile(std::string strFile);
	int readCSRFile(std::string strFile, bool hasVal);
	

	double getValueAt(int row, int col);
	void setValueAt(double value, int row, int col);

	int getRowCount() { return m_row_count; }
	int getColumnCount() { return m_column_count; }
};

#endif