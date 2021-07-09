/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 1 - Day 3: Review and practice writing functions with array manipulation
 */

#include <iostream>
#include <string>
using namespace std;

/**
 * Given an array of integers, function finds the number of positive values,
 * number of negative values, number of zeroes, the largest number, and the
 * smallest number.
 *
 * @param arr Array of integers
 * @param size Length of array
 * @param posNum Number of positive values in array
 * @param negNum Number of negative values in array
 * @param zeroNum Number of zeroes in array
 * @param largeNum Largest number in array
 * @param smallNum Smallest number in array
 */
void analyzeNumList(const int arr[], int size, int &posNum, int &negNum,
                    int &zeroNum, int &largeNum, int &smallNum) {
    // initialize all variables
    posNum = 0;
    negNum = 0;
    zeroNum = 0;
    largeNum = arr[0];
    smallNum = arr[0];

    // to traverse array
    for (int ii = 0; ii < size; ++ii) {
        if (arr[ii] > 0) {
            ++posNum;
        } else if (arr[ii] < 0) {
            ++negNum;
        } else if (arr[ii] == 0) {
            ++zeroNum;
        }

        if (arr[ii] > largeNum) {
            largeNum = arr[ii];
        } else if (arr[ii] < smallNum) {
            smallNum = arr[ii];
        }
    }
}

/**
 * Function finds the first negative number in the given array and swaps it with
 * last found positive number in the array.
 *
 * @param arr Array of integers
 * @param size Length of array
 * @return True if a swap was made and false if no swap
 */
bool swapTheFirstNegativeWithTheLastPositive(int arr[], int size) {
    int firstNegIdx = -1;
    int lastPosIdx = -1;

    for (int ii = 0; ii < size; ++ii) {
        if (arr[ii] < 0 && firstNegIdx == -1) {
            firstNegIdx = ii;
        }

        if (arr[ii] > 0) {
            lastPosIdx = ii;
        }
    }

    if (firstNegIdx == -1 || lastPosIdx == -1) {
        return false;
    }

    int tmp = arr[firstNegIdx];
    arr[firstNegIdx] = arr[lastPosIdx];
    arr[lastPosIdx] = tmp;

    return true;
}

/**
 * Provided a string, function checks if the first three characters match
 * the last three characters (case sensitive).
 *
 * @param text Input string to be read
 * @return If first three characters match last three, returns true. False if
 * otherwise.
 */
bool areFirstThreeTheSameAsLastThreeChars(const string &text) {
    if (text.size() < 3) {
        return false;
    }

    string firstThree;
    string lastThree;
    for (int ii = 0; ii < text.size(); ++ii) {
        if (ii == 2) {
            firstThree = text.substr(0, 3);
        }

        if (ii == text.size() - 1) {
            lastThree = text.substr(text.size() - 3, text.size());
        }
    }

    return firstThree == lastThree;
}

/**
 * Function checks if the inputted array of characters contains only valid
 * grades (A-F excluding E and case insensitive). If there is an invalid
 * grade, function replaces grade with "I".
 *
 * @param arr Array of characters
 * @param length Length of array
 * @return Number of invalid grades found and replaced
 */
int replaceGrades(char arr[], int length) {
    int invalCount = 0;
    for (int ii = 0; ii < length; ++ii) {
        if (arr[ii] != 'A' && arr[ii] != 'a' && arr[ii] != 'B' &&
            arr[ii] != 'b' && arr[ii] != 'C' && arr[ii] != 'c' &&
            arr[ii] != 'D' && arr[ii] != 'd' && arr[ii] != 'F' &&
            arr[ii] != 'f') {
            arr[ii] = 'I';
            ++invalCount;
        }
    }

    return invalCount;
}

int main() {
    {
        // testing first function
        int posNum, negNum, zeroNum, largeNum, smallNum;
        int arr[] = {3, 2, 1, 0, -1, 8, -3};
        int size = 7;

        analyzeNumList(arr, size, posNum, negNum, zeroNum, largeNum, smallNum);
        cout << "Number of positive numbers: " << posNum << endl;
        cout << "Number of negative numbers: " << negNum << endl;
        cout << "Number of zeroes: " << zeroNum << endl;
        cout << "Largest number in provided array: " << largeNum << endl;
        cout << "Smallest number in provided array: " << smallNum << endl;
    }

    {
        // testing second function
        int arr[] = {12, -2, -1, -0, -1, -8, -10};
        int size = 7;

        bool ret = swapTheFirstNegativeWithTheLastPositive(arr, size);
        cout << (ret ? "swapped" : "intact") << endl;
    }

    {
        // testing third function
        string str = "ABCABC";
        bool ret = areFirstThreeTheSameAsLastThreeChars(str);
        cout << (ret ? "same" : "not same") << endl;
    }

    {
        // testing fourth function
        char arr[] = {
            'A',
            'B',
            'X',
            'C',
        };

        /*
         * sizeof() gives the amount of memory used not the length so to get the
         * length, divide total memory used by the memory used for one entry
         */
        int length = sizeof(arr) / sizeof(arr[0]);
        cout << "The number of invalid grades found: "
             << replaceGrades(arr, length) << endl;
    }
}