#include "CPU.h"

void CPU::execute() {
    // initialize
    bool needALU = true;
    e_Cnd = true;

    // ALUA
    switch (E_icode) {
        case rrmovq:
        case opq:
            ALUA = E_valA;
            break;
        case irmovq:
        case rmmovq:
        case mrmovq:
            ALUA = E_valC;
            break;
        case call:
        case pushq:
            ALUA = -8;
            break;
        case ret:
        case popq:
            ALUA = 8;
            break;
        default:
            needALU = false;
            break;
    }
    // ALUB
    switch (E_icode) {
        case rmmovq:
        case mrmovq:
        case opq:
        case call:
        case pushq:
        case ret:
        case popq:
            ALUB = E_valB;
            break;
        case rrmovq:
        case irmovq:
            ALUB = 0;
            break;
        default:
            needALU = false;
    }
    // ALU
    if (needALU) {
        ALU_fun = E_icode == opq ? E_ifun : addq;
        // set_CC();
        switch (ALU_fun) {
            case addq:
                e_valE = ALUA + ALUB;
                E_set_CC();
                break;
            case subq:
                e_valE = ALUB - ALUA;
                E_set_CC();
                break;
            case andq:
                e_valE = ALUA & ALUB;
                E_set_CC();
                break;
            case xorq:
                e_valE = ALUA ^ ALUB;
                E_set_CC();
                break;
        }
    }

    // Set Cnd for conditional jumps and conditional rrmov
    if (E_icode == jxx || E_icode == rrmovq) {
        E_set_Cnd();
    }
}

void CPU::E_set_CC() {
    if (m_stat != STAT_AOK || W_stat != STAT_AOK)
        return;

    CC[CC_ZF] = (e_valE == 0);
    CC[CC_OF] = (ALUA > 0 && ALUB > 0 && e_valE < 0) ||
                (ALUA < 0 && ALUB < 0 && e_valE > 0);
    CC[CC_SF] = (e_valE < 0);
}

void CPU::E_set_Cnd() {
    switch (E_ifun) {
        case 0:
            e_Cnd = true;
            break;
        case 1:
            e_Cnd = (this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF];
            break;
        case 2:
            e_Cnd = this->CC[CC_SF] ^ this->CC[CC_OF];
            break;
        case 3:
            e_Cnd = this->CC[CC_ZF];
            break;
        case 4:
            e_Cnd = !this->CC[CC_ZF];
            break;
        case 5:
            e_Cnd = !(this->CC[CC_SF] ^ this->CC[CC_OF]);
            break;
        case 6:
            e_Cnd = !((this->CC[CC_SF] ^ this->CC[CC_OF]) | this->CC[CC_ZF]);
            break;
        default:
            this->STAT = STAT_INS;
            break;
    }
}