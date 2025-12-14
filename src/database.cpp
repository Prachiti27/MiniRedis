#include "database.h"
#include <iostream>

using namespace std;

void Database::set(const string& key,const string& value){
    store[key] = value;
}

optional<string> Database::get(const string& key){
    if(store.find(key)!=store.end()){
        return store[key];
    }
    return nullopt;
}

bool Database::del(const string& key){
    return store.erase(key) > 0;
}

optional<long long> Database::incr(const string& key){
    long long num = 0;

    if(store.find(key)!=store.end()){
        try{
            num = stoll(store[key]);
        }catch(...){
            return nullopt;
        }
    }

    num++;
    store[key] = to_string(num);
    return num;
}