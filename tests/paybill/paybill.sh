#!/bin/sh
make
echo "Running Paybill Tests"
for i in 1 2 3 4 5
do
    echo "\n\nRunning Test $i\n\n"
    cat input/paybill/paybill$i.inp | ./bank > actual/paybill/paybill$i.atf
    diff actual/paybill/paybill$i.atf expected/paybill/paybill$i.etf
done