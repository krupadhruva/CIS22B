/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 6 - Day 1: Final exam practice on polymorphism and exception
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Cert {
private:
    int m_year;
    std::string m_subject;

public:
    Cert(std::string subject, int year) : m_subject(subject), m_year(year) {}
    int getYear() { return m_year; }
    std::string getSubject() { return m_subject; }

    virtual std::string toString() {
        std::stringstream stream;
        stream << "SUBJECT(" << m_subject << ") YEAR(" << m_year << ")";
        return stream.str();
    }
};

class InvalidYear : public runtime_error {
private:
    const int _badYear;

public:
    InvalidYear(int badYear, const char *msg)
        : runtime_error(msg), _badYear(badYear) {}

    int getBadYear() const { return _badYear; }
};

class Award : public Cert {
private:
    const string _name;

public:
    Award(const string &subject, int year, const string &name)
        : Cert(subject, year), _name(name) {
        if (year < 1900 || year > 2018) {
            throw InvalidYear(year,
                              "Invalid year, should be between 1900-2018");
        }
    }

    const string &getName() const { return _name; }

    bool isEqual(Award &other) {
        return getYear() == other.getYear() &&
               getSubject() == other.getSubject() &&
               getName() == other.getName();
    }

    string toString() override {
        ostringstream oss;
        oss << "NAME(" << getName() << ") " << Cert::toString();
        return oss.str();
    }
};

bool getCertByYear(Cert *certs[], size_t len, int year, vector<Cert *> &res) {
    const size_t sz = res.size();
    for (size_t ii = 0; ii < len; ++ii) {
        if (certs[ii] && certs[ii]->getYear() == year) {
            res.push_back(certs[ii]);
        }
    }

    return res.size() > sz;
}

Cert *createCert() {
    cout << "Enter subject: ";
    string subject;
    cin >> subject;

    cout << "Enter year (1900-2018): ";
    int year;
    cin >> year;

    cout << "Enter name: ";
    string name;
    cin.ignore();
    getline(cin, name);

    Cert *res = nullptr;
    try {
        if (name.empty()) {
            res = new Cert(subject, year);
        } else {
            res = new Award(subject, year, name);
        }
    } catch (const InvalidYear &ex) {
        cout << ex.getBadYear() << ", " << ex.what() << endl;
    }

    return res;
}

int main(int argc, const char *argv[]) {
    Cert c1("math", 1918);
    Cert c2("physics", 2000);
    Cert c3("chemistry", 2010);

    Award a1("math", 2000, "Jack");
    Award a2("physics", 2010, "John");
    Award a3("math", 2000, "Jack");

    cout << "Compare 2 awards (expecting false): " << boolalpha
         << a1.isEqual(a2) << endl;
    assert(!a1.isEqual(a2));

    cout << "Compare 2 awards (expecting true): " << boolalpha << a1.isEqual(a3)
         << endl;
    assert(a1.isEqual(a3));

    vector<Cert *> res;
    Cert *certs[] = {&a1, &c1, &a2, &c2, &a3, &c3};
    getCertByYear(certs, 6, 2000, res);
    cout << "Search for certificates and awards for year 2000 (expecting 3): "
         << res.size() << endl;
    assert(res.size() == 3);

    try {
        new Award("arts", 1899, "Pablo");
        assert(!"should not reach here!");
    } catch (const InvalidYear &ex) {
        cout << "Error propagation via exception (expecting year=1899): year="
             << ex.getBadYear() << ", exception=" << ex.what() << endl;
    }

    // Tests till user enters invalid year
    cout << endl
         << "Run user interaction tests till invalid year is entered:" << endl;
    Cert *cert;
    while ((cert = createCert())) {
        cout << cert->toString() << endl;
        delete cert;
    }

    return 0;
}