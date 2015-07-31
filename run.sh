p=`pwd`

ulimit -n 4096
export DYLD_FALLBACK_LIBRARY_PATH=$p/libs/lib:$DYLD_FALLBACK_LIBRARY_PATH

if test $# -gt 1; then
  $p/build/realtime_server $1 $2
else
  echo $p

  $p/build/realtime_server
fi

