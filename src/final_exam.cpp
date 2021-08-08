/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 6 - Day 4: Final exam
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Instructor provided pure abstract class (interface)
 */
class Shipping {
public:
    static const int BASE_COST_PER_MILE = 2;
    virtual double getCost() = 0;
    virtual int getDistance() = 0;
    virtual std::string toString() = 0;

    virtual ~Shipping() = default;
};

/**
 * Exception thrown for invalid inputs when create package shipping instance
 */
class ShippingException : public runtime_error {
private:
    int _distance;
    const string _destination;

public:
    ShippingException(const string &errorMessage, int distance,
                      const string &destination)
        : runtime_error(errorMessage), _distance(distance),
          _destination(destination) {}

    int getDistance() const { return _distance; }

    const string &getDestination() const { return _destination; }
};

/**
 * Class implementing a regular package shipping
 */
class Package : public Shipping {
private:
    int _distance;
    const string _destination;

public:
    Package(int distance, const string &destination)
        : _distance(distance), _destination(destination) {
        const bool invalidDistance = (_distance < 0);
        const bool invalidDestination =
            (destination.empty() ||
             destination.find_first_not_of(" \t") == string::npos);

        ostringstream oss;
        if (invalidDistance) {
            oss << "Error: Invalid distance";
        }

        if (invalidDestination) {
            if (oss.tellp()) {
                oss << " and invalid destination";
            } else {
                oss << "Error: Invalid destination";
            }
        }

        if (invalidDistance || invalidDestination) {
            throw ShippingException(oss.str(), distance, destination);
        }
    }

    double getCost() override {
        return _distance * Shipping::BASE_COST_PER_MILE;
    }

    int getDistance() override { return _distance; }
    const string &getDestination() { return _destination; }

    bool doesCostMore(Package &other) { return getCost() > other.getCost(); }

    std::string toString() override {
        ostringstream oss;
        oss << "DESTINATION(" << getDestination() << ") MILES(" << getDistance()
            << ") SHIPPING_COST($" << getCost() << ")";
        return oss.str();
    }
};

/**
 * Class implementing a rushed shipping package extending a regular shipping
 * package
 */
class RushedPackage : public Package {
private:
    static const int RUSH_FEE_PER_MILE = 1;

public:
    RushedPackage(int distance, const string &destination)
        : Package(distance, destination) {}

    double getCost() override {
        return Package::getCost() + (getDistance() * RUSH_FEE_PER_MILE);
    }

    std::string toString() override { return Package::toString() + " RUSHED"; }
};

/**
 * Find the most expensive package in a vector of package pointers
 *
 * @param packages Vector of pointers to package (both regular and rushed
 * shipping)
 * @return Pointer to most expensive shipping for package
 */
Shipping *getMostExpensive(const vector<Shipping *> &packages) {
    Shipping *mostExpensive = nullptr;

    for (auto pkg : packages) {
        if (!mostExpensive || pkg->getCost() > mostExpensive->getCost()) {
            mostExpensive = pkg;
        }
    }

    return mostExpensive;
}

/**
 * Helper method with user interface to create a shipping package.
 * Provides 3 attempts to successfully create a valid shipping package
 *
 * @return Pointer to a shipping package
 */
Shipping *createPackage() {
    Shipping *res = nullptr;
    size_t remainingAttempts = 3;

    do {
        cout << endl << "Enter destination name (non empty/blanks): ";
        string destination;
        getline(cin, destination);

        cout << "Enter distance (positive integer): ";
        int distance;
        cin >> distance;

        cout << "Do you want rushed delivery (yes/no): ";
        string resp;
        cin.ignore();
        getline(cin, resp);
        const bool rushed = (resp == "yes");

        try {
            res = rushed ? new RushedPackage(distance, destination)
                         : new Package(distance, destination);
        } catch (const ShippingException &ex) {
            cout << endl << ex.what() << endl;

            if (--remainingAttempts) {
                cout << "Please retry, you have " << remainingAttempts
                     << " left." << endl;
            } else {
                cout << "Sorry, you have exhausted attempts!" << endl;
            }
        }
    } while (!res && remainingAttempts);

    return res;
}

int main(int argc, const char *argv[]) {
    // Setup different packages for testing
    Package p1(1, "one");
    Package p2(2, "two");
    Package p3(3, "three");

    RushedPackage r1(1, "one rushed");
    RushedPackage r2(2, "two rushed");
    RushedPackage r3(3, "three rushed");

    vector<Shipping *> packages = {&p1, &r1, &p2, &r2, &p3, &r3};

    {
        cout << endl << "Test string output:" << endl;
        for (auto pkg : packages) {
            cout << "\t" << pkg->toString() << endl;
        }
    }

    {
        cout << endl << "Testing error handling for negative distance:" << endl;
        try {
            new Package(-1, "negative distance");
            assert(!"should not reach here!!!");
        } catch (const ShippingException &ex) {
            cout << "\t" << ex.what() << endl;
            assert(-1 == ex.getDistance());
        }

        cout << endl << "Testing error handling for empty destination:" << endl;
        try {
            new Package(1, "");
            assert(!"should not reach here!!!");
        } catch (const ShippingException &ex) {
            cout << "\t" << ex.what() << endl;
            assert(ex.getDestination().empty());
        }

        cout << endl
             << "Testing error handling for blank/spaces destination:" << endl;
        try {
            new Package(1, "   ");
            assert(!"should not reach here!!!");
        } catch (const ShippingException &ex) {
            cout << "\t" << ex.what() << endl;
            assert(ex.getDestination().find_first_not_of(" \t") ==
                   string::npos);
        }

        cout << endl
             << "Testing error handling for negative distance and blank/spaces "
                "destination:"
             << endl;
        try {
            new Package(-1, "   ");
            assert(!"should not reach here!!!");
        } catch (const ShippingException &ex) {
            cout << "\t" << ex.what() << endl;
            assert(ex.getDistance() < 0 &&
                   ex.getDestination().find_first_not_of(" \t") ==
                       string::npos);
        }
    }

    {
        Shipping *mostExpensive = getMostExpensive(packages);
        assert(mostExpensive == &r3);
        cout << endl
             << "Test get most expensive package (expecting: " << r3.toString()
             << "): " << mostExpensive->toString() << endl;
    }

    {
        cout << endl << "Test package by user interaction:" << endl;
        Shipping *package = createPackage();
        if (package) {
            cout << "User added package: " << package->toString() << endl;
            delete package;
        }
    }

    return 0;
}