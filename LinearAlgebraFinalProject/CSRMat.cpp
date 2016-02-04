/*

http://netlib.org/linalg/html_templates/node91.html
*/

#include "CSRMat.h"


// Default Constructor
CSRMat::CSRMat(void)
{

}

CSRMat::CSRMat(std::string fileName)
{
	if (readCSRFile(fileName) == 0)
	{
		std::cout << "Congrats\n";
	}
	else
	{
		std::cout << "Error\n";
	}
}

// Constructor that takes in three arrays to form the CSR
CSRMat::CSRMat(double vals[], int colIndx[], int rowPtr[])
{

}


// Multiplication of the current CSR Matrix with another CSR Matrix
CSRMat CSRMat::Multiply(CSRMat matB)
{
	CSRMat val;

	return val;
}

// Takes the current CSR Matrix and transposes it
CSRMat CSRMat::Transpose()
{
	CSRMat val;

	return val;
}

// Checks if the current CSR Matrix is a symmetric matrix
bool CSRMat::isSymmetric()
{
	bool val = false;

	return val;
}

// Reads in a CSR formatted file
/* Format is as follows:
	    val = [0, 1, 2, 2, 4]
		cIndx = [0, 0, 1, 2]
		rowPtr = [1, 2, 3, 4]
*/
int CSRMat::readCSRFile(std::string strFile)
{
	std::ifstream file;
	// Vectors that store file read values temporarily
	std::vector<double> weights;
	std::vector<int> colIndex;
	std::vector<int> rowPointer;
	// Temp vars for reading in values from file
	double dNum = 0.0;
	int iNum = 0;
	char strTitle[50];
	std::string temp;

	file.open(strFile);  // Open the file

	if (!file.is_open()) {
		std::cout << "Error opening the input file!\n";
		return -1;
	}

	std::getline(file, temp);
	// http://stackoverflow.com/questions/19887232/how-to-loop-through-a-string-by-space-how-do-i-know-the-index-no-of-the-word-i
	while (!file.eof())
	{
		// find "val = ["
		while (file >> dNum)
			weights.push_back(dNum);
		// if next char is ], then go to next line

		// Find "cIndx = ["
		while (file >> iNum)
			colIndex.push_back(iNum);
		// if next char is ], then go to next line

		// Find "rowPtr = ["
		while (file >> iNum)
			rowPointer.push_back(iNum);
		// if next char is ], then finish reading in file

		std::getline(file, temp);
	}

	file.close();

	//verify that the weights were stored correctly:
	std::cout << "\n Weights:\n";
	for (int i = 0; i < weights.size(); ++i) {
		std::cout << weights[i] << std::endl;
	}

	std::cout << "\n colIndx:\n";
	for (int i = 0; i < colIndex.size(); ++i) {
		std::cout << colIndex[i] << std::endl;
	}

	std::cout << "\n rowPtr:\n";
	for (int i = 0; i < rowPointer.size(); ++i) {
		std::cout << rowPointer[i] << std::endl;
	}

	// Allocate memory for arrays

	return 0;
}