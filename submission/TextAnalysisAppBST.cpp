#include "TextAnalysisBST.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;    

/*
 * Author: rtuyishi
 * Date of Submission: March 11, 2025
 *
 * Functionality: This program reads text files from "input.txt," builds a Binary Search Tree (BST) 
                for each, and tracks unique words, their frequency, and position in the tree. 
                It also calculates the maximum and average probes needed for insertion. Finally, 
                The results are saved in "output.txt," and the program treats words case-insensitively.
 *
 * Input Format:
 * - A file named "input.txt" containing one file path per line (e.g., "data/textfile1.txt").
 *
 * Output Format:
 * - The output file "output.txt" includes:
 *   1. Andrew ID ("rtuyishi") at the top.
 *   2. For each processed file:
 *      - Filename
 *      - "Maximum number of probes: [value]"
 *      - "Average number of probes: [value]"
 *      - A list of words formatted as "word frequency (level)"
 *      - A separator line "--------------------"
 *
 * Solution Strategy:
 * - Read "input.txt" and process each file listed:
 *   1. Read the file line by line, tokenize words, and insert them into a BST.
 *   2. Track probe counts during insertion:
 *      - Maximum probes: Highest number of steps taken to insert any word.
 *      - Average probes: Total probes divided by the number of words inserted.
 *   3. Assign and record the level of each word node.
 *   4. Perform an in-order traversal to write results to "output.txt".
 *
 * Pseudocode:
 * - Maximum Probes:
 *   - For each word insertion:
 *     - Initialize `probes = 1`
 *     - While traversing the BST to find the insertion point:
 *       - Increment `probes`
 *     - Update `maxProbes` if `probes` exceeds the current maximum.
 *
 * - Average Probes:
 *   - Sum all probe counts during insertion: `totalProbes += probes`
 *   - Compute the average at the end: `avgProbes = totalProbes / wordCount`
 *
 * - Level Assignment:
 *   - Start at the root (level 0).
 *   - Increment level for each left or right move during insertion.
 *   - Assign the final level to the new node upon insertion.
 *
 * Algorithm Complexity:
 * - BST insertion: O(h), where h is the tree height.
 * - In-order traversal: O(n), where n is the number of nodes.
 */


 int main() { 
    ifstream inputFile("data/input.txt");  
    if (!inputFile) {  
        cout << "Error: Cannot open input.txt" << endl;
        return 1; 
    }

    // Creating  the output file.
    ofstream outputFile("data/output.txt");  
    if (!outputFile) {  
        cout << "Error: Cannot create output.txt" << endl; 
        return 1;  
    }

    outputFile << "rtuyishi" << endl; 

    char filename[100];  

    // Reading each line from the input file.
    while (inputFile.getline(filename, 100)) { 
        
        // Starting with an empty tree.
        Node* root = NULL;  


         // Setting up variables to track the probes and word counting.
        int maxProbes = 0, totalProbes = 0, wordCount = 0; 

        // Reading the file and building the tree.
        fileProcessing(filename, root, maxProbes, totalProbes, wordCount);  

        // Writing the filename to the output file.
        outputFile << filename << endl;  

         // Writing the highest probe count.
        outputFile << "Maximum number of probes:" << maxProbes << endl; 

        // Calculating the average probe count.
        float avgProbes = (wordCount == 0) ? 0 : (float)totalProbes / wordCount;  
        outputFile << "Average number of probes: " << avgProbes << endl; 

        // Listing all words from the tree in order to the output file.
        inOrderTraversal(root, outputFile);  
        outputFile << "--------------------" << endl;  

        deleteTree(root);  
    }

    inputFile.close(); 
    outputFile.close(); 

    return 0;  
}