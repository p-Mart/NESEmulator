#ifndef TESTHARNESS_H
#define TESTHARNESS_H


#include <vector>
#include <string>
#include <functional>

class UnitTest{

    
    std::function<bool()> test_def;

    public:
        std::string test_name;
        std::string status;
        bool passing = true;

        UnitTest(std::string, std::function<bool()>);
        void run();
        bool checkEqual(int, int);
};

class TestHarness{

    std::vector<UnitTest*> registered_tests;
    UnitTest *current_test;
    public:
        TestHarness();
        void registerTest(UnitTest*);
        void runAllTests();
        bool checkEqual(std::string, int, int);
};



#endif