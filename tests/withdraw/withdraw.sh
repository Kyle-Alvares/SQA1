#!/bin/sh
make
cat input/withdraw/withdraw4.inp | ./bank > actual/withdraw/withdraw4.atf
diff actual/withdraw/withdraw4.atf expected/withdraw/withdraw4.etf
