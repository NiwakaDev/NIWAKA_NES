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
    this->joy_pad = new JoyPad();
    assert(this->joy_pad!=NULL);
    this->ines_parser = new InesParser(argv[1]);
    assert(this->ines_parser!=NULL);
    switch(this->ines_parser->GetMapperNumber()){
        case 0:
            this->mapper = new Mapper0(this->ines_parser);
            break;
        case 3:
            this->mapper = new Mapper3(this->ines_parser);
            break;
        default:
            this->Error("Not implemented:  mapper_number = %d at Emulator::Emulator", this->ines_parser->GetMapperNumber());
    }
    assert(this->mapper!=NULL);
    this->interrupt_manager = new InterruptManager();
    assert(this->interrupt_manager!=NULL);
    this->memory = new Memory(this->ines_parser);
    assert(this->memory!=NULL);
    this->dma = new Dma(this->memory);
    assert(this->dma!=NULL);
    this->gui = new Gui(this->joy_pad);
    assert(this->gui!=NULL);
    this->ppu = new Ppu(this->ines_parser, this->gui, this->mapper);
    assert(this->ppu!=NULL);
    this->bus = make_unique<Bus>(this->memory, this->ppu, this->joy_pad, this->dma, this->ines_parser, this->mapper);
    this->cpu = make_unique<Cpu>(this->bus.get()); 
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