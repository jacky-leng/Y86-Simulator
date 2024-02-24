#include <vector>
#include <iostream>
#include "CPU.h"

int main()
{
    CPU cpu;
    cpu.MEM.readin(std::cin);

    size_t count = 0;

    while (count < MAX_CLOCK_CYCLE)
    {
        count++;
        cpu.fetch();
        cpu.decode();
        cpu.execute();
        if (cpu.STAT != STAT_AOK)
            break;
        cpu.memory_access();
        cpu.write_back();
        if (cpu.STAT != STAT_AOK)
            break;
        cpu.PC_update();
        cpu.record_stage();
    }
    cpu.record_stage();

    cpu.print(std::cout);
    return 0;
}