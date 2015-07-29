if test $# -gt 0; then
  voltdb create jikken.jar -d voltdbroot/deployment.xml -H $1
else
  voltdb create jikken.jar -d voltdbroot/deployment.xml -H localhost
fi

