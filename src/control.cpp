#include "CPU.h"

void CPU::control() {
    // F_stall
    if (((E_icode == mrmovq || E_icode == popq) &&
         (E_dstM == d_srcA || E_dstM == d_srcB))  // Load/use hazard
        ||
        (D_icode == ret || E_icode == ret || M_icode == ret))  // Processing ret
        F_stall = true;
    else
        F_stall = false;

    // D_stall
    if ((E_icode == mrmovq || E_icode == popq) &&
        (E_dstM == d_srcA || E_dstM == d_srcB))  // Load/use hazard
        D_stall = true;
    else
        D_stall = false;

    // D_bubble
    if ((E_icode == jxx &&
         !e_Cnd)  // MisPredicted branch
                  // Stalling at fetch while ret passes through pipeline
        || (!((E_icode == mrmovq || E_icode == popq) &&
              (E_dstM == d_srcA || E_dstM == d_srcB))  // !Load/use hazard
            && (D_icode == ret || E_icode == ret ||
                M_icode == ret))  // Processing ret
    )
        D_bubble = true;
    else
        D_bubble = false;

    // E_bubble
    if ((E_icode == jxx && !e_Cnd)  // MisPredicted branch
        || ((E_icode == mrmovq || E_icode == popq) &&
            (E_dstM == d_srcA || E_dstM == d_srcB))  // Load/use hazard
    )
        E_bubble = true;
    else
        E_bubble = false;
}
