#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

enum class ValueType{
    STRING,
    LIST,
    SET
};

struct RedisValue{
    ValueType type;
    string string_val;
    vector<string> list_val;
    unordered_set<string> set_val;
};

#endif