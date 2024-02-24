#include "memory.h"
#include "utils.h"

using namespace std;

void memory::readin(istream &inputFile)
{
    vector<byte_t> bytes;
    string line;
    while (getline(inputFile, line))
    {
        std::string cmd;
        size_t delimiterPos = line.find('|');

        if (delimiterPos != std::string::npos)
            cmd = line.substr(0, delimiterPos);
        else
            continue;

        line = cmd;

        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        if (line.empty() || line.back() == ':')
            continue;

        size_t colonPos = line.find(":");
        std::string addr_str = line.substr(0, colonPos);
        std::string val_str = line.substr(colonPos + 1);
        addr address = stoull(addr_str, nullptr, 16);
        bytes.resize(address);

        if (val_str.size() % 2 != 0)
        {
            throw std::invalid_argument("Hex string should have an even length.");
        }

        for (int i = 0; i < val_str.size(); i += 2)
        {
            std::string byteStr = val_str.substr(i, 2);
            try
            {
                unsigned long byteVal = std::stoul(byteStr, nullptr, 16);
                if (byteVal > 0xFF)
                {
                    throw std::out_of_range("Hex value out of byte range.");
                }
                bytes.push_back(static_cast<uint8_t>(byteVal));
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Failed to parse hex string: " + byteStr + " Error: " + e.what());
            }
        }
    }

    auto alignSize = (bytes.size() / BYTE_SIZE + 1) * BYTE_SIZE;
    bytes.resize(alignSize);

    for (size_t i = 0; i < bytes.size(); i += BLOCK_SIZE)
    {
        for (int j = 0; j < BYTE_SIZE; j++)
        {
            memory_map[i].data[7 - j] = bytes[i + j];
            memory_map[i].rdata[j] = bytes[i + j];
        }
    }
}

mem_value_64_t memory::read(addr address)
{
    addr addr_block = address & ~static_cast<addr>(7);
    int offset = address & 7;

    if (memory_map.find(addr_block) == memory_map.end() || ((offset != 0) && memory_map.find(addr_block + 8) == memory_map.end()))
        throw runtime_error("Invalid memory address\n" + std::to_string(address));

    auto firstPart = memory_map[addr_block].rdata;

    firstPart.insert(firstPart.end(), memory_map[addr_block + 8].rdata.begin(), memory_map[addr_block + 8].rdata.end());
    value_t result = 0;
    for (int i = 0; i < BYTE_SIZE; i++)
    {
        result |= static_cast<value_t>(firstPart[offset + i]) << (i * BYTE_SIZE);
    }
    return result;
}

byte_t memory::readByte(addr address, int len)
{
    addr addr_block = address & ~static_cast<addr>(7);
    if (memory_map.find(addr_block) == memory_map.end())
        throw runtime_error("Invalid memory address\n");
    int offset = address & 7;

    mem_value_t tmp = memory_map[addr_block];
    return tmp.rdata[offset];
}

void memory::write(addr address, mem_value_64_t value)
{
    // Address addr_blockment
    if (address % BYTE_SIZE != 0)
        throw runtime_error("Unaddr_blocked memory address\n");

    for (size_t i = 0; i < BLOCK_SIZE; ++i)
    {
        memory_map[address].rdata[i] = value & BYTE_MASK;
        memory_map[address].data[BLOCK_SIZE - i - 1] = value & BYTE_MASK;
        value >>= BYTE_SIZE;
    }
}

void memory::writeByte(addr address, byte_t value)
{
    addr addr_block = address & ~static_cast<addr>(7);
    int offset = address & 7;

    memory_map[addr_block].data[7 - offset] = value;
    memory_map[addr_block].rdata[offset] = value;
}
