#include "TestHarness.h"
#include "CPU.h"
#include "instructions.h"

#include <string>
#include <iostream>

using namespace std;

int main(void){

    TestHarness th;

    th.registerTest(new UnitTest("Sanity test", [&th] (){
        return th.checkEqual("1 is equal to 1", 1, 1);
    }));
    
    th.registerTest(new UnitTest("ADC Tests", [&th] (){
        initializeRegisters();
        ADC(127);
        th.checkEqual("0+127 = 127", A, 127);
        th.checkEqual("P is 0x00", P, 0x00);
        ADC(2);
        th.checkEqual("2+127 = 129", A, 129);
        th.checkEqual("P is 0xC0", P, 0xC0);
        ADC(200);
        th.checkEqual("129 + 200 = 73 (overflow)", A, 0x49);
        th.checkEqual("P is 0x41", P, 0x41);
        return 1;
    }));
    
    th.runAllTests();
    return 0;
}