/*
 * Author: Krupa Dhruva
 *
 * Description:
 * Week 3 - Day 2: Midterm exam practice set 01
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class User {
private:
    string userName;
    string password;
    int userID;

public:
    User(const string &name, const string &pass, int id)
        : userName(name), password(pass), userID(id) {}

    const string &getUserName() const { return userName; }

    const string &getPassword() const { return password; }

    int getUserId() const { return userID; }

    void setPassword(const string &password) { User::password = password; }

    bool hasValidPassword() {
        ostringstream oss;
        oss << userID;
        return userName != password ||
               strcasestr(password.c_str(), oss.str().c_str());
    }

    bool isDuplicate(const User &other) {
        return password == other.password && userName == other.userName;
    }

    string toString() {
        ostringstream oss;
        oss << "NAME(" << userName << ") PASSWORD(" << password << ") ID("
            << userID << ")";
        return oss.str();
    }
};

/**
 * Find a unique user given user name and return match count with password on
 * unique match
 *
 * @param users Array of User pointers
 * @param len Length of array of users
 * @param name user name to search
 * @param passwd Password of user on unique match
 * @return 2 if unique match, 0 is missing or 1 if multiple match
 */
int findPasswordIfUnique(const User *const users[], size_t len,
                         const string &name, string &passwd) {
    int matchingCount = 0;
    string uniquePasswd;

    for (int ii = 0; ii < len; ++ii) {
        if (users[ii]->getUserName() == name) {
            ++matchingCount;

            if (1 == matchingCount) {
                uniquePasswd = users[ii]->getPassword();
            }
        }
    }

    if (1 == matchingCount) {
        passwd = uniquePasswd;
        return 2;
    }

    // This is an easy way to convert any non-0 to 1 and retain 0 as 0
    return !!matchingCount;
}

/**
 * Find matching user ID among a vector of Users (pointers) and change
 * their password. Assumes the vector contains users with unique user IDs
 *
 * @param users Vector of User pointers
 * @param userID User ID to change password
 * @param passwd New password
 * @return True if user ID found and password changed
 */
bool changePassword(const vector<User *> &users, int userID,
                    const string &passwd) {
    for (auto user : users) {
        if (user->getUserId() == userID) {
            user->setPassword(passwd);
            return true;
        }
    }

    return false;
}

/**
 * Function checks to see if argument starts and ends with "--"
 *
 * @param argc length of array of strings (char*)
 * @param argv array of char pointers (command line arguments)
 * @return
 */
bool hasDashDashOnBothEnds(int argc, const char *argv[]) {
    for (int ii = 0; ii < argc; ++ii) {
        size_t len = strlen(argv[ii]);
        if (len < 2 || 0 != strncmp(argv[ii], "--", 2)) {
            continue;
        }

        const char *ptr = argv[ii] + len - 2;
        if (0 == strncmp(ptr, "--", 2)) {
            return true;
        }
    }
    return false;
}

int main(int argc, const char *argv[]) {
    --argc;
    ++argv;

    User u1("j1o2hn", "j1o2hn", 12);
    User u2("j1o2hn", "j1o2hn", 12);
    User u3("hello", "hello14", 14);

    {
        cout << endl << "Tests for duplicate check & password validity" << endl;
        cout << u1.toString() << " password valid? " << boolalpha
             << u1.hasValidPassword() << endl;
        cout << u1.toString() << " == " << u2.toString() << " duplicate? "
             << boolalpha << u1.isDuplicate(u2) << endl;
        cout << u1.toString() << " == " << u3.toString() << " duplicate? "
             << boolalpha << u1.isDuplicate(u3) << endl;
    }

    {
        cout << endl << "Tests for find password if unique user" << endl;
        int ret;
        string passwd;
        const User *const userArr[] = {&u1, &u2, &u3};

        ret = findPasswordIfUnique(userArr, 3, "not found", passwd);
        cout << "Expecting 0 due to no matches and empty password: ret=" << ret
             << ", passwd empty=" << boolalpha << passwd.empty() << endl;

        ret = findPasswordIfUnique(userArr, 3, u1.getUserName(), passwd);
        cout << "Expecting 1 due to 2 matches and empty password: ret=" << ret
             << ", passwd empty=" << boolalpha << passwd.empty() << endl;

        ret = findPasswordIfUnique(userArr, 3, u3.getUserName(), passwd);
        cout << "Expecting 2 due to 1 match and non-empty password: ret=" << ret
             << ", passwd empty=" << boolalpha << passwd.empty() << endl;
    }

    {
        cout << endl
             << "Test change password for user based on user ID" << endl;

        bool ret;
        const vector<User *> users = {&u1, &u2, &u3};
        ret = changePassword(users, u3.getUserId(), "random14");
        cout << "Expecting true since unique user is found and password "
                "changed: "
             << boolalpha << ret << endl;

        ret = changePassword(users, 777, "random14");
        cout << "Expecting false since unique user not found and password "
                "unchanged: "
             << boolalpha << ret << endl;
    }

    {
        cout << endl
             << "Checks to see if argument(s) starts and ends with '--'"
             << endl;
        cout << "Testing for:";
        for (int ii = 0; ii < argc; ++ii) {
            cout << " " << argv[ii];
        }
        cout << ": " << boolalpha << hasDashDashOnBothEnds(argc, argv) << endl;
    }

    return 0;
}
