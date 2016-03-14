#include <cstdio>
#include <fstream>
#include "CSRMat.h"

using namespace std;

void checkTranspose();
void checkMultiplyVector();
void checkMultiplyMat();
void checkIdentity();
void checkAdjFileInput();

int main(int argc, char *argv[])
{
	//checkTranspose();
	//checkMultiplyVector();
	checkMultiplyMat();
	//checkIdentity();
	//checkAdjFileInput();

	return 0;
}

// Checks the transpose function
void checkTranspose()
{
	cout << "\n============ Matrix Transpose =============\n";
	CSRMat mat4 = CSRMat("Matrix_sym2_partial.txt", 1, false);
	cout << "\n ----- Mat Original -----\n";
	mat4.print();

	cout << "\n ----- Mat Transposed -----\n";
	mat4.Transpose();
	mat4.print();
}

void checkMultiplyVector()
{
	cout << "\n============ Multiplying Vector by Vect =============\n";
	CSRMat mat2 = CSRMat("Matrix3.txt", 1, true);
	vector<double> vect(mat2.getColumnCount());
	vect = {2, 3, 1, 5, 9, 1};

	cout << endl << "Vector values: [";
	for (int i = 0; i < vect.size(); ++i)
		cout << vect[i] << " ";
	cout << "]\n\n";

	mat2.print();
	cout << "\n----- Multiplied Result: -----\n";
	vector<double> res = mat2.MulVector(&vect);
	
	cout << endl << "Result Vector values: [";
	for (int i = 0; i < res.size(); ++i)
		cout << res[i] << " ";
	cout << "]\n";
}

void checkMultiplyMat()
{
	cout << "\n============ Multiplying Mat by Mat =============\n";
	CSRMat mat1 = CSRMat("Matrix3.txt", 1, true);
	CSRMat mat2 = CSRMat("Matrix3b.txt", 1, true);
	CSRMat matRes = CSRMat();

	
	cout << "\n----- Mat 1 --------\n";
	mat1.print();
	
	cout << "\n----- Mat 2 --------\n";
	mat2.print();

	cout << "\n----- Multiplied Result: ------\n";
	matRes = mat1.MulByMat(mat2);
	matRes.print();
}

void checkIdentity()
{
	cout << "\n============ Create an Identity Mat =============\n";
	CSRMat mat1 = CSRMat();
	CSRMat matId44 = CSRMat("Matrix_Zero.txt", 1, true);
	mat1.Identity(5,3);
	mat1.print();
}

void checkAdjFileInput()
{
	cout << "\n============ Check Graph List file input =============\n";
	CSRMat matAdj = CSRMat("test1.edgelist", 2, true);

	cout << "\n----- ADJ MAT -----\n";
	matAdj.print();

	/*if (matAdj.isSymmetric())
	cout << "MatAdj is symmetric\n";
	else
	cout << "MatAdj is not symmetric\n";*/
}