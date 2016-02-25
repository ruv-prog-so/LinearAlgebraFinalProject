/*

http://netlib.org/linalg/html_templates/node91.html
*/

#include "CSRMat.h"


// Default Constructor
CSRMat::CSRMat(void)
{

}

// Destructor
CSRMat::~CSRMat(void)
{
}

CSRMat::CSRMat(std::string fileName)
{
	if (readCSRFile(fileName) != 0)
	{
		std::cout << "Error\n";
	}
}

// Constructor that takes in three arrays to form the CSR
CSRMat::CSRMat(double vals[], int colIndx[], int rowPtr[])
{

}


// Copy a given matrix over to current matrix
CSRMat::CSRMat(CSRMat* givenMatrix)
{
	int gmValSize = givenMatrix->vals.size();
	int gmColSize = givenMatrix->colIndx.size();
	int gmRowSize = givenMatrix->rowIndx.size();

	for (int i = 0; i < gmValSize; ++i)
		vals.push_back(givenMatrix->vals[i]);

	for (int i = 0; i < gmColSize; ++i)
		colIndx.push_back(givenMatrix->colIndx[i]);

	for (int i = 0; i < gmRowSize; ++i)
		rowIndx.push_back(givenMatrix->rowIndx[i]);

	m_value_count = givenMatrix->m_value_count;
	m_column_count = givenMatrix->m_column_count;
	m_row_count = givenMatrix->m_row_count;
}


// Multiplication of the current CSR Matrix with a scalar value
void CSRMat::MulScalar(double s)
{
	for (int i = 0; i < m_value_count; ++i)
	{
		vals[i] = s*vals[i];
	}
}

// Mutiply current matrix with the given matrix
void CSRMat::MulMatrix(CSRMat mul)
{
	// check if this matrix row size == to given matrix column size
	try
	{
		if (m_row_count != mul.getColumnCount())
		{
			throw 1;
		}
		else
		{
			// Multiply the matrix
		}

	}
	catch (int e)
	{
		std::cout << "Error " << e << " - Column size and row size mismatch.";
	}
	return;
}

// Takes the current CSR Matrix and transposes it
// Takes in nothing
// Returns nothing
void CSRMat::Transpose()
{
	CSRMat val;
	std::vector<std::tuple<double, int, int>> tempList;
	int rowT = 0;
	int cnt = vals.size();


	// Parse CSR format to tuples of three values
	for (int i = 0; i < m_row_count; ++i)
	{
		int nextRItem = 0;
		if ((i+1) < rowIndx.size())
			nextRItem = rowIndx[i + 1];
		for (int j = rowIndx[i]; j < nextRItem; ++j)
		{
			//std::tuple<double, int, int>
			auto tempTu  = std::make_tuple (vals[j], i+1, colIndx[j]);  // temporary tuple
			tempList.push_back(tempTu);  // Add to the list
		}
	}

	// Sort the temp list
	std::sort(tempList.begin(), tempList.end(), [](auto const &t1, auto const &t2) {
		return std::get<2>(t1) < std::get<2>(t2);
	});

	vals.clear(); // Clear the values
	rowIndx.clear(); // Clear the row indexes
	colIndx.clear(); // Clear the column indexes
	
	int tmpRowCnt = m_row_count;  // Reset the counters
	m_row_count = m_column_count;
	m_column_count = tmpRowCnt;

	int prevColIndx = 0;
	int tListSize = tempList.size();
	rowIndx.push_back(0); // For beginning of index

	// Loop through the list to create a new array
	for (int i = 0; i < tListSize; ++i)
	{
		std::tuple<double, int, int> tempT = tempList[i];
		int colI = std::get<1>(tempT);

		vals.push_back(std::get<0>(tempT)); // Add the new value to the vals list
		colIndx.push_back(colI); // Add the new value to the col index list

		if (prevColIndx >= colI)
			rowIndx.push_back(i);  // Add the current column index to the row index list

		prevColIndx = colI;  // Set the previous column index for next iteration
		if (i == (tListSize - 1))
			rowIndx.push_back(i + 1); // Add the last index for the col index
	}
}


