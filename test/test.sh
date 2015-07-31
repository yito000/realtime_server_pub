ulimit -n 4096

for i in `seq 1 1`
do
  ./test_app
done

exit

