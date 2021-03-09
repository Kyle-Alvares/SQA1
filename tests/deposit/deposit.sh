#!/bin/sh
make
echo "Running Deposit Tests"
for i in 1 2 3 4 5
do
    echo "\n\nRunning Test $i\n\n"
    cat input/deposit/deposit$i.inp | ./bank > actual/deposit/deposit$i.atf
    diff actual/deposit/deposit$i.atf expected/deposit/deposit$i.etf
done