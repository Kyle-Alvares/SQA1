#!/bin/sh
make
echo "Running Create Tests"
for i in 1 2 3 4 5 6
do
    echo "\n\nRunning Test $i\n\n"
    cat input/create/create$i.inp | ./bank > actual/create/create$i.atf
    diff actual/create/create$i.atf expected/create/create$i.etf
done
