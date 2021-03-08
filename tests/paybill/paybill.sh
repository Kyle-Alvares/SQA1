#!/bin/sh
make
cat input/paybill/paybill1.inp | ./bank > actual/paybill/paybill1.atf
diff actual/paybill/paybill1.atf expected/paybill/paybill1.etf