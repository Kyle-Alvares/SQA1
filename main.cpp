#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "request.h"
#include "session.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc == 3) {
        string cmd = "./cmdline.sh ";
        string arg1 = argv[1]; // input file
        string arg2 = argv[2]; // expected file
        cmd += arg1;
        cmd += " "; 
        cmd += arg2;
        return system(cmd.c_str());
    }
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
        } else if(action == -1) {
            break;
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