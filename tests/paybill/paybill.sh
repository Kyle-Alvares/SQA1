#!/bin/sh
make
cat input/paybill/paybill5.inp | ./bank > actual/paybill/paybill5.atf
diff actual/paybill/paybill5.atf expected/paybill/paybill5.etf