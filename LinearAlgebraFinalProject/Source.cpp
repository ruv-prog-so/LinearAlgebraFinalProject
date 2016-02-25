#include <cstdio>
#include <fstream>
#include "CSRMat.h"

using namespace std;

int main(int argc, char *argv[])
{
	CSRMat mat1 = CSRMat("Matrix1.txt");
	CSRMat mat2 = CSRMat("Matrix2.txt");
	CSRMat mat4 = CSRMat("Matrix4.txt");
	CSRMat mat4t = CSRMat("Matrix4t.txt");

	//mat1.print();
	//mat2.print();
	mat4t.print();
	mat4.print();
	mat4.Transpose();
	mat4.print();

	return 0;
}