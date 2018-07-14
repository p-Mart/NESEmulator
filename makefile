CXX=g++
CXXFLAGS=-std=c++11 -g

main: CPU.o instructions.o
	$(CXX) -o CPUTest.out CPU.o instructions.o

tests: tests.o TestHarness.o instructions.o CPU.o MMU.o 
	$(CXX) $(CXXFLAGS) -o RunTests.out tests.o TestHarness.o instructions.o CPU.o MMU.o 