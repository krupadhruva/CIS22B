/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 3 - Day 3: Midterm exam practice set 02
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Degree {
private:
    const int _year;
    const string _subject;
    const string _name;

public:
    int getYear() const { return _year; }

    const string &getSubject() const { return _subject; }

    const string &getName() const { return _name; }

    Degree(const string &name, const string &sub, int year)
        : _year(year), _subject(sub), _name(name) {}

    string toString() const {
        ostringstream oss;
        oss << "NAME(" << getName() << ") SUBJECT(" << getSubject() << ") YEAR("
            << getYear() << ")";
        return oss.str();
    }

    bool hasTheSameDegree(const Degree &other) const {
        return _year == other._year && _subject == other._subject;
    }
};

/**
 * Finds degree based on year and subject
 *
 * @param arr Array of pointers to Degree
 * @param len Length of array
 * @param year Year to compare
 * @param sub Subject to compare
 * @return Vector of pointers to Degree objects that match the criteria
 */
vector<Degree *> findDegreesByYearAndSubject(Degree *arr[], size_t len,
                                             int year, const string &sub) {
    vector<Degree *> results;
    const Degree compare("unused", sub, year);

    for (int ii = 0; ii < len; ++ii) {
        if (arr[ii]->hasTheSameDegree(compare)) {
            results.push_back(arr[ii]);
        }
    }

    return results;
}

/**
 * Find degrees by subject and the latest year of degree for the subject
 *
 * @param arr Array of Degree objects
 * @param len Length of array
 * @param sub Subject to search
 * @param latestYear Out parameter with latest year of degree for matching
 * subject
 * @return Vector of indices to matching Degree objects in the input array of
 * Degree objects
 */
vector<int> findLatestDegreesBySubject(Degree arr[], size_t len,
                                       const string &sub, int &latestYear) {
    int latestIdx = -1;
    vector<int> results;
    for (int ii = 0; ii < len; ++ii) {
        if (arr[ii].getSubject() == sub) {
            if (latestIdx == -1 ||
                (arr[ii].getYear() > arr[latestIdx].getYear())) {
                latestIdx = ii;
            }

            results.push_back(ii);
        }
    }

    if (!results.empty()) {
        latestYear = arr[latestIdx].getYear();
    }

    return results;
}

/**
 * Checks if words in an array have the same ending character
 *
 * @param argc Length of array (argument count)
 * @param argv Array of strings (char pointer)
 * @return True if all words have same last char, single argument or no
 * arguments. Else, false
 */
bool HasTheSameEndingChar(int argc, const char *argv[]) {
    if (argc < 2) {
        return true;
    }

    const char last = argv[0][strlen(argv[0]) - 1];
    for (int ii = 1; ii < argc; ++ii) {
        if (argv[ii][strlen(argv[ii]) - 1] != last) {
            return false;
        }
    }

    return true;
}

int main(int argc, const char *argv[]) {
    --argc;
    ++argv;

    Degree d1("Bose", "Physics", 2010);
    Degree d2("Planck", "Physics", 2016);
    Degree d3("Bill", "Computer Science", 2010);
    Degree d4("Steve", "Computer Science", 2011);

    {
        cout << endl
             << "Test find degrees by year \"" << d1.getYear()
             << "\" & subject \"" << d1.getSubject() << "\": ";
        Degree *arr[] = {&d1, &d2, &d3, &d4};
        vector<Degree *> results =
            findDegreesByYearAndSubject(arr, 4, d1.getYear(), d1.getSubject());
        assert(results.size() == 1 && results.front() == &d1);
        cout << boolalpha << true << endl;

        cout << "Results:" << endl;
        for (auto ptr : results) {
            cout << ptr->toString() << endl;
        }
    }

    {
        cout << endl
             << "Test find latest year and degrees by subject \""
             << d1.getSubject() << "\": ";
        int latestYear = -1;
        Degree arr[] = {d1, d2, d3, d4};
        vector<int> results =
            findLatestDegreesBySubject(arr, 4, d1.getSubject(), latestYear);
        assert(results.size() == 2 && latestYear == 2016);
        cout << boolalpha << true << endl;

        cout << "Results:" << endl;
        cout << "Latest year: " << latestYear << endl;
        for (auto idx : results) {
            cout << arr[idx].toString() << endl;
        }
    }

    {
        ostringstream oss;
        for (int ii = 0; ii < argc; ++ii) {
            oss << argv[ii] << " ";
        }
        cout << endl
             << "Test same ending char \"" << oss.str() << "\": " << boolalpha
             << HasTheSameEndingChar(argc, argv) << endl;
    }

    return 0;
}