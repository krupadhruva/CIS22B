/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 5 - Day 3: Programming project #3: Transact stocks using inheritance and
 * exception
 *
 * To compile:
 * clang++ src/transact_stocks.cpp -o TransactStocks
 */

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
 * Extends runtime error specialized for stock transaction errors
 */
class StockException : public runtime_error {
private:
    const int _error;

    static string errorMessage(int error, const string &msg) {
        ostringstream oss;
        oss << "Error: ";
        switch (error) {
        case ERROR_INVALID_PRICE:
            oss << "Invalid price. ";
            break;
        case ERROR_INVALID_SYMBOL:
            oss << "Invalid symbol. ";
            break;
        case ERROR_INVALID_QUANTITY:
            oss << "Invalid quantity. ";
            break;
        default:
            break;
        }
        oss << msg;

        return oss.str();
    }

public:
    // Error codes for various invalid stock transactions
    static const int ERROR_INVALID_PRICE = -1;
    static const int ERROR_INVALID_SYMBOL = -2;
    static const int ERROR_INVALID_QUANTITY = -3;

    /**
     * Get the actual error code for custom handling of errors
     *
     * @return Error code for invalid transaction
     */
    int getErrorCode() const { return _error; }

    StockException(int error, const string &msg)
        : runtime_error(errorMessage(error, msg)), _error(error) {}
};

/**
 * Implements the base class with common properties to model different stock
 * transactions.
 *
 * Performs validation on constructor inputs and throws StockException in errors
 */
class Stock {
private:
    size_t _numShares;
    const string _symbol;
    double _pricePerShare;

public:
    static const int BUY = 1;
    static const int SELL = 2;

    // Base class destructor should be virtual to ensure derived class
    // destructor gets invoked when deleting a derived class pointer stored as a
    // base class pointer
    virtual ~Stock() = default;

    size_t getNumberOfShares() const { return _numShares; }

    const string &getSymbol() const { return _symbol; }

    double getPricePerShare() const { return _pricePerShare; }

    virtual int getTransactionType() const = 0;

    /**
     * Constructor performs input validations and throws StockException on
     * errors
     *
     * @param numShares Number of shares to buy/sell, positive value
     * @param symbol Stock symbol, non empty string
     * @param pricePerShare Price per share, positive value
     */
    Stock(int numShares, const string &symbol, double pricePerShare)
        : _symbol(symbol) {
        if (numShares < 0) {
            throw StockException(StockException::ERROR_INVALID_QUANTITY,
                                 "Ignored");
        }

        if (pricePerShare < 0.0) {
            throw StockException(StockException::ERROR_INVALID_PRICE,
                                 "Ignored");
        }

        if (symbol.empty() || symbol.find_first_not_of(" \t") == string::npos) {
            throw StockException(StockException::ERROR_INVALID_SYMBOL,
                                 "Ignored");
        }

        _numShares = numShares;
        _pricePerShare = pricePerShare;
    }

    /**
     * Generic string representation of Stock instance
     *
     * @return String representation
     */
    virtual string toString() const {
        ostringstream oss;
        oss << fixed;
        oss << "TYPE(" << (getTransactionType() == BUY ? "buy" : "sell")
            << ") SYMBOL(" << getSymbol() << ") PRICE($" << setprecision(2)
            << getPricePerShare() << ") QUANTITY(" << getNumberOfShares()
            << ") TOTAL($" << setprecision(2)
            << (getPricePerShare() * (double)getNumberOfShares()) << ")";
        return oss.str();
    }
};

/**
 * Specializes Stock to support purchase/buy transaction
 */
class BuyTransaction : public Stock {
public:
    BuyTransaction(int numShares, const string &symbol, double pricePerShare)
        : Stock(numShares, symbol, pricePerShare) {}

    int getTransactionType() const override { return Stock::BUY; }
};

/**
 * Specializes Stock to support sell transaction
 */
class SellTransaction : public Stock {
public:
    SellTransaction(int numShares, const string &symbol, double pricePerShare)
        : Stock(numShares, symbol, pricePerShare) {}
    int getTransactionType() const override { return Stock::SELL; }
};

/**
 * Class implementing various commands and orchestration of stock transactions
 */
class Transactions {
private:
    // Allows reading/writing to file instead of console
    istream &input;
    ostream &output;

    // All valid transactions performed so far
    vector<Stock *> _transactions;

    /**
     * Show usage details
     */
    void usage() {
        output << "usage: " << endl;
        output << "\tbuy - Start a stock purchase/buy transaction" << endl;
        output << "\tsell - Start a stock sell transaction" << endl;
        output << "\tdisplay - Display all transactions" << endl;
        output << "\tsummary - Display summary of buy & sell transactions"
               << endl;
        output << "\texit - Quit the application." << endl;
        output << "\thelp - Display this help message" << endl;
    }

