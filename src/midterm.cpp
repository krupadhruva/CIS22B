/*
 * Author: Krupa Dhruva
 * Date: 21, July 2021
 * Description: Midterm exam
 * Compile from shell: clang++ src/midterm.cpp -std=c++11 -o midterm
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
 * Class implementing purchase order
 *
 * Note: Constructor does not perform any validation, use helper function to
 * create by allocating memory on heap.
 */
class PurchaseOrder {
private:
    const int _numberOfItems;
    const double _costPerItem;
    const string _description;

public:
    static int compareDouble(double a, double b) {
        a = round(a);
        b = round(b);
        if (a < b) {
            return -1;
        } else if (a > b) {
            return 1;
        }

        return 0;
    }

    PurchaseOrder(int numItems, double itemCost, const string &desc)
        : _numberOfItems(numItems), _costPerItem(itemCost), _description(desc) {
    }

    int getNumberOfItems() const { return _numberOfItems; }

    double getTotalCost() const {
        return getNumberOfItems() * getCostPerItem();
    }

    double getCostPerItem() const { return _costPerItem; }

    const string &getDescription() const { return _description; }

    /**
     * String representation of PurchaseOrder class with round off
     * @return String representation
     */
    string toString() const {
        ostringstream oss;
        oss << fixed;
        oss << "Description(" << getDescription() << ") Quantity("
            << getNumberOfItems() << ") Cost($" << setprecision(2)
            << getCostPerItem() << ") Total($" << setprecision(2)
            << getTotalCost() << ")";
        return oss.str();
    }

    /**
     * Compares 2 purchase order instances based on total cost with rounding to
     * handle precision issues with double
     *
     * @param other PurchaseOrder object to compare with
     * @return -1 if less, 0 if equal or 1 if greater (in terms of total cost)
     */
    int compareTotal(const PurchaseOrder &other) const {
        return PurchaseOrder::compareDouble(getTotalCost(),
                                            other.getTotalCost());
    }
};

/**
 * Helper method to validate inputs and create a PurchaseOrder object by
 * allocating memory on heap
 *
 * @param quantity Number of items (> 0)
 * @param costPerItem Cost per item (> 0.0)
 * @param desc Purchase order description (non empty and not filled with
 * spaces/tabs)
 * @return Purchase order object allocated on heap if input is valid OR NULL
 * (nullptr)
 */
PurchaseOrder *createPurchaseOrder(int quantity, double costPerItem,
                                   const string &desc) {
    if (quantity > 0 && costPerItem > 0.0 && !desc.empty() &&
        desc.find_first_not_of(" \t") != string::npos) {
        return new PurchaseOrder(quantity, costPerItem, desc);
    }

    return nullptr;
}

/**
 * Function to combine multiple purchase orders for same item (item cost and
 * description) into a single purchase order
 *
 * @param arr Array of pointers to purchase orders to combine from
 * @param len Length of array
 * @param costPerItem Cost per item of purchase orders to combine
 * @param desc Description of purchase orders to combine
 * @param countMatches Out parameter with number of purchase orders combined
 * @return Combined purchase order object allocated on heap OR NUL (nullptr) if
 * none were found to combine
 */
PurchaseOrder *getCombinedPurchaseOrder(const PurchaseOrder *arr[], size_t len,
                                        double costPerItem, const string &desc,
                                        size_t &countMatches) {
    countMatches = 0;
    int totalQuantity = 0;
    for (int ii = 0; ii < len; ++ii) {
        // Compare double with round off always
        if (PurchaseOrder::compareDouble(arr[ii]->getCostPerItem(),
                                         costPerItem) == 0 &&
            arr[ii]->getDescription() == desc) {
            ++countMatches;
            totalQuantity += arr[ii]->getNumberOfItems();
        }
    }

    PurchaseOrder *cumulativeOrder = nullptr;
    if (totalQuantity > 0) {
        cumulativeOrder = new PurchaseOrder(totalQuantity, costPerItem, desc);
    }

    return cumulativeOrder;
}

/**
 * Remove purchase order instances with negative cost per item from the input
 * vector of purchase orders
 *
 * @param orders Vector of points to purchase order objects
 * @return Number of items removed from the vector based on matching criteria
 */
size_t removeNegativePrice(vector<PurchaseOrder *> &orders) {
    const size_t initialSize = orders.size();
    vector<PurchaseOrder *>::const_iterator it = orders.begin();
    while (it != orders.end()) {
        if (PurchaseOrder::compareDouble((*it)->getCostPerItem(), 0.0) < 0) {
            it = orders.erase(it);
        } else {
            ++it;
        }
    }

    return initialSize - orders.size();
}

/**
 * Looks for words with "??" in them
 *
 * @param argc Length of input array of words (C strings)
 * @param argv Array of words (C strings)
 * @return True if any word matched criteria, else false
 */
bool hasTwoQuestionMarks(int argc, const char *argv[]) {
    for (int ii = 0; ii < argc; ++ii) {
        if (strstr(argv[ii], "??")) {
#ifndef NDEBUG
            // C++ way involves constructing a C++ string
            const string str(argv[ii]);
            assert(str.find("??") != string::npos);
#endif
            return true;
        }
    }

    return false;
}

