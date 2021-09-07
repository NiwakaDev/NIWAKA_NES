#pragma once
#include "common.h"
#include "Mapper.h"
class InesParser;

class Mapper0:public Mapper{
    public:
        Mapper0(InesParser* ines_parser);
        void Write(uint8_t data);
        uint8_t ReadChrRom(uint16_t addr);
};