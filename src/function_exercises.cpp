/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 1 - Day 2: Review and practice writing functions
 */

#include <iostream>
using namespace std;

int readScores(int &countA, int &countB, int &countC, int &countD,
               int &countF) {
    countA = countB = countC = countD = countF = 0;
    int countInvalid = 0;

    int score = 0;
    do {
        cout << "Enter a score (-1 to terminate): ";
        double entry;
        cin >> entry;
        score = (int)entry;

        if (score >= 90) {
            ++countA;
        } else if (score >= 80) {
            ++countB;
        } else if (score >= 70) {
            ++countC;
        } else if (score >= 60) {
            ++countD;
        } else if (score >= 0) {
            ++countF;
        } else if (score != -1) {
            ++countInvalid;
        }
    } while (score != -1);

    return countInvalid;
}

void printSquare(char ch, int dimension) {
    for (int row = 0; row < dimension; ++row) {
        for (int col = 0; col < dimension; ++col) {
            if (col == 0 || col == dimension - 1 || row == 0 ||
                row == dimension - 1) {
                cout << ch;
            } else {
                cout << ' ';
            }
        }

        cout << endl;
    }
}

int main() {
    // Test reading scores
    int countA, countB, countC, countD, countF;
    int countInvalid = readScores(countA, countB, countC, countD, countF);
    cout << endl
         << "A = " << countA << ", B = " << countB << ", C = " << countC
         << ", D = " << countD << ", F = " << countF
         << ", Invalid = " << countInvalid << endl;

    // Test printing ASCII square
    printSquare('#', 5);
}
