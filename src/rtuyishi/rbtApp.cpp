#include "RedBlackTree.h"

#define SPECIAL_DELIMITER '?' // Special delimiter for quick debugging (change here to test '?', '!', etc.)

/*
 * Author: rtuyishi
 * Date of Submission: March 19, 2025
 * Functionality: Reads sentences from test_input.txt, breaks at any delimiter,
 *                builds BST and RBT, performs searches, and writes results to output.txt
 *                and performance metrics to performance.txt.
 * Input Format: test_input.txt with sentences ending in delimiters (e.g., '.', '!', '?').
 * Output Format: output.txt with Andrew ID, search results, separator;
 *                performance.txt with BST/RBT insert/search times.
 * Solution Strategy:
 *  - Parse sentences at any delimiter, compute word count and avg word length.
 *  - Include all delimiters except SPECIAL_DELIMITER in metrics.
 *  - Insert into BST and RBT, measure insertion times.
 *  - Search RBT for length 5 and range 2-4, output to output.txt.
 *  - Measure and compare BST/RBT performance, output to performance.txt.
 */

int main() {
    // FILE* inputFile = fopen("data/test_input.txt", "r");
    FILE* dataDir = fopen("assignment4/data/test_input.txt", "r");
    if (!inputFile) {
        printf("Error: Cannot open test_input.txt\n");
        return 1;
    }

    FILE* outputFile = fopen("data/output.txt", "w");
    if (!outputFile) {
        printf("Error: Cannot open data/output.txt\n");
        fclose(inputFile);
        return 1;
    }

    FILE* perfFile = fopen("data/performance.txt", "w");
    if (!perfFile) {
        printf("Error: Cannot open data/performance.txt\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    fprintf(outputFile, "rtuyishi\n");

    void* rbt = createRBT();
    void* bst = createBST();

    char sentence[MAX_SENTENCE_LENGTH];
    char line[MAX_SENTENCE_LENGTH];
    int sentencePos = 0;

    while (fgets(line, MAX_SENTENCE_LENGTH, inputFile)) {
        for (int i = 0; line[i] != '\0'; i++) {
            sentence[sentencePos] = line[i];
            sentencePos++;
            if (ispunct(line[i])) { // Break at any punctuation (delimiter)
                if (sentencePos > 1) { // Ensure there's content before the delimiter
                    sentence[sentencePos] = '\0'; // Include the delimiter in the sentence
                    int wordCount;
                    float avgWordLength;
                    analyzeSentence(sentence, &wordCount, &avgWordLength, SPECIAL_DELIMITER);
                    insertRBT(rbt, wordCount, avgWordLength, sentence);
                    insertBST(bst, wordCount, avgWordLength, sentence);
                }
                sentencePos = 0;
            } else if (sentencePos >= MAX_SENTENCE_LENGTH - 1) {
                sentence[sentencePos] = '\0';
                int wordCount;
                float avgWordLength;
                analyzeSentence(sentence, &wordCount, &avgWordLength, SPECIAL_DELIMITER);
                insertRBT(rbt, wordCount, avgWordLength, sentence);
                insertBST(bst, wordCount, avgWordLength, sentence);
                sentencePos = 0;
            }
        }
    }
    
    if (sentencePos > 0) { // Handle remaining text without a delimiter
        sentence[sentencePos] = '\0';
        int wordCount;
        float avgWordLength;
        analyzeSentence(sentence, &wordCount, &avgWordLength, SPECIAL_DELIMITER);
        insertRBT(rbt, wordCount, avgWordLength, sentence);
        insertBST(bst, wordCount, avgWordLength, sentence);
    }

    searchRBT(rbt, 5, outputFile);         // Search RBT for length 5 (removed BST search)
    searchRangeRBT(rbt, 2, 4, outputFile); // Search range 2-4
    fprintf(outputFile, "--------------------\n");

    fprintf(perfFile, "BST Insert: %.6f seconds\n", getBSTInsertTime());
    fprintf(perfFile, "RBT Insert: %.6f seconds\n", getRBTInsertTime());
    fprintf(perfFile, "BST Search: %.6f seconds\n", getBSTSearchTime());
    fprintf(perfFile, "RBT Search: %.6f seconds\n", getRBTSearchTime());

    deleteBST(bst);
    deleteRBT(rbt);
    fclose(inputFile);
    fclose(outputFile);
    fclose(perfFile);

    return 0;
}
