CXX=g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
CXXFLAGS=-std=c++11 -g $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)


main: CPU.o instructions.o
	$(CXX) -o CPUTest.out CPU.o instructions.o

tests: tests.o TestHarness.o instructions.o CPU.o MMU.o  PPU.o
	$(CXX) $(CXXFLAGS)  -o RunTests.out tests.o TestHarness.o instructions.o CPU.o MMU.o PPU.o $(LDFLAGS)