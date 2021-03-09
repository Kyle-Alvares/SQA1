#!/bin/sh
make
echo "Running Login Tests"
for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
    echo "\n\nRunning Test $i\n\n"
    cat input/login/login$i.inp | ./bank > actual/login/login$i.atf
    diff actual/login/login$i.atf expected/login/login$i.etf
done
