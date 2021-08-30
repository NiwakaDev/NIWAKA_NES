#include "Dma.h"
#include "Gui.h"
#include "Ppu.h"
#include "Cpu.h"
#include "InesParser.h"
#include "Memory.h"
#include "Bus.h"
#include "InterruptManager.h"
#include "JoyPad.h"
#include "Emulator.h"

Emulator::Emulator(int argc, char** argv){
    this->joy_pad = new JoyPad();
    this->ines_parser = new InesParser(argv[1]);
    this->interrupt_manager = new InterruptManager();
    this->memory = new Memory(this->ines_parser);
    this->dma = new Dma(this->memory);
    this->gui = new Gui(this->joy_pad);
    this->ppu = new Ppu(this->ines_parser, this->gui);
    this->bus = new Bus(this->memory, this->ppu, this->joy_pad, this->dma, this->ines_parser);
    this->cpu = new Cpu(this->bus);
}

void Emulator::Execute(){
    int cycle;
    bool flg = false;
    uint32_t start;
    uint32_t end;
    uint32_t offset;
    const uint32_t DELAY = 16;
    this->cpu->Reset();
    while(!this->gui->IsQuit()){
        start = SDL_GetTicks();
        this->gui->PollEvents();
        while(!flg){
            cycle = 0;
            if(this->interrupt_manager->HasNmi()){
                this->cpu->HandleNmi(this->interrupt_manager);
            }
            if(this->dma->IsRunning()){
                this->dma->Execute(this->ppu);
                cycle = 512;
            }
            cycle += this->cpu->Execute();
            flg = this->ppu->Execute(cycle*3, this->interrupt_manager);
        }
        flg = false;
        offset = SDL_GetTicks() - start;
        if(DELAY>offset){
            SDL_Delay(DELAY-offset);
        }  
        this->gui->Update();
    }
}