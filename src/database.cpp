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

void Database::lpush(const string& key,const string& value){
    if (store.find(key) == store.end()) {
        RedisValue rv;
        rv.type = ValueType::LIST;
        rv.list_val.push_back(value);
        store[key] = rv;
        return;
    }

    RedisValue& rv = store[key];

    if (rv.type != ValueType::LIST) {
        throw runtime_error("WRONGTYPE");
    }

    rv.list_val.insert(rv.list_val.begin(), value);
}

optional<string> Database::rpop(const string& key){
    if(store.find(key) == store.end()) return nullopt;

    RedisValue& rv = store[key];

    if(rv.type != ValueType::LIST) throw runtime_error("WRONGTYPE");

    if(rv.list_val.empty()) return nullopt;

    string val = rv.list_val.back();
    rv.list_val.pop_back();

    if(rv.list_val.empty()) store.erase(key);

    return val;
}

optional<size_t> Database::llen(const string& key) {
    if (store.find(key) == store.end())
        return 0;

    RedisValue& rv = store[key];

    if (rv.type != ValueType::LIST)
        throw runtime_error("WRONGTYPE");

    return rv.list_val.size();
}
