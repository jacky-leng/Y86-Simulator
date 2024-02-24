#include "CPU.h"

void CPU::write_back()
{
    if (W_dstE != no_reg)
        RF[W_dstE] = W_valE;
    if (W_dstM != no_reg)
        RF[W_dstM] = W_valM;
}
