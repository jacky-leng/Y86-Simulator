#include "CPU.h"

void CPU::PC_update()
{
    if (this->STAT != STAT_AOK)
        return;
    switch (this->icode)
    {
    case halt:
        break;
    case nop:
    case rrmovq:
    case irmovq:
    case rmmovq:
    case mrmovq:
    case opq:
    case pushq:
    case popq:
        this->PC = this->valP;
        break;
    case jxx:
        this->PC = this->Cnd ? this->valC : this->valP;
        break;
    case call:
        this->PC = this->valC;
        break;
    case ret:
        this->PC = this->valM;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}
