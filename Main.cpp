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
#include <algorithm>

#pragma warning(disable : 4996)

#define MAX_EXAMPLES 100
#define MAX_ATTRIBUTES 10
#define MAX_LABEL_LEN 100

using namespace std;


string fname = "Wholesale_customers_data.data";
int outIndex = 0; //starting at 0
vector<string> attrList = { "Region","Fresh","Milk","Grocery","Frozen","Detergents_Paper","Delicassen" }; //Wholesale_customers_data.data
//vector<string> attrList = { "age", "income", "student", "credit_rating"}; // textbook
/*vector<string> attrList = {"cap-shape", "cap-surface", "cap-color", "bruises?", "odor", "gill-attachment", "gill-spacing", "gill-size",
                  "gill-color", "stalk-shape", "stalk-root", "stalk-surface-above-ring", "stalk-surface-below-ring",
                  "stalk-color-above-ring", "stalk-color-below-ring","veil-type","veil-color","ring-number","ring-type",
                  "spore-print-color","population", "habitat" };*/ //mushroom
                  //vector<string> attrList = { "buying", "maint", "doors", "persons", "lug_boot", "safety" };
/*vector<string> attrList = {"Handicapped - infants", "water-cost-sharing", "adopt-budget-resolution", "phys-fee-freeze", "el-salvador-aid" ,"Religious - groups - in - school",
                            "anti - satellite - test - ban", "aid - to - nicaraguan - contras","mx - missile",
                            "Immigration","synfuel - corporation - cutback","education - spending","superfund - right - to - sue","crime","duty - free - export","export - administration - act - south - africa" };*/
vector<int> usedAtrributes = {};

typedef struct TreeNode {
    int attribute_index; //index of attribute its children split based off of
    char attribute_value[MAX_LABEL_LEN];
    int is_leaf;
    char predicted_label[MAX_LABEL_LEN];
    char parentResult[MAX_LABEL_LEN];
    struct TreeNode* children[MAX_EXAMPLES]; // For simplicity, assuming each attribute value creates a child node
} TreeNode;

void create_branches(TreeNode* node, const vector<tuple<string, vector<string>>>& D, vector<string> attrList, vector<string> vec[100]);
TreeNode* generate_decision_tree(vector<tuple<string, vector<string>>>& D, vector<string> attrList, vector<string> vec[100], TreeNode* node);

TreeNode* create_node() {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->is_leaf = 0;
    node->attribute_index = -1;
    memset(node->parentResult, 0, sizeof(node->parentResult));
    memset(node->predicted_label, 0, sizeof(node->predicted_label));
    memset(node->attribute_value, 0, sizeof(node->attribute_value));
    for (int i = 0; i < MAX_EXAMPLES; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Function to check if all tuples in D are of the same class
int are_all_same_class(std::vector<std::tuple<std::string, std::vector<std::string>>>& D) {
    // Implement this function based on your data structure and class representation
    // Return 1 if all tuples are of the same class, 0 otherwise
    if (D.size() <= 1) {
        return 1; // 1 or 0 tuples then same class
    }
    // Compare the class label of the first tuple with the class labels of the rest
    for (size_t i = 1; i < D.size(); i++) {
        if (get<0>(D[0]) != get<0>(D[i])) {
            // If any class label is different, return 0
            return 0;
        }
    }
    // if all class labels same return 1
    return 1;
}

// Function to find the majority class in D
string find_majority_class(std::vector<std::tuple<std::string, std::vector<std::string>>>& D) {
    // Use an unordered_map to count occurrences of each string
    std::unordered_map<std::string, int> stringCount;

    // Iterate through the tuple vector
    for (const auto& myTuple : D) {
        // Extract the string from the tuple
        std::string strValue = std::get<0>(myTuple);

        // Increment the count for the string in the map
        stringCount[strValue]++;
    }

    // Find the most frequent string
    std::string mostFrequentString;
    int maxCount = 0;

    for (const auto& entry : stringCount) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            mostFrequentString = entry.first;
        }
    }
    return mostFrequentString;
}

