#include <iostream>
#include <fstream>
#include "user.h"
#include "request.h"
#include "session.h"

using namespace std;

int main() {
    while(true) {
        User user;
        bool isAdmin = false;
        int action;
        cout << endl << "=============HOME=============" << endl;
        cout << "[1] Login" << endl << "[2] Create Account" << endl;
        cout << "Action: ";
        cin >> action;
        if(action == 1) {
            int accountNumber;
            string password;
            bool validPassword = false;
            while(!validPassword) {
                validPassword = true;
                cout << "Enter Account Number: "; 
                accountNumber = req::askAccountNumber();
                user.getUserData(accountNumber);
                cout << "Enter Password: ";
                cin >> password;
                if(user.getPassword().compare(password) != 0) {
                    cout << "Error: Incorrect password!" << endl;
                    validPassword = false;
                }
            }
        } else if(action == 2) {
            user.createUser();
        } else {
            continue;
        }
        cout << endl << "===========LOGGED IN===========" << endl;
        Session session(user);
        session.save();
        cout << endl << "===========LOGGED OUT===========" << endl;
    }
    return 0;
}