// Transpose a given matrix
// Takes in nothing
// Returns the current CSRMatrix transposed
CSRMat CSRMat::getTransposed(CSRMat givenMat)
{
	CSRMat result;

	result = new CSRMat(givenMat);
	result.Transpose();

	return result;
}

// Make this matrix an identity matrix with the given columns and rows
void CSRMat::Identity(int r, int c)
{
	m_row_count = r;
	m_column_count = c;

	rowIndx.push_back(0);

	for (int i = 0; i < r; ++i)
	{
		vals.push_back(1);
		colIndx.push_back(i);
		rowIndx.push_back(i);
	}
}

// Make this matrix an identity with rxr size
void CSRMat::Identity(int r)
{
	Identity(r, r);
}

// Checks if the current CSR Matrix is a symmetric matrix
bool CSRMat::isSymmetric()
{
	bool val = false;

	return val;
}


// Prints the CSR Matrix
// Takes in nothing
// Returns nothing
void CSRMat::print()
{
	int vArrSize = vals.size();
	int cArrSize = colIndx.size();
	int rArrSize = rowIndx.size();

	std::cout << "Rows: [" << m_row_count << "]" << std::endl
		<< "Columns: [" << m_column_count << "]" << std::endl;

	std::cout << "Values: \t[";
	for (int i = 0; i < vArrSize; ++i)
		std::cout << vals[i] << " ";

	std::cout << "]\n" << "Columns: \t[";
	for (int i = 0; i < cArrSize; ++i)
		std::cout << colIndx[i] << " ";

	std::cout << "]\n" << "Row Pointer: \t[";
	for (int i = 0; i < rArrSize; ++i)
		std::cout << rowIndx[i] << " ";

	std::cout << "]" << std::endl << std::endl;
}


// Reads in a CSR formatted file
// Takes in a file name string
// Returns nothing
/* Format is as follows:
	    val = [0, 1, 2, 2, 4]
		cIndx = [0, 0, 1, 2]
		rowPtr = [1, 2, 3, 4]
*/
int CSRMat::readCSRFile(std::string strFile)
{
	std::ifstream file;
	// Temp vars for reading in values from file
	double dNum = 0.0;
	int iNum = 0;
	int col = 0;
	int colCnt = 0;
	int rowCnt = 1;
	bool rowFlag = false;
	std::string line;


	// ====== Read in File ======
	file.open(strFile);  // Open the file

	if (!file.is_open()) {
		std::cout << "Error opening the input file!\n";
		return -1;
	}

	//std::getline(file, line);  // Prime the pump

	while (!file.eof())  // Loop through every line in file
	{
		std::getline(file, line);  // Get the next line

		std::stringstream stream(line);  // Use stream to convert string to stream
		colCnt = 0;  // Init cindex to be 1 for beginning of every row

		while (stream)
		{
			double n = 0;
			stream >> n;  // put value into n

			// Check incoming value
			if (n > 0)
			{
			  vals.push_back(n);  // Add the value to the list
			  ++m_value_count;
			  colIndx.push_back(colCnt);  // Add the column index of 

			  if (!rowFlag)  // Row value has not been set
			  {
				  rowIndx.push_back(colIndx.size() - 1);
				  rowFlag = true;
			  }
			  //else
				  //rowIndx.push_back(NULL);
			}
			
			++colCnt;  // Increment the column count
		}

		++rowCnt;  // Increment the row
		rowFlag = false;  // Reset row flag
	}

	rowIndx.push_back(colIndx.size());
	m_row_count = rowCnt - 1;
	m_column_count = colCnt - 1;

	file.close();
	
	return 0;
}

// For comparing tuples with three values
// Takes and compares the third item in the tuple
//bool CSRMat::compareTuple(const std::tuple<double, int, int> lhs, const std::tuple<double, int, int> rhs)
//{
//	return std::get<2>(lhs) < std::get<2>(rhs);
//}