//============================================================================
// Name        : CS 300 Project 2.cpp
// Author      : Jordan Dayedes
// Version     : 1.0
// Copyright   : NA
// Description : //
//============================================================================

#include <iostream>
#include <sstream>
#include <time.h>

#include "CSVparser.hpp"
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;


//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

/// Define a structure to hold course information
struct Course {
    string courseNum;
    string courseTitle;
    string prerequisites;
    string userCourse;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    int height;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node*& node, Course course);
    void inOrder(Node* node);

    int calculateHeight(Node* node);
    int getBalance(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course search(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

//destructor helper function
void deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}
// Destructor
BinarySearchTree::~BinarySearchTree() {
    deleteTree(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // FixMe (2): In order root
    // call inOrder fuction and pass root
    inOrder(root);
}


/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node*& node, Course course) {
    if (node == nullptr) {
        node = new Node(course);
        node->height = 1;
        return;
    }

    // compare courseNum strings
    if (course.courseNum < node->course.courseNum) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            addNode(node->left, course);
        }
    }
    else if (course.courseNum > node->course.courseNum) {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }

        // Recalculate the height of the tree
        int leftHeight = (node->left != nullptr) ? node->left->height : 0;
        int rightHeight = (node->right != nullptr) ? node->right->height : 0;
        node->height = 1 + max(leftHeight, rightHeight);
     
}


void BinarySearchTree::inOrder(Node* node) {
    
    if (node == nullptr) {
        return;
    }
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);

        //output CourseNum, description, prereqs
        
        //InOder right
        inOrder(node->right);
    }
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // FIXME (5) Implement inserting a bid into the tree
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);
    }
    else {
        // add Node root and bid
        cout << "Inserting bid with ID: " << course.courseNum << endl;
        addNode(root, course);
    }
}

/*
* Search for a Course
*/
Course BinarySearchTree::search(string courseNum) {
    Node* node = root;

    // loop until bottom reached or matching courseNum found
    while (node != nullptr) {
        // if match found, return current course
        if (courseNum == node->course.courseNum) {
            return node->course;
        }
        // if course is smaller than current node, go left
        else if (courseNum < node->course.courseNum) {
            node = node->left;
        }
        // else go right
        else {
            node = node->right;
        }
    }

    // Return an empty course if not found
    Course course;
    return course;
}

// Validate course file data
bool validateCourseFile(const string& inputLine, Course& course) {
    stringstream ss(inputLine);
    vector<string> tokens;

    string token;
    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    // Ensure at least two elements (courseNum and courseTitle) before accessing them
    if (tokens.size() >= 2) {
        course.courseNum = tokens[0];
        course.courseTitle = tokens[1];

        // Check if prerequisites are present, set to an empty string if not
        if (tokens.size() >= 3) {
            course.prerequisites = tokens[2];
        }
        else {
            course.prerequisites = "";  // or set to any default value
        }

        return true;
    }
    else {
        cerr << "Error: Invalid data in line - " << inputLine << endl;
        return false;
    }
}

// Read course file function
vector<Course> readCourseFile(const string& filePath) {
    vector<Course> courseData;

    try {
        ifstream openFile(filePath);
        if (!openFile.is_open()) {
            throw ios_base::failure("Error, no file found!");
        }

        string line;
        while (getline(openFile, line)) {
            Course course;

            // Call validateCourseFile to parse the line and validate
            if (validateCourseFile(line, course)) {
                courseData.push_back(course);
            }
        }

        openFile.close();
    }
    catch (const ios_base::failure& ex) {
        cerr << ex.what() << endl;
    }

    return courseData;
}

// Function to display courses
void displayCourses(const vector<Course>& courses) {
    cout << endl << "Here is a sample schedule:" << endl << endl;
    for (const auto& course : courses) {
        cout << course.courseNum << ", " << course.courseTitle << endl;
        //cout << "Course Title: " << course.courseTitle << endl;
        //cout << "Prerequisites: " << course.prerequisites << endl << endl;
    }
    cout << endl;
}

// Function to find an input course and output course prerequisites

void findCourse(const vector<Course>& courses, const string& targetCourseNum) {
    
    bool courseFound = false;

    // search courses for input course
    auto findCourses = find_if(courses.begin(), courses.end(),
        [targetCourseNum, &courseFound](const Course& course) {
            if (course.courseNum == targetCourseNum) {
                // debug if input course found
                cout << "Course: " << targetCourseNum << " found" << endl;
                courseFound = true;
                return true;
            }
            return false;

        });
    // check if course is found
     if (findCourses != courses.end()) {
                // Output found course information
                cout << findCourses->courseNum << ", " << findCourses->courseTitle << endl;
                
                // Output course prerequisisites
                if (!findCourses->prerequisites.empty()) {
                    // debug print statement
                    cout << "Prerequisite courses: " ;

                    // Iterate through course prerequisites
                    size_t start = 0;
                    size_t end = findCourses->prerequisites.find(',');

                    while (end != string::npos) {
                        cout << findCourses->prerequisites.substr(start, end - start) << " ";
                        start = end + 1;
                        end = findCourses->prerequisites.find(",", start);
                    }
                    cout << findCourses->prerequisites.substr(start) << endl;
                }
                else {
                    cout << "Prerequisites: None" << endl;
                }
    }
}

// main function mainly does main stuff
int main() {
    // Default course file
    string filePath = "MATH201_Discrete Mathematics.csv";
    string userCourse = "";
    vector<Course> courses = readCourseFile(filePath);

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner. " << endl << endl;
        cout << "1. Load Data Structure. " << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;

        cout << "What would you like to do?" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            // Case 1 code goes here

            courses = readCourseFile(filePath);
            cout << "Loading courses" << endl << endl;

            // insert courses into the BST
            for (const auto& course : courses) {
                bst->Insert(course);
            }

            break;
        case 2:
            // Case 2 code goes here
            displayCourses(courses);
            break;
        case 3:
            // Case 3 code goes here
            string userCourse;
            cout << "What course do you want to know about? " << endl;
            // TODO Add logic to print a specific course based on user input
            cin >> userCourse;
            findCourse(courses, userCourse);
            cout << endl;
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}