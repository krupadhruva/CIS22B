//
// Created by Krupa Dhruva on 7/19/21.
//

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

/**
 * Method to check if command line arguments follow specified syntax/rules
 *
 * @param argc Number of arguments in an array of command line arguments
 * @param argv Array of command line arguments
 */
void showHelp(int argc, const char *argv[]) {
    bool isValid = false;
    bool hasNum = false;
    long number = 0;
    for (int ii = 0; ii < argc && !isValid; ++ii) {
        if (!strcmp(argv[ii], "/?") || !strcmp(argv[ii], "/help") ||
            !strcmp(argv[ii], "-help")) {
            isValid = true;

            // Look for topic number if present
            if (ii + 1 < argc) {
                char *end;
                number = strtol(argv[1], &end, 0);
                hasNum = !(*end);
                break;
            }
        }
    }

    if (isValid) {
        cout << "yes, topic number";
        if (hasNum) {
            cout << " " << number;
        }
        cout << endl;
    } else {
        cout << "no, topic number (N/A)" << endl;
    }
}

/**
 * Stores an array of integer pointers with used provided integers
 *
 * Note: Does not perform any validation of user entered number
 *
 * @param count Number of user inputs to accept and store in array
 * @return Array of integer pointers with user input data, nullptr if count <= 0
 */
int **getNumList(int count) {
    // Check if valid argument
    if (count <= 0) {
        return nullptr;
    }

    // Allocate array of integer pointers to store user entries
    int **arr = new int *[count];
    for (int ii = 0; ii < count; ++ii) {
        cout << "Enter a number: ";

        // Allocate memory to store use entry in the array
        arr[ii] = new int();

        // Read user input directly into allocated memory in the array
        // and store value at the allocated address
        cin >> *(arr[ii]);
    }

    return arr;
}

int main(int argc, const char *argv[]) {
    // We are not interested in the program name
    --argc;
    ++argv;
    showHelp(argc, argv);

    const int count = 5;
    int **arr = getNumList(count);
    if (!arr) {
        return -1;
    }

    // Print the data from an array of integers and free allocated memory
    for (int ii = 0; ii < count; ++ii) {
        cout << *(arr[ii]) << endl;
        delete arr[ii];
    }
    delete[] arr;

    return 0;
}
