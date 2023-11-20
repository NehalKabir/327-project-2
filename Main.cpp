// trial1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

#define MAX_EXAMPLES 100
#define MAX_ATTRIBUTES 10
#define MAX_LABEL_LEN 10

typedef struct {
    char attributes[MAX_ATTRIBUTES][MAX_LABEL_LEN];
    char label[MAX_LABEL_LEN];
} DataPoint;

typedef struct TreeNode {
    int attribute_index;
    char attribute_value[MAX_LABEL_LEN];
    int is_leaf;
    char predicted_label[MAX_LABEL_LEN];
    struct TreeNode* children[MAX_EXAMPLES]; // For simplicity, assuming each attribute value creates a child node
} TreeNode;

TreeNode* create_node() {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->is_leaf = 0;
    node->attribute_index = -1;
    memset(node->predicted_label, 0, sizeof(node->predicted_label));
    memset(node->attribute_value, 0, sizeof(node->attribute_value));
    for (int i = 0; i < MAX_EXAMPLES; i++) {
        node->children[i] = NULL;
    }
    return node;
}


void print_node(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("| ");
    }

    if (node->is_leaf) {
        printf("Predicted Label: %s\n", node->predicted_label);
    }
    else {
        printf("Attribute %d = %s\n", node->attribute_index, node->attribute_value);
        for (int i = 0; i < 2; i++) {
            print_node(node->children[i], level + 1);
        }
    }
}
void print_tree(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) {
        printf("| ");
    }

    if (node->is_leaf) {
        printf("Predicted Label: %s\n", node->predicted_label);
    }
    else {
        printf("Attribute %d = %s\n", node->attribute_index, node->attribute_value);
        for (int i = 0; i < 2; i++) {
            print_tree(node->children[i], level + 1);
        }
    }
}

std::vector<std::tuple<std::string, std::vector<std::string>>> getDTuple(const std::string& filename, int N) {
    std::vector<std::tuple<std::string, std::vector<std::string>>> result;

    // Open the file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return result; // Return an empty vector on error
    }

    // Read each line from the file
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string nthString;
        std::vector<std::string> restOfStrings;

        // Tokenize the line by spaces
        int i = 0;
        std::string token;
        while (std::getline(iss, token, ',')) {
            if (i == N) {
                nthString = token;
            }
            else {
                restOfStrings.push_back(token);
            }
            i++;
        }

        // Create a tuple and add it to the result vector
        result.push_back(std::make_tuple(nthString, restOfStrings));
    }

    // Close the file
    inputFile.close();

    return result;
}


int main()
{
    TreeNode* root = create_node();  // Create the root node
    root->attribute_index = 0;       // Set the attribute index (for decision)
    strcpy(root->attribute_value, "Attribute_0_Value");  // Set the attribute value

    // Create left child node and connect it to the root
    TreeNode* left_child = create_node();
    left_child->is_leaf = 1;  // For leaf node (final decision)
    strcpy(left_child->predicted_label, "Label_A");  // Set the predicted label for this leaf node

    // Connect the left child to the root node as the first child
    root->children[0] = left_child;

    // Create right child node and connect it to the root
    TreeNode* right_child = create_node();
    right_child->attribute_index = 1;  // Another attribute index for decision
    strcpy(right_child->attribute_value, "Attribute_1_Value");  // Set attribute value for this node

    // Connect the right child to the root node as the second child
    root->children[1] = right_child;
    print_tree(root, 0);
    printf(root->children[1]->attribute_value);
    std::cout << "Hello World!\n";


    // Example usage:
    std::vector<std::tuple<std::string, std::vector<std::string>>> result =
        getDTuple("car.data", 6);

    // Display the result
    for (const auto& tuple : result) {
        std::cout << "Nth String: " << std::get<0>(tuple) << ", Rest of Strings: ";
        for (const auto& str : std::get<1>(tuple)) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }
}
