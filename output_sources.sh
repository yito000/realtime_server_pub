find ./src -regex ".*\\.cc$" | sed "s/\.\//  /g"
find ./src -regex ".*\\.cpp$" | sed "s/\.\//  /g"
find ./src -regex ".*\\.h$" | sed "s/\.\//  /g"
find ./src -regex ".*\\.hpp$" | sed "s/\.\//  /g"
find ./src -regex ".*\\.ipp$" | sed "s/\.\//  /g"

