echo "Input: $1"
echo "Expected: $2"
echo "Running Tests..."
cat $1 | ./bank > actual/cmdline/cmdline.atf
diff actual/cmdline/cmdline.atf $2