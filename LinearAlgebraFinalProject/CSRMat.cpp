/*

http://netlib.org/linalg/html_templates/node91.html
*/

#include "CSRMat.h"

using namespace std;

// Default Constructor
CSRMat::CSRMat(void)
{

}

// Destructor
CSRMat::~CSRMat(void)
{
}

// Takes in a filename string to import a matrix
// Takes in a type for specifying which type of matrix file
CSRMat::CSRMat(std::string fileName, int type, bool hasVal)
{
	if (type == FILE_TYPE_MAT) {
		if (readMatFile(fileName) != 0)
			std::cout << "Error\n";
	}
	else if (type == FILE_TYPE_ADJ) {
		if (readCSRFile(fileName, hasVal) != 0)
			std::cout << "Error\n";
	}
	else {
		std::cout << "Invalid File Type\n";
	}
}

// Constructor that takes in three arrays to form the CSR
CSRMat::CSRMat(double valArr[], int colArr[], int rowArr[], int rows, int cols)
{
	int vSize = sizeof(valArr) / sizeof(double);
	int cSize = sizeof(colArr) / sizeof(int);
	int rSize = sizeof(rowArr) / sizeof(int);

	std::vector<double> vals_list(vSize);
	std::vector<int> col_indx_list(cSize);
	std::vector<int> row_indx_list(rSize);

	for (int i = 0; i < vSize; ++i)
		vals_list.push_back(valArr[i]);

	for (int i = 0; i < vSize; ++i)
		col_indx_list.push_back(colArr[i]);

	for (int i = 0; i < vSize; ++i)
		row_indx_list.push_back(rowArr[i]);

	m_value_count = vSize;
	m_row_count = rows;
	m_column_count = cols;
}

// Constructor that takes in row size and column size to initialize an matrix
CSRMat::CSRMat(int rows, int cols)
{
	m_row_count = rows;
	m_column_count = cols;
}

// Copy a given matrix over to current matrix
CSRMat::CSRMat(CSRMat* givenMatrix)
{
	int gmValSize = givenMatrix->vals_list.size();
	int gmColSize = givenMatrix->col_indx_list.size();
	int gmRowSize = givenMatrix->row_indx_list.size();

	for (int i = 0; i < gmValSize; ++i)
		vals_list.push_back(givenMatrix->vals_list[i]);

	for (int i = 0; i < gmColSize; ++i)
		col_indx_list.push_back(givenMatrix->col_indx_list[i]);

	for (int i = 0; i < gmRowSize; ++i)
		row_indx_list.push_back(givenMatrix->row_indx_list[i]);

	m_value_count = givenMatrix->m_value_count;
	m_column_count = givenMatrix->m_column_count;
	m_row_count = givenMatrix->m_row_count;
}

// Multiplication of the current CSR Matrix with a scalar value
void CSRMat::MulScalar(double s)
{
	for (int i = 0; i < m_value_count; ++i)
		vals_list[i] = s*vals_list[i];
}

// Multiple matrix by a vector
// Takes in an address to a vector
// Returns a vector with values
vector<double> CSRMat::MulVector(vector<double> *vect)
{
	if (m_column_count != vect->size()) {
		throw ROW_COL_MISMATCH;
	}

	vector<double> result(m_row_count, 0);  // Initialize the result vector

	for (int i = 1; i <= m_row_count; i++) {
		for (int j = 1; j <= m_column_count; j++) {
			result[i - 1] += (getValueAt(i, j)*(vect->at(j - 1)));
		}
	}

	return result;
}

// Mutiply current matrix with the given matrix
// Takes in a CSRMat object
// Returns a CSRMat object
CSRMat CSRMat::MulByMat(CSRMat mulMat)
{
	// check if this matrix row size == to given matrix column size
	CSRMat result(m_row_count, mulMat.getColumnCount());  // the result will be this size

	try
	{
		if (m_row_count != mulMat.getColumnCount())
			throw ROW_COL_MISMATCH;
		
		// given const SparseMatrix & m
		
		int rowSum;

		// Loop through the row index array
		for (int i = 1; i <= row_indx_list.size(); i++) {
			// Loop through the given matrix columns
			for (int j = 1; j <= mulMat.getColumnCount(); j++) {
				rowSum = 0;
				// Loop through the column index
				for (int k = 1; k <= m_column_count; k++) {
					double a = getValueAt(i, k);
					double b = mulMat.getValueAt(k, j);
					rowSum += a*b;
				}

				result.setValueAt(rowSum, i, j);  // Places the row sum into the matrix
			}
		}
		return result;
	}
	catch (int e) {
		std::cout << "Error " << e << " - Column size and row size mismatch.";
		return result;
	}
	
}

