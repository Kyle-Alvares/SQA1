#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <time.h>
#include "session.h"
#include "user.h"
#include "request.h"

using namespace std;

// constructors
Session::Session(User user)
{
    this->user = user;
    this->username = user.getFirstName() + user.getLastName() + to_string(user.getAccountNumber());
    prompt();
}

// accessor methods
User Session::getUser()
{
    return user;
}
/*
*  Withdraws from the specified account. First asks for the account to withdraw from,
*  then the amount that is to be withdrawn. Check that the amount to withdraw is > 0 && <= 500 (per session)
*/
double Session::withdraw(double max)
{
    cout << endl
         << "============WITHDRAW===========" << endl;
    if (user.isAdmin())
    {
        bool validNames = false;
        while (!validNames)
        {
            validNames = req::askNames(user);
        }
    }
    string accountName = req::askAccountName(user, -1);
    int index = user.getAccountIndex(accountName);
    double balance = user.getAccounts()[index].balance;
    double amount = req::askAmount(min(balance, max));
    user.withdraw(accountName, amount);
    transactions.push_back("01" + username + to_string(amount));
    return amount;
}
/*
*  Remove balance from the active account and add it to the specified account’s chequing 
*  account balance. Check that the amount to transfer is > 0 && <= 1000 (per session)
*/
void Session::transfer()
{
    if (user.isAdmin())
    {
        bool validNames = false;
        while (!validNames)
        {
            validNames = req::askNames(user);
        }
    }
    string mssg = "Enter depositee account number: ";
    cout << endl
         << "============TRANSFER===========" << endl;
    cout << mssg;
    int accountNumber = req::askAccountNumber(mssg);
    double amount = withdraw(1000);
    User other(accountNumber);
    other.deposit("chequings", amount);
    other.saveUserData();
    transactions.push_back("02" + username + to_string(amount));
}
/*
*  Similarly to transfer, it removes balance from the active account, 
*  and moves it over to the specified account’s checkings balance. 
*  The maximum Max amount that may be paid is $2000, per session. 
*  his limitation doesn’t exist for admins. Balance must be at least $0.00 afterwards.
*/
void Session::payBill()
{
    if (user.isAdmin())
    {
        bool validNames = false;
        while (!validNames)
        {
            validNames = req::askNames(user);
        }
    }
    string mssg = "Enter company account number: ";
    cout << endl
         << "============PAYBILL===========" << endl;
    if (user.isAdmin())
    {
        bool validNames = false;
        while (!validNames)
        {
            validNames = req::askNames(user);
        }
    }
    cout << mssg;
    int accountNumber = req::askAccountNumber(mssg);
    string company;
    bool validCompany = false;
    while (!validCompany)
    {
        cout << "Enter company name: ";
        cin >> company;
        if (company.compare("EC") == 0 || company.compare("CQ") == 0 || company.compare("FI") == 0)
        {
            validCompany = true;
        }
        else
        {
            cerr << "Error: invalid name!" << endl;
        }
    }
    double amount = withdraw(2000);
    User other(accountNumber);
    other.deposit("chequings", amount);
    other.saveUserData();
    transactions.push_back("03" + username + to_string(amount));
    cout << endl
         << "Bill paid successfully!" << endl;
}
/*
*  Deposits into the specified account. First asks for the account 
*  to deposit to, then the amount that is to be deposited.
*/
bool Session::deposit()
{
    if (user.isAdmin())
    {
        bool validNames = false;
        while (!validNames)
        {
            validNames = req::askNames(user);
        }
    }
    cout << endl
         << "============DEPOSIT============" << endl;
    string accountName = req::askAccountName(user, -1);
    double amount = req::askAmount();
    transactions.push_back("04" + username + to_string(amount));
    return user.deposit(accountName, amount);
}
/*
*  ***ADMIN ONLY*** 
*  Creates a bank account with the specified name.
*/
void Session::createAccount()
{
    cout << endl
         << "========CREATE ACCOUNT========" << endl;
    if (!user.isAdmin())
    {
        cerr << "Error: privileged transaction!" << endl;
    }
    else
    {
        if (req::askNames(user))
        {
            string accountName = req::askAccountName(user, 0);
            user.createAccount(accountName);
            transactions.push_back("05" + username + "00000000");
        }
        else
        {
            cerr << "Error: admin name doesn't match!" << endl;
        }
    }
}
/*
*  ***ADMIN ONLY*** 
* Removes the given account from the bank’s database (by name).
*/
void Session::deleteAccount()
{
    cout << endl
         << "========DELETE ACCOUNT========" << endl;
    if (!user.isAdmin())
    {
        cerr << "Error: privileged transaction!" << endl;
    }
    else
    {
        if (req::askNames(user))
        {
            string accountName = req::askAccountName(user, -1);
            user.deleteAccount(accountName);
            transactions.push_back("06" + username + "00000000");
        }
        else
        {
            cerr << "Error: admin name doesn't match!" << endl;
        }
    }
}
/*
*  ***ADMIN ONLY*** 
*  Disables the give account (by name) .
*/
void Session::disableAccount()
{
    cout << endl
         << "========DISABLE ACCOUNT=======" << endl;
    if (!user.isAdmin())
    {
        cerr << "Error: privileged transaction!" << endl;
    }
    else
    {
        if (req::askNames(user))
        {
            string accountName = req::askAccountName(user, -1);
            user.disableAccount(accountName);
            transactions.push_back("07" + username + "00000000");
        }
        else
        {
            cerr << "Error: admin name doesn't match!" << endl;
        }
    }
}
/*
*  ***ADMIN ONLY*** 
*  Changes an account’s type. (student or standard)
*/
void Session::changePlan()
{
    cout << endl
         << "=========CHANGE PLAN==========" << endl;
    if (!user.isAdmin())
    {
        cerr << "Error: privileged transaction!" << endl;
    }
    else
    {
        if (req::askNames(user))
        {
            string accountName = req::askAccountName(user, -1);
            user.changePlan(accountName);
            transactions.push_back("08" + username + "00000000");
        }
        else
        {
            cerr << "Error: admin name doesn't match!" << endl;
        }
    }
}
/*
*  Shows all available bank actions. Only accounts 
*  marked as admin can see all the options.
*/
void Session::prompt()
{
    int max = user.isAdmin() ? 8 : 4;
    while (true)
    {
        int option;
        bool validOption = false;
        cout << endl
             << "===========OPTIONS============" << endl;
        cout << "[0] Logout" << endl;
        cout << "[1] Withdraw" << endl;
        cout << "[2] Transfer" << endl;
        cout << "[3] Pay-Bill" << endl;
        cout << "[4] Deposit" << endl;
        if (user.isAdmin())
        {
            cout << "[5] Create Account" << endl;
            cout << "[6] Delete Account" << endl;
            cout << "[7] Disable Account" << endl;
            cout << "[8] Change Plan" << endl;
        }
        while (!validOption)
        {
            validOption = true;
            cout << "Option: ";
            cin >> option;
            if (option < 0 || option > max)
            {
                validOption = false;
            }
        }
        if (option == 0)
        {
            break;
        }
        else if (option == 1)
        {
            withdraw(500);
        }
        else if (option == 2)
        {
            transfer();
        }
        else if (option == 3)
        {
            payBill();
        }
        else if (option == 4)
        {
            deposit();
        }
        else if (option == 5)
        {
            createAccount();
        }
        else if (option == 6)
        {
            deleteAccount();
        }
        else if (option == 7)
        {
            disableAccount();
        }
        else if (option == 8)
        {
            changePlan();
        }
    }
}

// Calls “User” saveUserData().
void Session::save()
{
    user.saveUserData();
    time_t rawtime;
    time(&rawtime);
    ofstream ofs("./transactions/" + to_string(user.getAccountNumber()) + ".txt", ios::app);
    if (ofs.fail())
    {
        cerr << "Error! Unable to save transactions!";
        exit(1);
    }
    ofs << endl;
    ofs << ctime(&rawtime) << endl;
    for (int i = 0; i < transactions.size(); i++)
    {
        ofs << transactions[i] << endl;
    }
    ofs.close();
    transactions.clear();
}