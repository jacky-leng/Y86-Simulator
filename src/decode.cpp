#include "CPU.h"

void CPU::decode()
{
    // Initialize return output wires
    d_srcA = no_reg;
    d_srcB = no_reg;
    d_dstE = no_reg;
    d_dstM = no_reg;
    d_valA = 0;
    d_valB = 0;

    // d_dstE
    if (D_icode == irmovq || D_icode == rrmovq || D_icode == opq)
        d_dstE = D_rB;
    else if (D_icode == pushq || D_icode == popq || D_icode == call ||
             D_icode == ret)
        d_dstE = rsp;

    // d_dstM
    if (D_icode == mrmovq || D_icode == popq)
        d_dstM = D_rA;

    // d_srcA
    if (D_icode == rrmovq || D_icode == rmmovq || D_icode == pushq ||
        D_icode == opq)
        d_srcA = D_rA;
    else if (D_icode == popq || D_icode == ret)
        d_srcA = rsp;

    // d_srcB
    if (D_icode == opq || D_icode == mrmovq || D_icode == rmmovq)
        d_srcB = D_rB;
    else if (D_icode == pushq || D_icode == popq || D_icode == ret ||
             D_icode == call)
        d_srcB = rsp;
}