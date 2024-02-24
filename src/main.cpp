#include "CPU.h"
// #include "../include/httplib.h"
// #include <jsoncpp/json/json.h>
#include <vector>
#include <iostream>

using namespace std;

int main()
{

    CPU cpu;
    cpu.MEM.readin(cin);

    try
    {
        cpu.run();
    }
    catch (runtime_error &e)
    {
        cerr << e.what();
    }
    cpu.print(cout);
    return 0;
}