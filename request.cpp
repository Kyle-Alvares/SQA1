#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "request.h"
#include "user.h"

using namespace std;

int req::askAccountNumber()
{
    return req::askAccountNumber("Enter account number: ");
}

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

bool req::askNames(User user)
{
    string fName;
    string lName;
    cout << "Please confirm your first name: ";
    cin >> fName;
    cout << "Please confirm your last name: ";
    cin >> lName;
    if (fName != user.getFirstName() && lName != user.getLastName())
    {
        cerr << "Error: Name doesn't match admin name!" << endl;
        return false;
    }
    else
    {
        return true;
    }
}

double req::askAmount()
{
    return req::askAmount(numeric_limits<double>::max());
}

double req::askAmount(double max)
{
    double amount;
    cout << "Enter amount: $";
    cin >> amount;
    while (amount <= 0 || amount > max)
    {
        cerr << "Error: Invalid amount!" << endl;
        cout << "Enter amount: $";
        cin >> amount;
    }
    return amount;
}