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

#endif /* MATRIX_H_ */#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(char* matrixFilePath) : rows(0), cols(0), rowHeads(nullptr) {
    FILE* inFileStream = fopen(matrixFilePath, "r");
    if (!inFileStream) {
        char* message = new char[2048];
        sprintf(message, "Cannot open input file for reading");
        throw std::ios_base::failure(message);
        delete[] message;
    }

    LogManager::writePrintfToLog(LogManager::Level::Status, "SparseMatrix::SparseMatrix",
                                "Loading input file: %s", matrixFilePath);

    char line[1024];
    if (!fgets(line, sizeof(line), inFileStream)) {
        fclose(inFileStream);
        char* message = new char[2048];
        sprintf(message, "Input file has wrong format");
        throw std::invalid_argument(message);
        delete[] message;
    }

    // Parse rows
    if (strncmp(line, "rows=", 5) != 0 || sscanf(line, "rows=%d", &rows) != 1 || rows <= 0) {
        fclose(inFileStream);
        char* message = new char[2048];
        sprintf(message, "Input file has wrong format");
        throw std::invalid_argument(message);
        delete[] message;
    }

    // Parse cols
    if (!fgets(line, sizeof(line), inFileStream)) {
        fclose(inFileStream);
        char* message = new char[2048];
        sprintf(message, "Input file has wrong format");
        throw std::invalid_argument(message);
        delete[] message;
    }

    if (strncmp(line, "cols=", 5) != 0 || sscanf(line, "cols=%d", &cols) != 1 || cols <= 0) {
        fclose(inFileStream);
        char* message = new char[2048];
        sprintf(message, "Input file has wrong format");
        throw std::invalid_argument(message);
        delete[] message;
    }

    // Initialize rowHeads
    rowHeads = new Node*[rows];
    for (int i = 0; i < rows; i++) {
        rowHeads[i] = nullptr;
    }

    // Read matrix entries
    while (fgets(line, sizeof(line), inFileStream)) {
        // Skip empty or whitespace-only lines
        bool onlyWhitespace = true;
        for (int i = 0; line[i] != '\0'; i++) {
            if (!isspace(line[i])) {
                onlyWhitespace = false;
                break;
            }
        }
        if (onlyWhitespace) continue;

        int row, col, value;
        if (sscanf(line, "(%d, %d, %d)", &row, &col, &value) != 3) {
            fclose(inFileStream);
            char* message = new char[2048];
            sprintf(message, "Input file has wrong format");
            throw std::invalid_argument(message);
            delete[] message;
        }

        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            fclose(inFileStream);
            char* message = new char[2048];
            sprintf(message, "Invalid row or column index in input file");
            throw std::invalid_argument(message);
            delete[] message;
        }

        setElement(row, col, value);
    }

    fclose(inFileStream);
}

SparseMatrix::SparseMatrix(int numRows, int numCols) : rows(numRows), cols(numCols), rowHeads(nullptr) {
    if (numRows <= 0 || numCols <= 0) {
        char* message = new char[2048];
        sprintf(message, "Invalid matrix dimensions");
        throw std::invalid_argument(message);
        delete[] message;
    }

    rowHeads = new Node*[numRows];
    for (int i = 0; i < numRows; i++) {
        rowHeads[i] = nullptr;
    }
}

SparseMatrix::~SparseMatrix() {
    for (int i = 0; i < rows; i++) {
        Node* current = rowHeads[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] rowHeads;
}

void SparseMatrix::printToASCIIFile(char* outputFileName) {
    FILE* outFileStream = fopen(outputFileName, "w");
    if (!outFileStream) {
        char* message = new char[2048];
        sprintf(message, "Cannot open output file for writing");
        throw std::ios_base::failure(message);
        delete[] message;
    }

    LogManager::writePrintfToLog(LogManager::Level::Status, "SparseMatrix::printToASCIIFile",
                                "Writing matrix to file: %s", outputFileName);
    fprintf(outFileStream, "rows=%d\n", rows);
    fprintf(outFileStream, "cols=%d\n", cols);

    for (int i = 0; i < rows; i++) {
        Node* current = rowHeads[i];
        while (current != nullptr) {
            fprintf(outFileStream, "(%d, %d, %d)\n", i, current->col, current->value);
            current = current->next;
        }
    }

    fclose(outFileStream);
}

int SparseMatrix::getElement(int currRow, int currCol) {
    if (currRow < 0 || currRow >= rows || currCol < 0 || currCol >= cols) {
        return 0;
    }

    Node* current = rowHeads[currRow];
    while (current != nullptr) {
        if (current->col == currCol) {
            return current->value;
        }
        if (current->col > currCol) {
            return 0; 
        }
        current = current->next;
    }
    return 0;
}

int SparseMatrix::setElement(int currRow, int currCol, int value) {
    if (currRow < 0 || currRow >= rows || currCol < 0 || currCol >= cols) {
        return -1;
    }

    Node* current = rowHeads[currRow];
    Node* prev = nullptr;

    while (current != nullptr && current->col < currCol) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr && current->col == currCol) {
        if (value == 0) {
            if (prev == nullptr) {
                rowHeads[currRow] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return 1;
        } else {
            current->value = value;
            return 1;
        }
    }

    if (value != 0) {
        Node* newNode = new Node(currCol, value);
        if (prev == nullptr) {
            newNode->next = rowHeads[currRow];
            rowHeads[currRow] = newNode;
        } else {
            newNode->next = current;
            prev->next = newNode;
        }
        return 1;
    }

    return 1;
}

SparseMatrix SparseMatrix::operator+(SparseMatrix& inputObject) {
    if (inputObject.rows != rows || inputObject.cols != cols) {
        char* message = new char[2048];
        sprintf(message, "Number of rows and cols are not the same");
        throw std::invalid_argument(message);
        delete[] message;
    }

    SparseMatrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int value = getElement(i, j) + inputObject.getElement(i, j);
            if (value != 0) {
                result.setElement(i, j, value);
            }
        }
    }
    return result;
}

SparseMatrix SparseMatrix::operator-(SparseMatrix& inputObject) {
    if (inputObject.rows != rows || inputObject.cols != cols) {
        char* message = new char[2048];
        sprintf(message, "Number of rows and cols are not the same");
        throw std::invalid_argument(message);
        delete[] message;
    }

    SparseMatrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int value = getElement(i, j) - inputObject.getElement(i, j);
            if (value != 0) {
                result.setElement(i, j, value);
            }
        }
    }
    return result;
}

SparseMatrix SparseMatrix::operator*(SparseMatrix& inputObject) {
    if (cols != inputObject.rows) {
        char* message = new char[2048];
        sprintf(message, "Number of columns in first matrix must equal number of rows in second matrix");
        throw std::invalid_argument(message);
        delete[] message;
    }

    SparseMatrix result(rows, inputObject.cols);
    for (int i = 0; i < rows; i++) {
        Node* rowA = rowHeads[i];
        while (rowA != nullptr) {
            int k = rowA->col; 
            int valA = rowA->value;
            for (int j = 0; j < inputObject.cols; j++) {
                int valB = inputObject.getElement(k, j);
                if (valB != 0) {
                    int currVal = result.getElement(i, j);
                    result.setElement(i, j, currVal + valA * valB);
                }
            }
            rowA = rowA->next;
        }
    }
    return result;
}

void SparseMatrixTester::generateTestCases(char* outputFolderPath) {

}

void SparseMatrixTester::generateTestCases(char* outputFilePath, int min, int max, int numRows, int numCols) {
   
}