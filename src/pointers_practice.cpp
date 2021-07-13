/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 1 - Day 4: Practice on pointers
 */

#include <iostream>
using namespace std;

/**
 * Function moves all the zeros to end of array
 *
 * @param arr Input array of integers
 * @param arrSize Length of array
 * @return Number of '0' in function
 */
int deleteZeros(int arr[], int arrSize) {
    // Pointer pointing to the memory address of the first entry
    for (int *arrPtr = &arr[0]; arrPtr < &arr[arrSize - 1]; ++arrPtr) {
        if (*arrPtr == 0) {
            for (int *innerPtr = &arrPtr[0]; innerPtr < &arr[arrSize - 1];
                 ++innerPtr) {
                if (*innerPtr != 0) {
                    *arrPtr = *innerPtr;
                    *innerPtr = 0;
                    break;
                }
            }
        }
    }

    // Since we have moved all zeros to the end, count from end
    // till we find a non zero
    int *arrPtr = &arr[arrSize - 1];
    while (*arrPtr == 0 && --arrPtr > &arr[0])
        ;

    // Difference in position from end to first non-zero entry will give count
    // of zeros
    return (int)(&arr[arrSize - 1] - arrPtr);
}

/**
 * Function changes the capitalization of each alphabetic character in the given
 * text. All non-alphabets remain unchanged.
 *
 * @param text Input text
 * @return Number of alphabets that changed capitalization
 */
int reverseCase(char *text) {
    int count = 0;

    while (*text) {
        if (islower(*text)) {
            *text = toupper(*text);
            ++count;
        } else if (isupper(*text)) {
            *text = tolower(*text);
            ++count;
        }

        ++text;
    }

    return count;
}

/**
 * Function allocates memory for 'count' number of integers
 *
 * @param count Number of integers to store in returned array
 * @return Address of the array
 */
int *getNumList(int count) {
    int *arr = new int[count];

    for (int ii = 0; ii < count; ++ii) {
        cout << "Enter a number: ";
        cin >> arr[ii];
    }

    return arr;
}

int main() {
    {
        int arr[] = {0, 0, 10, 20, 30, 40, 50, 0, 60, 0};
        int size = sizeof(arr) / sizeof(arr[0]);

        cout << deleteZeros(arr, size) << endl;
    }

    {
        char text[] = "Hello World!";
        int count = reverseCase(text);

        cout << text << ": " << count << endl;
    }

    {
        int *arr = getNumList(5);
        for (int ii = 0; ii < 5; ++ii) {
            cout << arr[ii] << endl;
        }

        delete arr;
    }
}