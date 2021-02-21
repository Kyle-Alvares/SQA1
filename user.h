#ifndef user_h
#define user_h
#include <iostream>
#include <vector>

using namespace std;

class User {
    protected: 
        struct account {
            bool disabled;
            bool isStudentPlan;
            string accountName;
            double balance;
        };
        vector<account> accounts;  
        int accountNumber;
        string firstName;
        string middleName;
        string lastName;
        bool isAdminAccount;
        string password;
    public:
        User();
        User(int accountNumber);
        int getAccountNumber();
        string getFirstName();
        string getMiddleName();
        string getLastName();
        string getPassword();
        vector<account> getAccounts();
        bool isAdmin();
        void setFirstName(string firstName);
        void setMiddleName(string middleName);
        void setLastName(string lastName);
        void setPassword(string password);
        void getUserData(int accountNumber);
        void createUser();
        void saveUserData();
        int getAccountIndex(string accountName);
        bool deposit(string accountName, double amount);
        bool withdraw(string accountName, double amount);
        bool createAccount(string accountName);
        bool disableAccount(string accountName);
        bool isDisabled(string accountName);
        bool deleteAccount(string accountName);
        bool changePlan(string accountName); 
        friend std::ostream& operator<<(std::ostream& stream, const User& u) {
            stream << u.firstName << " " << u.lastName << " [" << u.accountNumber << "]" << endl;
            for(int i = 0; i < u.accounts.size(); i++) {
                stream << u.accounts[i].accountName << ": $" << u.accounts[i].balance << endl;
            }
            return stream;
        }
};

#endif
