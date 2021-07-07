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

bool isComment() {
    cout << "Enter text: ";
    string text;
    cin >> text;

    if (text.empty()) {
        return false;
    }

    for (int ii = 0; ii < text.length() - 1; ++ii) {
        if (text.at(ii) == ' ') {
            continue;
        }

        return (text.at(ii) == '/' && text.at(ii + 1) == '/');
    }

    return false;
}

bool CheckAgainstPasswordRules(string password) {
    if (password.length() < 8) {
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasSpecial = false;

    for (int ii = 0; ii < password.length(); ++ii) {
        const char ch = password.at(ii);

        if (isspace(ch)) {
            return false;
        } else if (isupper(ch)) {
            hasUpper = true;
        } else if (islower(ch)) {
            hasLower = true;
        } else if (ch == '!' || ch == '@' || ch == '#') {
            hasSpecial = true;
        }
    }

    return hasUpper && hasLower && hasSpecial;
}

int main() {
    // Test reading scores
    {
        int countA, countB, countC, countD, countF;
        int countInvalid = readScores(countA, countB, countC, countD, countF);
        cout << endl
             << "A = " << countA << ", B = " << countB << ", C = " << countC
             << ", D = " << countD << ", F = " << countF
             << ", Invalid = " << countInvalid << endl;
    }

    // Test printing ASCII square
    printSquare('#', 5);

    // Test C++ comment
    {
        bool retVal = isComment();
        cout << "Is string is a comment: " << (retVal ? "true" : "false")
             << endl;
    }

    // Test if password is valid
    {
        bool retVal = CheckAgainstPasswordRules("hello!#D");
        cout << "Is password valid: " << (retVal ? "true" : "false") << endl;
    }
}
