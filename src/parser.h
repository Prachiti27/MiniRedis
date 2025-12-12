#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>

using namespace std;

struct Command{
    string name;
    vector<string> args;
};

class Parser{
    public:
        static Command parse(const string& input);
};

#endif
