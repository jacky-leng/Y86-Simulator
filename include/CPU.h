#pragma once
#include "utils.h"
#include "fifo_map.hpp"
#include "json.hpp"
#include "memory.h"

using namespace nlohmann;

// A workaround to give to use fifo_map as map, we are just ignoring the 'less'
// compare
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
    }

    void print(std::ostream &out);

    // Status of the CPU
    addr PC;                                // Program Counter
    std::array<mem_value_64_t, NUM_REG> RF; // Register File
    std::array<bool, NUM_CC> CC;            // Condition Codes
    int STAT;                               // Status
    bool Cnd;                               // Condition
    memory MEM;                             // Memory

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

    // Update the PC
    void PC_update();

    // Set the condition codes
    void set_CC();

    // Set the condition
    void set_Cnd();

    // Perform the operation
    mem_value_t operate();

    void record_stage();

private:
    // Validate address
    bool check_validity(addr address);

    uint8_t icode, ifun, rA, rB;
    mem_value_64_t valA, valB, valP, valC, valE, valM;

    jsonValue answer;
};