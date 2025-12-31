#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <optional>
#include "value.h"

using namespace std;

class Database{
    public:
        void set(const string& key,const string& value);
        optional<string> get(const string& key);
        bool del(const string& key);
        optional<long long> incr(const string& key);

        //LIST operations
        void lpush(const string& key,const string& value);
        optional<string> rpop(const string& key);
        optional<size_t> llen(const string& key);

        //SET operations
        bool sadd(const string& key,const string& member);
        bool srem(const string& key,const string& member);
        optional<unordered_set<string>> smembers(const string& key);

        //TTL operations
        bool expire(const string& key,int seconds);
        optional<long long> ttl(const string& key);
        
    private:
        unordered_map<string,RedisValue> store;
};

#endif