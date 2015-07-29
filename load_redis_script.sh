cd db/redis/script
cat user_add.lua | redis-cli -x script load 

