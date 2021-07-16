/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 2 - Day 1: Programming project #1 (array practice)
 *
 * To compile:
 * clang++ src/track_expenses.cpp -o ManageListOfExpenses
 */

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Define errors in processing expense file
#define ERROR_MISSING_FILE (-1)
#define ERROR_INVALID_FILE (-2)

/**
 * Reads expenses from a given file and populates the arrays with
 * expenses and descriptions
 *
 * @param file Name of the file containing expenses with descriptions
 * @param expenses Array storing expenses
 * @param description Array storing description for each expense
 * @return Number of entries read from file (limited to max array size) or -1 on
 * error
 */
static int readExpenses(const char *file, double expenses[],
                        string description[], const size_t maxSize) {
    int count = 0;
    ifstream ifs;
    ifs.open(file, ifstream::in);
    if (ifs.fail()) {
        return ERROR_MISSING_FILE;
    }

    while (ifs.good() && count < maxSize) {
        ifs >> expenses[count];
        if (expenses[count] < 0) {
            count = ERROR_INVALID_FILE;
            break;
        }

        string desc;
        getline(ifs, desc);

        if (!desc.empty()) {
            description[count] = desc.substr(desc.find_first_not_of(" \t"),
                                             desc.find_last_not_of(" \t") + 1);
            ++count;
        }
    }

    ifs.close();

    return count;
}

/**
 * Finds all expenses that are equal to or greater than the input expense
 *
 * @param expense Expense to compare
 * @param expenses Array of expenses to compare against
 * @param results Array of indices into the expenses array matching the criteria
 * @param maxSize Size of entries in the expenses array
 * @return Count of indices in the results array
 */
static int fineExpensesGreaterThan(double expense, double expenses[],
                                   int results[], const size_t maxSize) {
    int count = 0;
    for (int ii = 0; ii < maxSize; ++ii) {
        if (expenses[ii] >= expense) {
            results[count++] = ii;
        }
    }

    return count;
}

/**
 * Performs a case insensitive match of keyword(s) against description in
 * expenses
 *
 * @param txt Keyword(s) to compare against descriptions
 * @param descriptions Array of descriptions
 * @param results Array of indices into the descriptions array matching the
 * criteria
 * @param maxSize Size of entries in the descriptions array
 * @return Count of indices in the results array
 */
static int findDescription(const string &txt, string descriptions[],
                           int results[], const size_t maxSize) {
    int count = 0;
    for (int ii = 0; ii < maxSize; ++ii) {
        if (strcasestr(descriptions[ii].c_str(), txt.c_str())) {
            results[count++] = ii;
        }
    }

    return count;
}

/**
 * Display the expenses
 *
 * @param expenses Array of expenses
 * @param descriptions Array of descriptions
 * @param indices Array of indices into expenses/descriptions array to display
 * OR nullptr to list all expenses
 * @param maxSize Number of entries in expenses/descriptions array
 */
static void displayExpenses(double expenses[], string descriptions[],
                            int indices[], const size_t maxSize) {
    for (int ii = 0; ii < maxSize; ++ii) {
        int idx = indices ? indices[ii] : ii;
        cout << fixed;
        cout << "AMOUNT($" << setprecision(2) << expenses[idx] << ")\tDESC("
             << descriptions[idx] << ")" << endl;
    }
}

/**
 * Helper method to display the usage of program
 */
static void usage(const string &prog) {
    cout << prog << ": Manage expenses program supports the following commands:"
         << endl;
    cout << "\thelp                 Displays the available commands" << endl;
    cout << "\tadd                  Add expense with a description" << endl;
    cout << "\tlist                 Displays all expenses and descriptions"
         << endl;
    cout << "\tamount>= <amount>    List all expenses greater than or equal to "
            "amount"
         << endl;
    cout << "\tsearch <keyword>     Search expenses based on description (case "
            "insensitive)"
         << endl;
    cout << "\texit                 Exit the program" << endl;
}

/**
 * Main entry point into the program
 *
 * @param argc Count of command line arguments (plus 1 for program name)
 * @param argv Array of command line arguments for the program
 * @return Exit status of the program, 0 on success
 */
int main(int argc, const char *argv[]) {
    string progName = argv[0];
    progName = progName.substr(progName.find_last_of('/') + 1);

    if (argc == 1) {
        cerr << "error: missing expenses file argument" << endl;
        return -1;
    }

    const size_t maxSize = 1024;
    int results[maxSize];
    double expenses[maxSize];
    string descriptions[maxSize];

    int count = readExpenses(argv[1], expenses, descriptions, maxSize);
    if (ERROR_MISSING_FILE == count) {
        cerr << "error: missing expenses file \"" << argv[1]
             << "\", failed to open" << endl;
        return count;
    } else if (ERROR_INVALID_FILE == count) {
        cerr << "error: invalid entries in expenses file \"" << argv[1]
             << "\", failed to load expenses" << endl;
        return count;
    }

    cout << "Welcome to managing a list of expenses." << endl;
    cout << "Please enter a command or 'exit' to end:" << endl;
    usage(progName);

    string cmd;
    while (true) {
        getline(cin, cmd);

        if (cmd == "help") {
            usage(progName);
        } else if (cmd == "add") {
            // Ensure we do not go out of bounds
            if (count >= maxSize) {
                cerr << "error: no space left for additional entries" << endl;
                continue;
            }

            double exp;
            cin >> exp;
            if (exp < 0) {
                cout << "Invalid price " << exp
                     << ", cannot be negative. Please try again" << endl;
                continue;
            }

            string txt;
            getline(cin, txt);
            txt = txt.substr(txt.find_first_not_of(" \t"),
                             txt.find_last_not_of(" \t") + 1);

            expenses[count] = exp;
            descriptions[count] = txt;
            ++count;
        } else if (cmd == "list") {
            displayExpenses(expenses, descriptions, nullptr, count);
        } else if (cmd == "amount>=") {
            double exp;
            cin >> exp;

            // Consume/ignore till end of line to ensure it is not interpreted
            // as a command
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            int resultCount =
                fineExpensesGreaterThan(exp, expenses, results, count);
            if (resultCount > 0) {
                displayExpenses(expenses, descriptions, results, resultCount);
            }
        } else if (cmd == "search") {
            string txt;
            getline(cin, txt);
            txt = txt.substr(txt.find_first_not_of(" \t"),
                             txt.find_last_not_of(" \t") + 1);
            int resultCount =
                findDescription(txt, descriptions, results, count);
            if (resultCount > 0) {
                displayExpenses(expenses, descriptions, results, resultCount);
            }
        } else if (cmd == "exit") {
            cout << "Thank you for using " << progName << endl;
            break;
        } else {
            cerr << "error: invalid command \"" << cmd
                 << "\" entered, please retry" << endl;
            usage(progName);
        }
    }

    return 0;
}