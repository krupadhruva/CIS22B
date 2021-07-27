/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 4 - Day 1: Programming project #2: Acronym Lookup Program
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

/**
 * Class implementing Acronym with description and methods to match
 */
class Acronym {
private:
    const string _acronym;
    const string _desc;

public:
    Acronym(const string &acronym, const string &desc)
        : _acronym(acronym), _desc(desc) {}

    // npos returned when entry is not found
    bool match(const string &str) const {
        return toString().find(str) != string::npos;
    }

    string toString() const {
        ostringstream oss;
        oss << getDesc() << " (" << getAcronym() << ")";
        return oss.str();
    }

    const string &getAcronym() const { return _acronym; }

    const string &getDesc() const { return _desc; }
};

/**
 * Class implementing a collection of acronyms and methods to
 * perform loading from file, search, add, delete and save back to file
 */
class AcronymList {
private:
    const string _fileName;
    vector<const Acronym *> _acronyms;

public:
    explicit AcronymList(const string &fileName) : _fileName(fileName) {}

    ~AcronymList() {
        for (auto ac : _acronyms) {
            delete ac;
        }
    }

    int load() {
        int err = -1;

        ifstream ifs(_fileName);
        if (ifs.is_open()) {
            while (ifs.good()) {
                string acronym;
                getline(ifs, acronym);
                if (acronym.empty()) {
                    break;
                }

                if (ifs.eof()) {
                    break;
                }

                string desc;
                getline(ifs, desc);
                if (desc.empty()) {
                    break;
                }

                add(acronym, desc);
            }
            ifs.close();

            err = 0;
        }

        return err;
    }

    int save() const {
        int err = -1;
        ofstream ofs(_fileName);
        if (ofs.is_open()) {
            for (auto ac : _acronyms) {
                ofs << ac->getAcronym() << endl << ac->getDesc() << endl;
            }

            ofs.close();
            err = 0;
        }

        return err;
    }

    void list() const {
        for (auto ac : _acronyms) {
            cout << ac->toString() << endl;
        }
    }

    void search(const string &str) const {
        for (auto ac : _acronyms) {
            if (ac->match(str)) {
                cout << ac->toString() << endl;
            }
        }
    }

    bool remove(const string &str) {
        bool removed = false;
        for (auto it = _acronyms.begin(); it != _acronyms.end();) {
            if ((*it)->getAcronym() == str) {
                it = _acronyms.erase(it);
                removed = true;
            } else {
                ++it;
            }
        }

        return removed;
    }

    bool add(const string &acro, const string &desc) {
        bool found = false;
        const string acronym = acro.substr(acro.find_first_not_of(" \t"),
                                           acro.find_last_not_of(" \t") + 1);

        for (auto ac : _acronyms) {
            if (ac->getAcronym() == acronym) {
                found = true;
                break;
            }
        }

        if (!found) {
            const string description =
                desc.substr(desc.find_first_not_of(" \t"),
                            desc.find_last_not_of(" \t") + 1);
            _acronyms.push_back(new Acronym(acronym, description));
        }

        return !found;
    }
};

/**
 * Print usage/help
 */
static void usage() {
    ostringstream oss;
    oss << "help\n"
           "    Available commands:\n"
           "    -  end  terminate the program\n"
           "    -  help show all available commands\n"
           "    -  list Show all acronyms in the current list\n"
           "    -  add  Add a new acronym to the current list\n"
           "    -  search search-string Show all acronyms in the list that "
           "contains the given search string\n"
           "    -  delete an-acronym    Delete a given acronym from the list\n"
           "    -  save Save the current list of acronyms back to the data "
           "file list";

    cout << oss.str() << endl;
}

/**
 * Main loop with user interaction with acronyms list
 *
 * @param argc Length of arguments array
 * @param argv Array of command line arguments (without program name)
 * @return 0 on success, non-zero on error
 */
int AcronymLookup(int argc, const char *argv[]) {
    AcronymList acronyms(argv[0]);
    int err = acronyms.load();

    if (!err) {
        string cmd;
        do {
            cout << endl
                 << "Please enter a command.\nType in 'help' for more info and "
                    "'end' to exit the program: ";
            cin >> cmd;
            if (cmd == "help") {
                usage();
            } else if (cmd == "list") {
                acronyms.list();
            } else if (cmd == "search") {
                string arg;
                cin >> arg;
                acronyms.search(arg);
            } else if (cmd == "add") {
                cout << "Please enter an acronym name: ";
                string acro;
                cin >> acro;
                cout << "Please enter what it stands for: ";
                string desc;
                cin.ignore();
                getline(cin, desc);
                if (acronyms.add(acro, desc)) {
                    cout << "Added acronym: " << acro << " into list" << endl;
                } else {
                    cerr << "Could not add acronym (duplicate found): " << acro
                         << " into list" << endl;
                    cout << flush;
                }
            } else if (cmd == "delete") {
                string arg;
                cin >> arg;
                if (acronyms.remove(arg)) {
                    cout << "Removed acronym: " << arg << " from list" << endl;
                } else {
                    cerr << "Could not removed acronym (acronym missing): "
                         << arg << " from list" << endl;
                    cout << flush;
                }
            } else if (cmd == "save") {
                if (acronyms.save()) {
                    cerr << "Failed to save list" << endl << flush;
                    cout << flush;
                }
            }
        } while (cmd != "end");
    } else {
        cerr << "Unable to open file." << endl;
    }

    return err;
}

int main(int argc, const char *argv[]) {
    // Skip program name
    --argc;
    ++argv;

    return (argc > 0) ? AcronymLookup(argc, argv) : -1;
}
