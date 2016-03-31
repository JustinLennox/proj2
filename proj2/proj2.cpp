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
    
    Node(int val){
        leftNode = nullptr;
        rightNode = nullptr;
        value = val;
    }
    
    Node* remove(int value, Node *parent) {
        if (value < this->value) {
            if (leftNode != nullptr){
                return leftNode->remove(value, this);
            }else{
                return nullptr;
            }
        } else if (value > this->value) {
            if (rightNode != nullptr){
                return rightNode->remove(value, this);
            }else{
                return nullptr;
            }
        } else {
            if (leftNode != nullptr && rightNode != nullptr) {
                this->value = rightNode->minValue();
                return rightNode->remove(this->value, this);
            } else if (parent->leftNode == this) {
                parent->leftNode = (leftNode != nullptr) ? leftNode : rightNode;
                return this;
            } else if (parent->rightNode == this) {
                parent->rightNode = (leftNode != nullptr) ? leftNode : rightNode;
                return this;
            }else{
                return nullptr;
            }
        }
    }
    
    int minValue() {
        if (leftNode == nullptr)
            return value;
        else
            return leftNode->minValue();
    }
};

Node *rootNode = nullptr;

void parseInputFile(ifstream& inputFile);
void insert(Node *&root, int value);
int getNumberOfNodes( Node *root );
string GetPreorderTraversalString(Node *root);
string GetInorderTraversalString(Node *root);
string GetPostorderTraversalString(Node *root);
bool treeContainsValue(Node *root, int value);
int GetTreeHeight(Node *root);
bool removeNodeWithValue(int value);

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
        outputFile << "Pre-order traversal: " << GetPreorderTraversalString(rootNode) << endl;
        outputFile << "In-order traversal: " << GetInorderTraversalString(rootNode) << endl;
        outputFile << "Post-order traversal: " << GetPostorderTraversalString(rootNode) << endl;
    }

    cout << "Number of nodes in the bst: " << getNumberOfNodes(rootNode) << endl;
    cout << "Height of the bst: " << GetTreeHeight(rootNode) << endl;
    cout << "Pre-order traversal: " << GetPreorderTraversalString(rootNode) << endl;
    cout << "In-order traversal: " << GetInorderTraversalString(rootNode) << endl;
    cout << "Post-order traversal: " << GetPostorderTraversalString(rootNode) << endl;
    
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
                    }else if(!inserting && treeContainsValue(rootNode, value)){
                        removeNodeWithValue(value);
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

//8 3 -2 -3 -5 0 6 4 7 20 13 9 15 17

/**
 * Prints the tree in Preorder: Root, Left, Right
 */
string GetPreorderTraversalString(Node *root) {
    if (root != nullptr) {  // Standard null checking
        string preorderTraversal = to_string(root->value) + " ";      // Print the root integer value
        preorderTraversal += GetPreorderTraversalString(root->leftNode);    // Print the left subtree
        preorderTraversal += GetPreorderTraversalString(root->rightNode);   // Print the right subtree
        return preorderTraversal;
    }else{
        return "";
    }
}

/**
 * Prints the tree in Inorder: Left, Root, Right
 */
string GetInorderTraversalString(Node *root) {
    if (root != nullptr) {  // Standard null checking
        string inorderTraversal = GetInorderTraversalString(root->leftNode) + " ";    // Print the left subtree
        inorderTraversal += to_string(root->value);      // Print the root integer value
        inorderTraversal += GetInorderTraversalString(root->rightNode);   // Print the right subtree
        return inorderTraversal;
    }else{
        return "";
    }
}

/**
 * Prints the tree in Postorder: Left, Right, Root
 */
string GetPostorderTraversalString(Node *root) {
    if (root != nullptr) {  // Standard null checking
        string postorderTraversal = GetPostorderTraversalString(root->leftNode);    // Print the left subtree
        postorderTraversal += GetPostorderTraversalString(root->rightNode);   // Print the right subtree
        postorderTraversal += to_string(root->value) + " ";      // Print the root integer value
        return postorderTraversal;
    }else{
        return "";
    }
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

bool removeNodeWithValue(int value) {
    if (rootNode == NULL)
        return false;
    else {
        if (rootNode->value == value) {
            Node auxRoot = Node(0);
            
            auxRoot.leftNode = rootNode;
            Node *removedNode = rootNode->remove(value, &auxRoot);
            rootNode = auxRoot.leftNode;
            if (removedNode != nullptr) {
                delete removedNode;
                return true;
            } else
                return false;
        } else {
            Node *removedNode = rootNode->remove(value, NULL);
            if (removedNode != NULL) {
                delete removedNode;
                return true;
            } else
                return false;
        }
    }
}


