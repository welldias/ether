#/bin/sh
echo "Usage:"
echo "  cov_test.sh valgrind to execute unit_test, code coverage and valgrind"

VALGRIND=$1

make clean 
make
if [[ "$?" != "0" ]]
then
	exit 1
fi

if [[ "$VALGRIND" == "" ]]
then
src/bin/example1
else
valgrind --leak-check=yes src/bin/example1
fi

lcov -d src/lib --base-directory src/lib --capture --output-file coverage.info
genhtml coverage.info -o ./coverage
firefox ./coverage/index.html
