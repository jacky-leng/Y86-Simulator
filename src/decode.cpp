#include "CPU.h"

void CPU::decode()
{
    switch (this->icode)
    {
    case halt:
    case nop:
    case irmovq:
    case jxx:
        break;
    case rrmovq:
    case rmmovq:
    case mrmovq:
    case opq:
        this->valA = this->RF[this->rA];
        this->valB = this->RF[this->rB];
        break;
    case call:
    case ret:
    case popq:
        this->valA = this->RF[rsp];
        this->valB = this->RF[rsp];
        break;
    case pushq:
        this->valA = this->RF[this->rA];
        this->valB = this->RF[rsp];
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}