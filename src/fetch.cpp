#include "CPU.h"

void CPU::fetch()
{
    //    if (f_stat > 0)return;

    // Select_PC
    // Select_PC choose f_PC from 3 sources based on icode
    if (M_icode == jxx && !M_Cnd)
        f_PC = M_valA; // dest address
    else if (W_icode == ret)
        f_PC = W_valM; // return address
    else
        //        f_PC = f_predPC;  // predicted value
        f_PC = F_predPC; // predicted value

    byte_t temp_byte = this->MEM.readByte(f_PC);
    // The icode is the first 4 bits
    f_icode = temp_byte >> 4 & 0xf;
    // The ifun is the last next 4 bits
    f_ifun = temp_byte & 0xf;
    f_rA = no_reg;
    f_rB = no_reg;
    f_valC = 0;

    switch (f_icode)
    {
    // 1-byte code
    case halt:
        f_stat = STAT_HLT;
        return;
    case nop:
    case ret:
        f_valP = f_PC + 1;
        break;
        // 2-bytes
    case rrmovq:
    case opq:
    case pushq:
    case popq:
        temp_byte = this->MEM.readByte(f_PC + 1);
        f_rA = (temp_byte >> 4) & 0xf;
        f_rB = temp_byte & 0xf;
        f_valP = f_PC + 2;
        break;
    case irmovq:
    case rmmovq:
    case mrmovq:
        //        case 12:
        temp_byte = this->MEM.readByte(f_PC + 1);
        f_rA = (temp_byte >> 4) & 0xf;
        f_rB = temp_byte & 0xf;
        f_valC = this->MEM.read(f_PC + 2);
        f_valP = f_PC + 10;
        break;
    case jxx:
    case call:
        f_valC = this->MEM.read(f_PC + 1);
        f_valP = f_PC + 9;
        break;
    default:
        //            this->STAT = STAT_INS;
        f_stat = STAT_INS;
        return;
    }

    // PC_Predict
    // always-jump strategy
    if (f_icode == jxx || f_icode == call)
        f_predPC = f_valC;
    else
        f_predPC = f_valP;
}
