#include "TestHarness.h"
#include "CPU.h"
#include "instructions.h"
#include "calltable.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(void){

    TestHarness th;
    CPU* cpu = CPU::getInstance();
    cpu->printRegisters();

    
    
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

        return th.checkEqual("A is 255", cpu->A, 16);
    }));

    th.runAllTests();

    return 0;
}