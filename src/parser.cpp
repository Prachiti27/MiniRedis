#include "parser.h"
#include <sstream>

using namespace std;

Command Parser::parse(const string& input){
    Command cmd;
    stringstream ss(input);
    string token;

    bool first = true;

    while(ss >> token){
        if(first){
            cmd.name = token;
            first = false;
        }
        else{
            cmd.args.push_back(token);
        }
    }

    return cmd;
}