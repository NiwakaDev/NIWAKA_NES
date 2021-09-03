#pragma once
#include "common.h"

class Gui;
class Ppu;
class Dma;
class Bus;
class Memory;
class InesParser;
class InterruptManager;
class JoyPad;
class Cpu;

class Emulator:public Object{
    private:
        JoyPad* joy_pad;
        InesParser* ines_parser;
        Cpu* cpu;
        Memory* memory;
        Dma* dma;
        Ppu* ppu;
        Bus* bus;
        Gui* gui;
        InterruptManager *interrupt_manager;
        thread* emu_thread;
        int now_cycle = 0;
    public:
        Emulator(int argc, char** argv);
        void Execute();
};
