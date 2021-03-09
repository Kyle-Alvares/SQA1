#!/bin/sh
make
echo "Running Logout Tests"
for i in 1 2 3 4 
do
    echo "\n\nRunning Test $i\n\n"
    cat input/logout/logout$i.inp | ./bank > actual/logout/logout$i.atf
    diff actual/logout/logout$i.atf expected/logout/logout$i.etf
done