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

    private:
        unordered_map<string,RedisValue> store;
};

#endif