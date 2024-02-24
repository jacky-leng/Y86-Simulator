#include "CPU.h"

void CPU::execute()
{
    switch (this->icode)
    {
    case halt:
        this->STAT = STAT_HLT;
        break;
    case nop:
        break;
    case rrmovq:
    case jxx:
        this->valE = this->valA;
        this->set_Cnd();
        break;
    case irmovq:
        this->valE = this->valC;
        break;
    case rmmovq:
    case mrmovq:
        this->valE = this->valB + this->valC;
        break;
    case opq:
        this->valE = this->operate().get_val();
        this->set_CC();
        break;
    case call:
    case pushq:
        this->valE = this->valB - 8;
        break;
    case ret:
    case popq:
        this->valE = this->valB + 8;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}
