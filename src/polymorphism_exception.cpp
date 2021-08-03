/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 5 - Day 2: Practice on polymorphism and exception
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
 * Class provided by instructor. Base class that represents Item
 * and its properties like ID and availability.
 */
class Item {
private:
    int m_id;
    bool m_isAvail;

public:
    Item(int id, bool isAvail) : m_id(id), m_isAvail(isAvail) {}

    virtual string toString() {
        string status = (m_isAvail) ? "Available" : "Not Available";
        stringstream ss;
        ss << "ID(" << m_id << ") STATUS(" << status << ")";

        return ss.str();
    }

    bool isAvailable() { return m_isAvail; }

    int getMId() const { return m_id; }
};

/**
 * Class manages error reason code for negative IDs, empty/blank
 * titles, or empty/blank authors.
 */
class InvalidItemException : public exception {
public:
    static const int ERROR_NEGATIVE_ID = 1;
    static const int ERROR_TITLE_EMPTY_OR_BLANK = 2;
    static const int ERROR_AUTHOR_EMPTY_OR_BLANK = 3;

private:
    int _reason;
    string _errMsg;

public:
    explicit InvalidItemException(int reason) : _reason(reason) {
        switch (_reason) {
        case ERROR_NEGATIVE_ID:
            _errMsg = "Document ID cannot be negative.";
            break;
        case ERROR_TITLE_EMPTY_OR_BLANK:
            _errMsg = "Document title cannot be empty or blank.";
            break;
        case ERROR_AUTHOR_EMPTY_OR_BLANK:
            _errMsg = "Document author cannot be empty or blank.";
            break;
        }
    }

    const char *what() const _NOEXCEPT override { return _errMsg.c_str(); }

    int getReason() const { return _reason; }
};

/**
 * Class derives from Item class. Maintains document id
 * and title of document.
 */
class Document : public Item {
private:
    const string _docTitle;

public:
    Document(int docID, bool isAvail, const string &title)
        : Item(docID, isAvail), _docTitle(title) {
        if (docID < 0) {
            throw InvalidItemException(InvalidItemException::ERROR_NEGATIVE_ID);
        } else if (title.empty() ||
                   title.find_first_not_of(" \t") == string::npos) {
            throw InvalidItemException(
                InvalidItemException::ERROR_TITLE_EMPTY_OR_BLANK);
        }
    }

    const string &getDocTitle() const { return _docTitle; }

    string toString() override {
        stringstream ss;
        ss << Item::toString() << " TITLE(" << getDocTitle() << ")";
        return ss.str();
    }

    bool isDuplicate(const Document &other) {
        return getMId() == other.getMId() &&
               getDocTitle() == other.getDocTitle();
    }

    bool isAlphabeticallyBefore(const Document &other) const {
        return getDocTitle().compare(other.getDocTitle()) < 0;
    }
};

/**
 * Class derives from Document class. Class maintains book's author.
 */
class Book : public Document {
private:
    const string _author;

public:
    Book(int docID, bool avail, const string &title, const string &author)
        : Document(docID, avail, title), _author(author) {
        if (author.empty() || author.find_first_not_of(" \t") == string::npos) {
            throw InvalidItemException(
                InvalidItemException::ERROR_AUTHOR_EMPTY_OR_BLANK);
        }
    }

    const string &getAuthor() const { return _author; }

    string toString() override {
        stringstream ss;
        ss << Document::toString() << " AUTHOR(" << getAuthor() << ")";
        return ss.str();
    }
};

/**
 * This function creates and returns all created Items, Documents, and Books.
 * Values with "N/A" for either author or title will create object with
 * appropriate type.
 *
 * @param ids Array of document IDs
 * @param avails Array of document availability
 * @param titles Array of document titles
 * @param authors Array of authors
 * @param len Length of arrays
 * @param createdCount Count of Items created
 * @return Array of pointers to Item
 */
static Item **createItems(int ids[], bool avails[], string titles[],
                          string authors[], size_t len, size_t &createdCount) {
    Item **res = new Item *[len];
    createdCount = 0;

    for (size_t ii = 0; ii < len; ++ii) {
        try {
            if (titles[ii] == "N/A") {
                res[ii] = new Item(ids[ii], avails[ii]);
            } else if (authors[ii] == "N/A") {
                res[ii] = new Document(ids[ii], avails[ii], titles[ii]);
            } else {
                res[ii] =
                    new Book(ids[ii], avails[ii], titles[ii], authors[ii]);
            }

            ++createdCount;
        } catch (InvalidItemException &ex) {
            res[ii] = nullptr;
        }
    }

    return res;
}

/**
 * Function finds the first and last inputted title in alphabetical order.
 *
 * @param docs Vector of type Document pointers
 * @param first Pointer of type Document for first title
 * @param last Pointer of type Document for last title
 */
static void findFirstLastTitle(vector<Document *> &docs, Document *&first,
                               Document *&last) {
    first = last = nullptr;
    if (!docs.empty()) {
        first = last = docs.front();
        for (auto d : docs) {
            if (d->isAlphabeticallyBefore(*first)) {
                first = d;
            }

            if (last->isAlphabeticallyBefore(*d)) {
                last = d;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    {
        cout << endl << "Test exceptions:" << endl;
        try {
            Document d(-1, true, "Negative ID");
        } catch (InvalidItemException &ex) {
            cout << "\t" << ex.what() << endl;
        }

        try {
            Document d(1, true, " ");
        } catch (InvalidItemException &ex) {
            cout << "\t" << ex.what() << endl;
        }

        try {
            Book b(1, true, "Blank/tab author", "\t");
        } catch (InvalidItemException &ex) {
            cout << "\t" << ex.what() << endl;
        }
    }
    {
        cout << endl << "Testing item creation:" << endl;
        int ids[] = {1234, -1, 1235, 1236, 1237, 1238};
        bool avails[] = {true, true, true, true, true, true};
        string titles[] = {"C++", "Python", "N/A", "How to", "", "HTML"};
        string authors[] = {"John", "Nancy", "Bob", "N/A", "Kathy", ""};

        size_t count;
        Item **items = createItems(ids, avails, titles, authors, 6, count);
        cout << "\tCreated item count: " << count << endl;
        for (size_t ii = 0; ii < 6; ++ii) {
            if (items[ii]) {
                cout << "\t" << items[ii]->toString() << endl;
                delete items[ii];
            }
        }

        delete[] items;
    }

    {
        cout << endl << "Testing findFirstLastTitle():" << endl;
        vector<Document *> docs;
        docs.push_back(new Document(1236, true, "How To"));
        docs.push_back(new Document(1237, true, "Python"));
        docs.push_back(new Book(1234, true, "C++", "John"));
        docs.push_back(new Book(1235, true, "Java", "John"));

        Document *first;
        Document *last;
        findFirstLastTitle(docs, first, last);
        if (first) {
            cout << "\t" << first->toString() << endl;
        }

        if (last) {
            cout << "\t" << last->toString() << endl;
        }

        for (auto docPtr : docs) {
            delete docPtr;
        }
    }
    return 0;
}