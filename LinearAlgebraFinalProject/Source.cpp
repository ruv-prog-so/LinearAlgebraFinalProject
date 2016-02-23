#include <cstdio>
#include <fstream>
#include "CSRMat.h"

using namespace std;

int main(int argc, char *argv[])
{
	CSRMat mat1 = CSRMat("Matrix1.txt");
	CSRMat mat2 = CSRMat("Matrix2.txt");
	CSRMat mat3 = CSRMat("Matrix4.txt");

	mat1.print();
	mat2.print();
	mat3.print();

	return 0;
}