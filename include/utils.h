#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const int NUM_REG = 16;
const int NUM_CC = 3;
const int MEM_SIZE = 65536;

const int CC_ZF = 0;
const int CC_SF = 1;
const int CC_OF = 2;

const int rax = 0;
const int rcx = 1;
const int rdx = 2;
const int rbx = 3;
const int rsp = 4;
const int rbp = 5;
const int rsi = 6;
const int rdi = 7;
const int r8 = 8;
const int r9 = 9;
const int r10 = 10;
const int r11 = 11;
const int r12 = 12;
const int r13 = 13;
const int r14 = 14;
const int no_reg = 15;

const int STAT_AOK = 1;
const int STAT_HLT = 2;
const int STAT_ADR = 3;
const int STAT_INS = 4;

const uint64_t BYTE_MASK = 0xff;
const int BYTE_SIZE = 8;
const int BLOCK_SIZE = 8;

const int halt = 0;
const int nop = 1;
const int rrmovq = 2;
const int irmovq = 3;
const int rmmovq = 4;
const int mrmovq = 5;
const int opq = 6;
const int jxx = 7;
const int call = 8;
const int ret = 9;
const int pushq = 0xA;
const int popq = 0xB;

const int addq = 0x0;
const int subq = 0x1;
const int andq = 0x2;
const int xorq = 0x3;

#define jmp 0x0
#define jle 0x1
#define jl 0x2
#define je 0x3
#define jne 0x4
#define jge 0x5
#define jg 0x6

const size_t MAX_CLOCK_CYCLE = 214;

using byte_t = unsigned char;
using value_t = long long;
using mem_value_64_t = long long;
using addr = unsigned long long;

struct mem_value_t {
  std::vector<unsigned char> data = std::vector<unsigned char>(8);
  std::vector<unsigned char> rdata = std::vector<unsigned char>(8);

  mem_value_t() = default;

  mem_value_t(mem_value_64_t _value) {
    for (auto i = 0; i < BLOCK_SIZE; ++i) {
      rdata[i] = _value & BYTE_MASK;
      data[BLOCK_SIZE - i - 1] = _value & BYTE_MASK;
      _value >>= BYTE_SIZE;
    }
  }

  value_t get_val() {
    value_t result = 0;
    for (int i = 0; i < BLOCK_SIZE; ++i) {
      result |= static_cast<value_t>(data[i]) << ((8 - i - 1) * BYTE_SIZE);
    }
    return result;
  }
};
