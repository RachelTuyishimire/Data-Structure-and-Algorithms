#include "TextAnalysisBST.h" 

// Adding a word to the tree and tracking the probes statistics.
Node* insert(Node* root, const char* word, int& maxProbes, int& totalProbes, int& wordCount) {
    int currentProbes = 1; 

    // Checking if the tree is empty.
    if (root == NULL) {  
        // Making a new spot for the word.
        Node* newNode = new Node;  
        strcpy(newNode->word, word);  
        newNode->frequency = 1;  
        newNode->level = 0; 
        newNode->left = newNode->right = NULL;

        // Adding probes to the total.
        totalProbes += currentProbes;  
        wordCount++; 
        // Updating the maximum probes.
        if (currentProbes > maxProbes) maxProbes = currentProbes;  
        return newNode;  
    }

    Node* current = root; 
    int depth = 0; 
    while (true) {  
        // Comparing the new word with the current word.
        int cmp = strcmp(word, current->word); 
        // Checking if the words are the same. 
        if (cmp == 0) {  
            current->frequency++; 
              // Adding the  probes to the total.
            totalProbes += currentProbes; 
            if (currentProbes > maxProbes) maxProbes = currentProbes;  
            return root;  
        }

        currentProbes++;  
        if (cmp < 0) { 
             // Checking if the left spot is empty.
            if (current->left == NULL) { 

                 // Making a new spot on the left.
                current->left = new Node; 
                 // Putting the word in the left spot.
                strcpy(current->left->word, word); 
                current->left->frequency = 1;  
                current->left->level = depth + 1; 
                current->left->left = current->left->right = NULL;  

                // Adding probes to the total.
                totalProbes += currentProbes;  
                wordCount++;  
                if (currentProbes > maxProbes) maxProbes = currentProbes;  
                return root; 
            }
             // Moving to the left spot.
            current = current->left; 
        } else {  
            // Checking if the right spot is empty.
            if (current->right == NULL) { 

                 // Making a new spot on the right.
                current->right = new Node; 
                strcpy(current->right->word, word);  
                current->right->frequency = 1;  
                current->right->level = depth + 1; 
                current->right->left = current->right->right = NULL;  

                // Adding probes to the total.
                totalProbes += currentProbes;  
                wordCount++;  
                if (currentProbes > maxProbes) maxProbes = currentProbes; 
                return root;  
            }
            // Moving to the right spot.
            current = current->right;  
        }
        depth++;
    }
}

// Listing words in order and saving them to a file.
void inOrderTraversal(Node* root, ofstream &outFile) {

    // Stopping if there’s no tree.
    if (root == NULL) return;  

     // Checking the left side first.
    inOrderTraversal(root->left, outFile); 

    // Writing the word, counting, and levelling to the file.
    outFile << root->word << " " << root->frequency << " (" << root->level << ")" << endl;  

    // Checking the right side next.
    inOrderTraversal(root->right, outFile);  
}

// Converting a word to a lowercase.
void toLowerCase(char* word) {
    for (int i = 0; word[i]; i++) {  
        word[i] = tolower(word[i]);  
    }
}

// Breaking a line into words and adding them to the tree.
void lineProcessing(char* line, Node*& root, int& maxProbes, int& totalProbes, int& wordCount) {
    // Spliting the line into words.
    char* token = strtok(line, " .,!?\";:()[]{}");  
    while (token != NULL) { 
        toLowerCase(token); 

        // Adding the word to the tree.
        root = insert(root, token, maxProbes, totalProbes, wordCount);  

        // Getting the next word.
        token = strtok(NULL, " .,!?\";:()[]{}");  
    }
}

// Reading a file and adding its words to the tree.
void fileProcessing(const char* filename, Node*& root, int& maxProbes, int& totalProbes, int& wordCount) {

    // Opening the file.
    ifstream file(filename);  
    if (!file) {  
        cout << "Error opening file: " << filename << endl; 
        return;  
    }

    // Making space to hold each line.
    char line[256];  
    while (file.getline(line, 256)) { 
        lineProcessing(line, root, maxProbes, totalProbes, wordCount); 
    }
    file.close(); 
}

// Deleting the tree and space freeing.
void deleteTree(Node* &root) {
    if (root == NULL) return;  
    deleteTree(root->left);  
    deleteTree(root->right);  
    delete root;  
    root = NULL;  
}