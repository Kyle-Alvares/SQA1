#!/bin/sh
make
echo "Running Changeplan Tests"
for i in 1 2 3 4 
do
    echo "\n\nRunning Test $i\n\n"
    cat input/changeplan/changeplan$i.inp | ./bank > actual/changeplan/changeplan$i.atf
    diff actual/changeplan/changeplan$i.atf expected/changeplan/changeeplan$i.etf
done
