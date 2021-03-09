#!/bin/sh
make
echo "Running Disable Tests"
for i in 1 2 3 4 5 6
do
    echo "\n\nRunning Test $i\n\n"
    cat input/disable/disable$i.inp | ./bank > actual/disable/disable$i.atf
    diff actual/disable/disable$i.atf expected/disable/disable$i.etf
done