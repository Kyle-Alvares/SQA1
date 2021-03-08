#!/bin/sh
make
echo "Running Paybill Tests"
for i in 1 2 3 4 5 6 7
do
    echo "\n\nRunning Test $i\n\n"
    cat input/withdraw/withdraw$i.inp | ./bank > actual/withdraw/withdraw$i.atf
    diff actual/withdraw/withdraw$i.atf expected/withdraw/withdraw$i.etf
done
