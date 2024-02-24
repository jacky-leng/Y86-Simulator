#include "CPU.h"

void CPU::write_back()
{
    switch (this->icode)
    {
    case halt:
    case nop:
    case rmmovq:
    case jxx:
        break;
    case rrmovq:
        this->RF[this->rB] = this->Cnd ? this->valE : this->valB;
        break;
    case irmovq:
    case opq:
        this->RF[this->rB] = this->valE;
        break;
    case mrmovq:
        this->RF[this->rA] = this->valM;
        break;
    case call:
    case ret:
    case pushq:
        this->RF[rsp] = this->valE;
        break;
    case popq:
        this->RF[rsp] = this->valE;
        this->RF[this->rA] = this->valM;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}