int main(int argc, const char *argv[]) {
    // Skip the program name in command line arguments
    --argc;
    ++argv;

    const PurchaseOrder orders[] = {
        PurchaseOrder(1, 1.0, "one"),
        PurchaseOrder(2, 2.0, "two"),
        PurchaseOrder(3, 3.0, "three"),
        PurchaseOrder(4, 4.0, "four"),
    };

    {
        cout << endl << "Test PurchaseOrder class:" << endl;
        cout << "\ttoString: " << orders[1].toString() << endl;
        cout << "\tgetTotalCost: " << orders[2].getTotalCost() << endl;
        cout << "\tcompareTotal (equal): " << boolalpha
             << (0 == orders[0].compareTotal(orders[0])) << endl;
        cout << "\tcompareTotal (less than): " << boolalpha
             << (-1 == orders[0].compareTotal(orders[1])) << endl;
        cout << "\tcompareTotal (greater than): " << boolalpha
             << (1 == orders[1].compareTotal(orders[0])) << endl;
    }

    {
        cout << endl << "Test creating purchase order function:" << endl;
        PurchaseOrder *po = createPurchaseOrder(1, 1.0000023, "one");
        assert(po);
        cout << "\tcreatePurchaseOrder (valid):" << boolalpha << (po != nullptr)
             << endl;
        cout << "\tcompareDouble (with round off): " << boolalpha
             << (0 == PurchaseOrder::compareDouble(orders[0].getCostPerItem(),
                                                   po->getCostPerItem()))
             << endl;
        delete po;

        po = createPurchaseOrder(-1, 1.0000023, "one");
        cout << "\tcreatePurchaseOrder (invalid quantity):" << boolalpha
             << (po == nullptr) << endl;

        po = createPurchaseOrder(1, -1.0000023, "one");
        assert(po == nullptr);
        cout << "\tcreatePurchaseOrder (invalid cost):" << boolalpha
             << (po == nullptr) << endl;

        po = createPurchaseOrder(1, 1.0000023, "     \t");
        assert(po == nullptr);
        cout << "\tcreatePurchaseOrder (invalid description):" << boolalpha
             << (po == nullptr) << endl;
    }

    {
        cout << endl << "Test get combined purchase order:" << endl;
        const PurchaseOrder *arr[] = {&orders[0], &orders[0], &orders[1],
                                      &orders[2]};

        size_t matches = 0;
        PurchaseOrder *po =
            getCombinedPurchaseOrder(arr, 4, orders[0].getCostPerItem(),
                                     orders[0].getDescription(), matches);
        assert(po);
        cout << "\tgetCombinedPurchaseOrder (2 items): " << boolalpha
             << (po != nullptr) << endl;
        cout << "\t\tCombined item count (expecting 2): " << matches << endl;
        delete po;

        po = getCombinedPurchaseOrder(arr, 4, orders[4].getCostPerItem(),
                                      orders[4].getDescription(), matches);
        assert(po == nullptr);
        cout << "\tgetCombinedPurchaseOrder (0 items): " << boolalpha
             << (po == nullptr) << endl;
        cout << "\t\tCombined item count (expecting 0): " << matches << endl;
    }

    {
        cout << endl << "Test remove negative from vector:" << endl;
        vector<PurchaseOrder *> vec;
        vec.push_back(new PurchaseOrder(10, -1.0, "-1.0"));
        vec.push_back(new PurchaseOrder(10, 2.0, "2.0"));
        vec.push_back(new PurchaseOrder(10, 3.0, "3.0"));
        vec.push_back(new PurchaseOrder(10, -4.0, "-4.0"));
        vec.push_back(new PurchaseOrder(10, 5.0, "5.0"));
        size_t removed = removeNegativePrice(vec);
        assert(removed == 2);
        cout << "\tremoveNegativePrice (2 negative): " << boolalpha
             << (removed == 2) << endl;

        // Free allocated memory
        for (auto o : vec) {
            delete o;
        }
    }

    {
        cout << endl << "Test match for \"??\":" << endl;

        // Pre constructed words to match against for valid match
        {
            const char *words[] = {"one", "two?", "he??o", "there"};
            bool ret = hasTwoQuestionMarks(3, words);
            assert(ret);
            cout << "\thasTwoQuestionMarks (expecting true): " << boolalpha
                 << ret << endl;
        }

        // Pre constructed words to match against for invalid match
        {
            const char *words[] = {"one", "two?", "?he?o", "there"};
            bool ret = hasTwoQuestionMarks(3, words);
            assert(!ret);
            cout << "\thasTwoQuestionMarks (expecting false): " << boolalpha
                 << ret << endl;
        }

        // Words taken from command line to match against for valid match
        if (argc) {
            ostringstream oss;
            for (int ii = 0; ii < argc; ++ii) {
                oss << argv[ii] << " ";
            }

            bool ret = hasTwoQuestionMarks(argc, argv);
            cout << "\thasTwoQuestionMarks (cli args: " << oss.str()
                 << "): " << boolalpha << ret << endl;
        }
    }

    return 0;
}