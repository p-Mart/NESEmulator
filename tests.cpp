#include "TestHarness.h"
#include "CPU.h"
#include "instructions.h"
#include "calltable.h"

#include <SDL2/SDL.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>


int main(void){

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 256, 224, SDL_WINDOW_SHOWN);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    //SDL_RenderSetScale(ren, 2.0, 2.0);

    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawPoint(ren, 128, 128);

    SDL_RenderPresent(ren);
    SDL_Delay(3000);


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    TestHarness th;
    CPU* cpu = CPU::getInstance();
    cpu->printRegisters();
    
    /*
    th.registerTest(new UnitTest("Sanity test", [&] (){
        return th.checkEqual("1 is equal to 1", 1, 1);
    }));
    

    th.registerTest(new UnitTest("Test Program", [&] (){
        cpu->clearRegisters();
        cpu->loadProgram("test_program.nes");
        cpu->runProgram();

        return th.checkEqual("A is 16", cpu->A, 16);
    }));

    th.registerTest(new UnitTest("Test Program 2", [&] (){
        cpu->clearRegisters();
        cpu->loadProgram("test_program_2.nes");
        cpu->runProgram();

        return th.checkEqual("A is 255", cpu->A, 255);
    }));
    */

    th.registerTest(new UnitTest("Dummy Reads", [&] (){
        cpu->clearRegisters();
        cpu->loadProgram("cpu_dummy_reads.nes");
        cpu->runProgram();

        return th.checkEqual("255 is 255", 255, 255);
    }));
    
    //th.runAllTests();

    return 0;
}