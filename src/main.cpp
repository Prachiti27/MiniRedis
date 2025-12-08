#include <iostream>
#include "server.h"

using namespace std;

int main(){
    cout<<"Server started...."<<endl;

    Server server(6379);
    server.start();
    
    return 0;
}