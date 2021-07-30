/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 5 - Day 1: Practice on inheritance
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Base class implementing boolean value based variable
 */
class BooleanVariable {
private:
    const string _name;
    const bool _value;

    // Getting a string representation of value allows sharing toString()
    // implementation with derived class
    virtual string getValueString() const {
        ostringstream oss;
        oss << boolalpha << bool(getValue());
        return oss.str();
    }

public:
    BooleanVariable(const string &name, bool value)
        : _name(name), _value(value) {}

    const string &getName() const { return _name; }

    // C++ bool can be evaluated as an int. This allows us to
    // handle both bool and int types and not have to reimplement
    virtual int getValue() const { return _value; }

    // Using getValueString() allows us to have a type agnostic implementation
    // by converting bool/int to a string
    virtual string toString() const {
        ostringstream oss;
        oss << getName() << "(" << getValueString() << ")";
        return oss.str();
    }

    virtual bool contains(const string &str) const {
        return strcasestr(getName().c_str(), str.c_str()) != nullptr;
    }
};

/**
 * Class implementing integer value based variable
 *
 * Derives from boolean value based variable to share common implementations
 */
class IntegerVariable : public BooleanVariable {
private:
    const int _value;

    // Override base class implementation to show integer value instead of
    // boolean value
    string getValueString() const override {
        ostringstream oss;
        oss << _value;
        return oss.str();
    }

public:
    IntegerVariable(const string &name, int value)
        : BooleanVariable(name, bool(value)), _value(value) {}

    bool isGreater(const IntegerVariable &other) const {
        return getValue() > other.getValue();
    }
};

/**
 * Case insensitive match of variable names in input collection
 *
 * @param collection Input collection of boolean or integer variable pointers
 * @param str String to match against variable names
 * @param exactMatchCount Count of exact name matches if found
 * @return Vector of pointers of variables with partial name match
 */
static vector<const BooleanVariable *>
contains(const vector<const BooleanVariable *> &collection, const string &str,
         int &exactMatchCount) {
    exactMatchCount = 0;
    vector<const BooleanVariable *> matches;
    for (auto ent : collection) {
        if (ent->contains(str)) {
            matches.push_back(ent);
            if (ent->getName() == str) {
                ++exactMatchCount;
            }
        }
    }

    return matches;
}

/**
 * Helper method with user interaction to create boolean or integer variables
 * No validation of values is performed and hence expects 'true'/'false' for
 * boolean and a valid integer for integer type variable
 *
 * @return Pointer to BooleanVariable or IntegerVariable instance
 */
static const BooleanVariable *createNewVariable() {
    BooleanVariable *ptr = nullptr;

    cout << "Enter the name of variable: ";
    string name;
    cin >> name;

    cout << "Enter the value (true/false OR integer): ";
    string value;
    cin >> value;

    if (value == "true") {
        ptr = new BooleanVariable(name, true);
    } else if (value == "false") {
        ptr = new BooleanVariable(name, false);
    } else {
        istringstream iss(value);
        int intValue;
        iss >> intValue;
        ptr = new IntegerVariable(name, intValue);
    }

    return ptr;
}

void usage() {
    cout
        << "Following commands are supported\n"
           "\thelp             - Usage help\n"
           "\tend              - ends the program\n"
           "\tadd              - Add a boolean or integer type entry\n"
           "\tlist             - List created boolean or integer variables\n"
           "\tcontains <name>  - Matches name with created variables and lists "
           "found matches along with exact match count\n"
        << endl;
}

/**
 * Helper method to accept user command
 * @return Command entered by user
 */
string getCmd() {
    cout << "Enter a command or 'end' to stop: ";
    string cmd;
    cin >> cmd;
    return cmd;
}

int main(int argc, const char *argv[]) {
    cout << endl;
    usage();

    string cmd;
    vector<const BooleanVariable *> collection;
    while ("end" != (cmd = getCmd())) {
        if (cmd == "add") {
            collection.push_back(createNewVariable());
        } else if (cmd == "list") {
            for (auto ent : collection) {
                cout << ent->toString() << endl;
            }
        } else if (cmd == "contains") {
            string str;
            cin >> str;
            int matches = 0;
            vector<const BooleanVariable *> res =
                contains(collection, str, matches);
            if (matches) {
                cout << "exact matches count: " << matches << endl;
            }
            for (auto ent : res) {
                cout << "\t" << ent->toString() << endl;
            }
        } else {
            usage();
        }
    };

    // Test comparison of 2 IntegerVariable instances
    const IntegerVariable v1("int1", 1), v2("int2", 2);
    bool res = v1.isGreater(v2);
    assert(!res);
    cout << "Is " << v1.toString() << " > " << v2.toString() << "? "
         << boolalpha << res << endl;

    return 0;
}