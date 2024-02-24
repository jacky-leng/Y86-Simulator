#include "CPU.h"

void CPU::memory_access()
{
    switch (this->icode)
    {
    case halt:
    case nop:
    case rrmovq:
    case irmovq:
    case opq:
    case jxx:
        break;
    case rmmovq:
    case pushq:
        if (this->check_validity(this->valE))
            this->MEM.write(this->valE, this->valA);
        else
            this->STAT = STAT_ADR;
        break;
    case mrmovq:
        if (this->check_validity(this->valE))
            this->valM = this->MEM.read(this->valE);
        else
            this->STAT = STAT_ADR;
        break;
    case call:
        if (this->check_validity(this->valE))
            this->MEM.write(this->valE, this->valP);
        else
            this->STAT = STAT_ADR;
        break;
    case ret:
    case popq:
        if (this->check_validity(this->valA))
            this->valM = this->MEM.read(this->valA);
        else
            this->STAT = STAT_ADR;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}