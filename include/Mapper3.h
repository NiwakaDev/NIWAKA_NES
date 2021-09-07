#pragma once
#include "common.h"
#include "Mapper.h"
class InesParser;

class Mapper3:public Mapper{
    private:
        uint8_t bank_select_register = 0;
    public:
        Mapper3(InesParser* ines_parser);
        void Write(uint8_t data);
        uint8_t ReadChrRom(uint16_t addr);
};