//
//  proj2.cpp
//  proj2
//
//  Created by Justin Lennox on 3/24/16.
//  Copyright © 2016 Justin Lennox. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using std::ifstream;
using namespace std;

class Node {
public:
    int value;
    Node *leftNode;
    Node *rightNode;
    
    Node(){
        leftNode = nullptr;
        rightNode = nullptr;
    }
};

Node *rootNode = nullptr;

void parseInputFile(ifstream& inputFile);
void insert(Node *&root, int value);
int getNumberOfNodes( Node *root );
string GetPreorderTraversalString(Node *root);
string preorderTraversal = "";
bool treeContainsValue(Node *root, int value);
int GetTreeHeight(Node *root);

int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc < 2) {
        std::cerr << "Error: Please pass a text input file to parse." << std::endl;
        return EXIT_FAILURE;
    }else if(argc > 1){
        ifstream inputFile;
        inputFile.open(argv[1]); // open a file
        
        if(inputFile.is_open()){
            parseInputFile(inputFile);
        }else{
            std::cerr << "Error: Couldn't open the input file." << std::endl;
            return EXIT_FAILURE;
        }
    }
    
    ofstream outputFile("output.txt");
    if(outputFile.good()){
        outputFile << "Number of nodes in the bst: " << getNumberOfNodes(rootNode) << endl;
        outputFile << "Height of the bst: " << GetTreeHeight(rootNode) << endl;
        outputFile << "Pre-order traversal: ";
        outputFile << GetPreorderTraversalString(rootNode) << endl;
        
        cout << endl;
    }

    cout << "Number of nodes in the bst: " << getNumberOfNodes(rootNode) << endl;
    cout << "Height of the bst: " << GetTreeHeight(rootNode) << endl;
    cout << "Pre-order traversal: ";
    cout << GetPreorderTraversalString(rootNode);
    cout << endl;
    
    return EXIT_SUCCESS;
}

/**
 * Parses the input text file to read binary search tree commands and their corresponding values
 */
void parseInputFile(ifstream& inputFile){
    string line;
    while(getline(inputFile, line)) //Read the input line by line
    {
        line = line.erase(line.find_last_not_of("\n\r") + 1);
        stringstream delimiterStream(line); //Separating the command from the integer
        string command;
        int value;
        bool inserting = false;
        string sectionHolder;
        for(int i = 0; i < 2; i++){
            getline(delimiterStream, sectionHolder, ' ');
            if(i == 0){ //We're reading a command
                command = sectionHolder;
                if(command.compare("insert") == 0){
                    inserting = true;
                }else{
                    inserting = false;
                }
            }else if(i == 1){   //We're reading the value
                if(sectionHolder.length() > 0){ //Make sure we're not reading an empty string
                    value = std::stoi(sectionHolder);
                    if(inserting && !treeContainsValue(rootNode, value)){
                        insert(rootNode, value);
                    }
                }
            }
        }
    }
    inputFile.close();
}

/**
 * Traverses the tree to check if the passed value already exists. Returns true if the
 * value is already contained in a node in the tree.
 */
bool treeContainsValue (Node *root, int value) {
    if (root == nullptr) {  //There's not even a root
        return false;
    }else if ( value == root->value ) { //The root is the value
        return true;
    }else if ( value > root->value) {   //Recursively check right subtree
        return treeContainsValue(root->rightNode, value);
    }else { //Recursively check left subtree
        return treeContainsValue(root->leftNode, value);
    }
}

/**
 * Inserts the integer passed into the binary search tree
 */
void insert(Node *&root, int value){
    if(root == nullptr){    //There's no root node, set this as the root
        root = new Node();
        root->value = value;
        return;
    }else if (value > root->value) {    //Recursively branch into right subtree
        insert(root->rightNode, value);
    }else { //Recursively branch into left subtree
        insert(root->leftNode, value);

    }
}

/**
 * Function that traverses the tree to sum the number of nodes in it.
 */
int getNumberOfNodes( Node *root ) {
    if (root == nullptr){   //There are no nodes in the tree
        return 0;
    }else {
        int numberOfNodes = 1;
        numberOfNodes += getNumberOfNodes(root->leftNode);  //Recursively add left
        numberOfNodes += getNumberOfNodes(root->rightNode); //Recursively add right
        return numberOfNodes;
    }
}

/**
 * Prints the tree in Preorder: Root, Left, Right
 */
string GetPreorderTraversalString(Node *root) {
    if (root != nullptr) {  // Standard null checking
        preorderTraversal += to_string(root->value) + " ";      // Print the root integer value
        GetPreorderTraversalString( root->leftNode );    // Print the left subtree
        GetPreorderTraversalString( root->rightNode );   // Print the right subtree
    }
    return preorderTraversal;
}

/**
 * Gets the height of the bst in integer form
 */
int GetTreeHeight(Node *root) {
    int leftHeight = 0;
    if(root->leftNode != nullptr){
        leftHeight = GetTreeHeight(root->leftNode);
    }
    int rightHeight = 0;
    if(root->rightNode != nullptr){
        rightHeight = GetTreeHeight(root->rightNode);
    }
    return 1 + max(leftHeight, rightHeight);
}