// Takes the current CSR Matrix and transposes it
// Since 0 rows are not recorded, a symmetric matrix will not be semmetric after transpose.
// Takes in nothing
// Returns nothing
void CSRMat::Transpose()
{
	CSRMat val;
	std::vector<std::tuple<double, int, int>> tempList;
	int rowT = 0;
	int cnt = vals_list.size();

	// Parse CSR format to tuples of three values
	for (int i = 0; i < row_indx_list.size(); ++i)
	{
		int nextRItem = 0;

		if ((i+1) < row_indx_list.size())
			nextRItem = row_indx_list[i + 1];  // Get the next row item (col Value)

		for (int j = row_indx_list[i]; j < nextRItem; ++j)
		{
			//std::tuple<double, int, int>
			auto tempTu  = std::make_tuple (vals_list[j], i, col_indx_list[j]);  // temporary tuple
			tempList.push_back(tempTu);  // Add to the list
		}
	}

	//cout << "\nsorted list \n\n";
	// Sort the temp list
	std::sort(tempList.begin(), tempList.end(), [](auto const &t1, auto const &t2) {
		return std::get<2>(t1) < std::get<2>(t2);
	});

	vals_list.clear(); // Clear the values
	row_indx_list.clear(); // Clear the row indexes
	col_indx_list.clear(); // Clear the column indexes
	
	int tmpRowCnt = m_row_count;  // Reset the counters
	m_row_count = m_column_count;
	m_column_count = tmpRowCnt;

	int prevColIndx = 0;
	int prevRowVal = 0;
	int tListSize = tempList.size();
	//row_indx_list.push_back(0); // For beginning of index

	// Loop through the list to create a new array
	for (int i = 0; i < tListSize; ++i)
	{
		std::tuple<double, int, int> tempT = tempList[i];
		int colI = std::get<1>(tempT);
		int rowVal = std::get<2>(tempT);

		vals_list.push_back(std::get<0>(tempT)); // Add the new value to the vals_list list

		if (colI == rowVal)
			col_indx_list.push_back(colI); // Add the new value to the col index list
		else
			col_indx_list.push_back(colI); // Add the new value to the col index list

		if (prevRowVal < rowVal) {
			row_indx_list.push_back(i);  // Add the current column index to the row index list
			prevRowVal = rowVal;
		}

		prevColIndx = colI;  // Set the previous column index for next iteration
		if (i == (tListSize - 1))
			row_indx_list.push_back(i); // Add the last index for the col index
	}
}


// Transpose a given matrix
// Takes in nothing
// Returns the current CSRMatrix transposed
CSRMat CSRMat::TransposeMat(CSRMat givenMat)
{
	CSRMat result;

	result = CSRMat(givenMat);
	result.Transpose();

	return result;
}

