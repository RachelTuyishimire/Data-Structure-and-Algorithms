#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_SENTENCES 100
#define MAX_SENTENCE_LENGTH 1000
#define MAX_WORD 100

// BST structure
typedef struct BSTNode {
    int sentenceLength;
    float customMetric;
    char sentences[MAX_SENTENCES][MAX_SENTENCE_LENGTH];
    int sentenceCount;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
} BST;

// RBT structure
typedef enum { RED, BLACK } Color;

typedef struct RBTNode {
    int sentenceLength;
    float customMetric;
    char sentences[MAX_SENTENCES][MAX_SENTENCE_LENGTH];
    int sentenceCount;
    Color color;
    struct RBTNode* left;
    struct RBTNode* right;
    struct RBTNode* parent;
} RBTNode;

typedef struct {
    RBTNode* root;
} RBT;

// Parser function
void analyzeSentence(char* sentence, int* wordCount, float* avgWordLength, char endSentence);

// BST functions
void* createBST();
void insertBST(void* tree, int sentenceLength, float customMetric, const char* sentence);
double getBSTInsertTime();
double getBSTSearchTime();
void deleteBST(void* tree);

// RBT functions
void* createRBT();
void insertRBT(void* tree, int sentenceLength, float customMetric, const char* sentence);
void searchRBT(void* tree, int sentenceLength, FILE* outputFile);
void searchRangeRBT(void* tree, int minLength, int maxLength, FILE* outputFile);
double getRBTInsertTime();
double getRBTSearchTime();
void deleteRBT(void* tree);

#endif // DATA_STRUCTURES_H


