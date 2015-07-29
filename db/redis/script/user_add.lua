local next_id = redis.call('get', 'next_id')
if next_id == false then
    redis.call('set', 'next_id', 1)
    next_id = redis.call('get', 'next_id')
end

redis.call('incr', 'next_id')
local user_key = 'user' .. next_id

local result = redis.call('hmset', user_key, 'id', next_id, 'name', ARGV[1])

if result then
    return user_key
else
    return 'NG'
end

