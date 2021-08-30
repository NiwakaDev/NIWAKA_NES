#pragma once
#include "common.h"
using namespace std;

class Memory;
class Ppu;
class JoyPad;
class Dma;
class InesParser;

class Bus:public Object{
    private:
        Ppu* ppu;
        Memory* memory;
        JoyPad* joy_pad;
        Dma* dma;
        InesParser* ines_parser;
    public: 
        Bus(Memory* memory, Ppu* ppu, JoyPad* joy_pad, Dma* dma, InesParser* ines_parser);
        uint8_t Read8(uint16_t addr);
        void Write8(uint16_t addr, uint8_t value);
};