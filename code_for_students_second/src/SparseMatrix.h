#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include <string>

#include "../../util/GetMemUsage.h"
#include "../../util/LogManager.h"

class SparseMatrix {
protected:
	int rows, cols;

	struct Node {
		int col;
		int value;
		Node* next;

		Node(int c, int v) : col(c), value(v), next(nullptr) {}
	};

	Node** rowHeads;

public:
	SparseMatrix(char* matrixFilePath);
	SparseMatrix(int numRows, int numCols);
	~SparseMatrix();
	void printToASCIIFile(char* outputFileName);
	int getElement(int currRow, int currCol);
	int setElement(int currRow, int currCol, int value);

	SparseMatrix operator+(SparseMatrix& inputObject);
	SparseMatrix operator-(SparseMatrix& inputObject);
	SparseMatrix operator*(SparseMatrix& inputObject);
};

class SparseMatrixTester {
	static int genRandomInt();
	static int getRandomInt(int min, int max);
	static void generateTestCases(char* outputFilePath, int min, int max, int numRows, int numCols);

public:
	static void generateTestCases(char* outputFolderPath);
};

#endif /* MATRIX_H_ */
