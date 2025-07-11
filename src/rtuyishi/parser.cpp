#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SENTENCE 1000
#define MAX_WORD 50

void analyzeSentence(char* sentence, int* wordCount, float* avgWordLength, char specialDelimiter) {
    *wordCount = 0;
    int totalLetters = 0;
    bool inWord = false;

    // Skip leading spaces
    int i = 0;
    while (isspace(sentence[i])) i++;

    for (; sentence[i] != '\0'; i++) {
        if (sentence[i] == specialDelimiter) {
            continue; // Skip special delimiter completely
        }
        
        if (isalnum(sentence[i])) {
            // It's a letter or number
            if (!inWord) {
                (*wordCount)++; // Start of a new word
                inWord = true;
            }
            totalLetters++; // Count only alphanumeric characters for letter count
        } else if (ispunct(sentence[i])) {
            // It's a non-special punctuation - keep it as part of the current word
            // Don't increment word count, don't add to letter count
            // Just maintain the current word state
        } else {
            // It's a space - marks the end of a word
            inWord = false;
        }
    }

    *avgWordLength = (*wordCount > 0) ? ((float)totalLetters / (float)*wordCount) : 0.0f;
    // Round to the nearest tenth
    *avgWordLength = roundf(*avgWordLength * 10.0f) / 10.0f;
}