#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <time.h>
#include "session.h"
#include "user.h"
#include "request.h"

using namespace req;

const string Session::companies[] = {
            "The Bright Light Electric Company", // 52777
            "Credit Card Company Q",             // 56915  
            "Fast Internet Inc" };               // 77793

const string Session::companyAbbrv[] = { "EC", "CQ", "FI" };

const int Session::companyAcc[] = { 52777, 56915, 77793 }; 

// constructors
Session::Session(User user) {
    this->user = user;
    this->username = user.getFirstName() + user.getLastName() + to_string(user.getAccountNumber());
    prompt();
}

// accessor methods
User Session::getUser() {
    return user;
}

double Session::withdraw(double max, bool displayHeader) {
    if(displayHeader)
        cout << endl << "============WITHDRAW===========" << endl;
    if(user.isAdmin())
        askNames(user);
    bool valid = false;
    string accountName;
    while(!valid) {
        accountName = askAccountName(user, -1);
        valid = user.isDisabled(accountName) ? false : true;
        if(!valid) cout << "Error: disabled account!" << endl;
    }
    int index = user.getAccountIndex(accountName);
    double balance = user.getAccounts()[index].balance;
    double amount;
    if(user.isAdmin()) amount = askAmount(balance);
    else amount = askAmount(min(balance, max));
    user.withdraw(accountName, amount);
    transactions.push_back("01" + username + to_string(amount));
    return amount;
}

void Session::transfer() {
    string mssg = "Enter depositee account number: ";
    cout << endl << "============TRANSFER===========" << endl;
    cout << mssg;
    int accountNumber = askAccountNumber(mssg);
    double amount = withdraw(1000, false);
    User other(accountNumber);
    other.deposit("chequings", amount);
    other.saveUserData();
    transactions.push_back("02" + username + to_string(amount));
}

void Session::payBill() {
    cout << endl << "============PAYBILL===========" << endl;
    // cin.ignore(); // clears buffer from cin for getline
    int numCompanies = sizeof(companyAcc) / sizeof(int);
    string company;
    int accountNumber;
    bool validCompany = false;
    while(!validCompany) {
        cout << "Companies: " << endl;
        for(int i = 0; i < numCompanies; i++)
            cout << companies[i] + " (" + companyAbbrv[i] << ")" << endl;
        cout << "Enter company name: ";
        cin >> company;
        for(int i = 0; i < numCompanies; i++) {
            if(company.compare(companyAbbrv[i]) == 0) {
                accountNumber = companyAcc[i];
                validCompany = true;
            }
        }
        if (!validCompany)
            cerr << "Error: Invalid company name!" << endl;
    }
    cout << "Choose account to use to pay bill." << endl;
    double amount = withdraw(2000, false);
    User other(accountNumber);
    other.deposit("chequings", amount);
    other.saveUserData();
    transactions.push_back("03" + username + to_string(amount));
    cout << endl <<  "Bill paid successfully!" << endl;
}

bool Session::deposit() {
    cout << endl << "============DEPOSIT============" << endl;
    string accountName = askAccountName(user, -1);
    double amount = askAmount();
    transactions.push_back("04" + username + to_string(amount));
    return user.deposit(accountName, amount); 
}

void Session::createAccount() {
    cout << endl << "========CREATE ACCOUNT========" << endl;
    if(!user.isAdmin()) { 
        cerr << "Error: privileged transaction!" << endl; 
    } else {
        int session = 0;
        while(session == 0) {
            string accountName = askAccountName(user, 0);
            if (std::any_of(accountName.begin(), accountName.end(), ::isdigit) == true){
                cerr << "Error: Invalid account name!" << endl;
            } else if (accountName.size() > 20) {
                cerr << "Error: Invalid account name!" << endl;
            } else {
                session += 1;
                user.createAccount(accountName);
                transactions.push_back("05" + username + "00000000");
            }
        }
    }
}

void Session::deleteAccount() {
    cout << endl << "========DELETE ACCOUNT========" << endl;
    if(!user.isAdmin()) { 
        cerr << "Error: privileged transaction!" << endl; 
    } else {
        string accountName = askAccountName(user, -1);
        user.deleteAccount(accountName);
        transactions.push_back("06" + username + "00000000");
    } 
}

void Session::disableAccount() {
    cout << endl << "========DISABLE ACCOUNT=======" << endl;
    if(!user.isAdmin()) { 
        cerr << "Error: privileged transaction!" << endl; 
    } else {
        string accountName = askAccountName(user, -1);
        user.disableAccount(accountName);
        transactions.push_back("07" + username + "00000000");
    }
}

void Session::changePlan() {
    cout << endl << "=========CHANGE PLAN==========" << endl;
    if(!user.isAdmin()) { 
        cerr << "Error: privileged transaction!" << endl; 
    } else {
        string accountName = askAccountName(user, -1);
        user.changePlan(accountName);
        transactions.push_back("08" + username + "00000000");
    }
}

void Session::prompt() {
    int max = user.isAdmin() ? 8 : 4;
    while(true) {
        int option;
        bool validOption = false;
        cout << endl << "===========OPTIONS============" << endl;
        cout << "[0] Logout" << endl;
        cout << "[1] Withdraw" << endl;
        cout << "[2] Transfer" << endl;
        cout << "[3] Pay-Bill" << endl;
        cout << "[4] Deposit" << endl;
        if(user.isAdmin()) {
            cout << "[5] Create Account" << endl;
            cout << "[6] Delete Account" << endl;
            cout << "[7] Disable Account" << endl;
            cout << "[8] Change Plan" << endl;
        }
        while(!validOption) {
            validOption = true;
            cout << "Option: ";
            cin >> option;
            if(option < 0 || option > max) { validOption = false; } 
        }
        if(option == 0) {
            break;
        } else if (option == 1) {
            withdraw(500, true);
        } else if (option == 2) {
            transfer();
        } else if (option == 3) {
            payBill();
        } else if (option == 4) {
            deposit();
        } else if (option == 5) {
            createAccount();
        } else if (option == 6) {
            deleteAccount();
        } else if (option == 7) {
            disableAccount();
        } else if (option == 8) {
            changePlan();
        }
    }
}

void Session::save() {
    user.saveUserData();
    time_t rawtime;
    time (&rawtime);
    ofstream ofs("./transactions/" + to_string(user.getAccountNumber()) + ".txt", ios::app);
    if(ofs.fail()) {
        cerr << "Error! Unable to save transactions!";
        exit(1);
    }
    ofs << endl;
    ofs << ctime (&rawtime) << endl;
    for(int i = 0; i < transactions.size(); i++) {
        ofs << transactions[i] << endl;
    }
    ofs.close();
    transactions.clear();
}