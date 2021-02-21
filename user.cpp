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
int User::getAccountNumber() { return accountNumber; }
string User::getFirstName() { return firstName; }
string User::getMiddleName() { return middleName; }
string User::getLastName() { return lastName; }
string User::getPassword() { return password; }
bool User::isAdmin() { return isAdminAccount; }
vector<User::account> User::getAccounts() { return accounts; }

// mutator methods
void User::setFirstName(string firstName) { 
    this->firstName = firstName; 
}
void User::setMiddleName(string middleName) { 
    this->middleName = middleName; 
}
void User::setLastName(string lastName) { 
    this->lastName = lastName; 
}
void User::setPassword(string password) { 
    this->password = password; 
}

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

int User::getAccountIndex(string accountName) {
    for(int i = 0; i < accounts.size(); i++) {
        if(accounts[i].accountName.compare(accountName) == 0) {
            return i;
        }
    }
    return -1;
}

bool User::deposit(string accountName, double amount) {
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    if(amount <= 0) { return false; }
    accounts[index].balance += amount;
    return true;
}

bool User::withdraw(string accountName, double amount) {
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    if(amount <= 0 || amount > accounts[index].balance) { return false; }
    accounts[index].balance -= amount;
    return true;
}

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

bool User::disableAccount(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts[index].disabled = true;
    return true;
}

bool User::isDisabled(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    return accounts[index].disabled;
}

bool User::deleteAccount(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts.erase(accounts.begin() + index);
    return true;
}

bool User::changePlan(string accountName) {
    if(!isAdmin()) return false;
    int index = getAccountIndex(accountName);
    if(index == -1) { return false; }
    accounts[index].isStudentPlan = !accounts[index].isStudentPlan;
    return true;
}