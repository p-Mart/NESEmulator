#include "TestHarness.h"
#include <iostream>
#include <string>

using namespace std;

UnitTest::UnitTest(string test_name, std::function<bool()> test_def){
    this->test_name = test_name;
    this->test_def = test_def;
}

void UnitTest::run(){
    cout << test_name << ": ";
    
    test_def();
    if (this->passing == true){
        cout << "\nTest passed.\n";
    }else{
        cout << "\nTest failed.\n";
    }

    cout << endl;
}


TestHarness::TestHarness(){

}

void TestHarness::registerTest(UnitTest *test){
    registered_tests.push_back(test);
}

void TestHarness::runAllTests(){
    for(int i = 0; i < registered_tests.size(); i++){
        current_test = registered_tests[i];

        printf("\n(%d / %d) ", i+1, (int) registered_tests.size());

        current_test->run();
    }
}

bool TestHarness::checkEqual(string description, int a, int b){
    cout << "\n\t" << description;

    bool result = (a == b);
    if (result == true){
        printf("\t\tPass");
    }else{
        printf("\t\tFailed: %d != %d", a, b);
        current_test->passing = false;
    }

    return result;
}