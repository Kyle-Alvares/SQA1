#include <iostream>
#include <fstream>
#include <vector>
#include "user.h"

using namespace std;

// constructors
User::User() {}

User::User(int accountNumber) {
    getUserData(accountNumber);
}

// accessor methods

// Gets the current user’s account number.
int User::getAccountNumber() { return accountNumber; }
// Gets the current user’s first name.
string User::getFirstName() { return firstName; }
// Gets the current user’s middle name.
string User::getMiddleName() { return middleName; }
// Gets the current user’s last name.
string User::getLastName() { return lastName; }
// Gets the current user’s password.
string User::getPassword() { return password; }
// Checks if the current user has admin abilities.
bool User::isAdmin() { return isAdminAccount; }
// Gets the current user’s account(s).
vector<User::account> User::getAccounts() { return accounts; }

// mutator methods

// Sets the current user’s first name to the account.
void User::setFirstName(string firstName) { 
    this->firstName = firstName; 
}
// Sets the current user’s middle name to the account.
void User::setMiddleName(string middleName) { 
    this->middleName = middleName; 
}
// Sets the current user’s last name to the account.
void User::setLastName(string lastName) { 
    this->lastName = lastName; 
}
// Sets the current user’s password to the account.
void User::setPassword(string password) { 
    this->password = password; 
}
/* 
*  Creates a new user account. 
*  Rolls a dice between 1000000 -  9999999, until we get an ID that doesn’t already exist. 
*  New bank accounts only have cheqings with balance 0 by default. 
*/
void User::createUser() {
    bool accountExists = true;
    while(accountExists) {
        int min = 10000, max = 99999;
        accountNumber = min + rand() % (max - min + 1); 
        ifstream ifs("./users/" + to_string(accountNumber) + ".txt");
        if(ifs.fail()){
            accountExists = false;
        }
    }
    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter middle name: ";
    cin >> middleName;
    cout << "Enter last name: ";
    cin >> lastName;
    cout << "Enter password: ";
    cin >> password;
    isAdminAccount = false;
    account acc;
    acc.disabled = false;
    acc.isStudentPlan = false;
    acc.accountName = "chequings";
    acc.balance = 0;
    accounts.push_back(acc);
}
// Loads the user’s data from a file if we have a matching account #.
void User::getUserData(int accountNumber) {
    this->accountNumber = accountNumber;
    
    ifstream ifs("./users/" + to_string(accountNumber) + ".txt");
    if(ifs.fail()){
        cerr << "Error. Unable to load user." << endl;
        exit(1);
    }
    string line;
    ifs >> line;
    firstName = line;
    ifs >> line;
    middleName = line;
    ifs >> line;
    lastName = line;
    ifs >> line;
    password = line;
    ifs >> line;
    isAdminAccount = stoi(line);
    while(ifs >> line) {
        account acc;
        int index = line.find("$");
        acc.disabled = line.substr(0,1).compare("0") != 0;
        acc.isStudentPlan = line.substr(1,1).compare("0") != 0;
        acc.accountName = line.substr(2, index-2);
        acc.balance = stoi(line.substr(index+1)); 
        accounts.push_back(acc);
    }
    ifs.close();
}
/*
*  Exports user info to a file. Resulting filename will be account#.txt. 
*  First name, middle name, last name, password, admin flag, account disabled flag, 
*  account plan, account name, and account balance. Each field in a new line.
*/
void User::saveUserData() {
    ofstream ofs("./users/" + to_string(accountNumber) + ".txt");
    if(ofs.fail()) {
        cerr << "Error! Unable to save user data!";
        exit(1);
    }
    ofs << firstName << endl;
    ofs << middleName << endl;
    ofs << lastName << endl;
    ofs << password << endl;
    ofs << isAdminAccount << endl;
    for(int i = 0; i < accounts.size(); i++) {
        ofs << accounts[i].disabled << accounts[i].isStudentPlan << accounts[i].accountName 
            << "$" << accounts[i].balance << endl;
    }
    accounts.clear();
    ofs.close(); 
} 

// Locates the index corresponding to the account.
int User::getAccountIndex(string accountName) {
    for(int i = 0; i < accounts.size(); i++) {
        if(accounts[i].accountName.compare(accountName) == 0) {
            return i;
        }
    }
    return -1;
}
/*
*  Adds balance to the user’s specified account. 
*  Must be > 0. Negative values aren’t allowed.
*/
bool User::deposit(string accountName, double amount) {
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    if(amount <= 0) { return false; }
    accounts[index].balance += amount;
    return true;
}
/*
*  Removes balance to the user’s specified account. 
*  Must be > 0, and <= 500.
*/
bool User::withdraw(string accountName, double amount) {
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    if(amount <= 0 || amount > accounts[index].balance) { return false; }
    accounts[index].balance -= amount;
    return true;
}
/*
*  ***ADMIN ONLY***
*  Add a new account to the banking system. New accounts may 
*  be assigned to existing users, and have a balance of $0.00 by default.
*/
bool User::createAccount(string accountName) {
    if(!isAdmin()) return false;
    if(getAccountIndex(accountName) != -1) { return false; }
    account acc;
    acc.disabled = false;
    acc.isStudentPlan = false;
    acc.accountName = accountName;
    acc.balance = 0;
    accounts.push_back(acc);
    return true;
}
/* 
*  ***ADMIN ONLY*** 
*  Disables an account.
*/
bool User::disableAccount(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts[index].disabled = true;
    return true;
}
/*
*  ***ADMIN ONLY*** 
*  Checks if the specified account is disabled. Disabled accounts exist
*  in the banking system, but they may not login, and therefore, they 
*  have no access to any of the actions that require it.
*/
bool User::isDisabled(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    return accounts[index].disabled;
}
/*
*  ***ADMIN ONLY*** 
*  Removes the specified account from the banking system. 
*  This action is non-reversible. 
*/
bool User::deleteAccount(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts.erase(accounts.begin() + index);
    return true;
}
/*
*  ***ADMIN ONLY*** 
*  Changes an account’s plan. Can be set as “student” or “standard”. 
*/
bool User::changePlan(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts[index].isStudentPlan = !accounts[index].isStudentPlan;
    return true;
}