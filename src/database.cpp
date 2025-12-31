#include "database.h"
#include <iostream>

using namespace std;

static long long now()
{
    return time(nullptr);
}

bool isExpired(RedisValue &rv)
{
    return rv.expire_at != -1 && rv.expire_at <= now();
}

void Database::set(const string &key, const string &value)
{
    RedisValue rv;
    rv.type = ValueType::STRING;
    rv.string_val = value;

    store[key] = rv;
}

optional<string> Database::get(const string &key)
{
    if (store.find(key) == store.end())
        return nullopt;

    RedisValue rv = store[key];

    if (isExpired(rv))
    {
        store.erase(key);
        return nullopt;
    }

    if (rv.type != ValueType::STRING)
        return nullopt;
    return rv.string_val;
}

bool Database::del(const string &key)
{
    return store.erase(key) > 0;
}

optional<long long> Database::incr(const string &key)
{
    long long num = 0;

    if (store.find(key) != store.end())
    {
        RedisValue &rv = store[key];

        if (isExpired(rv))
        {
            store.erase(key);
            return nullopt;
        }

        if (rv.type != ValueType::STRING)
            return nullopt;

        try
        {
            num = stoll(rv.string_val);
        }
        catch (...)
        {
            return nullopt;
        }
    }

    num++;
    RedisValue rv;
    rv.type = ValueType::STRING;
    rv.string_val = to_string(num);

    store[key] = rv;
    return num;
}

void Database::lpush(const string &key, const string &value)
{
    if (store.find(key) == store.end())
    {
        RedisValue rv;
        rv.type = ValueType::LIST;
        rv.list_val.push_back(value);
        store[key] = rv;
        return;
    }

    RedisValue &rv = store[key];

     if (isExpired(rv)) {
        store.erase(key);

        RedisValue new_rv;
        new_rv.type = ValueType::LIST;
        new_rv.list_val.push_back(value);
        store[key] = new_rv;
        return;
    }

    if (rv.type != ValueType::LIST)
    {
        throw runtime_error("WRONGTYPE");
    }

    rv.list_val.insert(rv.list_val.begin(), value);
}

optional<string> Database::rpop(const string &key)
{
    if (store.find(key) == store.end())
        return nullopt;

    RedisValue &rv = store[key];

    if (isExpired(rv)) {
        store.erase(key);
        return nullopt;
    }

    if (rv.type != ValueType::LIST)
        throw runtime_error("WRONGTYPE");

    if (rv.list_val.empty())
        return nullopt;

    string val = rv.list_val.back();
    rv.list_val.pop_back();

    if (rv.list_val.empty())
        store.erase(key);

    return val;
}

optional<size_t> Database::llen(const string &key)
{
    if (store.find(key) == store.end())
        return 0;

    RedisValue &rv = store[key];

    if (isExpired(rv)) {
        store.erase(key);
        return nullopt;
    }

    if (rv.type != ValueType::LIST)
        throw runtime_error("WRONGTYPE");

    return rv.list_val.size();
}

bool Database::sadd(const string &key, const string &member)
{

    if (store.find(key) == store.end())
    {
        RedisValue rv;
        rv.type = ValueType::SET;
        rv.set_val.insert(member);
        store[key] = rv;
        return true;
    }

    RedisValue &rv = store[key];

    if (isExpired(rv)) {
        store.erase(key);

        RedisValue new_rv;
        new_rv.type = ValueType::SET;
        new_rv.set_val.insert(member);
        store[key] = new_rv;
        return true;
    }

    if (rv.type != ValueType::SET)
    {
        throw runtime_error("WRONGTYPE");
    }

    return rv.set_val.insert(member).second; // true if newly added
}

bool Database::srem(const string &key, const string &member)
{

    if (store.find(key) == store.end())
        return false;

    RedisValue &rv = store[key];

    if (isExpired(rv)) {
        store.erase(key);
        return false;
    }

    if (rv.type != ValueType::SET)
    {
        throw runtime_error("WRONGTYPE");
    }

    bool removed = rv.set_val.erase(member) > 0;

    if (rv.set_val.empty())
    {
        store.erase(key); // Redis behavior
    }

    return removed;
}

optional<unordered_set<string>> Database::smembers(const string &key)
{

    if (store.find(key) == store.end())
        return nullopt;

    RedisValue &rv = store[key];

    if (isExpired(rv)) {
        store.erase(key);
        return nullopt;
    }

    if (rv.type != ValueType::SET)
    {
        throw runtime_error("WRONGTYPE");
    }

    return rv.set_val;
}

bool Database::expire(const string &key, int seconds)
{

    if (store.find(key) == store.end())
        return false;

    RedisValue &rv = store[key];

    if (isExpired(rv))
    {
        store.erase(key);
        return false;
    }

    rv.expire_at = now() + seconds;
    return true;
}

optional<long long> Database::ttl(const string &key)
{

    if (store.find(key) == store.end())
        return nullopt;

    RedisValue &rv = store[key];

    if (isExpired(rv))
    {
        store.erase(key);
        return nullopt;
    }

    if (rv.expire_at == -1)
        return -1; // no expiry

    return rv.expire_at - now();
}