    /**
     * Implements stock buy/purchase command
     */
    bool buy() {
        bool status = false;

        output << "Enter stock symbol to buy: ";
        string symbol;
        input >> symbol;

        output << "Enter number of shares to buy: ";
        int numShares;
        input >> numShares;

        output << "Enter price per share: ";
        double pricePerShare;
        input >> pricePerShare;

        try {
            _transactions.push_back(
                new BuyTransaction(numShares, symbol, pricePerShare));
            status = true;
        } catch (const StockException &ex) {
            output << ex.what() << endl;
        }

        return status;
    }

    /**
     * Implements stock sell transaction
     */
    bool sell() {
        bool status = false;

        output << "Enter stock symbol to sell: ";
        string symbol;
        input >> symbol;

        output << "Enter number of shares to sell: ";
        int numShares;
        input >> numShares;

        output << "Enter price per share: ";
        double pricePerShare;
        input >> pricePerShare;

        try {
            _transactions.push_back(
                new SellTransaction(numShares, symbol, pricePerShare));
            status = true;
        } catch (const StockException &ex) {
            output << ex.what() << endl;
        }

        return status;
    }

    /**
     * Implements command to display all stock transactions
     */
    void display() {
        for (auto ptr : _transactions) {
            output << "\t" << ptr->toString() << endl;
        }
    }

    /**
     * Implements displaying the summary of buy & sell transactions
     */
    void summary() {
        size_t totalBuyTransactions = 0, totalSellTransactions = 0;
        double totalBuyPrice = 0.0, totalSellPrice = 0.0;

        for (auto ptr : _transactions) {
            if (ptr->getTransactionType() == Stock::BUY) {
                totalBuyPrice += (ptr->getPricePerShare() *
                                  (double)ptr->getNumberOfShares());
                ++totalBuyTransactions;
            } else if (ptr->getTransactionType() == Stock::SELL) {
                totalSellPrice += (ptr->getPricePerShare() *
                                   (double)ptr->getNumberOfShares());
                ++totalSellTransactions;
            }
        }

        output << fixed;
        output << "\t";
        output << "BUY-transactions(" << totalBuyTransactions << ") BUY-total($"
               << setprecision(2) << totalBuyPrice << ")  SELL-transactions("
               << totalSellTransactions << ") SELL-total($" << setprecision(2)
               << totalSellPrice << ")" << endl;
    }

public:
    Transactions() : Transactions(cin, cout) {}
    Transactions(istream &in, ostream &out) : input(in), output(out) {}

    /**
     * Free memory allocated to store transactions
     */
    virtual ~Transactions() {
        for (auto ptr : _transactions) {
            delete ptr;
        }
    }

    /**
     * Main command loop with user interaction to perform various stock
     * transactions
     */
    void run() {
        string cmd = "help";
        output << endl;

        while ("exit" != cmd) {
            if ("help" == cmd) {
                usage();
            } else if ("buy" == cmd) {
                buy();
            } else if ("sell" == cmd) {
                sell();
            } else if ("display" == cmd) {
                display();
            } else if ("summary" == cmd) {
                summary();
            } else if ("exit" != cmd) {
                output << "Invalid command '" << cmd << "', please retry."
                       << endl;
            }

            output << endl
                   << "Enter a command, ('help' for usage OR 'exit' to quit): ";
            input >> cmd;
        }
    }

    /**
     * Optionally load transactions from a file
     *
     * Contains multiple lines in the below format
     * transaction_type  quantity symbol price_per_share
     *
     * transaction_type: Integer, 1 (buy) or 2 (sell)
     * quantity: Integer, number of shares to transact
     * symbol: String, stock symbol
     * price_per_share: Double, price per share in '$'
     *
     * @param in Input file stream with stock transactions
     */
    void load(ifstream &in) {
        while (in.good()) {
            int type;
            in >> type;

            int quantity;
            in >> quantity;

            string sym;
            in >> sym;

            double price;
            in >> price;

            try {
                if (Stock::BUY == type) {
                    _transactions.push_back(
                        new BuyTransaction(quantity, sym, price));
                } else if (Stock::SELL == type) {
                    _transactions.push_back(
                        new SellTransaction(quantity, sym, price));
                }
            } catch (const StockException &ex) {
                output << ex.what() << endl;
            }

            // Note: The file handle becomes bad only when you try to read
            // beyond a file Read a character and if the file still has more
            // data to process, add it back into stream. If the file handle
            // becomes bad, we have reached the end
            char ch;
            in >> ch;
            if (in.good()) {
                in.putback(ch);
            }
        }
    }
};

int main(int argc, const char *argv[]) {
    // Skip program name
    --argc;
    ++argv;

    Transactions transact;

    // If we have to load transactions from file, load it before starting user
    // interactions
    if (argc) {
        ifstream in(argv[0]);
        if (in.is_open()) {
            cout << "Loading transaction data from file: " << argv[0] << endl;
            transact.load(in);
            in.close();
        } else {
            cout << "Error loading transaction data from file: " << argv[0]
                 << endl;
        }
    }

    // Start user interaction command loop
    transact.run();

    return 0;
}