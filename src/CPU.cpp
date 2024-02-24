#include "CPU.h"
#include "utils.h"

void CPU::set_CC()
{
    this->CC[CC_ZF] = (this->valE == 0);
    this->CC[CC_SF] = (this->valE < 0);
    switch (this->ifun)
    {
    case 0:
        this->CC[CC_OF] = ((this->valA < 0) == (this->valB < 0)) &&
                          ((this->valE < 0) != (this->valA < 0));
        break;
    case 1:
        this->CC[CC_OF] = ((this->valA < 0) != (this->valB < 0)) &&
                          ((this->valE < 0) == (this->valA < 0));
        break;
    case 2:
    case 3:
        this->CC[CC_OF] = false;
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

void CPU::set_Cnd()
{
    switch (this->ifun)
    {
    case 0:
        this->Cnd = true;
        break;
    case 1:
        this->Cnd = (this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF];
        break;
    case 2:
        this->Cnd = this->CC[CC_SF] ^ this->CC[CC_OF];
        break;
    case 3:
        this->Cnd = this->CC[CC_ZF];
        break;
    case 4:
        this->Cnd = !this->CC[CC_ZF];
        break;
    case 5:
        this->Cnd = !(this->CC[CC_SF] ^ this->CC[CC_OF]);
        break;
    case 6:
        this->Cnd = !((this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF]);
        break;
    default:
        this->STAT = STAT_INS;
        break;
    }
}

mem_value_t CPU::operate()
{
    switch (this->ifun)
    {
    case addq:
        return this->valA + this->valB;
    case subq:
        return this->valB - this->valA;
    case andq:
        return this->valA & this->valB;
    case xorq:
        return this->valA ^ this->valB;
    default:
        this->STAT = STAT_INS;
        break;
    }
    return 0;
}

bool CPU::check_validity(addr address)
{
    return address >= 0 && address < MEM_SIZE;
}

void CPU::record_stage()
{
    jsonValue curr_stage;

    curr_stage["PC"] = this->PC;
    curr_stage["CC"]["ZF"] = (int)this->CC[CC_ZF];
    curr_stage["CC"]["SF"] = (int)this->CC[CC_SF];
    curr_stage["CC"]["OF"] = (int)this->CC[CC_OF];
    curr_stage["STAT"] = this->STAT;
    curr_stage["REG"]["rax"] = this->RF[rax];
    curr_stage["REG"]["rcx"] = this->RF[rcx];
    curr_stage["REG"]["rdx"] = this->RF[rdx];
    curr_stage["REG"]["rbx"] = this->RF[rbx];
    curr_stage["REG"]["rsp"] = this->RF[rsp];
    curr_stage["REG"]["rbp"] = this->RF[rbp];
    curr_stage["REG"]["rsi"] = this->RF[rsi];
    curr_stage["REG"]["rdi"] = this->RF[rdi];
    curr_stage["REG"]["r8"] = this->RF[r8];
    curr_stage["REG"]["r9"] = this->RF[r9];
    curr_stage["REG"]["r10"] = this->RF[r10];
    curr_stage["REG"]["r11"] = this->RF[r11];
    curr_stage["REG"]["r12"] = this->RF[r12];
    curr_stage["REG"]["r13"] = this->RF[r13];
    curr_stage["REG"]["r14"] = this->RF[r14];

    for (auto &[address, value] : this->MEM.memory_map)
    {
        if (value.get_val() != 0)
        {
            curr_stage["MEM"][std::to_string(address)] = value.get_val();
        }
    }

    this->answer.push_back(curr_stage);
}

void CPU::print(std::ostream &out) { out << this->answer.dump() << std::endl; }