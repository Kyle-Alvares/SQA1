#!/bin/sh
make
echo "Running Transfer Tests"
for i in 1 2 3 4 5 
do
    echo "\n\nRunning Test $i\n\n"
    cat input/transfer/transfer$i.inp | ./bank > actual/transfer/transfer$i.atf
    diff actual/transfer/transfer$i.atf expected/transfer/transfer$i.etf
done