// trial1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <unordered_map>

#define MAX_EXAMPLES 100
#define MAX_ATTRIBUTES 10
#define MAX_LABEL_LEN 10

using namespace std;
string fname = "agaricus-lepiota.data";

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
// Function to check if all tuples in D are of the same class
int are_all_same_class(DataPoint* D, int num_tuples) {
    // Implement this function based on your data structure and class representation
    // Return 1 if all tuples are of the same class, 0 otherwise
    if (num_tuples <= 1) {
        return 1;//1 or 0 tuples then same class
    }
    // Compare the class label of the first tuple with the class labels of the rest
    for (int i = 1; i < num_tuples; i++) {
        if (strcmp(D[0].label, D[i].label) != 0) {
            // If any class label is different, return 0
            return 0;
        }
    }
    //if all class labels same return 1
    return 1;
}

// Function to find the majority class in D
char find_majority_class(DataPoint* D, int num_tuples) {
    if (num_tuples <= 0) {
        // Handle the case when there are no tuples
        // You may return a default or error value based on your requirements
        return '\0'; // Return null character for simplicity
    }

    // Assuming the class labels are strings, you can use an array to store counts
    int label_counts[MAX_LABEL_LEN] = { 0 };

    // Count the occurrences of each class label
    for (int i = 0; i < num_tuples; i++) {
        label_counts[D[i].label[0]]++; // Assuming class labels are single characters
    }

    // Find the class label with the maximum count
    char majority_class = D[0].label[0]; // Default to the first label
    int max_count = label_counts[majority_class];

    for (int i = 1; i < MAX_LABEL_LEN; i++) {
        if (label_counts[i] > max_count) {
            majority_class = (char)i;
            max_count = label_counts[i];
        }
    }

    return majority_class;
}

// Function to implement the decision tree construction algorithm
TreeNode* generate_decision_tree(DataPoint* D, int num_tuples, int num_attributes) {
    TreeNode* node = create_node();

    // Step (2): Check if tuples in D are all of the same class
    if (are_all_same_class(D, num_tuples)) {
        // Step (3): Return N as a leaf node labeled with the class C
        node->predicted_label[0] = D[0].label[0];
        node->predicted_label[1] = '\0'; // Null-terminate the string
        node->is_leaf = 1;
        return node;
    }

    // Step (4): If attribute list is empty
    if (num_attributes == 0) {
        // Step (5): Return N as a leaf node labeled with the majority class in D
        node->predicted_label[0] = find_majority_class(D, num_tuples);
        node->predicted_label[1] = '\0'; // Null-terminate the string
        node->is_leaf = 1;
        return node;
    }
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

vector<tuple<string, vector<string>>> getDTuple(const string& filename, int N) {
    vector<tuple<string, vector<string>>> result;

    // Open the file
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return result; // Return an empty vector on error
    }

    // Read each line from the file
    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string nthString;
        vector<string> restOfStrings;

        // Tokenize the line by spaces
        int i = 0;
        string token;
        while (getline(iss, token, ',')) {
            if (i == N) {
                nthString = token;
            }
            else {
                restOfStrings.push_back(token);
            }
            i++;
        }

        // Create a tuple and add it to the result vector
        result.push_back(make_tuple(nthString, restOfStrings));
    }

    // Close the file
    inputFile.close();

    return result;
}

template <typename S>

// with_separator() function accepts
// two  arguments i.e., a vector and
// a separator string
void with_separator(const vector<S>& vec,
    string sep = " ")
{
    // Iterating over all elements of vector
    for (auto elem : vec) {
        cout << elem << sep;
    }

    cout << endl;
}


double info(const vector<string>& data) {
    unordered_map<string, int> frequency;
    int dataSize = data.size();

    // Count occurrences of each unique string
    for (const string& value : data) {
        frequency[value]++;
    }

    double iv = 0.0;

    // Calculate probability and entropy for each unique string
    for (const auto& pair : frequency) {
        double probability = static_cast<double>(pair.second) / dataSize;
        iv -= probability * log2(probability);
        //cout <<"prob: " << probability << endl;
    }
    //cout <<"info: " <<  iv << endl;
    return iv;
}





vector<vector<string>> tupp(const string& filename) {
    vector< vector<string>> result;

    // Open the file
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return result; // Return an empty vector on error
    }

    // Read each line from the file
    string line;
    while (getline(inputFile, line)) {
        
        string nthString;
        istringstream iss(line);
        vector<string> restOfStrings;

        // Tokenize the line by spaces
        int i = 0;
        string token;
        while (getline(iss, token, ',')) {

            restOfStrings.push_back(token);

        }

        // Create a tuple and add it to the result vector
        result.push_back(restOfStrings);
    }

    // Close the file
    inputFile.close();

    return result;
}

vector<string> col(const string& filename, int co) {
    vector<vector<string>> st = tupp(filename);
    int size = st.size();
    vector<string> result;
    for (int i = 0; i < size; i++) {
        result.push_back(st[i][co]);
    }
    return result;

}


vector<string> col2(const string& filename, int co, string pi, int p) {
    vector<vector<string>> st = tupp(filename);
    int size = st.size();
    vector<string> result;
    for (int i = 0; i < size; i++) {
        if (st[i][co] == pi) {
            result.push_back(st[i][p]);
        }
    }
    return result;

}

double info2(const vector<string>& data, int co, int p, vector<string>* vec) {
    unordered_map<string, int> frequency;
    double result = 0;
    int size = data.size();
    for (const string& value : data) {
            frequency[value]++;
        }
    
    for (const auto& pair : frequency) {
        double prob = static_cast<double>(pair.second) / size;
        vector<string> c2 = col2(fname, co, pair.first, p);
        double i1 = info(c2);
        result += prob * i1;
        //cout << "result: " <<result << endl;
        //cout << prob << endl << "i1: " << i1 << endl;
    }
    return result;
}

double gain(int A, int pi, vector<string>* vec) {
    double i1 = info2(col(fname, A), A,pi, vec);
    double i2 = info(col(fname, pi));
    //cout << i1 << " " << i2;
    double g = i2 - i1;
    return g;
}


int main()
{
    DataPoint D[MAX_EXAMPLES];  // Sample data
    int num_tuples = 100;  // Number of tuples in D
    int num_attributes = 10;  // Number of attributes
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

    vector<string> vec[23]; //change depending on the value of attlen (so the number of different attributes)
    ifstream myfile(fname);

    vector<string> v1;

    string data;
    getline(myfile, data);
    char f='\0';
    int d = strlen(data.c_str());
    int attlen = d / 2 + 1;
    int ff = 0;
    myfile.seekg(0);

    while (getline(myfile, data)) {
        for (int i = 0; i < d; i++) {
            string temp = "";
            string w;

            w = data[i];
            while (strcmp(w.c_str(), ",") && strcmp(w.c_str(), "\0")) {
                temp.append(w);
                i++;
                w = data[i];
            }
            if (find(vec[ff].begin(), vec[ff].end(), temp) == vec[ff].end())
            {
                vec[ff].push_back(temp);
            }

            temp = "";
            ff++;
        }
        ff = 0;
    }

    with_separator(vec[1], ", ");
 
    getline(myfile, data);
    cout << data << endl;

    myfile.close();

  
    double g = gain(0, 4, vec);
    cout << "gain: " << g << endl;



}
