#ifndef TEXT_ANALYSIS_BST_H
#define TEXT_ANALYSIS_BST_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

struct Node {
    char word[50];  
    int frequency;  
    int level;     
    Node* left;
    Node* right;
};

// Function prototypes
Node* insert(Node* root, const char* word, int& maxProbes, int& totalProbes, int& wordCount);
void inOrderTraversal(Node* root, ofstream& outFile);
void toLowerCase(char* word);
void plineProcessing(char* line, Node*& root, int& maxProbes, int& totalProbes, int& wordCount);
void fileProcessing(const char* filename, Node*& root, int& maxProbes, int& totalProbes, int& wordCount);
void deleteTree(Node*& root);

#endif
