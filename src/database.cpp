#include "database.h"
#include <iostream>

using namespace std;

void Database::set(const string& key,const string& value){
    RedisValue rv;
    rv.type = ValueType::STRING;
    rv.string_val = value;

    store[key] = rv;
}

optional<string> Database::get(const string& key){
    if(store.find(key) == store.end()) return nullopt;

    RedisValue rv = store[key];
    if(rv.type!=ValueType::STRING) return nullopt;
    return rv.string_val;
}

bool Database::del(const string& key){
    return store.erase(key) > 0;
}

optional<long long> Database::incr(const string& key){
    long long num = 0;

    if(store.find(key)!=store.end()){
        RedisValue& rv = store[key];

        if(rv.type!=ValueType::STRING) return nullopt;

        try{
            num = stoll(rv.string_val);
        }
        catch(...){
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