#ifndef request_h
#define request_h

#include <string>
#include "user.h"

using namespace std;

namespace req {
    int askAccountNumber();
    int askAccountNumber(string errMsg);
    string askAccountName(User user, int valid);
    double askAmount();
    double askAmount(double max);
    bool askNames(User user); // For use with privileged users.
}

#endif
