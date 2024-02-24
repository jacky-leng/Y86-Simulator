#include "CPU.h"
#include "utils.h"

using namespace std;

void CPU::run() {
    while (true) {
        fetch();
        decode();
        execute();
        memory_access();
        write_back();
        forward();
        control();
        pass_params();
        if (this->STAT != STAT_AOK) break;
        this->record_stage();
    }
    this->record_stage();
}

bool CPU::check_validity(addr address) {
    return address >= 0 && address < MEM_SIZE;
}

void CPU::record_stage() {
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

    for (auto& [address, value] : this->MEM.memory_map) {
        if (value.get_val() != 0) {
            curr_stage["MEM"][std::to_string(address)] = value.get_val();
        }
    }

    this->answer.push_back(curr_stage);
}

void CPU::print(std::ostream& out) {
    out << this->answer.dump() << std::endl;
}