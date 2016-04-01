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
#include <map>
#include <algorithm>

using std::ifstream;
using namespace std;

/**
 * A node class for use in a binary search tree.
 */
class Node {
public:
    int value;
    Node *leftNode;
    Node *rightNode;
    
    //Default constructor
    Node(){
        leftNode = nullptr;
        rightNode = nullptr;
    }
    
    //Constructor with value passed
    Node(int val){
        leftNode = nullptr;
        rightNode = nullptr;
        value = val;
    }
    
    //Method to remove a node from its parent and replace it with its successor
    Node* deleteNode(int value, Node *parent) {
        if (value < this->value) {
            if (leftNode != nullptr){
                return leftNode->deleteNode(value, this);
            }else{
                return nullptr;
            }
        } else if (value > this->value) {
            if (rightNode != nullptr){
                return rightNode->deleteNode(value, this);
            }else{
                return nullptr;
            }
        } else {
            if (leftNode != nullptr && rightNode != nullptr) {
                this->value = rightNode->minValue();
                return rightNode->deleteNode(this->value, this);
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
    
    Node* FindMin(Node* root){
        while(root->leftNode != NULL) root = root->leftNode;
        return root;
    }
    
    /**
     * Gets the minimum value of a node by left tree recursion
     */
    int minValue() {
        if (leftNode == NULL){ //Base step
            return value;
        }else{
            return leftNode->minValue(); //Recursive step
        }
    }
    
    Node& getNodeWithValue(Node* root, int value){
        if(root->value == value){
            return *root;
        }else if(value > root->value){
            return getNodeWithValue(root->rightNode, value);
        }else{
            return getNodeWithValue(root->leftNode, value);
        }
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
int getLevelForValue(Node *root, int value, int level = 0);
vector<int> treeMap[100];
vector<int> treeValues;
int treeMapSize = 0;
string visualString;
int GetTreeHeight(Node *root);
void PrintTree();
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
    
    PrintTree();
    cout << "Number of nodes in the bst: " << getNumberOfNodes(rootNode) << endl;
    cout << "Height of the bst: " << treeMapSize << endl;
    cout << "Pre-order traversal: " << GetPreorderTraversalString(rootNode) << endl;
    cout << "In-order traversal: " << GetInorderTraversalString(rootNode) << endl;
    cout << "Post-order traversal: " << GetPostorderTraversalString(rootNode) << endl;
    cout << "Extra credit tree visualization: " << endl << visualString << endl;
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
        treeValues.push_back(value);
        return;
    }else if (value > root->value) {    //Recursively branch into right subtree
        insert(root->rightNode, value);
    }else { //Recursively branch into left subtree
        insert(root->leftNode, value);
        
    }
}

void PrintTree(){
    string treeString = "";
    for(int i = 0; i < treeValues.size(); i++){
        vector<int> levelVector = treeMap[getLevelForValue(rootNode, treeValues.at(i))];
        levelVector.push_back(treeValues.at(i));
        treeMap[getLevelForValue(rootNode, treeValues.at(i))] = levelVector;
    }
    int maxCount = 0;
    for (int i = 0; i < treeMapSize + 1; i++)
    {
        vector<int> currentVector = treeMap[i];
        maxCount = currentVector.size() > maxCount ? (int)currentVector.size() : maxCount;
    }
    for (int i = 0; i < treeMapSize + 1; i++)
    {
        vector<int> currentVector = treeMap[i];
        sort (currentVector.begin(), currentVector.begin() + currentVector.size());
        for(int i = (int)currentVector.size(); i <= maxCount; i++){
            treeString += "\t";
        }
        for(int i = 0; i < currentVector.size(); i++){
            treeString += to_string((int)currentVector.at(i)) + "\t";
        }
        treeString += "\n";
    }
    visualString = treeString;
}

int getLevelForValue(Node *root, int value, int level){
    if(root != nullptr){
        if(root->value == value){
            if(level > treeMapSize){
                treeMapSize = level;
            }
            return level;
        }else if(value > root->value){
            return getLevelForValue(root->rightNode, value, level + 1);
        }else{
            return getLevelForValue(root->leftNode, value, level + 1);
        }
    }else{
        return 0;
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
        if(root->leftNode != nullptr){
            numberOfNodes += getNumberOfNodes(root->leftNode);  //Recursively add left
        }
        if(root->rightNode != nullptr){
            numberOfNodes += getNumberOfNodes(root->rightNode); //Recursively add right
        }
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
    if(root == nullptr){
        return -1;
    }
    
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

/**
 * Removes a node from the binary search tree
 */
bool removeNodeWithValue(int value) {

    for(int i = 0; i < treeValues.size(); i++){
        if(treeValues.at(i) == value){
            treeValues.erase(treeValues.begin() + i);
        }
    }
    if (rootNode == NULL){
        return false;
    }else {
        if (rootNode->value == value) {
            Node temporaryNode = Node(0);
            temporaryNode.leftNode = rootNode;
            Node *removedNode = rootNode->deleteNode(value, &temporaryNode);
            rootNode = temporaryNode.leftNode;
            if (removedNode != nullptr) {
                delete removedNode;
                return true;
            }else {
                return false;
            }
        } else {
            Node *removedNode = rootNode->deleteNode(value, nullptr);
            if (removedNode != nullptr) {
                delete removedNode;
                return true;
            } else {
                return false;
            }
        }
    }
}

