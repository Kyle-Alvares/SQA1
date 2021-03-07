#!/bin/sh
make
cat input/withdraw/withdraw1.txt | ./bank > actual/withdraw/withdraw1.txt
diff actual/withdraw/withdraw1.txt expected/withdraw/withdraw1.txt