// Make this matrix an identity matrix with the given columns and rows
void CSRMat::Identity(int r, int c)
{
	int iter = 0;

	m_row_count = r;
	m_column_count = c;

	row_indx_list.push_back(0);

	if (r >= c)
		iter = c;
	else
		iter = r;

	for (int i = 0; i < iter; ++i)
	{
		vals_list.push_back(1);
		col_indx_list.push_back(i);
		row_indx_list.push_back(i+1);
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
	bool result = true;

	if (m_column_count > m_row_count || m_row_count > m_column_count)
		result = false;
	else {
		CSRMat temp(this);
		temp.Transpose();  // Transpose the temp array
		
		// Compare the values
		for (int i = 0; i < vals_list.size() && result; ++i) {
			if (vals_list[i] != temp.vals_list[i]) {
				result = false;
				break;
			}
		}

		// Compare the columns
		for (int i = 0; i < col_indx_list.size() && result; ++i) {
			if (col_indx_list[i] != temp.col_indx_list[i]) {
				result = false;
				break;
			}
		}

		// Compare the rows
		for (int i = 0; i < row_indx_list.size() && result; ++i) {
			if (row_indx_list[i] != temp.row_indx_list[i]) {
				result = false;
				break;
			}
		}
	}

	return result;
}


// Prints the CSR Matrix
// Takes in nothing
// Returns nothing
void CSRMat::print()
{
	int vArrSize = vals_list.size();
	int cArrSize = col_indx_list.size();
	int rArrSize = row_indx_list.size();

	std::cout << "Rows x Cols: [" << m_row_count << "] x [" << m_column_count << "]\n";
	std::cout << "Vals List Count [" << vals_list.size()
		<< "]\t Cols List Count [" << col_indx_list.size()
		<< "]\t Rows List Count [" << row_indx_list.size() << "]\n";

	std::cout << "Values: \t[ ";
	for (int i = 0; i < vArrSize; ++i) {
		std::cout << vals_list[i] << " ";
	}

	std::cout << "]\n" << "Columns: \t[ ";
	for (int i = 0; i < cArrSize; ++i) {
		std::cout << col_indx_list[i] << " ";
	}

	std::cout << "]\n" << "Row Pointer: \t[ ";
	for (int i = 0; i < rArrSize; ++i) {
		std::cout << row_indx_list[i] << " ";
	}

	std::cout << "]" << std::endl << std::endl;
}


// Reads in a Matrix formatted file
// Takes in a file name string
// Returns nothing
/* Format for a 4x4 matrix is as follows:
	    0 1 2 3 4
		5 6 7 8 9
		10 11 12 13
*/
int CSRMat::readMatFile(std::string strFile)
{
	std::ifstream file;
	// Temp vars for reading in values from file
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

	// Loop through every line in file
	while (!file.eof())
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
			  vals_list.push_back(n);  // Add the value to the list
			  ++m_value_count;
			  col_indx_list.push_back(colCnt);  // Add the column index of 

			  if (!rowFlag)  // Row value has not been set
			  {
				  row_indx_list.push_back(col_indx_list.size() - 1);
				  rowFlag = true;
			  }
			}
			
		++colCnt;  // Increment the column count
		}

		++rowCnt;  // Increment the row
		rowFlag = false;  // Reset row flag
	}

	row_indx_list.push_back(col_indx_list.size());
	m_row_count = rowCnt - 1;
	m_column_count = colCnt - 1;

	file.close();

	return 0;
}

// Reads in a CSR formatted file
// Takes in a file name string
// Returns nothing
/* Format for a 4x4 matrix is as follows:

*/
int CSRMat::readCSRFile(std::string strFile, bool hasVal)
{
	std::ifstream file;
	// Temp vars for reading in values from file
	int colCnt = 0;
	int rowCnt = 1;
	bool rowFlag = false;
	std::string line;
	int vertCnt = 0;  // Verticies Count
	int edgeCnt = 0;  // Edge Count
	bool gotVertCnt = false;
	bool gotEdgeCnt = false;

	int lineNum = 1;  // Line number in the file
	vector<std::tuple<int, int, double>> tuples;  // Stores the tuples and the values

	int randNum = 0;
	int Max_Rng = 1;
	int Min_Rng = -1;
	srand(time(NULL));  // Random number seed

	// ====== Read in File ======
	file.open(strFile);  // Open the file

	if (!file.is_open()) {
		std::cout << "Error opening the input file!\n";
		return -1;
	}

	// Loop through every line in file
	while (!file.eof())
	{
		std::getline(file, line);  // Get the next line
		std::stringstream ss(line);  // Use stream to convert string to stream

		colCnt = 0;  // Init cindex to be 1 for beginning of every row
		tuple<int, int, double> tempTuple;
		int vert1 = -1;
		int vert2 = -1;
		int lineVal = -1;
		double randVal;     // Random number
		vector<int> lineVals;

		if (lineNum <= 2) {
			

			while (ss)
			{
				double n = 0;
				ss >> n;  // put value into n

				if (lineNum == 1 && !gotVertCnt) {
					vertCnt = (int)floor(n); // get the verticies count
					gotVertCnt = true;
				}
				else if (lineNum == 2 && !gotEdgeCnt) {
					edgeCnt = (int)floor(n); // Get the edge count
					gotEdgeCnt = true;
				}
			}
		}
		else {
			if (hasVal)  // file has a value field i j val
			{
				ss >> vert1 >> vert2 >> lineVal;

				// Check the values taken in
				if (lineNum > 2 && vert1 > -1 && vert2 > -1) {
					randVal = rand() % 10 + 1; // Generated random number between min and max

					if (randVal <= 5)
						randVal = -1;
					else
						randVal = 1;

					auto tempTuple = make_tuple(vert1, vert2, lineVal);  // Make temporary tuple
					tuples.push_back(tempTuple);  // Add to tuples list
				}
			}
			else { // File does not have value, so just i,j
				ss >> vert1 >> vert2;

				// Check the values taken in
				if (lineNum > 2 && vert1 > -1 && vert2 > -1) {
					randVal = rand() % 10 + 1; // Generated random number between min and max

					if (randVal <= 5)
						randVal = -1;
					else
						randVal = 1;

					auto tempTuple = make_tuple(vert1, vert2, 1);  // Make temporary tuple
					tuples.push_back(tempTuple);  // Add to tuples list
				}
			}
		}

		++lineNum; // Increment the line count
	}

	file.close();
	processTuples(&tuples, vertCnt);  // Process the given tuples

	return 0;
}

