#!/bin/sh
make
cat input/withdraw/withdraw1.inp | ./bank > actual/withdraw/withdraw1.atf
diff actual/withdraw/withdraw1.atf expected/withdraw/withdraw1.etf
