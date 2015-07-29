p=`pwd`

export DYLD_FALLBACK_LIBRARY_PATH=$p/libs/lib:$DYLD_FALLBACK_LIBRARY_PATH

if test $# -gt 1; then
  $p/realtime_server $1 $2
else
  echo $p

  $p/realtime_server
fi

