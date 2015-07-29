src_path=`pwd`/src
voltdb_dir=$VOLTDB_JAR

# export VOLTDB_DIR=/Users/ito/voltdb
# export VOLTDB_BIN=$VOLTDB_DIR/bin
# export VOLTDB_JAR=$VOLTDB_DIR/voltdb

javac \
-cp "$src_path:$voltdb_dir/*" \
src/UserSelect.java \
src/UserUpdate.java \
src/UserList.java \
src/UserDeleteAll.java \
src/UserAdd.java \
-d classes/

