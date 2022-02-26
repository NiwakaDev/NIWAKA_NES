#include "Dma.hpp"
#include "Gui.hpp"
#include "Ppu.hpp"
#include "Cpu.hpp"
#include "InesParser.hpp"
#include "Memory.hpp"
#include "Bus.hpp"
#include "InterruptManager.hpp"
#include "JoyPad.hpp"
#include "Mapper.hpp"
#include "Mapper0.hpp"
#include "Mapper3.hpp"
#include "Emulator.hpp"

Emulator::Emulator(int argc, char** argv){
    this->joy_pad = make_unique<JoyPad>();
    this->ines_parser = make_unique<InesParser>(argv[1]);
    switch(this->ines_parser->GetMapperNumber()){
        case 0:
            this->mapper =  make_unique<Mapper0>(this->ines_parser.get());
            break;
        case 3:
            this->mapper = make_unique<Mapper3>(this->ines_parser.get());
            break;
        default:
            this->Error("Not implemented:  mapper_number = %d at Emulator::Emulator", this->ines_parser->GetMapperNumber());
    }
    this->interrupt_manager = make_unique<InterruptManager>();
    this->memory = make_unique<Memory>(this->ines_parser.get());
    this->dma    = make_unique<Dma>(this->memory.get());
    this->gui    = make_unique<Gui>(this->joy_pad.get());
    this->ppu    = make_unique<Ppu>(this->ines_parser.get(), this->gui.get(), this->mapper.get());
    this->bus    = make_unique<Bus>(this->memory.get(), this->ppu.get(), this->joy_pad.get(), this->dma.get(), this->ines_parser.get(), this->mapper.get());
    this->cpu    = make_unique<Cpu>(this->bus.get()); 
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
                this->cpu->HandleNmi(this->interrupt_manager.get());
            }
            if(this->dma->IsRunning()){
                this->dma->Execute(this->ppu.get());
                cycle = 512;
            }
            cycle += this->cpu->Execute();
            flg = this->ppu->Execute(cycle*3, this->interrupt_manager.get());
        }
        flg = false;
        offset = SDL_GetTicks() - start;
        if(DELAY>offset){
            SDL_Delay(DELAY-offset);
        }
        this->gui->Update();
    }
}