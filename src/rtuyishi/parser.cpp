#include <stdio.h>    
#include <string.h>   
#include <ctype.h>    
#include <stdbool.h>  

#define MAX_SENTENCE 1000
#define MAX_WORD 50


void analyzeSentence(char* sentence, int* wordCount, float* avgWordLength, char endSentence) {
    *wordCount = 0;
    int totalLetters = 0;
    bool inWord = false;

    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isalnum(sentence[i])) {
            if (!inWord) {
                (*wordCount)++;
                inWord = true;
            }
            totalLetters++;
        } else {
            inWord = false; // End of a word
        }
    }

    *avgWordLength = (*wordCount > 0) ? ((float)totalLetters / *wordCount) : 0.0;
}



