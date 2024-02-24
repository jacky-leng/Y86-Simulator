#pragma once
#include "utils.h"

class memory {
public:
  std::map<addr, mem_value_t> memory_map;
  void readin(std::string &inputFile);

  byte_t readByte(addr address, int len = 1);
  mem_value_64_t read(addr address);

  void writeByte(addr address, byte_t value);
  void write(addr address, mem_value_64_t value);
};
