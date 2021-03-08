#!/bin/sh
make
cat input/withdraw/withdraw7.inp | ./bank > actual/withdraw/withdraw7.atf
diff actual/withdraw/withdraw7.atf expected/withdraw/withdraw7.etf
