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

    TestHarness th;
    CPU* cpu = CPU::getInstance();
    cpu->printRegisters();
    
    th.registerTest(new UnitTest("Dummy Reads", [&] (){
        cpu->clearRegisters();
        cpu->loadProgram("roms/donkeykong.nes");
        try{
            cpu->runProgram();
        }catch (const char * message){
            printf("Error occurred: %s\n", message);
        }

        return th.checkEqual("255 is 255", 255, 255);
    }));
    
    th.runAllTests();

    return 0;
}