void print_tree(TreeNode* node, int level) {
    if (node == NULL || (node->is_leaf == 1 && strlen(node->predicted_label) == 0)) return;

    for (int i = 0; i < level; i++) {
        printf("| ");
    }
    if (strlen(node->parentResult) != 0)
        printf("%s: ", node->parentResult);
    if (node->is_leaf) {
        printf("Predicted Label: %s\n", node->predicted_label);
    }
    else {
        printf("Attribute %d = %s\n", node->attribute_index, node->attribute_value);
        int i = 0;
        while (node->children[i] != NULL) {
            print_tree(node->children[i], level + 1);
            i++;
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
vector<string> with_separator(const vector<S>& vec,
    string sep = " ")
{
    vector<string> lst;
    // Iterating over all elements of vector
    for (auto elem : vec) {
        if (elem != "?")
        {
            lst.push_back(elem);
            //cout << elem << sep;
        }
    }

    //cout << endl;
    return lst;
}


//counts the number of occurences of each string for the column inputed, then divide that by the total line in that column
//then do that for all the different strings, and add the value together, that is the info
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

//tokenize the input file into a vector of vector of strings to make it easier to loop around
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
//just returns a vector of strings of the desired column from the input file
vector<string> col(const string& filename, int co) {
    vector<vector<string>> st = tupp(filename);
    int size = st.size();
    vector<string> result;
    for (int i = 0; i < size; i++) {
        result.push_back(st[i][co]);
    }
    return result;

}

//returns a vector of strings of the desired column p, but this time only when the value in column co matches the string pi
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

//does the same thing as info, but this time we also need to multiply info(Dj) to the prob
//we get info(Dj) by making a vector of strings for column p that only saves the value when the value for column co is the same
//then we just take info of that
//we multiply prob and that info, then do the same for each different string in the class attribute
double info2(const vector<string>& data, int co, int p) {
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

//A is the attribute we want to find the gain of
//pi is the class attribute
double gain(int A, int pi, vector<string> c) {
    if (A == pi)
        return -2;
    double i1 = info2(c, A, pi);
    double i2 = info(c);
    //cout << i1 << " " << i2;
    double g = i2 - i1;
    return g;
}

bool isAllDigits(vector<string> str) {
    for (string ch : str) {
        for (char c : ch)
        {
            if (!std::isdigit(c)) {
                return false;
            }
        }
    }
    return true;
}

int getAttributeType(const vector<tuple<string, vector<string>>>& D, vector<string>* vec, int best_attribute)
{
    vector<string> tester = with_separator(vec[best_attribute]);
    if (isAllDigits(tester))
        return 1; // attribute is continous number
    if (tester.size() == 2)
        return 2; // binary discrete
    return 3; //discrete
}

void rem(string s, int col, vector<string>* vec) {
    int holder = -1;
    for (int i = 0; i < vec[col].size(); i++) {
        if (s.compare(vec[col][i]) == 0) {
            holder = i;
        }
    }

    if (holder == -1) {
        cout << "string not found";
    }
    else {
        vec[col].erase(vec[col].begin() + holder);
    }

}

void remlist(string s, vector<string>& vec) {
    int holder = -1;
    for (int i = 0; i < vec.size(); i++) {
        if (s.compare(vec[i]) == 0) {
            holder = i;
        }
    }
    if (holder == -1) {
        cout << "string not found";
    }
    else {
        vec.erase(vec.begin() + holder);
    }

}

// Function to find the best split point for a continuous attribute
double find_best_split_point(const vector<string>& data) {
    // Convert the string values to double for sorting
    vector<double> numeric_values;
    for (const string& value : data) {
        numeric_values.push_back(stod(value));
    }

    // Sort the numeric values
    sort(numeric_values.begin(), numeric_values.end());

    // Initialize variables to keep track of the best split point and information gain
    double best_split_point = -1.0;
    double max_information_gain = -1.0;

    // Iterate through the sorted values to find the best split point
    for (size_t i = 1; i < numeric_values.size(); i++) {
        double split_point = (numeric_values[i - 1] + numeric_values[i]) / 2.0;
        vector<string> less_than_split, greater_than_split;

        // Split the data based on the current split point
        for (const string& value : data) {
            double numeric_value = stod(value);
            if (numeric_value <= split_point) {
                less_than_split.push_back(value);
            }
            else {
                greater_than_split.push_back(value);
            }
        }

        // Calculate information gain for the current split point
        double information_gain = info(data) - (less_than_split.size() / static_cast<double>(data.size())) * info(less_than_split)
            - (greater_than_split.size() / static_cast<double>(data.size())) * info(greater_than_split);

        // Update the best split point if information gain is greater
        if (information_gain > max_information_gain) {
            max_information_gain = information_gain;
            best_split_point = split_point;
        }
    }

    return best_split_point;
}

// ...

// Helper function to create branches in the decision tree for continuous attributes
void create_continuous_branches(TreeNode* node, const vector<tuple<string, vector<string>>>& D, int num_attributes, vector<string> vec[100]) {
    // Check if the current node is a leaf node
    if (node->is_leaf) {
        return;
    }

    vector<string> splitVec;
    for (const auto& tuple : D) {
        splitVec.push_back(std::get<1>(tuple)[node->attribute_index]);
    }

    // Find the best split point for the continuous attribute
    double split_point = find_best_split_point(splitVec);
    string tmp;
    cout << "Split point: " << split_point << endl;
    // Create child nodes for the two branches (less than and greater than the split point)
    for (int i = 0; i < 2; i++) {
        TreeNode* child_node = create_node();
        if (i == 0)
        {
            tmp = "<" + to_string(split_point);
        }
        else
            tmp = ">" + to_string(split_point);
        strcpy(child_node->parentResult, tmp.c_str());
        strcpy(child_node->attribute_value, tmp.c_str());
        vector<tuple<string, vector<string>>> subset_D;

        // Populate the subset dataset for the current branch
        for (const auto& tuple : D) {
            double numeric_value = stod(get<1>(tuple)[node->attribute_index]);
            if ((i == 0 && numeric_value <= split_point) || (i == 1 && numeric_value > split_point)) {
                subset_D.push_back(tuple);
            }
        }

        // Check if the subset is empty or if all tuples in the subset have the same class
        if (subset_D.empty() || are_all_same_class(subset_D)) {
            // Make the child node a leaf node
            child_node->is_leaf = 1;
            if (!subset_D.empty()) {
                strcpy(child_node->predicted_label, get<0>(subset_D[0]).c_str());
            }
        }
        else {
            // Recursively create branches for the child node
            child_node = generate_decision_tree(subset_D, attrList, vec, child_node);
        }

        // Connect the child node to the current node using array indexing
        node->children[i] = child_node;
    }


    //// Create child nodes for each attribute value
    //for (size_t i = 0; i < attribute_values.size(); i++) {
    //    TreeNode* child_node = create_node();
    //    strcpy(child_node->parentResult, attribute_values[i].c_str());
    //    strcpy(child_node->attribute_value, attribute_values[i].c_str());
    //    vector<tuple<string, vector<string>>> subset_D;

    //    // Populate the subset dataset for the crrent attribute value
    //    for (const auto& tuple : D) {
    //        if (get<1>(tuple)[node->attribute_index] == attribute_values[i]) {
    //            subset_D.push_back(tuple);
    //        }
    //    }

    //    // Check if the subset is empty or if all tuples in the subset have the same class
    //    if (subset_D.empty() || are_all_same_class(subset_D)) {
    //        // Make the child node a leaf node
    //        child_node->is_leaf = 1;
    //        if (!subset_D.empty()) {
    //            strcpy(child_node->predicted_label, get<0>(subset_D[0]).c_str());
    //        }
    //    }
    //    else {
    //        // Recursively create branches for the child node
    //        create_continuous_branches(child_node, subset_D, num_attributes - 1, vec);
    //    }

    //    // Connect the child node to the current node using array indexing
    //    node->children[i] = child_node;
    //}

}

// Helper function to create branches in the decision tree
void create_branches(TreeNode* node, const vector<tuple<string, vector<string>>>& D, vector<string> attrList, vector<string> vec[100]) {
    // Check if the current node is a leaf node
    if (node->is_leaf) {
        return;
    }

    // Get the attribute values for the chosen attribute
    vector<string> attribute_values = with_separator(vec[node->attribute_index]);

    // Create child nodes for each attribute value
    for (size_t i = 0; i < attribute_values.size(); i++) {
        TreeNode* child_node = create_node();
        strcpy(child_node->parentResult, attribute_values[i].c_str());
        strcpy(child_node->attribute_value, attribute_values[i].c_str());
        vector<tuple<string, vector<string>>> subset_D;

        // Populate the subset dataset for the current attribute value
        for (const auto& tuple : D) {
            if (get<1>(tuple)[node->attribute_index] == attribute_values[i]) {
                subset_D.push_back(tuple);
            }
        }

        // Check if the subset is empty or if all tuples in the subset have the same class
        if (subset_D.empty() || are_all_same_class(subset_D)) {
            // Make the child node a leaf node
            child_node->is_leaf = 1;
            if (!subset_D.empty()) {
                strcpy(child_node->predicted_label, get<0>(subset_D[0]).c_str());
            }
        }
        else {
            // Recursively create branches for the child node
            child_node = generate_decision_tree(subset_D, attrList, vec, child_node);
        }

        // Connect the child node to the current node using array indexing
        node->children[i] = child_node;
    }
}

// Function to implement the decision tree construction algorithm
TreeNode* generate_decision_tree(vector<tuple<string, vector<string>>>& D, vector<string> attrList, vector<string> vec[100], TreeNode* node) {
    //TreeNode* node = create_node();
    vector<string> splitVec;

    // Step (2): Check if tuples in D are all of the same class
    if (are_all_same_class(D)) {
        // Step (3): Return N as a leaf node labeled with the class C
        node->predicted_label[0] = get<0>(D[0])[0];
        node->is_leaf = 1;
        return node;
    }

    // Step (4): If attribute list is empty
    if (attrList.size() == 0) {
        // Step (5): Return N as a leaf node labeled with the majority class in D
        strcpy(node->predicted_label, find_majority_class(D).c_str());
        node->is_leaf = 1;
        return node;
    }
    // Choose the best attribute to split on based on information gain
    double max_information_gain = -1.0;
    int best_attribute = -1;
    double information_gain;
    for (int attribute_index = 0; attribute_index < attrList.size(); attribute_index++) {
        if (find(usedAtrributes.begin(), usedAtrributes.end(), attribute_index) != usedAtrributes.end())
            information_gain = -2;
        else
        {
            splitVec.clear();
            for (const auto& tuple : D) {
                splitVec.push_back(std::get<1>(tuple)[attribute_index]);
            }
            information_gain = gain(attribute_index, outIndex, splitVec);
        }
        if (information_gain > max_information_gain) {
            max_information_gain = information_gain;
            best_attribute = attribute_index;
        }
    }

    cout << "best attribute: " << best_attribute << endl;
    if (best_attribute == -1) {
        // Step (5): Return N as a leaf node labeled with the majority class in D
        strcpy(node->predicted_label, find_majority_class(D).c_str());
        node->is_leaf = 1;
        return node;
    }
    // Set the chosen attribute index for the current node
    node->attribute_index = best_attribute;
    strcpy(node->attribute_value, attrList[best_attribute].c_str());
    //determine attribute type
    int attrType = getAttributeType(D, vec, best_attribute);
    //check for attribute removal
    //if (attrType >= 2)
        usedAtrributes.push_back(best_attribute);
    // Create branches for the decision tree
    if (attrType == 1)
        create_continuous_branches(node, D, attrList.size(), vec);
    else
        create_branches(node, D, attrList, vec);

    return node;
}

int main()
{
    // Create D Tuple
    vector<tuple<string, vector<string>>> D = getDTuple(fname, outIndex);

    //create vector of attributes
    vector<string> vec[100]; //change depending on the value of attlen (so the number of different attributes)
    ifstream myfile(fname);

    string data;
    getline(myfile, data);
    char f = '\0';
    int d = strlen(data.c_str());
    int attlen = d / 2 + 1;
    int ff = 0;
    myfile.seekg(0);

    while (getline(myfile, data)) {
        int d = strlen(data.c_str());
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

    getline(myfile, data);
    cout << data << endl;

    myfile.close();
    TreeNode* DecTree = create_node();
    string rt = "root";
    strcpy(DecTree->parentResult, rt.c_str());
    DecTree = generate_decision_tree(D, attrList, vec, DecTree);
    print_tree(DecTree, 0);
    cout << "done" << endl;

}

/* Testing for D tuple
for (const auto& tuple : result) {
    std::cout << "Nth String: " << std::get<0>(tuple) << ", Rest of Strings: ";
    for (const auto& str : std::get<1>(tuple)) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
}
*/

/* EXAMPLE NODE CREATION
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
*/
