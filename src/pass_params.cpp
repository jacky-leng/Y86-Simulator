#include "CPU.h"

void CPU::pass_params()
{
    // write_back
    W_icode = M_icode;
    W_valE = M_valE;
    W_valM = m_valM;
    W_dstE = M_dstE;
    W_dstM = M_dstM;

    W_stat = m_stat;

    // memory
    M_icode = E_icode;
    M_valE = e_valE;
    M_valA = E_valA;
    M_dstE = E_dstE;
    M_dstM = E_dstM;
    M_Cnd = e_Cnd;

    M_stat = E_stat;

    // execute
    E_icode = D_icode;
    E_ifun = D_ifun;
    E_valC = D_valC;
    E_valA = d_valA;
    E_valB = d_valB;
    E_dstE = d_dstE;
    E_dstM = d_dstM;
    E_srcA = d_srcA;
    E_srcB = d_srcB;

    E_stat = D_stat;

    // E_bubble
    if (E_bubble)
    {
        E_icode = nop;
        E_ifun = 0;
        E_dstE = no_reg;
        E_dstM = no_reg;
        E_srcA = no_reg;
        E_srcB = no_reg;
    }

    // decode
    if (!D_stall)
    {
        D_icode = f_icode;
        D_ifun = f_ifun;
        D_rA = f_rA;
        D_rB = f_rB;
        D_valC = f_valC;
        D_valP = f_valP;

        // stat
        D_stat = f_stat;
    }
    if (D_bubble)
    {
        D_icode = nop;
        D_ifun = 0;
    }

    // fetch
    if (!F_stall)
        F_predPC = f_predPC;

    this->STAT = W_stat;
}