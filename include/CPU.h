#pragma once

#include "fifo_map.hpp"
#include "json.hpp"
#include "memory.h"

using namespace nlohmann;

template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using jsonValue = basic_json<my_workaround_fifo_map>;

class CPU
{
public:
    // Constructor
    CPU() : PC(0), CC({true, false, false}), STAT(STAT_AOK)
    {
        this->RF.fill(0);
        // bubble&stall
        D_bubble = false;
        E_bubble = false;
        F_stall = false;
        D_stall = false;
        // icode & ifun
        f_icode = nop;
        f_ifun = 0;
        D_icode = nop;
        D_ifun = 0;
        E_icode = nop;
        E_ifun = 0;
        M_icode = nop;
        W_icode = nop;
        // fetch

        f_stat = STAT_AOK;
        F_predPC = 0;
        f_rA = no_reg;
        f_rB = no_reg;
        f_valC = 0;
        f_valP = 0;
        f_predPC = 0;
        f_PC = 0;
        // decode
        D_rA = no_reg;
        D_rB = no_reg;
        D_valC = 0;
        D_valP = 0;
        d_valA = 0;
        d_valB = 0;
        d_srcA = no_reg;
        d_srcB = no_reg;
        d_dstE = no_reg;
        d_dstM = no_reg;
        D_stat = STAT_AOK;
        // execute
        E_valC = 0;
        E_valA = 0;
        E_valB = 0;
        E_dstE = no_reg;
        E_dstM = no_reg;
        E_srcA = no_reg;
        E_srcB = no_reg;
        e_valE = 0;
        ALUA = 0;
        ALUB = 0;
        ALU_fun = 0;
        e_Cnd = true;
        E_stat = STAT_AOK;
        // memory
        M_valE = 0;
        M_valA = 0;
        M_dstE = no_reg;
        M_dstM = no_reg;
        m_valM = 0;
        m_Addr = 0;

        M_stat = STAT_AOK;
        m_stat = STAT_AOK;
        // write_back
        W_valE = 0;
        W_valM = 0;
        W_dstE = no_reg;
        W_dstM = no_reg;

        W_stat = STAT_AOK;
    }

    // Run the CPU
    void run();

    void print(std::ostream &out);

    // Status of the CPU
    addr PC;
    std::array<mem_value_64_t, NUM_REG> RF;
    std::array<bool, NUM_CC> CC;
    int STAT;
    memory MEM;

    // FETCH
    long long f_icode, f_ifun, f_rA, f_rB, f_valC, f_valP;
    long long F_predPC, f_predPC, f_PC, f_stat;

    // DECOUDE
    long long D_icode, D_ifun, D_rA, D_rB, D_valC, D_valP, D_stat;
    long long d_valA, d_valB, d_srcA, d_srcB, d_dstE, d_dstM;

    // EXECUTE
    long long E_icode, E_ifun, E_valC, E_valA, E_valB, E_dstE, E_dstM, E_srcA, E_srcB,
        E_stat;
    long long e_valE;
    long long ALUA, ALUB;
    long long ALU_fun;
    bool e_Cnd;

    // MEMORY
    long long M_icode, M_valE, M_valA, M_dstE, M_dstM, M_stat;
    long long m_valM, m_Addr, M_Cnd;
    long long m_stat;

    // WRITE_BACK
    long long W_icode, W_valE, W_valM, W_dstE, W_dstM, W_stat;

    // CONTROL
    bool D_bubble, E_bubble;
    bool F_stall, D_stall;

private:
    // Fetch the instruction
    void fetch();

    // Decode the instruction
    void decode();

    // Execute the instruction
    void execute();

    // Memory operation
    void memory_access();

    // Write back
    void write_back();

    // Set the condition codes
    void E_set_CC();

    // Set the condition
    void E_set_Cnd();

    // Forward Data
    void forward();

    // Validate address
    static bool check_validity(addr address);

    void record_stage();

    // Decide whether to stall or bubble,
    // Set regs for stall and bubble
    void control();

    // Pass all wired signals in pos-edge of a clk cycle
    void pass_params();

    jsonValue answer;
};