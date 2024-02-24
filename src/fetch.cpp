#include "CPU.h"

void CPU::fetch()
{
    byte_t ins = this->MEM.readByte(this->PC);
    this->icode = ins >> 4;
    this->ifun = ins & 0xf;

    switch (this->icode)
    {
    case halt:
    case nop:
    case ret:
        this->valP = this->PC + 1;
        break;
    case rrmovq:
    case opq:
    case pushq:
    case popq:
        ins = this->MEM.readByte(this->PC + 1);
        this->rA = (ins >> 4) & 0xf;
        this->rB = ins & 0xf;
        this->valP = this->PC + 2;
        break;
    case irmovq:
    case rmmovq:
    case mrmovq:
        ins = this->MEM.readByte(this->PC + 1);
        this->rA = (ins >> 4) & 0xf;
        this->rB = ins & 0xf;
        this->valC = this->MEM.read(this->PC + 2);
        this->valP = this->PC + 10;
        break;
    case jxx:
    case call:
        this->valC = this->MEM.read(this->PC + 1);
        this->valP = this->PC + 9;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}
