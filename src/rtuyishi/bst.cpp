#include "RedBlackTree.h"

static double bstInsertTime = 0.0;
static double bstSearchTime = 0.0;

static BSTNode* createBSTNode(int sentenceLength, float customMetric, const char* sentence) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    newNode->sentenceLength = sentenceLength;
    newNode->customMetric = customMetric;
    strcpy(newNode->sentences[0], sentence);
    newNode->sentenceCount = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void* createBST() {
    BST* tree = (BST*)malloc(sizeof(BST));
    tree->root = NULL;
    return (void*)tree;
}

void insertBST(void* treePtr, int sentenceLength, float customMetric, const char* sentence) {
    BST* tree = (BST*)treePtr;
    clock_t start = clock();

    BSTNode* newNode = createBSTNode(sentenceLength, customMetric, sentence);
    BSTNode* current = tree->root;
    BSTNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (sentenceLength < current->sentenceLength) {
            current = current->left;
        } else if (sentenceLength > current->sentenceLength) {
            current = current->right;
        } else {
            if (current->sentenceCount < MAX_SENTENCES) {
                strcpy(current->sentences[current->sentenceCount], sentence);
                current->sentenceCount++;
                free(newNode);
                clock_t end = clock();
                bstInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
                return;
            }
            free(newNode);
            clock_t end = clock();
            bstInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
            return;
        }
    }

    if (parent == NULL) {
        tree->root = newNode;
    } else if (sentenceLength < parent->sentenceLength) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    clock_t end = clock();
    bstInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
}

void searchBST(void* treePtr, int sentenceLength) {  // Changed to void* to match usage
    BST* tree = (BST*)treePtr;
    clock_t start = clock();
    BSTNode* current = tree->root;
    while (current != NULL) {
        if (sentenceLength == current->sentenceLength) break;
        else if (sentenceLength < current->sentenceLength) current = current->left;
        else current = current->right;
    }
    clock_t end = clock();
    bstSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
}

static void deleteBSTNode(BSTNode* node) {
    if (node == NULL) return;
    deleteBSTNode(node->left);
    deleteBSTNode(node->right);
    free(node);
}

void deleteBST(void* treePtr) {
    BST* tree = (BST*)treePtr;
    deleteBSTNode(tree->root);
    free(tree);
}

double getBSTInsertTime() {
    return bstInsertTime;
}

double getBSTSearchTime() {
    return bstSearchTime;  // Simply return accumulated time
}

