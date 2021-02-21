#ifndef session_h
#define session_h
#include <iostream>
#include <string>
#include <vector>
#include "user.h"
#include "request.h"

using namespace std;

class Session {
    private: 
        User user;
        string username;
        vector<string> transactions;
    public:
        Session(User user);
        User getUser();
        double withdraw(double max);
        void transfer();
        void payBill();
        bool deposit();
        // admin only
        void createAccount();
        void deleteAccount();
        void disableAccount();
        void changePlan();
        void prompt();
        void save();
};

#endif
