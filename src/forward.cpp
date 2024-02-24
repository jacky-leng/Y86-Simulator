#include "CPU.h"
void CPU::forward() {
    // Data forwarded from Decode
    // Sel+FwdA
    if (D_icode == call || D_icode == jxx)
        d_valA = D_valP;
    else if (d_srcA == E_dstE && d_srcA != no_reg)
        d_valA = e_valE;
    else if (d_srcA == M_dstM && d_srcA != no_reg)
        d_valA = m_valM;
    else if (d_srcA == M_dstE && d_srcA != no_reg)
        d_valA = M_valE;
    else if (d_srcA == W_dstM && d_srcA != no_reg)
        d_valA = W_valM;
    else if (d_srcA == W_dstE && d_srcA != no_reg)
        d_valA = W_valE;
    else if (d_srcA != no_reg)
        d_valA = RF[d_srcA];

    // FwdB
    if (d_srcB == E_dstE && d_srcB != no_reg)
        d_valB = e_valE;
    else if (d_srcB == M_dstM && d_srcB != no_reg)
        d_valB = m_valM;
    else if (d_srcB == M_dstE && d_srcB != no_reg)
        d_valB = M_valE;
    else if (d_srcB == W_dstM && d_srcB != no_reg)
        d_valB = W_valM;
    else if (d_srcB == W_dstE && d_srcB != no_reg)
        d_valB = W_valE;
    else if (d_srcB != no_reg)
        d_valB = RF[d_srcB];
}