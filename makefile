CXX=g++
CXXFLAGS=-std=c++11 -g

main: CPU.o instructions.o
	$(CXX) -o CPUTest CPU.o instructions.o

tests: tests.o TestHarness.o instructions.o CPU.o
	$(CXX) -o RunTests tests.o TestHarness.o instructions.o CPU.o