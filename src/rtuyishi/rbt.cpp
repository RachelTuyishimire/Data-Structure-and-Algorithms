#include "RedBlackTree.h"

static double rbtInsertTime = 0.0;
static double rbtSearchTime = 0.0;

static RBTNode* createRBTNode(int sentenceLength, float customMetric, const char* sentence) {
    RBTNode* newNode = (RBTNode*)malloc(sizeof(RBTNode));
    newNode->sentenceLength = sentenceLength;
    newNode->customMetric = customMetric;
    strcpy(newNode->sentences[0], sentence);
    newNode->sentenceCount = 1;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

void* createRBT() {
    RBT* tree = (RBT*)malloc(sizeof(RBT));
    tree->root = NULL;
    return (void*)tree;
}

static void rotateLeft(RBT* tree, RBTNode* x) {
    RBTNode* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void rotateRight(RBT* tree, RBTNode* x) {
    RBTNode* y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

static void fixInsert(RBT* tree, RBTNode* z) {
    RBTNode* y;
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(tree, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void insertRBT(void* treePtr, int sentenceLength, float customMetric, const char* sentence) {
    RBT* tree = (RBT*)treePtr;
    clock_t start = clock();

    RBTNode* z = createRBTNode(sentenceLength, customMetric, sentence);
    RBTNode* y = NULL;
    RBTNode* x = tree->root;

    while (x != NULL) {
        y = x;
        if (z->sentenceLength < x->sentenceLength) {
            x = x->left;
        } else if (z->sentenceLength > x->sentenceLength) {
            x = x->right;
        } else {
            if (x->sentenceCount < MAX_SENTENCES) {
                strcpy(x->sentences[x->sentenceCount], sentence);
                x->sentenceCount++;
                free(z);
                clock_t end = clock();
                rbtInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
                return;
            } else {
                free(z);
                clock_t end = clock();
                rbtInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
                return;
            }
        }
    }

    z->parent = y;
    if (y == NULL) {
        tree->root = z;
    } else if (z->sentenceLength < y->sentenceLength) {
        y->left = z;
    } else {
        y->right = z;
    }

    fixInsert(tree, z);

    clock_t end = clock();
    rbtInsertTime += (double)(end - start) / CLOCKS_PER_SEC;
}

void searchRBT(void* treePtr, int sentenceLength, FILE* outputFile) {
    RBT* tree = (RBT*)treePtr;
    clock_t start = clock();

    RBTNode* current = tree->root;
    while (current != NULL) {
        if (sentenceLength == current->sentenceLength) {
            fprintf(outputFile, "Length %d:\n", current->sentenceLength);
            for (int i = 0; i < current->sentenceCount; i++) {
                fprintf(outputFile, "%s [%.1f]\n", current->sentences[i], current->customMetric);
            }
            clock_t end = clock();
            rbtSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
            return;
        } else if (sentenceLength < current->sentenceLength) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    fprintf(outputFile, "No sentences found with %d words\n", sentenceLength);
    clock_t end = clock();
    rbtSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
}

static void checkNode(RBTNode* node, int minLength, int maxLength, FILE* outputFile) {
    if (node == NULL) return;
    checkNode(node->left, minLength, maxLength, outputFile);
    if (node->sentenceLength >= minLength && node->sentenceLength <= maxLength) {
        for (int i = 0; i < node->sentenceCount; i++) {
            fprintf(outputFile, "%s [%.1f]\n", node->sentences[i], node->customMetric);
        }
    }
    checkNode(node->right, minLength, maxLength, outputFile);
}

void searchRangeRBT(void* treePtr, int minLength, int maxLength, FILE* outputFile) {
    RBT* tree = (RBT*)treePtr;
    clock_t start = clock();

    fprintf(outputFile, "Range %d-%d:\n", minLength, maxLength);
    checkNode(tree->root, minLength, maxLength, outputFile);

    clock_t end = clock();
    rbtSearchTime += (double)(end - start) / CLOCKS_PER_SEC;
}

static void deleteRBTNode(RBTNode* node) {
    if (node == NULL) return;
    deleteRBTNode(node->left);
    deleteRBTNode(node->right);
    free(node);
}

void deleteRBT(void* treePtr) {
    RBT* tree = (RBT*)treePtr;
    deleteRBTNode(tree->root);
    free(tree);
}

double getRBTInsertTime() {
    return rbtInsertTime;
}

double getRBTSearchTime() {
    return rbtSearchTime;
}

