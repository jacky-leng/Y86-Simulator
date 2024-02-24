#include "CPU.h"

using namespace std;

void CPU::memory_access() {
    switch (M_icode) {
        case halt:
        case nop:
        case rrmovq:
        case irmovq:
        case opq:
        case jxx:
            break;
        case rmmovq:
        case pushq:
        case call:
            if (CPU::check_validity(M_valE))
                this->MEM.write(M_valE, M_valA);
            else
                m_stat = STAT_ADR;
            break;
        case mrmovq:
            if (CPU::check_validity(M_valE))
                m_valM = this->MEM.read(M_valE);
            else
                m_stat = STAT_ADR;
            break;
        case ret:
        case popq:
            if (CPU::check_validity(M_valA))
                m_valM = this->MEM.read(M_valA);
            else
                m_stat = STAT_ADR;
            break;
        default:  // Not supposed to be here
            m_stat = STAT_INS;
            break;
    }

    if (m_stat == STAT_AOK) m_stat = M_stat;
}