// Processes the tuples into values, columns, and rows to populate the CSR
// It is only for the readCSRFile() function
// takes in a pointer to the tuples vector list
// Returns nothing
void CSRMat::processTuples(vector<std::tuple<int, int, double>> *tuplesList, int vertCnt) {

	int tplListSize = tuplesList->size();
	int rowLine = 0;

	row_indx_list.push_back(0);

	for (int i = 0; i < tplListSize; ++i) {
		int tempRow = std::get<0>(tuplesList->at(i));
		int tempCol = std::get<1>(tuplesList->at(i));
		double tempVal = std::get<2>(tuplesList->at(i));

		vals_list.push_back(tempVal);     // Add value to the values list
		col_indx_list.push_back(tempCol); // Add the column

		if (rowLine != tempRow) {
			row_indx_list.push_back(i); // Add the index of the column to the row
			++rowLine;
		}
	}

	row_indx_list.push_back(tplListSize-1); // Add the index of the column to the row

	m_row_count = vertCnt;  // Gets the last row number
	m_column_count = vertCnt;                   // sets the column count
	m_value_count = vals_list.size();               // Sets the values list count
}

// Gets the value of the given row and column
// Takes in a row and column
// Returns a double value
double CSRMat::getValueAt(int row, int col)
{
	if (row > m_row_count || col > m_column_count)
		throw OUT_OF_RANGE;

	double result = 0;
	int colVal = row_indx_list[row - 1]-1; // get the column

	// Start at row index
	for (int i = colVal; i < row_indx_list[row]-1; ++i) {
		// if the column with given index value is the same as the input column
		if (col_indx_list[i] == col)
			result = vals_list[i];  // Set the value, otherwise it will remain 0
	}

	return result;
}

// Sets the value of the given row and column
// Takes in a value, row and column
// Returns nothing
void CSRMat::setValueAt(double value, int row, int col)
{
	// Validate row and column bounds
	if (row >= m_row_count && col >= m_column_count)
		throw OUT_OF_RANGE;

	int indx = row_indx_list[row - 1] - 1;
	int cIndx = -1;  // Column index

	for (int i = indx; i < row_indx_list[row] - 1; ++i) {
		cIndx = col_indx_list[i];

		if (cIndx == col)
			break;
		else if (cIndx > col)
			break;
	}

	// The value of the row pointer to col does not exist in the colIndex array
	if (cIndx != col) {
		// Insert the value if it is not a 0
		if (value != 0)
			insertAt(value, row, col, indx);
		else if (value == 0);
			// If the new value to overwrite is 0, 
			// then remove the selected value from CSR matrix
			removeAt(indx, row);
	} else
		vals_list[indx] = value; // Otherwise update the value in the vals_list array
}

// Insert a value at the given row / column
void CSRMat::insertAt(double value, int row, int column, int indx)
{
	// Validate row and column bounds
	if (row >= m_row_count && column >= m_column_count)
		throw OUT_OF_RANGE;

	// .....
}

// Remove the given value on given row and given col
// Takes in a column index value and a row value
void CSRMat::removeAt(int cIndex, int row)
{
	int rIndx = row - 1;     // row index
	int nextRowColIndx = 0;  // for checking if row value needs to be removed

	// Validate the row size
	if (nextRowColIndx < m_row_count)
		nextRowColIndx = row_indx_list[row];

	for (int i = cIndex; i < row_indx_list[row] - 1; ++i)
		row_indx_list[i] = row_indx_list[i] - 1; // Update the rows indexes after

	// Check to see if the row index needs to be removed from the list
	for (int j = rIndx; j < row_indx_list.size(); ++j) {
		int k = j + 1; // next index

		if (k < row_indx_list.size() - 1) {     // If in range
			if (row_indx_list[j] == row_indx_list[k]) // If the values are the same
				row_indx_list.erase(row_indx_list.begin() + rIndx); // Remove one
		} else
			break;
	}
}
