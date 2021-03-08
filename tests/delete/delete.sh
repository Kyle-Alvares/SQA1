#!/bin/sh
make
echo "Running Delete Tests"
for i in 1 2 3 4 5 6
do
    echo "\n\nRunning Test $i\n\n"
    cat input/delete/delete$i.inp | ./bank > actual/delete/delete$i.atf
    diff actual/delete/delete$i.atf expected/delete/delete$i.etf
done
