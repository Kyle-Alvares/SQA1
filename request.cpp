#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "request.h"
#include "user.h"

using namespace std;

/*
*  Asks for the account number. This fails if the information 
*  provided doesn’t match what we have on file.
*/
int req::askAccountNumber()
{
    return req::askAccountNumber("Enter account number: ");
}
/* 
*  Asks for the account number. This fails if the information 
*  provided doesn’t match what we have on file. Any error 
*  messages will be stored in the errorMsg parameter.
*/
int req::askAccountNumber(string errMsg)
{
    bool validAccount = false;
    int accountNumber;
    while (!validAccount)
    {
        validAccount = true;
        cin >> accountNumber;
        ifstream ifs("./users/" + to_string(accountNumber) + ".txt");
        if (ifs.fail())
        {
            cerr << "Error: Invalid account number!\n";
            cerr << errMsg;
            validAccount = false;
        }
    }
    return accountNumber;
}
/*
*  Asks for the account name. This fails if the information 
*  provided doesn’t match what we have on file.
*/
string req::askAccountName(User user, int valid)
{
    string accountName;
    cout << "Enter account name: ";
    cin >> accountName;
    while (user.getAccountIndex(accountName) == valid)
    {
        cerr << "Error: Invalid account name!" << endl;
        cout << "Enter account name: ";
        cin >> accountName;
    }
    return accountName;
}
/*
*  ***ADMIN ONLY*** 
*  Confirms admin name before proceeding with action.
*/
void req::askNames(User user)
{
    string fName;
    string lName;
    bool valid = false;
    while(!valid) {
        cout << "Enter your first name: ";
        cin >> fName;
        cout << "Enter your last name: ";
        cin >> lName;
        if (fName == user.getFirstName() && lName == user.getLastName())
            valid = true;
    }
}
/*
*  Asks for an amount for the transaction. 
*  [0 - MaxDoubleValue]
*/
double req::askAmount()
{
    return req::askAmount(numeric_limits<double>::max());
}
/*
*  Asks for an amount for the transaction. 
*  This version respects account limits.
*/
double req::askAmount(double max)
{
    double amount;
    bool validDecimal;
    bool valid = false;
    while(!valid) 
    {
        cout << "Enter amount: $";
        cin >> amount;
        string strAmount = to_string(amount);
        string decimal = strAmount.substr(strAmount.find('.') + 1);
        validDecimal = true;
        for(int i = 2; i < decimal.length(); i++)
            if(decimal[i] != '0')
                validDecimal = false;
        if(amount <= 0 || amount > max || !validDecimal)
            cerr << "Error: Invalid amount!" << endl;
        else 
            valid = true;
    }
    return amount;
}